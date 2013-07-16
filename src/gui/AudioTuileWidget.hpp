/***************************************************************************
 *  AudioInputWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _AudioInputWidget_H
#define _AudioInputWidget_H

#include "TuileWidget.hpp"

class AudioInputTuile;

class AudioInputWidget: public TuileWidget {
  public:
	AudioInputWidget(const std::string& name, AudioInputTuile*);
	~AudioInputWidget();

  private:
    
};

#endif

