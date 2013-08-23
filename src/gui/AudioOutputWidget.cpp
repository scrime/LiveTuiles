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
#include <sstream>

#include "../audio/AudioOutputTuile.hpp"
#include "AudioTuileParamWidget.hpp"

using namespace std;

AudioOutputWidget::AudioOutputWidget(const std::string& name, 
                                    AudioOutputTuile* tuile): 
                                        AudioTuileWidget(name, tuile), 
                                        m_audioOutputTuile(tuile) {
    m_paramWidget=new AudioTuileParamWidget(tuile);
}

AudioOutputWidget::~AudioOutputWidget() {}

void AudioOutputWidget::load() {
    LeafTuileWidget::load();
    ostringstream oss;
    oss<<m_tuile->getID();
    m_audioOutputTuile->load(m_name+oss.str());
}

void AudioOutputWidget::load(xmlNodePtr node) {
    LeafTuileWidget::load(node);
    load();
}

