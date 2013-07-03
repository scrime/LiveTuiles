/***************************************************************************
 *  AudioOutputWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _AudioOutputWidget_H
#define _AudioOutputWidget_H

#include "TuileWidget.hpp"

class AudioOutputTuile;

class AudioOutputWidget: public TuileWidget {
  public:
	AudioOutputWidget(const std::string& name, AudioOutputTuile*);
	~AudioOutputWidget();

  private:
    
};

#endif

