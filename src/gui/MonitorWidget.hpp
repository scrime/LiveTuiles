/***************************************************************************
 *  FaustWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _FaustWidget_H
#define _FaustWidget_H

#include "TuileWidget.hpp"

class FaustTuile;

class FaustWidget: public TuileWidget {
  public:
	FaustWidget(const std::string& name, FaustTuile*);
	~FaustWidget();

  private:
    
};

#endif

