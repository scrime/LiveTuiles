/***************************************************************************
 *  ConnectionWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "ConnectionWidget.hpp"

#include <iostream>
#include <sstream>
#include <math.h>
#include <cassert>
#include <Fl/fl_draw.H>

#include "../audio/AudioTuile.hpp"

#include "TreeWidget.hpp"

using namespace std;
using namespace tuiles;

ConnectionWidget::ConnectionWidget(const unsigned int& id,
                                AudioTuileWidget* from, 
                                AudioTuileWidget* to): Fl_Widget(0, 0, 10, 10),
                                                      m_id(id),
                                                      m_fromWidget(from),  
                                                      m_toWidget(to), 
                                                      m_color(FL_BLUE), 
                                                      m_deleting(false) {
    m_toWidget->getAudioTuile()->addInputTuile(m_fromWidget->getAudioTuile());
    m_toWidget->addConnection(this);
    m_fromWidget->addConnection(this);
}

ConnectionWidget::~ConnectionWidget() {
   m_toWidget->removeConnection(this);
   m_fromWidget->removeConnection(this);
   m_toWidget->getAudioTuile()->removeInputTuile(m_fromWidget->getAudioTuile());
}

void ConnectionWidget::drawConnection() {
    fl_color(m_color);
    fl_line_style(FL_SOLID, 3);
    fl_begin_line();
    fl_curve(m_fromWidget->getCenterReal(), m_fromWidget->y()+m_fromWidget->h(),
        m_fromWidget->getCenterReal(), m_fromWidget->y()+m_fromWidget->h()*2,
        m_toWidget->getCenterReal(), m_toWidget->y()-m_toWidget->h(),
        m_toWidget->getCenterReal(), m_toWidget->y());
    fl_end_line();
    fl_line_style(0);
    if(m_deleting) {
        fl_line(x(), y(), x()+w(), y()+h()); 
        fl_line(x()+w(), y(), x(), y()+h()); 
    }
    else {
        fl_arc(x(), y(), w(), h(), 0, 360);
    }
}

int ConnectionWidget::handle(int event) {
    switch(event) { 
        case FL_MOVE: {
            m_deleting=false;
            if(Fl::event_x()>x() && Fl::event_x()<x()+w() 
                    && Fl::event_y()>y() && Fl::event_y()<y()+h()) {
                m_deleting=true;
                TreeWidget::getInstance()->redraw();
                return 1;
            }
        }break;
        case FL_PUSH: {
            if(m_deleting) {
                TreeWidget::getInstance()->markConnectionForRemoval(this);
                return 1;
            }
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

void ConnectionWidget::save(xmlNodePtr parentNode) {
    xmlNodePtr conNode = xmlNewChild(parentNode, NULL, 
                                        BAD_CAST "Connection", NULL);
    ostringstream oss, oss1;
    oss<<m_fromWidget->getID();
    oss1<<m_toWidget->getID();
    xmlNewProp(conNode, BAD_CAST "from", BAD_CAST oss.str().c_str());
    xmlNewProp(conNode, BAD_CAST "to", BAD_CAST oss1.str().c_str());
}

