/***************************************************************************
 *  OpWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "OpWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>
#include <Fl/fl_draw.H>

#include <tuiles/OpTuile.hpp>

#include "TreeWidget.hpp"

using namespace std;
using namespace tuiles;

OpWidget::OpWidget(const std::string& name, 
                                OpTuile* opTuile): TuileWidget(name, opTuile), 
                                                    m_opTuile(opTuile) {}

OpWidget::~OpWidget() {}

void OpWidget::refreshChildrenTuileWidgets() {
    vector<TuileWidgetNode*>::iterator itWid=m_childrenTuileWidgets.begin();
    for(; itWid!=m_childrenTuileWidgets.end(); ++itWid) {
        remove(*itWid);
    }
    m_childrenTuileWidgets.clear();

    TreeWidget* tree=TreeWidget::getInstance();
    vector<Tuile*>::const_iterator itChild=m_opTuile->getChildren().begin();
    for(; itChild!=m_opTuile->getChildren().end(); ++itChild) {
        TuileWidget* wid = tree->getTuileWidget((*itChild)->getID());
        if(wid) {
            m_childrenTuileWidgets.push_back(wid);
            add(wid);
        }
    }
}




