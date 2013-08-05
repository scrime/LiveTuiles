/***************************************************************************
 *  TuileParamGroup.h
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef _TuileParamGroup_H
#define _TuileParamGroup_H
 
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Valuator.H>
#include <FL/Fl_Toggle_Button.H>
#include <iostream>
#include <string>
#include <vector>

#include "TuileParamWidget.hpp"

class TuileParamGroup : public Fl_Group {
  public:
    static TuileParamGroup* getInstance();
	~TuileParamGroup();
    void setWidget(TuileParamWidget*);

  protected:
	TuileParamGroup();

};

#endif
