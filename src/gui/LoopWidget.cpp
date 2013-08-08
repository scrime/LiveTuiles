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
	fl_rect(x()+m_real1X, y(), m_real2X-m_real1X, h());
    fl_line_style(0);
    vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
    for(; itChild!=m_childrenTuileWidgets.end(); ++itChild) {
        (*itChild)->drawComposition();
    }
}

void LoopWidget::drawExecution(const int& offset) {
    int maxPosX=TreeWidget::getInstance()->x()
                    +TreeWidget::getInstance()->w()
                    +TreeWidget::getInstance()->xposition();
    int posX=offset;
    while(posX<maxPosX) {
        vector<TuileWidget*>::iterator itChild=m_childrenTuileWidgets.begin();
        for(; itChild!=m_childrenTuileWidgets.end(); ++itChild) {
            (*itChild)->drawExecution(posX);
        }
        posX+=m_sync2X-m_sync1X;
    }
}

int LoopWidget::handle(int event) {
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
    if(m_childrenTuileWidgets.size()>=1) {
        m_childrenTuileWidgets[0]->notify();
        float pixPerFrame=TreeWidget::getInstance()->getPixelsPerFrame();
        float childPos=-m_loopTuile->getChildPositionAtPos(0, 0);
        childPos=min<float>(childPos, 
            childPos+m_childrenTuileWidgets[0]->getTuile()->getLeftOffset());
        Fl_Widget* wid = m_childrenTuileWidgets[0]->getWidget();
        wid->resize(x()+m_real1X+childPos*pixPerFrame, 
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
        if(inX>x()+m_real1X-m_magnetSize && inX<x()+m_real2X+m_magnetSize) {
            outX=x()+m_real1X;
            outY=y();
            if(drop) {
                TreeWidget* tree = TreeWidget::getInstance();
                TuileWidget* newWidget = tree->createTuileWidget(tuileName);
                if(newWidget) {
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
