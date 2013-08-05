/***************************************************************************
 *  AudioInputWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "AudioInputWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "../audio/AudioInputTuile.hpp"

using namespace std;

AudioInputWidget::AudioInputWidget(const std::string& name, 
                                    AudioInputTuile* tuile): 
                                        AudioTuileWidget(name, tuile) {
    m_canTakeInput=false;
}

AudioInputWidget::~AudioInputWidget() {}



