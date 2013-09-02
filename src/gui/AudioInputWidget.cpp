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
#include <sstream>

#include "../audio/AudioInputTuile.hpp"
#include "AudioTuileParamWidget.hpp"

using namespace std;

AudioInputWidget::AudioInputWidget(const std::string& name, 
                                    AudioInputTuile* tuile): 
                                        AudioTuileWidget(name, tuile),
                                        m_audioInputTuile(tuile) {
    m_paramWidget=new AudioTuileParamWidget(this, tuile);
    m_canTakeInput=false;
}

AudioInputWidget::~AudioInputWidget() {}

void AudioInputWidget::load() {
    LeafTuileWidget::load();
    ostringstream oss;
    oss<<m_tuile->getID();
    m_audioInputTuile->load(m_name+oss.str());
}

void AudioInputWidget::load(xmlNodePtr node) {
    LeafTuileWidget::load(node);
    load();
}

