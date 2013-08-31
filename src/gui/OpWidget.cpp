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
    end();
}

OpWidget::~OpWidget() {}

void OpWidget::refreshChildrenTuileWidgets() {
    m_childrenTuileWidgets.clear();
    TreeWidget* tree=TreeWidget::getInstance();
    vector<Tuile*>::const_iterator itChild=m_opTuile->getChildren().begin();
    for(; itChild!=m_opTuile->getChildren().end(); ++itChild) {
        TuileWidget* wid = tree->getTuileWidget((*itChild)->getID());
        if(wid) {
            cout<<"added child widget to op"<<endl;
            m_childrenTuileWidgets.push_back(wid);
        }
    }
    notifyUpdate();
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

void OpWidget::save(xmlNodePtr parentNode) {
    xmlNodePtr node = m_opTuile->save(parentNode);
    for(unsigned int c=0; c<m_childrenTuileWidgets.size(); ++c) {
        m_childrenTuileWidgets[c]->save(node);
    }
}

void OpWidget::load(xmlNodePtr node) {
    TuileWidget::load(node);
    xmlNodePtr childNode;
    for(childNode= node->children; childNode; childNode= childNode->next) {
        if(childNode->type == XML_ELEMENT_NODE) {
            TuileWidget* newWidget = 
                TreeWidget::getInstance()
                    ->createTuileWidget(string((const char*)childNode->name));
            if(newWidget) {
                newWidget->load(childNode);
            }
        }
    }
}

void OpWidget::notifyDelete() {
    TreeWidget::getInstance()->markWidgetForRemoval(this);
    cout<<"op widget delete notified"<<endl;
}

int OpWidget::handle(int event) {
    bool handled=false;
    vector<TuileWidget*>::iterator itChWid=m_childrenTuileWidgets.begin();
    for(; itChWid!=m_childrenTuileWidgets.end() && !handled; ++itChWid) {
        handled=(*itChWid)->getWidget()->handle(event);
    }
    return handled;
}

