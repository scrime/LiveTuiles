/***************************************************************************
 *  SoundFileParamWidget.h
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _SoundFileParamWidget_H
#define _SoundFileParamWidget_H

#include "AudioTuileParamWidget.hpp"

class SoundFileTuile;

class SoundFileParamWidget : public AudioTuileParamWidget {
  public:
	SoundFileParamWidget(TuileWidget*, SoundFileTuile*);
	~SoundFileParamWidget();

    void update();

  protected:
    SoundFileTuile* m_sfTuile;
    Fl_Value_Input *m_bpmInput;

};

#endif
