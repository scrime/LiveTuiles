/***************************************************************************
 *  ScrollZoomWidget.h
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef _ScrollZoomWidget_H
#define _ScrollZoomWidget_H
 
#include <FL/Fl.H>
#include <FL/Fl_Widget.H>

class ScrollZoomWidget: public Fl_Widget {
    public:
        ScrollZoomWidget(int x, int y, int w, int h, const char* label);
        ~ScrollZoomWidget();
        void draw();
        int handle(int event);
        void update();

    private:
        void updatePanelPosition(const int&, const int&);

    private:
        float m_totalWidth;
        float m_totalHeight;
        float m_treeZoomX;
        float m_treeZoomY;

        int m_panelPosX;
        int m_panelPosY;
        int m_panelWidth;
        int m_panelHeight;
        float m_displayedPanelZoom;
        float m_displayedRatio;
        float m_minPanelZoom;
        float m_realPanelZoom;
        bool m_dragging;
        int m_dragX;
        int m_dragY;
};

#endif

