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
#include <tuiles/Tuile.hpp>

#include "TuileParamWidget.hpp"
#include "TuileParamGroup.hpp"

using namespace std;


TuileWidget::TuileWidget(const std::string& name, 
                        Tuile* tuile):
								Fl_Widget(0, 0, 100, 20, ""), 
                                m_tuile(tuile),
                                m_name(name),
                                m_muted(false),
								m_height(20),
                                m_sync1X(0), m_sync2X(0),
                                m_sync1Color(FL_FOREGROUND_COLOR), 
                                m_sync2Color(FL_FOREGROUND_COLOR), 
                                m_rectColor(FL_FOREGROUND_COLOR), 
                                m_realColor(fl_darker(FL_BACKGROUND_COLOR)),
								m_clickedBegin(false),m_clickedEnd(false),
								m_clickedMiddle(false),m_magnetSize(5),
								m_measureDiv(32),
								m_backgroundColor(FL_BACKGROUND_COLOR),
                                m_selected(false),
                                m_paramWidget(NULL){}

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

int TuileWidget::handle(int event) {

    return Fl_Widget::handle(event);
}

void TuileWidget::drag(const int& dragX, const int& dragY) {


}

void TuileWidget::refresh(const int& baseX, const int& baseY, 
							const float& pixelsPerBeat, const int& yPos) {
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

