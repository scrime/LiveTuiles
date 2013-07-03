/***************************************************************************
 *  SwitchWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _SwitchWidget_H
#define _SwitchWidget_H

#include "TuileWidget.hpp"

class MidiOscSwitchTuile;

class SwitchWidget: public TuileWidget {
  public:
	SwitchWidget(const std::string& name, MidiOscSwitchTuile*);
	~SwitchWidget();

  private:
    
};

#endif

