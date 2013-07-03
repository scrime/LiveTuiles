/***************************************************************************
 *  SeqWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "SeqWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include <tuiles/SeqTuile.hpp>

using namespace std;
using namespace tuiles;

SeqWidget::SeqWidget(const std::string& name, 
                        SeqTuile* tuile): TuileWidget(name, tuile), 
                                            m_seqTuile(tuile) {}

SeqWidget::~SeqWidget() {}

void SeqWidget::draw() {
    if(m_childrenWidgets.size()==2) {
        m_childrenWidgets[0]->draw();
        m_childrenWidgets[1]->draw();
    }
}

void SeqWidget::drawExec(const float& alpha) {


}

void SeqWidget::setFirstChildWidget(TuileWidgetNode* child) {
    m_seqTuile->setFirstChild(child->getTuile());
    if(m_childrenWidgets.size()==0) {
        m_childrenWidgets.push_back(NULL); 
    }
    m_childrenWidgets[0]=child;
    child->setParent(this);
}

void SeqWidget::setSecondChildWidget(TuileWidgetNode* child) {
    m_seqTuile->setSecondChild(child->getTuile());
    while(m_childrenWidgets.size()<2) {
        m_childrenWidgets.push_back(child); 
    }
    m_childrenWidgets[1]=child;
    child->setParent(this);
}



