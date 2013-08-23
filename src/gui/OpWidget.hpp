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

        virtual void save(xmlNodePtr parentNode);
        virtual void load(xmlNodePtr node);
        virtual void notifyDelete();

        inline virtual void draw(){drawComposition();}
        virtual void drawExecution(const int& offset=0);
        void refreshChildrenTuileWidgets();
        inline virtual Fl_Widget* getWidget(){return this;}
        virtual bool testMagnetWithTuile(const int& inX, const int& inY,
                                        int& outX, int& outY,     
                                        const std::string& tuileName,
                                        const bool& drop);
    protected:
        tuiles::OpTuile* m_opTuile;
};

#endif

