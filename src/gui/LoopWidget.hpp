/***************************************************************************
 *  LoopWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _LoopWidget_H
#define _LoopWidget_H

#include "TuileWidget.hpp"

namespace tuiles{class LoopTuile;}

class LoopWidget: public TuileWidget {
  public:
	LoopWidget(const std::string& name, tuiles::LoopTuile*);
	~LoopWidget();

  private:
    
};

#endif

