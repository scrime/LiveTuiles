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
                            OpTuile* opTuile): TuileWidget(opTuile), 
                                                Fl_Group(0, 0, 100, 20),
                                                m_opTuile(opTuile) {
    clip_children(1);
    resizable(NULL);
}

OpWidget::~OpWidget() {}

void OpWidget::refreshChildrenTuileWidgets() {
    while(children()>0) {
        remove(child(0));
    }
    m_childrenTuileWidgets.clear();
    TreeWidget* tree=TreeWidget::getInstance();
    vector<Tuile*>::const_iterator itChild=m_opTuile->getChildren().begin();
    for(; itChild!=m_opTuile->getChildren().end(); ++itChild) {
        TuileWidget* wid = tree->getTuileWidget((*itChild)->getID());
        if(wid) {
            cout<<"added child widget to op"<<endl;
            m_childrenTuileWidgets.push_back(wid);
            add(wid->getWidget());
        }
    }
    notify();
}


void OpWidget::drawExecution(const int& offset) {
    vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild) {
        (*itChild)->drawExecution(offset);
    }
}

bool OpWidget::testMagnetWithTuile(const int& inX, const int& inY,
                                    int& outX, int& outY,     
                                    const std::string& tuileName,
                                    const bool& drop) {
    bool magnetized=false;
    vector<TuileWidget*>::iterator itChWid=m_childrenTuileWidgets.begin();
    for(; itChWid!=m_childrenTuileWidgets.end() && !magnetized; ++itChWid) {
        if((*itChWid)->testMagnetWithTuile(inX, inY, outX, 
                                            outY, tuileName, drop)) {
            magnetized=true;
        }
    }
    return magnetized;
}

