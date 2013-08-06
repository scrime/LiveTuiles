/***************************************************************************
 *  ConnectionWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _ConnectionWidget_H
#define _ConnectionWidget_H

#include <FL/Fl_Widget.H>

#include "AudioTuileWidget.hpp"

class ConnectionWidget: public Fl_Widget {
    public:
        ConnectionWidget(AudioTuileWidget*, AudioTuileWidget*);
        virtual ~ConnectionWidget();
        inline void draw(){}
        void drawConnection();
        int handle(int);
        void update();

    protected:
        AudioTuileWidget* m_fromWidget;
        AudioTuileWidget* m_toWidget;
        Fl_Color m_color;
};

#endif

