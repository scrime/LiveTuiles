/***************************************************************************
 *  AudioOutputTuile.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "AudioOutputTuile.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "AudioManager.hpp"

using namespace std;

AudioOutputTuile::AudioOutputTuile(): AudioTuile(){
    m_type="AudioOutput";
    DEBUG("Created an audio output tuile");
}

AudioOutputTuile::~AudioOutputTuile() {
    AudioManager* man=AudioManager::getInstance();
    jack_port_unregister(man->getJackClient(), m_outputPortLeft);
    jack_port_unregister(man->getJackClient(), m_outputPortRight);
}

void AudioOutputTuile::load(const std::string& output) {
    m_name=output;
    AudioManager* man=AudioManager::getInstance();
    m_outputPortLeft=jack_port_register(man->getJackClient(),
                                        (output+"-L").c_str(),
                                        JACK_DEFAULT_AUDIO_TYPE, 
                                        JackPortIsOutput, 0);
    m_outputPortRight=jack_port_register(man->getJackClient(),
                                        (output+"-R").c_str(),
                                        JACK_DEFAULT_AUDIO_TYPE, 
                                        JackPortIsOutput, 0);
    m_internalBuffer.resize(2, vector<jack_default_audio_sample_t>(0, 0));

    //connect to the soundcard by default
    jack_connect(man->getJackClient(), 
                    string("LiveTuiles:"+output+"-L").c_str(), 
                    "system:playback_1");
    jack_connect(man->getJackClient(), 
                    string("LiveTuiles:"+output+"-R").c_str(), 
                    "system:playback_2");
    DEBUG("AudioOutputTuile "<<m_name<<" loaded");
	m_loaded=true;
    updateLoaded();
}

void AudioOutputTuile::unload() {
	m_loaded=false;
    updateLoaded();
}

void AudioOutputTuile::processBuffers(const int& nbFrames) {
    if(!m_computed && m_procLoaded) {
        m_internalBuffer[0].assign(nbFrames, 0);
        m_internalBuffer[1].assign(nbFrames, 0);
        jack_default_audio_sample_t* bufL=(jack_default_audio_sample_t *)
                        jack_port_get_buffer(m_outputPortLeft, nbFrames);
        jack_default_audio_sample_t* bufR=(jack_default_audio_sample_t *)
                        jack_port_get_buffer(m_outputPortRight, nbFrames);
        if(m_procActive) {
            vector<AudioTuile*>::iterator itTui=m_procInputTuiles.begin();
            for(; itTui!=m_procInputTuiles.end(); ++itTui) {
                (*itTui)->processBuffers(nbFrames);
                for(unsigned int c=0; c<m_internalBuffer.size(); ++c) {
                    for(int f=0; f<nbFrames; ++f) {
                        m_internalBuffer[c][f]+=
                            (*itTui)->getBuffer()
                                        [c%(*itTui)->getBuffer().size()][f];
                    }
                }
            }
        }
        for(int f=0; f<nbFrames; ++f) {
            bufL[f]=m_internalBuffer[0][f]*m_procVolume;
            bufR[f]=m_internalBuffer[1][f]*m_procVolume;
        }
        m_computed=true;
     }
}

void AudioOutputTuile::load(xmlNodePtr node) {
    AudioTuile::load(node);
    load(m_name);
}

