/***************************************************************************
 *  ScrollZoomWidget.cpp
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "ScrollZoomWidget.hpp"

#include <iostream>
#include <Fl/fl_draw.H>

#include "TreeWidget.hpp"

using namespace std;

ScrollZoomWidget::ScrollZoomWidget(int x, int y, 
                                    int w, int h, 
                                    const char* label): 
                                        Fl_Widget(x, y, w, h, label),
                                        m_treeZoomX(1), 
                                        m_treeZoomY(1),
                                        m_panelPosX(0),
                                        m_panelPosY(0),
                                        m_panelWidth(w),
                                        m_panelHeight(h),
                                        m_displayedRatio(1),
                                        m_minPanelZoom(0.1) { 
    box(FL_DOWN_BOX);
    color(FL_BACKGROUND2_COLOR);
}

ScrollZoomWidget::~ScrollZoomWidget() {}

void ScrollZoomWidget::draw() {
    //draw the background
    draw_box();

    TreeWidget* tree = TreeWidget::getInstance();

    //for each tile in the tree widget, draw a rectangle 
    TreeWidget* man = TreeWidget::getInstance();
    map<unsigned int, TuileWidget*>::const_iterator itWid = 
                                    man->getTuileWidgets().begin();
    fl_color(FL_BLACK);
    for(; itWid!=man->getTuileWidgets().end(); ++itWid) {
        fl_rectf(x()+m_treeZoomX*itWid->second->getTuilePosX(), 
                 y()+m_treeZoomY*itWid->second->getTuilePosY(), 
                 max<float>(1,m_treeZoomX*itWid->second->getTuileLength()), 
                 max<float>(1,m_treeZoomY*itWid->second->getTuileHeight()));
    }

    //draw the zoom panel
    fl_draw_box(FL_DOWN_FRAME, 
                x()+m_panelPosX, y()+m_panelPosY, 
                m_panelWidth, m_panelHeight, FL_BACKGROUND2_COLOR);
}

void ScrollZoomWidget::update() {
    TreeWidget* tree = TreeWidget::getInstance();
    m_displayedRatio=float(tree->w())/float(tree->h());
    float treeRatio=tree->getTotalWidth()/tree->getTotalHeight();
    m_treeZoomY=float(h())/tree->getTotalHeight();
    m_treeZoomX=float(h()*treeRatio)/tree->getTotalWidth();
    m_panelHeight=(float(tree->h())/tree->getZoom())*m_treeZoomY;
    m_panelWidth=m_panelHeight*m_displayedRatio;
    m_panelPosX=tree->getScrollX()*m_treeZoomX;
    m_panelPosY=tree->getScrollY()*m_treeZoomY;
    redraw();
}

int ScrollZoomWidget::handle(int event) {
    switch(event) { 
        case FL_MOVE: {
            return 1; 
        }break; 
        case FL_PUSH: { 
            if(Fl::event_x()>x()+m_panelPosX 
                    && Fl::event_x()<x()+m_panelPosX+m_panelWidth
                    && Fl::event_y()>y()+m_panelPosY 
                    && Fl::event_y()<y()+m_panelPosY+m_panelHeight) {
                m_dragging=true;
                m_dragX=Fl::event_x();
                m_dragY=Fl::event_y();
                return 1;
            }
        }break;
        case FL_RELEASE: {
            m_dragging=false;
            return 1;
        }break;
        case FL_MOUSEWHEEL: {
            if(Fl::event_x()>x()
                    && Fl::event_x()<x()+w()
                    && Fl::event_y()>y() 
                    && Fl::event_y()<y()+h()) {
            
                TreeWidget* tree = TreeWidget::getInstance();
                float newPanelZoom=tree->getZoom()+
                                    float(Fl::event_dy())*0.00001;
                if(newPanelZoom>1) {
                    newPanelZoom=1;
                }
                else if(newPanelZoom<0) {
                    newPanelZoom=0;
                }
                tree->zoom(newPanelZoom);
                updatePanelPosition(m_panelPosX, m_panelPosY);
                redraw();
                return 1;
            }
        }break;
        case FL_DRAG: {
            if(m_dragging) {
                int newPanelPosX=m_panelPosX+(Fl::event_x()-m_dragX);
                int newPanelPosY=m_panelPosY+(Fl::event_y()-m_dragY);
                updatePanelPosition(newPanelPosX, newPanelPosY);
                m_dragX=Fl::event_x();
                m_dragY=Fl::event_y();
                redraw();
                return 1;
            }
        }break;
        case FL_FOCUS: 
        case FL_ENTER: {
            return 1;
        }break;
        case FL_UNFOCUS: 
        case FL_LEAVE: {
            return 1;
        }break;
        default:break;
     }
    return 0;
}

void ScrollZoomWidget::updatePanelPosition(const int& newPanelPosX, 
                                            const int& newPanelPosY) {
    if(newPanelPosX<0) {
        m_panelPosX=0;
    }
    else if(newPanelPosX+m_panelWidth>w()) {
        m_panelPosX=w()-m_panelWidth;
    }
    else {
        m_panelPosX=newPanelPosX;
    }
    if(newPanelPosY<0) {
        m_panelPosY=0;
    }
    else if(newPanelPosY+m_panelHeight>h()) {
        m_panelPosY=h()-m_panelHeight;
    }
    else {
        m_panelPosY=newPanelPosY;
    }
    TreeWidget* tree = TreeWidget::getInstance();
    tree->scroll(float(m_panelPosX)/m_treeZoomX,
                    float(m_panelPosY)/m_treeZoomY);
}

