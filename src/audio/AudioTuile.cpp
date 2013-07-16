/***************************************************************************
 *  AudioTuile.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of libTuiles
 ****************************************************************************/

#include "AudioTuile.hpp"

#include <iostream>

using namespace std;
using namespace tuiles;

AudioTuile::AudioTuile(): LeafTuile() {}

AudioTuile::~AudioTuile() {}

void AudioTuile::resetBuffers() {
    m_computed=false;
}

void AudioTuile::addInputTuile(AudioTuile* inputTuile) {
    m_inputTuiles.push_back(inputTuile);
}

void AudioTuile::removeInputTuile(AudioTuile* inputTuile) {
    vector<AudioTuile*>::iterator itTui=m_inputTuiles.begin();
    for(; itTui!=m_inputTuiles.end(); ++itTui) {
        if((*itTui)->getID()==inputTuile->getID()) {
            m_inputTuiles.erase(itTui++);
        }
        else {
            ++itTui;
        }
    }
    
}




