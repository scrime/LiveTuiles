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

using namespace std;
using namespace tuiles;

AudioTuile::AudioTuile(): LeafTuile() {
    m_protoUpdateInputTuiles = new UpdateInputTuiles();
    m_protoUpdateInputTuiles->createClones(m_nbCommands);
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
    for(; itTui!=m_inputTuiles.end(); ++itTui) {
        if((*itTui)->getID()==inputTuile->getID()) {
            m_inputTuiles.erase(itTui++);
        }
        else {
            ++itTui;
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

void AudioTuile::procUpdateInputTuiles(const std::vector<AudioTuile*>& 
                                                                inputTuiles) {
    m_procInputTuiles = inputTuiles;
    cout<<"tuile "<<m_name<<" now has "<<m_procInputTuiles.size()<<" proc inputs"<<endl;
}



