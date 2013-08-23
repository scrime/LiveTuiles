/***************************************************************************
 *  AudioInputWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _AudioInputWidget_H
#define _AudioInputWidget_H

#include "AudioTuileWidget.hpp"

class AudioInputTuile;

class AudioInputWidget: public AudioTuileWidget {
  public:
	AudioInputWidget(const std::string& name, AudioInputTuile*);
	~AudioInputWidget();

    void load();
    void load(xmlNodePtr);

  private:
    AudioInputTuile* m_audioInputTuile;
    
};

#endif

