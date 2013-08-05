/***************************************************************************
 *  LoopWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "LoopWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include <Fl/fl_draw.H>

#include <tuiles/LoopTuile.hpp>
#include "TreeWidget.hpp"

#include "TuileWidget.hpp"

using namespace std;
using namespace tuiles;

LoopWidget::LoopWidget(const std::string& name, 
                        LoopTuile* tuile): OpWidget(name, tuile), 
                                            m_loopTuile(tuile){
}

LoopWidget::~LoopWidget() {}

void LoopWidget::drawComposition() {
	fl_color(m_realColor);
	fl_rect(x()+m_real1X, y(), m_real2X-m_real1X, h());
    vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild) {
        (*itChild)->drawComposition();
    }
}

void LoopWidget::drawExecution(const float&) {


}

void LoopWidget::tryAddTuileChild(const std::string& tuileName) {
    TreeWidget* tree = TreeWidget::getInstance();
    TuileWidget* newWidget = tree->createTuileWidget(tuileName);
    if(newWidget) {
        m_loopTuile->setChild(newWidget->getTuile());
    }
    tree->refreshTuiles();
}

int LoopWidget::handle(int event) {
    //if(!Fl_Scroll::handle(event)) {
    if(!Fl_Group::handle(event)) {
        switch(event) { 
            case FL_ENTER:
            case FL_FOCUS: {
                return 1;
            }
            case FL_LEAVE: 
            case FL_UNFOCUS: {
                return 1;
            }break;
            default:break;
        }
        return 0;
    }
    else {
        return 1;
    }
}

void LoopWidget::notify() {
    TuileWidget::notify();
    if(children()>0) {
        resize(x(), child(0)->y(), w(), child(0)->h());
    }
    redraw();
}


