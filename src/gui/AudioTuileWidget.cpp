/***************************************************************************
 *  AudioTuileWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "AudioTuileWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "../audio/AudioTuile.hpp"

using namespace std;

AudioTuileWidget::AudioTuileWidget(const std::string& name, 
                                    AudioTuile* tuile):  
                                            TuileWidget(name, tuile), 
                                            m_audioTuile(tuile) {
}

AudioTuileWidget::~AudioTuileWidget() {}

void AudioTuileWidget::connectToWidget(AudioTuileWidget* widget) {
    m_inputWidgets.push_back(widget);
    m_audioTuile->addInputTuile(widget->getAudioTuile());
}

