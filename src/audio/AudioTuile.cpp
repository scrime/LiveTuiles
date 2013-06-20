/***************************************************************************
 *  AudioTuile.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of libTuiles
 ****************************************************************************/

#include "AudioTuile.hpp"

#include <iostream>

using namespace std;

AudioTuile::AudioTuile(): LeafTuile() {}

AudioTuile::~AudioTuile() {}

void AudioTuile::resetBuffers() {
    m_computed=false;
}

