/***************************************************************************
 *  SoundFileParamWidget.h
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _SoundFileParamWidget_H
#define _SoundFileParamWidget_H

#include "TuileParamWidget.hpp"

class SoundFileParamWidget : public TuileParamWidget{
  public:
	SoundFileParamWidget(int ,int , int, int, const unsigned int&, 
                                            const std::string&);
	~SoundFileParamWidget();

    void update();

    static void statTuileSoundFileInputs(Fl_Widget* w, void* f){ 
        SoundFileParamWidget *tpw = static_cast<SoundFileParamWidget *>(f); 
        tpw->cbTuileSoundFileInputs(w);
    }	
    void cbTuileSoundFileInputs(Fl_Widget*);

  protected:
    HitPack *m_audioPack;
    Fl_Slider *m_volumeSlider;
    Fl_Value_Input *m_bpmInput;
    Fl_Choice *m_outputChoice;


};

#endif
