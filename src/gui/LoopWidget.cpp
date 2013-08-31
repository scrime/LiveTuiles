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
                                            m_loopTuile(tuile) {
}

LoopWidget::~LoopWidget() {}

void LoopWidget::drawComposition() {
	fl_color(fl_lighter(FL_RED));
    fl_line_style(FL_SOLID, 3);
	fl_rect(x(), y(), w(), h());
    fl_line_style(0);
    vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild) {
        (*itChild)->drawComposition();
    }
}

void LoopWidget::drawExecution(const int& offset) {
    int maxPosX=TreeWidget::getInstance()->x()
                    +TreeWidget::getInstance()->w();
                    //+TreeWidget::getInstance()->xposition();
    int posX=offset;
    while(posX<maxPosX) {
        vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
        for(; itChild!=m_childrenTuileWidgets.end(); ++itChild) {
            (*itChild)->drawExecution(posX);
        }
        posX+=m_sync2X-m_sync1X;
    }
}

void LoopWidget::notifyUpdate() {
    TuileWidget::notifyUpdate();
    if(m_childrenTuileWidgets.size()>=1) {
        m_childrenTuileWidgets[0]->notifyUpdate();
        float pixPerFrame=TreeWidget::getInstance()->getPixelsPerFrame();
        float childPos=-m_loopTuile->getChildPositionAtPos(0, 0);
        Fl_Widget* wid = m_childrenTuileWidgets[0]->getWidget();
        wid->resize(x()+childPos*pixPerFrame, 
                    wid->y(),
                    wid->w(),
                    wid->h());
        Fl_Widget::resize(x(), 
                            m_childrenTuileWidgets[0]->getWidget()->y(), 
                            m_width, 
                            m_childrenTuileWidgets[0]->getWidget()->h());
    }
    redraw();
}


bool LoopWidget::testMagnetWithTuile(const int& inX, const int& inY,
                                    int& outX, int& outY,     
                                    const std::string& tuileName,
                                    const bool& drop) {
    if(OpWidget::testMagnetWithTuile(inX,inY,outX,outY,tuileName,drop)) {
        return true;
    }
    if(inY>y()-m_magnetSize && inY<y()+h()+m_magnetSize) {
        if(inX>x()-m_magnetSize && inX<x()+w()+m_magnetSize) {
            outX=x();
            outY=y();
            if(drop) {
                TreeWidget* tree = TreeWidget::getInstance();
                TuileWidget* newWidget = tree->createTuileWidget(tuileName);
                if(newWidget) {
                    newWidget->getTuile()->setName(tuileName);
                    newWidget->load();
                    newWidget->getWidget()->position(x(), y());
                    m_loopTuile->setChild(newWidget->getTuile());
                    tree->refreshChildrenTuileWidgets();
                }
                tree->refreshTuiles();
            }
            return true;
        }
    }
    return false;
}

void LoopWidget::load(xmlNodePtr node) {
    TuileWidget::load(node);
    xmlNodePtr childNode;
    for(childNode= node->children; childNode; 
            childNode= childNode->next) {
        if(childNode->type == XML_ELEMENT_NODE) {
            TuileWidget* newWidget = 
                TreeWidget::getInstance()
                    ->createTuileWidget(string((const char*)childNode->name));
            if(newWidget) {
                newWidget->load(childNode);
                m_loopTuile->setChild(newWidget->getTuile());
            }
        }
    }
}

int LoopWidget::getSync1Y() {
    if(m_childrenTuileWidgets.size()>0) {
        return m_childrenTuileWidgets[0]->getSync1Y();
    }
    else {
        return m_sync1Y;
    }
}

int LoopWidget::getSync2Y() { 
    if(m_childrenTuileWidgets.size()>0) {
        return m_childrenTuileWidgets[0]->getSync2Y();
    }
    else {
        return m_sync2Y;
    }
}

void LoopWidget::setSync1Y(const int& sync1Y) {
    if(m_childrenTuileWidgets.size()>0) {
        m_childrenTuileWidgets[0]->setSync1Y(sync1Y);
    }
}

void LoopWidget::setSync2Y(const int& sync2Y) {
    if(m_childrenTuileWidgets.size()>0) {
        m_childrenTuileWidgets[0]->setSync2Y(sync2Y);
    }
}
