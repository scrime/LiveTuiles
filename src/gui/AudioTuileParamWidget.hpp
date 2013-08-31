/***************************************************************************
 *  AudioTuileParamWidget.h
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _AudioTuileParamWidget_H
#define _AudioTuileParamWidget_H

#include "TuileParamWidget.hpp"

class AudioTuile;

class AudioTuileParamWidget : public TuileParamWidget {
  public:
	AudioTuileParamWidget(AudioTuile*);
	virtual ~AudioTuileParamWidget();

    void update();

    static void statAudioTuileInputs(Fl_Widget* w, void* f){ 
        AudioTuileParamWidget *tpw = static_cast<AudioTuileParamWidget *>(f); 
        tpw->cbAudioTuileInputs(w);
    }	
    void cbAudioTuileInputs(Fl_Widget*);

  protected:
    AudioTuile* m_audioTuile;
    Fl_Slider *m_volumeSlider;

};

#endif
