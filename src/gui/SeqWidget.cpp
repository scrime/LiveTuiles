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
                                SeqTuile* tuile): OpWidget(name, tuile), 
                                                    m_seqTuile(tuile) {
    size(w(), h()*2);
    m_realColor=fl_lighter(FL_BACKGROUND_COLOR);
}

SeqWidget::~SeqWidget() {}

void SeqWidget::drawComposition() {
	fl_color(m_realColor);
	fl_rect(x()+m_real1X, y(), m_real2X-m_real1X, h());
    vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild) {
        (*itChild)->drawComposition();
    }
}

void SeqWidget::drawExecution(const float& alpha) {

}

int SeqWidget::handle(int event) {
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
            case FL_PUSH: {
                return 1;
            }break;
            case FL_DRAG: {
                return 1;
            }break;
            case FL_RELEASE: {

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

void SeqWidget::setFirstChildWidget(TuileWidget* child) {
    m_seqTuile->setFirstChild(child->getTuile());
    DEBUG("in SeqWidget, set first child to "<<child->getID());
}

void SeqWidget::setSecondChildWidget(TuileWidget* child) {
    m_seqTuile->setSecondChild(child->getTuile());
    DEBUG("in SeqWidget, set second child to "<<child->getID());
}

void SeqWidget::notify() {
    TuileWidget::notify();
    float pixPerFrame=TreeWidget::getInstance()->getPixelsPerFrame();
    int minPosY=numeric_limits<int>::max();
    int maxPosY=0;
    int childID=0;
    vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
    //set the positions of child widgets
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild, ++childID) {
        if(*itChild) {
            (*itChild)->notify();
            float childPos=-m_seqTuile->getChildPositionAtPos(childID, 0);
            childPos=min<float>(childPos, 
                            childPos+(*itChild)->getTuile()->getLeftOffset());
            Fl_Widget* wid = (*itChild)->getWidget();
            wid->resize(x()+m_real1X+childPos*pixPerFrame, 
                                            wid->y(),
                                            wid->w(),
                                            wid->h());
            if(wid->y()<minPosY) {
                minPosY=wid->y();
            }
            if(wid->y()+wid->h()>maxPosY) {
                maxPosY=wid->y()+wid->h();
            }
        }
    }
    //resize and reposition according to the children
    Fl_Widget::resize(x(), minPosY, m_width, maxPosY-minPosY);

    //set children sync Y positions
    if(m_childrenTuileWidgets.size()>=2) {
        m_childrenTuileWidgets[1]
            ->setSync1Y((m_childrenTuileWidgets[0]->getWidget()->y() 
                            + m_childrenTuileWidgets[0]->getWidget()->h()/2)
                        -(m_childrenTuileWidgets[1]->getWidget()->y() 
                            + m_childrenTuileWidgets[1]->getWidget()->h()/2));
    }
    redraw();
}



