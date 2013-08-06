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
	virtual ~SeqWidget();

    void drawComposition();
    virtual int handle(int event);
    virtual void notify();

  private:
    tuiles::SeqTuile* m_seqTuile;
    
};

#endif

