/***************************************************************************
 *  SoundFileParamWidget.cpp
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "SoundFileParamWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "HitPack.hpp"
#include "../audio/SoundFileTuile.hpp"

using namespace std;

SoundFileParamWidget::SoundFileParamWidget(SoundFileTuile* sfTuile):
                                TuileParamWidget(sfTuile), m_sfTuile(sfTuile) {

    m_outputChoice = new Fl_Choice(0, 0, 50, 20, "Output");
    m_outputChoice->align(FL_ALIGN_LEFT|FL_ALIGN_TOP);
    m_outputChoice->callback(statTuileSoundFileInputs,this);
    m_outputChoice->add("Internal+Main", "", NULL); 
    m_outputChoice->add("Internal only", "", NULL); 
    m_outputChoice->value(0);
    m_pack->add(m_outputChoice);

    m_volumeSlider = new Fl_Slider(0, 0, 50, 20, "Volume");
    m_volumeSlider->callback(statTuileSoundFileInputs,this);
    m_volumeSlider->align(FL_ALIGN_LEFT|FL_ALIGN_TOP);
    m_volumeSlider->type(FL_HOR_NICE_SLIDER);
    m_volumeSlider->bounds(0, 2);
    m_volumeSlider->value(1);
    m_volumeSlider->labelsize(12);
    m_pack->add(m_volumeSlider);

    end();
}

SoundFileParamWidget::~SoundFileParamWidget() {}

void SoundFileParamWidget::update() {}

void SoundFileParamWidget::cbTuileSoundFileInputs(Fl_Widget* w) {
/*
    JackProcessesManager* pMan = JackProcessesManager::getInstance();
    if(m_volumeSlider->contains(w)) {
        pMan->setProcessVolume(m_id, m_volumeSlider->value());
    }
    else if(m_outputChoice->contains(w)) {
        pMan->setProcessOutput(m_id, m_outputChoice->value());
    }
*/
}

