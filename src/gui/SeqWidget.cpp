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
                        SeqTuile* tuile):   OpWidget(name, tuile), 
                                            m_seqTuile(tuile) {
    size(w(), h()*2);
    m_realColor=fl_lighter(FL_BACKGROUND_COLOR);
}

SeqWidget::~SeqWidget() {}

void SeqWidget::drawComposition() {
    fl_line_style(0);
	fl_color(fl_darker(FL_BACKGROUND_COLOR));
	fl_rect(x(), y(), w(), h());
    vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild) {
        (*itChild)->drawComposition();
    }
}

/*
int SeqWidget::handle(int event) {
    Fl_Group::handle(event);
    return 1;


    if(!Fl_Group::handle(event)) {
        //TODO allow clicking on borders
        switch(event) { 
            case FL_ENTER:
            case FL_FOCUS: {
                return 0;
            }break;
            case FL_LEAVE: 
            case FL_UNFOCUS: {
                return 0;
            }break;
            case FL_PUSH: {
                return 0;
            }break;
            case FL_DRAG: {
                return 0;
            }break;
            case FL_RELEASE: {
                return 0;
            }break;
            default:break;
        }
        return 0;
    }
    else {
        return 1;
    }
}
*/

void SeqWidget::notifyUpdate() {
    TuileWidget::notifyUpdate();
    float pixPerFrame=TreeWidget::getInstance()->getPixelsPerFrame();
    int minPosY=numeric_limits<int>::max();
    int maxPosY=0;
    int childID=0;
    vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
    //set the positions of child widgets
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild, ++childID) {
        if(*itChild) {
            (*itChild)->notifyUpdate();
            float childPos=-m_seqTuile->getChildPositionAtPos(childID, 0);
            //childPos=min<float>(childPos, 
            //                childPos+(*itChild)->getTuile()->getLeftOffset());
            Fl_Widget* wid = (*itChild)->getWidget();
            wid->resize(x()+childPos*pixPerFrame, 
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
            ->setSync1Y(m_childrenTuileWidgets[0]->getSync2Y()
                        -m_childrenTuileWidgets[1]->getSync1Y()); 
    }
    redraw();
}

void SeqWidget::load(xmlNodePtr node) {
    TuileWidget::load(node);
    xmlNodePtr childNode;
    int counter=0;
    for(childNode= node->children; childNode; 
            childNode= childNode->next) {
        if(childNode->type == XML_ELEMENT_NODE) {
            TuileWidget* newWidget = 
                TreeWidget::getInstance()
                    ->createTuileWidget(string((const char*)childNode->name));
            if(newWidget) {
                newWidget->load(childNode);
                if(counter==0) {
                    m_seqTuile->setFirstChild(newWidget->getTuile());
                }
                else if(counter==1) {
                    m_seqTuile->setSecondChild(newWidget->getTuile());
                }
                counter++;
            }
        }
    }
}

void SeqWidget::setSync1Y(const int& sync1Y) {
    if(m_childrenTuileWidgets.size()>1) {
        m_childrenTuileWidgets[0]->setSync1Y(sync1Y);
    }
}

void SeqWidget::setSync2Y(const int& sync2Y) {
    if(m_childrenTuileWidgets.size()>1) {
        m_childrenTuileWidgets[1]->setSync2Y(sync2Y);
    }
}

int SeqWidget::getSync1Y() {
    if(m_childrenTuileWidgets.size()>1) {
        return m_childrenTuileWidgets[0]->getSync1Y();
    }
    else {
        return m_sync1Y;
    }
}

int SeqWidget::getSync2Y() { 
    if(m_childrenTuileWidgets.size()>1) {
        return m_childrenTuileWidgets[1]->getSync2Y();
    }
    else {
        return m_sync2Y;
    }
}


