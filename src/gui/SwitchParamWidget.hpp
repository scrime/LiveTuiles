/***************************************************************************
 *  SwitchParamWidget.hpp
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _SwitchParamWidget_H
#define _SwitchParamWidget_H

#include <FL/Fl_Simple_Counter.H>

#include "TuileParamWidget.hpp"

class MidiOscSwitchTuile;

class SwitchParamWidget : public TuileParamWidget{
  public:
	SwitchParamWidget(TuileWidget*, MidiOscSwitchTuile*);
	~SwitchParamWidget();

    void update();

    static void statTuileSwitchInputs(Fl_Widget* w, void* f){ 
        SwitchParamWidget *tpw = static_cast<SwitchParamWidget *>(f); 
        tpw->cbTuileSwitchInputs(w);
    }	
    void cbTuileSwitchInputs(Fl_Widget*);

  protected:
    MidiOscSwitchTuile* m_switchTuile;
    HitPack *m_switchPack;
    Fl_Simple_Counter *m_selectInput;
};

#endif
