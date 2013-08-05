/***************************************************************************
 *  LeafTuileWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "LeafTuileWidget.hpp"

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

LeafTuileWidget::LeafTuileWidget(const std::string& name, 
                                Tuile* tuile):TuileWidget(tuile),
                                                Fl_Widget(0, 0, 100, 20),
                                                m_name(name) {
    m_sync2X=w();
    m_real2X=w();
    type(0);
}

LeafTuileWidget::~LeafTuileWidget() {}

void LeafTuileWidget::update() {

}

void LeafTuileWidget::resetHighlight() {
    m_sync1Color=FL_FOREGROUND_COLOR;
    m_sync2Color=FL_FOREGROUND_COLOR;
    m_syncLColor=FL_FOREGROUND_COLOR;
    highlightReal(false);
}

void LeafTuileWidget::highlightSyncInLine() {
    m_sync1Color=fl_lighter(FL_BACKGROUND_COLOR);
    m_syncLColor=fl_lighter(FL_BACKGROUND_COLOR);
    highlightReal(true);
}

void LeafTuileWidget::highlightSyncOutLine() {
    m_sync2Color=fl_lighter(FL_BACKGROUND_COLOR);
    m_syncLColor=fl_lighter(FL_BACKGROUND_COLOR);
    highlightReal(false);
}

void LeafTuileWidget::highlightReal(bool high) {
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
    TreeWidget::getInstance()->redraw();
}

void LeafTuileWidget::drawComposition() {
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
	fl_line(x()+m_sync1X, y()+h()/2+m_sync1Y, 
            x()+m_sync1X, y()+h()/2);
    fl_arc(x()+m_sync1X-m_syncWidth/2, y()+h()/2+m_sync1Y-m_syncHeight/2, 
            m_syncWidth, m_syncHeight, -90, 90); 
	fl_line(x()+m_sync1X, y()+h()/2+m_sync1Y-m_syncHeight/2, 
            x()+m_sync1X, y()+h()/2+m_sync1Y+m_syncHeight/2);
	fl_color(m_sync2Color);
	fl_line(x()+m_sync2X, y()+h()/2+m_sync2Y, 
            x()+m_sync2X, y()+h()/2);
    fl_arc(x()+m_sync2X-m_syncWidth/2, y()+h()/2+m_sync2Y-m_syncHeight/2, 
            m_syncWidth, m_syncHeight, 90, 270); 
	fl_line(x()+m_sync2X, y()+h()/2+m_sync2Y-m_syncHeight/2, 
            x()+m_sync2X, y()+h()/2+m_sync2Y+m_syncHeight/2);

	//name
    fl_color(FL_WHITE);
	fl_font(FL_HELVETICA_ITALIC, 10);
	fl_draw(m_name.c_str(), x()+m_real1X+2, y()+h()-2);
}

void LeafTuileWidget::drawExecution(const float&) {

}

int LeafTuileWidget::handle(int event) {
    switch(event) { 
        case FL_MOVE: { 
            if(abs(Fl::event_x()-(x()+m_sync2X))<m_magnetSize) {
                m_overSyncIn=false;
                m_overSyncOut=true;
                resetHighlight();
                highlightSyncOutLine();
                return 1;
            }
            else if(Fl::event_x()>x()+m_real1X && Fl::event_x()<x()+m_real2X) {
                m_overSyncIn=true;
                m_overSyncOut=false;
                resetHighlight();
                highlightSyncInLine();
                highlightReal();
                return 1;
            }
        }break;
        case FL_DRAG: {
            float diffX = Fl::event_x()-m_dragPosX;
            float pixPerFrame=TreeWidget::getInstance()->getPixelsPerFrame();
            if(m_draggingSyncOut) {
                m_tuile->setRightOffset(m_tuile->getRightOffset()
                                        -diffX/pixPerFrame);
            }
            else {
                m_tuile->setLeftOffset(m_tuile->getLeftOffset()
                                        -diffX/pixPerFrame);
                position(x(), Fl::event_y());
            }
            cout<<"dragging tuile "<<m_id<<endl;
            m_dragPosX=Fl::event_x();
            m_dragPosY=Fl::event_y();
            return 1;
        }break;
        case FL_PUSH: {
            if(m_overSyncIn) {
                m_draggingSyncIn=true;
                m_draggingSyncOut=false;
            }
            else if(m_overSyncOut) {
                m_draggingSyncIn=false;
                m_draggingSyncOut=true;
            }
            m_dragPosX=Fl::event_x();
            m_dragPosY=Fl::event_y();
            select();
            return 1;
        }break;
        case FL_RELEASE: {
            m_draggingSyncIn=false;
            m_draggingSyncOut=false;
            return 1;
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

void LeafTuileWidget::drag(const int& dragX, const int& dragY) {


}

void LeafTuileWidget::notify() {
    TuileWidget::notify();
    resize(x(), y(), m_width, h());
	this->redraw();
}

void LeafTuileWidget::select() {
    m_selected=true;
    highlightReal();
    if(m_paramWidget) {
        TuileParamGroup::getInstance()->setWidget(m_paramWidget);
    }
}

void LeafTuileWidget::deselect() {
    m_selected=false;
    resetHighlight();
}

void LeafTuileWidget::tryForkWithTuile(const std::string& tuileName) {
    DEBUG("in LeafTuileWidget "<<m_name<<" try fork with "<<tuileName);
/*
    TreeWidget* tree = TreeWidget::getInstance();
    LeafTuileWidget* newWidget = tree->createLeafTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        m_parentLeafTuileWidget->replaceChildLeafTuileWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(this);
        seqWidget->setSecondChildWidget(newWidget);
    }
*/
}

void LeafTuileWidget::trySeqWithTuile(const std::string& tuileName) {
    DEBUG("in LeafTuileWidget "<<m_name<<" try seq with "<<tuileName);
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    if(newWidget) {
        newWidget->getWidget()->position(x(), y());
        SeqWidget* newSeqWidget = tree->createSeqWidget(this, newWidget);
        newSeqWidget->notify();
    }
    tree->refreshTuiles();
}

void LeafTuileWidget::tryJoinWithTuile(const std::string& tuileName) {
    DEBUG("in LeafTuileWidget "<<m_name<<" try join with "<<tuileName);
/*
    TreeWidget* tree = TreeWidget::getInstance();
    LeafTuileWidget* newWidget = tree->createLeafTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        m_parentLeafTuileWidget->replaceChildLeafTuileWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(this);
        seqWidget->setSecondChildWidget(newWidget);
    }
*/
}

void LeafTuileWidget::tryLeftSeqWithTuile(const std::string& tuileName) {
    DEBUG("in LeafTuileWidget "<<m_name<<" try left seq with "<<tuileName);
/*
    TreeWidget* tree = TreeWidget::getInstance();
    LeafTuileWidget* newWidget = tree->createLeafTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        m_parentLeafTuileWidget->replaceChildLeafTuileWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(newWidget);
        seqWidget->setSecondChildWidget(this);
    }
*/
}

