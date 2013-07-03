/***************************************************************************
 *  MonitorWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _MonitorWidget_H
#define _MonitorWidget_H

#include "TuileWidget.hpp"

class MidiOscMonitorTuile;

class MonitorWidget: public TuileWidget {
  public:
	MonitorWidget(const std::string& name, MidiOscMonitorTuile*);
	~MonitorWidget();

  private:
    
};

#endif

