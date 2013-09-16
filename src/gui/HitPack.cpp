/***************************************************************************
 *  HitPack.cpp
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "HitPack.hpp"

using namespace std;

HitPack::HitPack(int x, int y, int w, int h, const char* t): 
                                                Fl_Group(x,y,w,h,t),
                                                m_internalSpacing(10),
                                                m_externalXSpacing(0),
                                                m_externalYSpacing(0),
                                                m_type(Fl_Pack::HORIZONTAL) {
    resizable(NULL);
    end();
}

HitPack::~HitPack() {}

void HitPack::add(Fl_Widget* widget) {
    Fl_Group::add(widget);
    repositionWidgets();
}

void HitPack::repositionWidgets() {
    if(type()==Fl_Pack::HORIZONTAL) {
        float maxSize=m_externalXSpacing;
        for(int c=0; c<children(); ++c) {
            if(!resizable()) { 
                maxSize+=child(c)->w()+m_internalSpacing;
            }
            else if(!resizable()->contains(child(c))) {
                maxSize+=child(c)->w()+m_internalSpacing;
            }
        }
        int posX=x()+m_externalXSpacing;
        for(int c=0; c<children(); ++c) {
            child(c)->resize(posX, y()+m_externalYSpacing, 
                            child(c)->w(),  
                            h()-2*m_externalYSpacing);
            if(resizable()) {
                if(resizable()->contains(child(c))) {
                    child(c)->resize(posX, y()+m_externalYSpacing, 
                                    w()-maxSize,  
                                    h()-2*m_externalYSpacing);
                }
            }
            posX+=child(c)->w()+m_internalSpacing;
        }
    }
    else {
        float maxSize=m_externalYSpacing;
        for(int c=0; c<children(); ++c) {
            if(!resizable()) { 
                maxSize+=child(c)->h()+m_internalSpacing;
            }
            else if(!resizable()->contains(child(c))) {
                maxSize+=child(c)->h()+m_internalSpacing;
            }
        }
        int posY=y()+m_externalYSpacing;
        for(int c=0; c<children(); ++c) {
            child(c)->resize(x()+m_externalXSpacing, posY, 
                            w()-2*m_externalXSpacing,
                            child(c)->h());
            if(resizable()) {
                if(resizable()->contains(child(c))) {
                    child(c)->resize(x()+m_externalXSpacing, posY, 
                                    w()-2*m_externalXSpacing,
                                    h()-maxSize);
                }
            }
            posY+=child(c)->h()+m_internalSpacing;
        }
    }
}


