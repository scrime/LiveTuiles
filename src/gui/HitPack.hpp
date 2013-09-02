/***************************************************************************
 *            HitPack.h
 *
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *
 *  This file is part of libTuiles
 ****************************************************************************/

#ifndef _HitPack_H
#define _HitPack_H
 
#include <algorithm>
#include <vector>

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Pack.H>

class HitPack : public Fl_Pack {
  public:
	HitPack(int, int, int, int, const char*);
	virtual ~HitPack();

    void draw();
    void add(Fl_Widget*);
    inline void internalSpacing(int s){m_internalSpacing=s;}
    inline void externalSpacing(int s){m_externalSpacing=s;}
    inline void resize(int x, int y, int w, int h) { 
        Fl_Group::resize(x,y,w,h);
    }

  protected:

    void repositionWidgets();
    int m_internalSpacing;
    int m_externalSpacing;
    std::vector<float> m_initWidths;
    std::vector<float> m_initHeights;

};

#endif
