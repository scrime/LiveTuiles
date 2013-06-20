/***************************************************************************
 *            HitPack.cpp
 *
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *
 *  This file is part of libTuiles
 ****************************************************************************/

#include "HitPack.hpp"

using namespace std;

HitPack::HitPack(int x, int y, int w, int h, const char* t): 
                                                        Fl_Pack(x,y,w,h,t),
                                                        m_internalSpacing(10),
                                                        m_externalSpacing(0), 
                                                        m_initWidth(w),
                                                        m_initHeight(h) { 
    end();
}

HitPack::~HitPack() {}


void HitPack::draw() {
	//box
	fl_draw_box(box(), x(), y(), w(), h(), color());

    repositionWidgets();
	//children
	draw_children();
}

void HitPack::add(Fl_Widget* widget) {
    Fl_Pack::add(widget);
}


void HitPack::repositionWidgets() {
    if(type()==Fl_Pack::HORIZONTAL) {
        int posX=x()+m_externalSpacing;
        for(int c=0;c<children();++c) {
            child(c)->resize(posX, y()+m_externalSpacing, 
                            child(c)->w(), h()-2*m_externalSpacing);
            posX+=child(c)->w()+m_internalSpacing;
        }
        size(max<int>(posX-x(), m_initWidth), h());
    }
    else {
        int posY=y()+m_externalSpacing;
        for(int c=0;c<children();++c) {
            child(c)->resize(x()+m_externalSpacing, posY, 
                            w()-2*m_externalSpacing, child(c)->h());
            posY+=child(c)->h()+m_internalSpacing;
        }
        size(w(), max<int>(posY-y(), m_initHeight));
    }
}


