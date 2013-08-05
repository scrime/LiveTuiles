/***************************************************************************
 *  LoopWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _LoopWidget_H
#define _LoopWidget_H

#include "OpWidget.hpp"

namespace tuiles{class LoopTuile;}

class LoopWidget: public OpWidget {
  public:
	LoopWidget(const std::string& name, tuiles::LoopTuile*);
	~LoopWidget();

    void drawComposition();
    void drawExecution(const float&);
    virtual void tryAddTuileChild(const std::string& tuileName);
    int handle(int);

    void notify();

  private:
    tuiles::LoopTuile* m_loopTuile;
};

#endif

