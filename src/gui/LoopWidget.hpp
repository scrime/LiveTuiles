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
    void drawExecution(const int&);

    virtual void updateChildrenPositions();
    void load(xmlNodePtr node);
    virtual bool testMagnetWithTuile(const int& inX, const int& inY,
                                    int& outX, int& outY,     
                                    const std::string& tuileName,
                                    const bool& drop);
    virtual void setSync1Y(const int& y);
    virtual void setSync2Y(const int& y);
    int getSync1Y();
    int getSync2Y();

  private:
    tuiles::LoopTuile* m_loopTuile;
};

#endif

