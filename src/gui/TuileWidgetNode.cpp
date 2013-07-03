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

TuileWidgetNode::TuileWidgetNode(Tuile* tuile): m_parent(NULL), 
                                                m_tuile(tuile) {}

TuileWidgetNode::~TuileWidgetNode() {}

void TuileWidgetNode::removeChildWidget(TuileWidgetNode* node) {
    vector<TuileWidgetNode*>::iterator itChild=m_childrenWidgets.begin();
    for(; itChild!=m_childrenWidgets.end(); ) {
        if((*itChild)->m_id==node->m_id) {
            itChild=m_childrenWidgets.erase(itChild);
        }
        else {
            itChild++;
        }
    }
}

void TuileWidgetNode::replaceChildWidget(TuileWidgetNode* replacedChild, 
                                        TuileWidgetNode* replacingChild) {
    bool found=false;
    vector<TuileWidgetNode*>::iterator itChild=m_childrenWidgets.begin();
    for(; itChild!=m_childrenWidgets.end(); ) {
        if((*itChild)->m_id==replacedChild->m_id) {
            found=true;
            (*itChild)=replacingChild;
            replacingChild->setParent(this);
        }
        else {
            itChild++;
        }
    }
    if(!found) {
        DEBUG("in TuileWidgetNode, child "
                <<replacedChild->m_id<<" not found in widget "<<m_id);
    }
}   

