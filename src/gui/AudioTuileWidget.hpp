/***************************************************************************
 *  AudioTuileWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _AudioTuileWidget_H
#define _AudioTuileWidget_H

#include "TuileWidget.hpp"

class AudioTuile;

class AudioTuileWidget: public TuileWidget {
  public:
	AudioTuileWidget(const std::string& name, AudioTuile*);
	~AudioTuileWidget();

    virtual void connectToWidget(AudioTuileWidget*);
    inline AudioTuile* getAudioTuile(){return m_audioTuile;}
    
  protected:
    AudioTuile* m_audioTuile;
    
};

#endif

