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
								Fl_Widget(0, 0, 100, 20, ""), 
                                TuileWidgetNode(tuile),
                                m_name(name),
                                m_muted(false),
								m_height(20),
                                m_sync1X(0), m_sync2X(0),
                                m_sync1Color(FL_FOREGROUND_COLOR), 
                                m_sync2Color(FL_FOREGROUND_COLOR), 
                                m_rectColor(FL_FOREGROUND_COLOR), 
                                m_realColor(fl_darker(FL_BACKGROUND_COLOR)),
								m_clickedBegin(false), m_clickedEnd(false),
								m_clickedMiddle(false), m_magnetSize(5),
								m_measureDiv(32), m_tmpConnectionActive(false),
								m_backgroundColor(FL_BACKGROUND_COLOR),
                                m_selected(false),
                                m_paramWidget(NULL) {
    if(m_tuile) {
        m_id=m_tuile->getID();
        DEBUG("in TuileWidget constructor called of "<<name<<" with id "<<m_id);
    }
}

TuileWidget::~TuileWidget() {}

void TuileWidget::update() {

}

void TuileWidget::resetHighlight() {
    m_sync1Color=FL_FOREGROUND_COLOR;
    m_sync2Color=FL_FOREGROUND_COLOR;
    highlightReal(false);
}

void TuileWidget::highlightSyncInLine() {
    m_sync1Color=fl_lighter(FL_FOREGROUND_COLOR);
    highlightReal();
}

void TuileWidget::highlightSyncOutLine() {
    m_sync2Color=fl_lighter(FL_FOREGROUND_COLOR);
    highlightReal();
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

void TuileWidget::draw() {
	//realwindow
	fl_color(m_realColor);
	fl_rectf(x(), y(), w()+1, h());
	fl_color(FL_FOREGROUND_COLOR);
	fl_rect(x(), y(), w()+1, h());

	//syncwindow
	fl_color(m_rectColor);
	fl_line(x()+m_sync1X, y()+h()/2, x()+w()-m_sync2X, y()+h()/2);
	fl_color(m_sync1Color);
	fl_line(x()+m_sync1X, y()-h()/2, x()+m_sync1X, y()+3*h()/2);
	fl_color(m_sync2Color);
	fl_line(x()+w()-m_sync2X, y()-h()/2, x()+w()-m_sync2X, y()+3*h()/2);

	//name
    fl_color(FL_WHITE);
	fl_font(FL_HELVETICA_ITALIC, 10);
	fl_draw(m_name.c_str(), x()+2, y()+h()-2);
}

void TuileWidget::drawConnections() {
    fl_color(FL_BLUE);
    if(m_tmpConnectionActive) {
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
            if(Fl::event_x()<x()+m_magnetSize) {
                //m_overWidget=*itWidget;
                //m_overWidgetPart=0;
                highlightSyncInLine();
                return 1;
            }
            else if(Fl::event_x()>x()+w()-m_magnetSize) {
                //m_overWidget=*itWidget;
                //m_overWidgetPart=2;
                highlightSyncOutLine();
                return 1;
            }
            else {
                //m_overWidget=*itWidget;
                //m_overWidgetPart=1;
                highlightReal();
                return 1;
            }
        }break;
        case FL_DRAG: {
            if(!m_tmpConnectionActive) {
                position(x(), y()+(Fl::event_y()-m_dragPosY));
            }
            m_dragPosX=Fl::event_x();
            m_dragPosY=Fl::event_y();
            return 1;
        }break;
        case FL_PUSH: {
            if(Fl::event_state(FL_CTRL|FL_COMMAND)) {
                m_tmpConnectionActive=true;
            }
            else {
                m_tmpConnectionActive=false;
            }
            m_dragPosX=Fl::event_x();
            m_dragPosY=Fl::event_y();
            return 1;
        }break;
        case FL_RELEASE: {
            m_tmpConnectionActive=false;
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
    return Fl_Widget::handle(event);
}

void TuileWidget::drag(const int& dragX, const int& dragY) {


}

void TuileWidget::refresh(const int& baseX, const int& baseY, 
							const float& pixelsPerBeat, const int& yPos) {
/*
	if(m_tuile) {
		float pos = float(m_tuile->getAbsolutePosition());
        cout<<"tuile "<<m_id<<" pos "<<pos<<endl;
		float length = float(m_tuile->getLength());
		resize(floor(float(baseX) - pos * pixelsPerBeat), 
                floor(float(baseY) + yPos*m_height), 
                floor(length*pixelsPerBeat), m_height);
		m_sync1X = floor(m_tuile->getLeftOffset() * pixelsPerBeat);
		m_sync2X = floor(m_tuile->getRightOffset() * pixelsPerBeat);
        m_muted=m_tuile->isMuted();
        highlightReal(m_selected);
	}
	this->redraw();
*/
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
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        m_parent->replaceChildWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(this);
        seqWidget->setSecondChildWidget(newWidget);
    }
}

void TuileWidget::trySeqWithTuile(const std::string& tuileName) {
    DEBUG("in TuileWidget "<<m_name<<" try seq with "<<tuileName);
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        DEBUG("in TuileWidget "<<m_name<<" seq with "<<tuileName<<" worked");
        m_parent->replaceChildWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(this);
        seqWidget->setSecondChildWidget(newWidget);
    }

}

void TuileWidget::tryJoinWithTuile(const std::string& tuileName) {
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        m_parent->replaceChildWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(this);
        seqWidget->setSecondChildWidget(newWidget);
    }

}

void TuileWidget::tryLeftSeqWithTuile(const std::string& tuileName) {
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    SeqWidget* seqWidget = tree->createSeqWidget();
    if(newWidget && seqWidget) {
        m_parent->replaceChildWidget(this, seqWidget);
        seqWidget->setFirstChildWidget(newWidget);
        seqWidget->setSecondChildWidget(this);
    }

}

