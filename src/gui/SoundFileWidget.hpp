/***************************************************************************
 *  SoundFileWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _SoundFileWidget_H
#define _SoundFileWidget_H

#include "AudioTuileWidget.hpp"

class SoundFileTuile;

class SoundFileWidget: public AudioTuileWidget {
  public:
	SoundFileWidget(const std::string& name, SoundFileTuile*);
	~SoundFileWidget();

    void load();
    void load(xmlNodePtr);

  private:
    SoundFileTuile* m_soundFileTuile;
    
};

#endif

