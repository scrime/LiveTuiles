/***************************************************************************
 *  SeqWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _SeqWidget_H
#define _SeqWidget_H

#include "OpWidget.hpp"

namespace tuiles{class SeqTuile;}

class SeqWidget: public OpWidget {
  public:
	SeqWidget(const std::string& name, tuiles::SeqTuile*);
	~SeqWidget();

    virtual void load(xmlNodePtr node);

    void drawComposition();
    virtual void updateChildrenPositions();
    int getSync1Y();
    int getSync2Y();
    virtual void setSync1Y(const int& y);
    virtual void setSync2Y(const int& y);

  private:
    tuiles::SeqTuile* m_seqTuile;
    
};

#endif

