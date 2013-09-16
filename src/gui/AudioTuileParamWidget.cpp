/***************************************************************************
 *  AudioTuileParamWidget.cpp
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "AudioTuileParamWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "HitPack.hpp"
#include "../audio/AudioTuile.hpp"

using namespace std;

AudioTuileParamWidget::AudioTuileParamWidget(TuileWidget* widget, 
                                            AudioTuile* audioTuile):
                                TuileParamWidget(widget, audioTuile), 
                                m_audioTuile(audioTuile) {
    end();

    m_volumeSlider = new Fl_Value_Slider(0, 0, 40, 20, "Volume");
    m_volumeSlider->callback(statAudioTuileInputs,this);
    m_volumeSlider->align(FL_ALIGN_LEFT|FL_ALIGN_TOP);
    m_volumeSlider->type(FL_HOR_NICE_SLIDER);
    m_volumeSlider->bounds(0, 2);
    m_volumeSlider->value(1);
    m_volumeSlider->labelsize(12);
    m_pack->add(m_volumeSlider);
}

AudioTuileParamWidget::~AudioTuileParamWidget() {}

void AudioTuileParamWidget::update() {}

void AudioTuileParamWidget::cbAudioTuileInputs(Fl_Widget* w) {
    if(m_volumeSlider->contains(w)) {
        m_audioTuile->setVolume(m_volumeSlider->value());
    }
}

