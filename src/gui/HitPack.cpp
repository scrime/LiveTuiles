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
                                                        m_externalSpacing(0) { 
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
    m_initWidths.push_back(widget->w());
    m_initHeights.push_back(widget->h());
    Fl_Group::add(widget);
}

void HitPack::repositionWidgets() {
    float maxSize=m_externalSpacing;
    if(type()==Fl_Pack::HORIZONTAL) {
        for(int c=0; c<children(); ++c) {
            if(!resizable()) { 
                maxSize+=child(c)->w()+m_internalSpacing;
            }
            else if(!resizable()->contains(child(c))) {
                maxSize+=child(c)->w()+m_internalSpacing;
            }
        }
        int posX=x()+m_externalSpacing;
        for(int c=0; c<children(); ++c) {
            if(resizable() && resizable()->contains(child(c))) {
                child(c)->resize(posX, y()+m_externalSpacing, 
                                w()-maxSize,  
                                h()-2*m_externalSpacing);
            }
            else {
                child(c)->resize(posX, y()+m_externalSpacing, 
                                child(c)->w(),  
                                h()-2*m_externalSpacing);
            }
            posX+=child(c)->w()+m_internalSpacing;
        }
    }
    else {
        for(int c=0; c<children(); ++c) {
            if(!resizable()) { 
                maxSize+=child(c)->h()+m_internalSpacing;
            }
            else if(!resizable()->contains(child(c))) {
                maxSize+=child(c)->h()+m_internalSpacing;
            }
        }
        int posY=y()+m_externalSpacing;
        for(int c=0; c<children(); ++c) {
            if(resizable() && resizable()->contains(child(c))) {
                child(c)->resize(x()+m_externalSpacing, posY, 
                                w()-2*m_externalSpacing,
                                h()-maxSize);
            }
            else {
                child(c)->resize(x()+m_externalSpacing, posY, 
                                w()-2*m_externalSpacing,
                                child(c)->h());
            }
            posY+=child(c)->h()+m_internalSpacing;
        }
    }
}


