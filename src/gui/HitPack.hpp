/***************************************************************************
 *  HitPack.hpp
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef _HitPack_H
#define _HitPack_H
 
#include <algorithm>
#include <vector>
#include <iostream>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Pack.H>

class HitPack : public Fl_Group {
    public:
        HitPack(int, int, int, int, const char*);
        virtual ~HitPack();

        void add(Fl_Widget*);
        inline void internalSpacing(int s){m_internalSpacing=s;}
        inline void externalXSpacing(int s){m_externalXSpacing=s;}
        inline void externalYSpacing(int s){m_externalYSpacing=s;}
        inline void resize(int x, int y, int w, int h) { 
            Fl_Widget::resize(x,y,w,h);
            repositionWidgets();
        }
        inline int type(){return m_type;}
        inline void type(int t){m_type=t;}
        void repositionWidgets();

    protected:
        int m_internalSpacing;
        int m_externalXSpacing;
        int m_externalYSpacing;
        std::vector<float> m_initWidths;
        std::vector<float> m_initHeights;
        int m_type;
};

#endif
