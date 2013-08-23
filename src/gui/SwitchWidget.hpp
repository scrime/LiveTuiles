/***************************************************************************
 *  SwitchWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _SwitchWidget_H
#define _SwitchWidget_H

#include "OpWidget.hpp"

class MidiOscSwitchTuile;

class SwitchWidget: public OpWidget {
  public:
	SwitchWidget(const std::string& name, MidiOscSwitchTuile*);
	~SwitchWidget();
    virtual void notifyUpdate();

  private:

    MidiOscSwitchTuile* m_switchTuile;
    
};

#endif

