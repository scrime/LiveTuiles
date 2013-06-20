/***************************************************************************
 *            TuileParamWidget.h
 *
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *
 *  This file is part of libTuiles
 ****************************************************************************/

#ifndef _TuileParamWidget_H
#define _TuileParamWidget_H
 
#include <iostream>
#include <string>
#include <vector>
#include <list>

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Valuator.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/filename.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Choice.H>

#include "HitPack.hpp"

class TuileParamWidget : public Fl_Scroll {
  public:
	TuileParamWidget(int ,int , int, int, const unsigned int&, 
                                            const std::string&);
	~TuileParamWidget();

    inline virtual void init(){}
    virtual void update();

    void updateTuileProps();

    inline const std::string& getName(){return m_name;}
	inline void setName(const std::string& name){m_name=name;}

    static void statTuileInputs(Fl_Widget* w, void* f){ 
        TuileParamWidget *tpw = static_cast<TuileParamWidget *>(f); 
        tpw->cbTuileInputs(w);
    }	
    void cbTuileInputs(Fl_Widget*);

    inline virtual void setInputList(const std::vector<std::string>&, 
                                        const std::vector<int>&){}

  protected:
	unsigned int m_id;
	std::string m_name;

    HitPack* m_pack;

    HitPack* m_tuilePack;
    Fl_Input* m_nameInput;
    Fl_Value_Input* m_lengthInput;
    Fl_Value_Input* m_lOffsetInput;
    Fl_Value_Input* m_rOffsetInput;
    Fl_Button* m_removeButton;

    std::list<Fl_Slider*> m_sliders;
    std::list<Fl_Button*> m_buttons;
    std::list<Fl_Value_Input*> m_inputs;

    Fl_Pack* m_buffersPack;
    Fl_Choice* m_inputBufferList;
    Fl_Choice* m_outputBufferList;
};

#endif
