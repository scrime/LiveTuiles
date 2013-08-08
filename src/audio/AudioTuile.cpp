/***************************************************************************
 *  AudioTuile.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of libTuiles
 ****************************************************************************/

#include "AudioTuile.hpp"

#include <iostream>

#include <tuiles/CommandsHandler.hpp>

#include "commands/UpdateInputTuiles.hpp"
#include "commands/SetProcVolume.hpp"

using namespace std;
using namespace tuiles;

AudioTuile::AudioTuile(): LeafTuile(), m_volume(1), m_procVolume(1) {
    m_protoUpdateInputTuiles = new UpdateInputTuiles();
    m_protoUpdateInputTuiles->createClones(m_nbCommands);
    m_protoSetProcVolume = new SetProcVolume();
    m_protoSetProcVolume->createClones(m_nbCommands);
}

AudioTuile::~AudioTuile() {}

void AudioTuile::resetBuffers() {
    m_computed=false;
}

void AudioTuile::addInputTuile(AudioTuile* inputTuile) {
    m_inputTuiles.push_back(inputTuile);
    updateInputTuiles();
}

void AudioTuile::removeInputTuile(AudioTuile* inputTuile) {
    vector<AudioTuile*>::iterator itTui=m_inputTuiles.begin();
    for(; itTui!=m_inputTuiles.end();) {
        if((*itTui)->getID()==inputTuile->getID()) {
            itTui=m_inputTuiles.erase(itTui);
        }
        else {
            itTui++;
        }
    }
    updateInputTuiles();
}

void AudioTuile::updateInputTuiles() {
    UpdateInputTuiles* com = 
        static_cast<UpdateInputTuiles*>(m_protoUpdateInputTuiles->popClone());
    if(com) {
        com->setAudioTuile(this); 
        com->setInputTuiles(m_inputTuiles);
        m_commandsToProc->runCommand(com);
    }
}

void AudioTuile::setVolume(const float& vol) {
    m_volume=vol;
    SetProcVolume* com = 
        static_cast<SetProcVolume*>(m_protoSetProcVolume->popClone());
    if(com) {
        com->setAudioTuile(this); 
        com->setVolume(vol);
        m_commandsToProc->runCommand(com);
    }

}

void AudioTuile::procUpdateInputTuiles(const std::vector<AudioTuile*>& 
                                                                inputTuiles) {
    m_procInputTuiles = inputTuiles;
}



