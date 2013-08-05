/***************************************************************************
 *  TuileWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "TuileWidget.hpp"

#include <iostream>
#include <tuiles/Tuile.hpp>

#include "TreeWidget.hpp"

using namespace std;
using namespace tuiles;

TuileWidget::TuileWidget(Tuile* tuile): 
                                    Observer(),
                                    m_tuile(tuile),
                                    m_muted(false),
                                    m_height(20),
                                    m_sync1X(0), m_sync1Y(0),
                                    m_sync2X(0), m_sync2Y(0),
                                    m_syncWidth(10), m_syncHeight(10),
                                    m_real1X(0), m_real2X(0),
                                    m_sync1Color(FL_FOREGROUND_COLOR), 
                                    m_sync2Color(FL_FOREGROUND_COLOR), 
                                    m_syncLColor(FL_FOREGROUND_COLOR), 
                                    m_rectColor(FL_FOREGROUND_COLOR), 
                                    m_realColor(fl_darker(FL_BACKGROUND_COLOR)),
                                    m_overPart(TUILE_NONE), 
                                    m_draggedPart(TUILE_NONE), 
                                    m_magnetSize(5),
                                    m_measureDiv(32), 
                                    m_backgroundColor(FL_BACKGROUND_COLOR),
                                    m_selected(false), 
                                    m_paramWidget(NULL) {
    if(m_tuile) {
        m_id=m_tuile->getID();
    }
}

TuileWidget::~TuileWidget() {}


void TuileWidget::notify() {
    DEBUG("TuileWidget "<<m_id<<" modified, "
            <<" left offset: "<<m_tuile->getLeftOffset()
            <<" right offset: "<<m_tuile->getRightOffset()
            <<" length: "<<m_tuile->getLength());
    float pixPerFrame=TreeWidget::getInstance()->getPixelsPerFrame();
    m_width=(m_tuile->getLength()
                    +max<float>(0, -m_tuile->getLeftOffset())+
                    +max<float>(0, -m_tuile->getRightOffset()))
                *pixPerFrame;
    m_sync1X=max<float>(0, m_tuile->getLeftOffset())*pixPerFrame;
    m_sync2X=m_width-max<float>(0, m_tuile->getRightOffset())*pixPerFrame;
    m_real1X=max<float>(0, -m_tuile->getLeftOffset())*pixPerFrame;
    m_real2X=m_width-max<float>(0, -m_tuile->getRightOffset())*pixPerFrame;
}

