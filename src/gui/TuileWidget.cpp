/***************************************************************************
 *  TuileWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "TuileWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include <Fl/fl_draw.H>

#include "TuileParamWidget.hpp"
#include "TuileParamGroup.hpp"
#include "TreeWidget.hpp"
#include "SeqWidget.hpp"

#include <tuiles/SeqTuile.hpp>
#include "../audio/AudioTuile.hpp"

using namespace std;
using namespace tuiles;

TuileWidget::TuileWidget(const std::string& name, 
                            Tuile* tuile):
                                TuileWidgetNode(tuile),
                                m_name(name),
                                m_muted(false),
                                m_canTakeInput(true),
								m_height(20),
                                m_sync1X(0), m_sync2X(w()),
                                m_real1X(0), m_real2X(w()),
                                m_sync1Color(FL_FOREGROUND_COLOR), 
                                m_sync2Color(FL_FOREGROUND_COLOR), 
                                m_syncLColor(FL_FOREGROUND_COLOR), 
                                m_rectColor(FL_FOREGROUND_COLOR), 
                                m_realColor(fl_darker(FL_BACKGROUND_COLOR)),
								m_overSyncIn(false), m_overSyncOut(false),
								m_draggingSyncIn(false), 
                                m_draggingSyncOut(false),
								m_dragging(false), m_magnetSize(5),
								m_measureDiv(32), m_connecting(false),
								m_backgroundColor(FL_BACKGROUND_COLOR),
                                m_selected(false),
                                m_paramWidget(NULL) {
}

TuileWidget::~TuileWidget() {}

void TuileWidget::update() {

}

void TuileWidget::resetHighlight() {
    m_sync1Color=FL_FOREGROUND_COLOR;
    m_sync2Color=FL_FOREGROUND_COLOR;
    m_syncLColor=FL_FOREGROUND_COLOR;
    highlightReal(false);
}

void TuileWidget::highlightSyncInLine() {
    m_sync1Color=fl_lighter(FL_BACKGROUND_COLOR);
    m_syncLColor=fl_lighter(FL_BACKGROUND_COLOR);
    highlightReal(false);
}

void TuileWidget::highlightSyncOutLine() {
    m_sync2Color=fl_lighter(FL_BACKGROUND_COLOR);
    m_syncLColor=fl_lighter(FL_BACKGROUND_COLOR);
    highlightReal(false);
}

void TuileWidget::highlightReal(bool high) {
    if(!m_muted) {
        m_rectColor=FL_FOREGROUND_COLOR;
        if(m_selected) {
            m_realColor=fl_darker(FL_DARK_GREEN);
        }
        else {
            m_realColor=fl_darker(fl_darker(FL_BACKGROUND_COLOR));
        }
    }
    else {
        m_realColor=FL_BACKGROUND2_COLOR;
        if(m_selected) {
            m_rectColor=fl_darker(FL_DARK_GREEN);
        }
        else {
            m_rectColor=fl_darker(FL_FOREGROUND_COLOR);
        }
    }
    if(high) {
        if(!m_muted) {
            m_realColor=fl_lighter(m_realColor);
        }
        else {
            m_rectColor=fl_lighter(m_rectColor);
        }
    }
}

void TuileWidget::drawComposition() {
	//realwindow
	fl_color(m_realColor);
	fl_rectf(x()+m_real1X, y(), m_real2X-m_real1X, h());
	fl_color(FL_FOREGROUND_COLOR);
	fl_rect(x()+m_real1X, y(), m_real2X-m_real1X, h());

	//syncwindow
	fl_color(m_syncLColor);
	fl_line(x()+m_sync1X, y()+h()/2, x()+m_real2X, y()+h()/2);
	fl_line(x()+m_sync2X, y()+h()/2, x()+m_real1X, y()+h()/2);
	fl_color(m_sync1Color);
	fl_line(x()+m_sync1X, y()-h()/2, x()+m_sync1X, y()+h()/2);
	fl_color(m_sync2Color);
	fl_line(x()+m_sync2X, y()+h()/2, x()+m_sync2X, y()+3*h()/2);

	//name
    fl_color(FL_WHITE);
	fl_font(FL_HELVETICA_ITALIC, 10);
	fl_draw(m_name.c_str(), x()+m_real1X+2, y()+h()-2);
}

void TuileWidget::drawExecution(const float&) {

}

void TuileWidget::drawConnections() {
    fl_color(FL_BLUE);
    if(m_connecting) {
        fl_begin_line();
        fl_curve(x()+w()/2, y()+h(),
                    x()+w()/2, y()+h()*2,
                    m_dragPosX, m_dragPosY-h(),
                    m_dragPosX, m_dragPosY);
        fl_end_line();
    }

    vector<TuileWidget*>::iterator itWid=m_inputWidgets.begin();
    for(; itWid!=m_inputWidgets.end(); ++itWid) {
        fl_begin_line();
        fl_curve((*itWid)->x()+(*itWid)->w()/2, (*itWid)->y()+(*itWid)->h(), 
                (*itWid)->x()+(*itWid)->w()/2, (*itWid)->y()+(*itWid)->h()*2,
                x()+w()/2, y()-h(),
                x()+w()/2, y());
        fl_end_line();
    }
}

int TuileWidget::handle(int event) {
    switch(event) { 
        case FL_MOVE: { 
            if(abs(Fl::event_x()-(x()+m_sync1X))<m_magnetSize) {
                m_overSyncIn=true;
                m_overSyncOut=false;
                highlightSyncInLine();
                return 1;
            }
            else if(abs(Fl::event_x()-(x()+m_sync2X))<m_magnetSize) {
                m_overSyncIn=false;
                m_overSyncOut=true;
                highlightSyncOutLine();
                return 1;
            }
            else if(Fl::event_x()>x()+m_real1X && Fl::event_x()<x()+m_real2X) {
                m_overSyncIn=false;
                m_overSyncOut=false;
                resetHighlight();
                highlightReal();
                cout<<"highlight real"<<endl;
                return 1;
            }
        }break;
        case FL_DRAG: {
            float diffX = Fl::event_x()-m_dragPosX;
            //float diffY = Fl::event_y()-m_dragPosY;
            float pixPerFrame=TreeWidget::getInstance()->getPixelsPerFrame();
            if(!m_connecting) {
                if(m_draggingSyncIn) {
                    m_tuile->setLeftOffset(m_tuile->getLeftOffset()
                                            +diffX/pixPerFrame);
                }
                else if(m_draggingSyncOut) {
                    m_tuile->setRightOffset(m_tuile->getRightOffset()
                                            -diffX/pixPerFrame);
                }
                else {
                    m_tuile->setLeftOffset(m_tuile->getLeftOffset()
                                            -diffX/pixPerFrame);
                    //m_tuile->setLength(m_tuile->getLength()
                    //                    -diffY/pixPerFrame);
                }
            }
            else {
                TreeWidget::getInstance()->testConnection(  this, 
                                                            Fl::event_x(),
                                                            Fl::event_y());
            }
            m_dragPosX=Fl::event_x();
            m_dragPosY=Fl::event_y();
            return 1;
        }break;
        case FL_PUSH: {
            if(Fl::event_state(FL_CTRL|FL_COMMAND)) {
                m_connecting=true;
            }
            else {
                m_connecting=false;
                if(m_overSyncIn) {
                    m_draggingSyncIn=true;
                    m_draggingSyncOut=false;
                }
                else if(m_overSyncOut) {
                    m_draggingSyncIn=false;
                    m_draggingSyncOut=true;
                }
                else {
                    m_draggingSyncIn=false;
                    m_draggingSyncOut=false;
                }
            }
            m_dragPosX=Fl::event_x();
            m_dragPosY=Fl::event_y();
            return 1;
        }break;
        case FL_RELEASE: {
            if(m_connecting) {
                TreeWidget::getInstance()->testConnection(this, 
                                                            Fl::event_x(),
                                                            Fl::event_y(), 
                                                            true);
            }
            m_connecting=false;
        }break;
        case FL_ENTER:
        case FL_FOCUS: {
            return 1;
        }
        case FL_LEAVE: 
        case FL_UNFOCUS: {
            resetHighlight();
            return 1;
        }break;
        default:break;
    }
    return 0;
}

void TuileWidget::drag(const int& dragX, const int& dragY) {


}

void TuileWidget::notify() {
    DEBUG("TuileWidget "<<m_id<<" modified, "
            <<" left offset: "<<m_tuile->getLeftOffset()
            <<" right offset: "<<m_tuile->getRightOffset()
            <<" length: "<<m_tuile->getLength());
    float pixPerFrame=TreeWidget::getInstance()->getPixelsPerFrame();
    size((m_tuile->getLength()
                +max<float>(0, -m_tuile->getLeftOffset())+
                +max<float>(0, -m_tuile->getRightOffset()))
            *pixPerFrame, 
        h());
    m_sync1X=max<float>(0, m_tuile->getLeftOffset())*pixPerFrame;
    m_sync2X=w()-max<float>(0, m_tuile->getRightOffset())*pixPerFrame;
    m_real1X=max<float>(0, -m_tuile->getLeftOffset())*pixPerFrame;
    m_real2X=w()-max<float>(0, -m_tuile->getRightOffset())*pixPerFrame;
    
	this->redraw();
}

void TuileWidget::select() {
    m_selected=true;
    highlightReal();
    if(m_paramWidget) {
        m_paramGroup->setWidget(m_paramWidget);
    }
}

void TuileWidget::deselect() {
    m_selected=false;
    resetHighlight();
}

void TuileWidget::tryForkWithTuile(const std::string& tuileName) {
    DEBUG("in TuileWidget "<<m_name<<" try fork with "<<tuileName);
/*
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        m_parentTuileWidget->replaceChildTuileWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(this);
        seqWidget->setSecondChildWidget(newWidget);
    }
*/
}

void TuileWidget::trySeqWithTuile(const std::string& tuileName) {
    DEBUG("in TuileWidget "<<m_name<<" try seq with "<<tuileName);
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    if(newWidget) {
        tree->createSeqWidget(this, newWidget);
    }
    tree->refreshTuiles();
}

void TuileWidget::tryJoinWithTuile(const std::string& tuileName) {
    DEBUG("in TuileWidget "<<m_name<<" try join with "<<tuileName);
/*
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        m_parentTuileWidget->replaceChildTuileWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(this);
        seqWidget->setSecondChildWidget(newWidget);
    }
*/
}

void TuileWidget::tryLeftSeqWithTuile(const std::string& tuileName) {
    DEBUG("in TuileWidget "<<m_name<<" try left seq with "<<tuileName);
/*
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        m_parentTuileWidget->replaceChildTuileWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(newWidget);
        seqWidget->setSecondChildWidget(this);
    }
*/
}

