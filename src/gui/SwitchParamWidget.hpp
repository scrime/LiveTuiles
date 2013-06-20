/***************************************************************************
 *            SwitchParamWidget.h
 *
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *
 *  This file is part of libTuiles
 ****************************************************************************/
 
 
#ifndef _SwitchParamWidget_H
#define _SwitchParamWidget_H

#include <FL/Fl_Simple_Counter.H>

#include "TuileParamWidget.hpp"

class SwitchParamWidget : public TuileParamWidget{
  public:
	SwitchParamWidget(int ,int , int, int, const unsigned int&, 
                                            const std::string&);
	~SwitchParamWidget();

    void update();

    static void statTuileSwitchInputs(Fl_Widget* w, void* f){ 
        SwitchParamWidget *tpw = static_cast<SwitchParamWidget *>(f); 
        tpw->cbTuileSwitchInputs(w);
    }	
    void cbTuileSwitchInputs(Fl_Widget*);

  protected:
    HitPack *m_switchPack;
    Fl_Simple_Counter *m_selectInput;


};

#endif
