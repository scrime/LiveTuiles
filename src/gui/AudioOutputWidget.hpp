/***************************************************************************
 *  AudioOutputWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _AudioOutputWidget_H
#define _AudioOutputWidget_H

#include "AudioTuileWidget.hpp"

class AudioOutputTuile;

class AudioOutputWidget: public AudioTuileWidget {
  public:
	AudioOutputWidget(const std::string& name, AudioOutputTuile*);
	~AudioOutputWidget();

    void load();
    void load(xmlNodePtr);
    int handle(int event);

  private:
    AudioOutputTuile* m_audioOutputTuile;
};

#endif

