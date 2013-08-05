/***************************************************************************
 *  OpWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _OpWidget_H
#define _OpWidget_H

#include <FL/Fl_Group.H>

#include "TuileWidget.hpp"


namespace tuiles{class OpTuile;}

class OpWidget: public TuileWidget, public Fl_Group {
    public:
        OpWidget(const std::string& name, tuiles::OpTuile*);
        virtual ~OpWidget();
        inline virtual void draw(){drawComposition();}
        void refreshChildrenTuileWidgets();
        inline virtual Fl_Widget* getWidget(){return this;}
    protected:
        tuiles::OpTuile* m_opTuile;
};

#endif

