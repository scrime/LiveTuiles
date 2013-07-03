/***************************************************************************
 *  AudioOutputWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "AudioOutputWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "../audio/AudioOutputTuile.hpp"

using namespace std;

AudioOutputWidget::AudioOutputWidget(const std::string& name, 
                        AudioOutputTuile* tuile): TuileWidget(name, tuile){}

AudioOutputWidget::~AudioOutputWidget() {}



