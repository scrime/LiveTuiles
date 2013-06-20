/***************************************************************************
 *  SoundFileWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _SoundFileWidget_H
#define _SoundFileWidget_H

#include "TuileWidget.hpp"

class SoundFileTuile;

class SoundFileWidget: public TuileWidget {
  public:
	SoundFileWidget(const std::string& name, SoundFileTuile*);
	~SoundFileWidget();

  private:
    
};

#endif

