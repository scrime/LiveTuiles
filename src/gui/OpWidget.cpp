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
#include <Fl/fl_draw.H>

#include <tuiles/SeqTuile.hpp>

#include "TreeWidget.hpp"

using namespace std;
using namespace tuiles;

SeqWidget::SeqWidget(const std::string& name, 
                                SeqTuile* tuile): TuileWidget(name, tuile), 
                                                    m_seqTuile(tuile) {
    size(w(), h()*2);
    m_realColor=fl_lighter(FL_BACKGROUND_COLOR);
}

SeqWidget::~SeqWidget() {}

void SeqWidget::drawComposition() {
	fl_color(m_realColor);
	fl_rect(x()+m_real1X, y(), m_real2X-m_real1X, h());
    vector<TuileWidgetNode*>::iterator itChild=m_childrenTuileWidgets.begin();
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild) {
        (*itChild)->drawComposition();
    }
}

void SeqWidget::drawExecution(const float& alpha) {

}

int SeqWidget::handle(int event) {
    if(!Fl_Scroll::handle(event)) {
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

void SeqWidget::setFirstChildWidget(TuileWidgetNode* child) {
    m_seqTuile->setFirstChild(child->getTuile());
    setChildTuileWidget(0, child);
    notify();
    DEBUG("in SeqWidget, set first child to "<<child->getID());
}

void SeqWidget::setSecondChildWidget(TuileWidgetNode* child) {
    m_seqTuile->setSecondChild(child->getTuile());
    setChildTuileWidget(1, child);
    notify();
    DEBUG("in SeqWidget, set second child to "<<child->getID());
}

void SeqWidget::notify() {
    TuileWidget::notify();
    float pixPerFrame=TreeWidget::getInstance()->getPixelsPerFrame();
    int height=0;
    int childID=0;
    vector<TuileWidgetNode*>::iterator itChild=m_childrenTuileWidgets.begin();
    //set the positions of child widgets
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild, ++childID) {
        if(*itChild) {
            float childPos=-m_seqTuile->getChildPositionAtPos(childID,0);
            childPos=min<float>(childPos, 
                            childPos+(*itChild)->getTuile()->getLeftOffset());
            (*itChild)->position(x()+m_real1X+childPos*pixPerFrame, y()+height);
            height+=(*itChild)->h();
        }
    }
    //resize according to total height of the children
    size(w(), height);
    redraw();
}



