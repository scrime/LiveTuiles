/***************************************************************************
 *  ConnexionWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _ConnexionWidget_H
#define _ConnexionWidget_H

#include <FL/Fl_Widget.H>

#include "LeafTuileWidget.hpp"

class ConnexionWidget: public Fl_Widget {
    public:
        ConnexionWidget(LeafTuileWidget*, LeafTuileWidget*);
        virtual ~ConnexionWidget();
        void draw();
        int handle(int);
    protected:
        LeafTuileWidget* m_fromWidget;
        LeafTuileWidget* m_toWidget;
};

#endif

