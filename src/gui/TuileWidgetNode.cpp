/***************************************************************************
 *  TuileWidgetNode.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "TuileWidgetNode.hpp"

#include <iostream>

#include <tuiles/Tuile.hpp>

using namespace std;
using namespace tuiles;

TuileWidgetNode::TuileWidgetNode(Tuile* tuile): Fl_Scroll(0, 0, 100, 20), 
                                                Observer(),
                                                m_tuile(tuile) {
    if(m_tuile) {
        m_id=m_tuile->getID();
        m_tuile->addObserver(this);
    }
}

TuileWidgetNode::~TuileWidgetNode() {}



