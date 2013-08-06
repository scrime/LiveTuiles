/***************************************************************************
 *  ConnectionWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "ConnectionWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>
#include <Fl/fl_draw.H>

#include "../audio/AudioTuile.hpp"

#include "TreeWidget.hpp"

using namespace std;
using namespace tuiles;

ConnectionWidget::ConnectionWidget(AudioTuileWidget* from, 
                                AudioTuileWidget* to): Fl_Widget(0, 0, 10, 10),
                                                      m_fromWidget(from),  
                                                      m_toWidget(to), 
                                                      m_color(FL_BLUE) {
    m_toWidget->getAudioTuile()->addInputTuile(m_fromWidget->getAudioTuile());
}

ConnectionWidget::~ConnectionWidget() {
    m_toWidget->getAudioTuile()->removeInputTuile(
                                    m_fromWidget->getAudioTuile());
}


void ConnectionWidget::drawConnection() {
    fl_color(m_color);
    fl_begin_line();
    fl_curve(m_fromWidget->getCenterReal(), m_fromWidget->y()+m_fromWidget->h(),
        m_fromWidget->getCenterReal(), m_fromWidget->y()+m_fromWidget->h()*2,
        m_toWidget->getCenterReal(), m_toWidget->y()-m_toWidget->h(),
        m_toWidget->getCenterReal(), m_toWidget->y());
    fl_end_line();
	fl_rect(x(), y(), w(), h());
}

int ConnectionWidget::handle(int event) {
    switch(event) { 
        case FL_ENTER:
        case FL_FOCUS: {
            m_color=fl_lighter(FL_BLUE);
            TreeWidget::getInstance()->redraw();
            return 1;
        }
        case FL_LEAVE: 
        case FL_UNFOCUS: {
            m_color=FL_BLUE;
            TreeWidget::getInstance()->redraw();
            return 1;
        }break;
        case FL_PUSH: {
            return 1;
        }break;
        case FL_DRAG: {
            return 1;
        }break;
        case FL_RELEASE: {
            return 1;
        }break;
        default:break;
    }
    return 0;
}

void ConnectionWidget::update() {
    position(
        (m_fromWidget->getCenterReal()+m_toWidget->getCenterReal())/2-w()/2, 
        (m_fromWidget->y()+m_toWidget->y())/2+h()/2);
}


