/***************************************************************************
 *  AudioInputTuile.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "AudioInputTuile.hpp"

#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>

#include "AudioManager.hpp"

using namespace std;

AudioInputTuile::AudioInputTuile(): AudioTuile() {
    m_type="AudioInput";
}

AudioInputTuile::~AudioInputTuile() {
    AudioManager* man=AudioManager::getInstance();
    jack_port_unregister(man->getJackClient(), m_inputPortLeft);
    jack_port_unregister(man->getJackClient(), m_inputPortRight);
}

void AudioInputTuile::load(const std::string& input) {
    m_name=input;
    AudioManager* man=AudioManager::getInstance();
    m_inputPortLeft=jack_port_register(man->getJackClient(),
                                        (input+"-L").c_str(),
                                        JACK_DEFAULT_AUDIO_TYPE, 
                                        JackPortIsInput, 0);
    m_inputPortRight=jack_port_register(man->getJackClient(),
                                        (input+"-R").c_str(),
                                        JACK_DEFAULT_AUDIO_TYPE, 
                                        JackPortIsInput, 0);
    m_internalBuffer.resize(2, vector<jack_default_audio_sample_t>(0, 0));

    //connect to the soundcard by default
    jack_connect(man->getJackClient(),"system:capture_1", 
                                string("LiveTuiles:"+input+"-L").c_str());
    jack_connect(man->getJackClient(),"system:capture_2", 
                                string("LiveTuiles:"+input+"-R").c_str());
    DEBUG("AudioInputTuile "<<m_name<<" loaded");
	m_loaded=true;
    updateLoaded();
}

void AudioInputTuile::unload() {
	m_loaded=false;
    updateLoaded();
}

void AudioInputTuile::processBuffers(const int& nbFrames) {
    if(!m_computed && m_procLoaded) {
        m_internalBuffer[0].assign(nbFrames, 0);
        m_internalBuffer[1].assign(nbFrames, 0);
        if(m_procActive) {
            jack_default_audio_sample_t* bufL=(jack_default_audio_sample_t *)
                            jack_port_get_buffer(m_inputPortLeft, nbFrames);
            jack_default_audio_sample_t* bufR=(jack_default_audio_sample_t *)
                            jack_port_get_buffer(m_inputPortRight, nbFrames);
            for(int f=0; f<nbFrames; ++f) {
                m_internalBuffer[0][f]=bufL[f]*m_procVolume;
                m_internalBuffer[1][f]=bufR[f]*m_procVolume;
            }
        }
        m_computed=true;
     }
}

void AudioInputTuile::load(xmlNodePtr node) {
    ostringstream oss;
    oss<<m_id;
    load("input"+oss.str());
}

