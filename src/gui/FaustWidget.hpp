/***************************************************************************
 *  FaustWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _FaustWidget_H
#define _FaustWidget_H

#include "AudioTuileWidget.hpp"

class FaustTuile;

class FaustWidget: public AudioTuileWidget {
  public:
	FaustWidget(const std::string& name, FaustTuile*);
	~FaustWidget();

  private:
    
};

#endif

