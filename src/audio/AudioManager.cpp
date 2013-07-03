/***************************************************************************
 *  AudioManager.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "AudioManager.hpp"

#include <iostream>

#include <tuiles/TuilesManager.hpp>
#include <tuiles/SeqTuile.hpp>
#include <tuiles/LoopTuile.hpp>

#include "FaustTuile.hpp"
#include "SoundFileTuile.hpp"
#include "AudioInputTuile.hpp"
#include "AudioOutputTuile.hpp"
#include "MidiOscSwitchTuile.hpp"
#include "MidiOscMonitorTuile.hpp"

#include "commands/AddAudioTuile.hpp"
#include "commands/RemoveAudioTuile.hpp"
#include "commands/SetProcStep.hpp"

using namespace std;
using namespace tuiles;

AudioManager::AudioManager(): m_bpm(120) {
    int nbCommands=50;
    m_protoAddAudioTuile = new AddAudioTuile();
    m_protoAddAudioTuile->createClones(nbCommands);
    m_protoRemoveAudioTuile = new RemoveAudioTuile();
    m_protoRemoveAudioTuile->createClones(nbCommands);
    m_protoSetProcStep= new SetProcStep();
    m_protoSetProcStep->createClones(nbCommands);
}

AudioManager::~AudioManager() {
    jack_deactivate(m_jackClient);
    jack_client_close(m_jackClient);
}

AudioManager* AudioManager::getInstance() {
    static AudioManager instance;
    return &instance;
}

int jackCallback(jack_nframes_t nbFrames, void *arg) {
    //move through the trees
    TuilesManager* tman = TuilesManager::getInstance();
    tman->processTrees(nbFrames);

    //AudioManager
    AudioManager* aman = AudioManager::getInstance();
    aman->process(nbFrames);

	return 0;
}

void AudioManager::init() {
	//create jack client
	m_jackClient = jack_client_open("LiveTuiles", JackNullOption, NULL);	
	m_sampleRate = jack_get_sample_rate(m_jackClient);	
	m_bufferSize = jack_get_buffer_size(m_jackClient);	

/*
    m_midiInputProc = new MidiInputProcess(0, 
                        jack_port_register(m_jackClient, "Midi-In", 
                        JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0));
*/

	//process callback
	jack_set_process_callback (m_jackClient, jackCallback, this);

	//activate the client
	jack_activate(m_jackClient);

}

void AudioManager::clear() {
    TuilesManager::getInstance()->clear(); 

}

void AudioManager::start() {
    TuilesManager::getInstance()->startTrees(); 

}

void AudioManager::stop() {
    TuilesManager::getInstance()->stopTrees(); 

}

void AudioManager::update() {
    TuilesManager::getInstance()->update(); 

}

void AudioManager::setBpm(const float& bpm) {
    m_bpm=bpm;

/*
    SetProcStep* com = static_cast<SetProcStep*>
                                    (m_protoSetProcStep->popClone());
    if(com) {
        com->setAudioManager(this);
        com->setStep(m_bpm*
        TuilesManager::getInstance()->getCommandsToProc()->runCommand(com);
    }
*/
}

bool AudioManager::isPlaying() {
    return TuilesManager::getInstance()->isPlaying();
}

void AudioManager::togglePlay() {
    TuilesManager* man = TuilesManager::getInstance();
    if(man->isPlaying()) {
        man->stopTrees();
    }
    else {
        man->startTrees();
    }
}

const float& AudioManager::getPlayPosition() {
    return TuilesManager::getInstance()->getPlayingPos();
}

const float& AudioManager::getPlayPositionInBeats() {
    m_playPosInBeats = 
        (TuilesManager::getInstance()->getPlayingPos()/float(m_sampleRate))
        *(m_bpm/60.0);
    return m_playPosInBeats;
}

void AudioManager::process(const int& nbFrames) {
    if(TuilesManager::getInstance()->isPlaying()) {
        //go through all the processes
        vector<AudioTuile*>::iterator itTui = m_procAudioTuiles.begin();
        for(; itTui!=m_procAudioTuiles.end(); ++itTui) {
                (*itTui)->resetBuffers();
        }
        itTui = m_procAudioTuiles.begin();
        for(; itTui!=m_procAudioTuiles.end(); ++itTui) {
                (*itTui)->processBuffers(nbFrames);
        }
    }
}

FaustTuile* AudioManager::addFaustTuile(const std::string& fileName) {
    FaustTuile* newTuile = new FaustTuile();
    newTuile->load(fileName);
    TuilesManager::getInstance()->addLeaf(newTuile);
    return newTuile;
}

SoundFileTuile* AudioManager::addSoundFileTuile(const std::string& fileName) {
    SoundFileTuile* newTuile = new SoundFileTuile();
    newTuile->load(fileName);
    TuilesManager::getInstance()->addLeaf(newTuile);
    return newTuile;
}

AudioInputTuile* AudioManager::addAudioInputTuile(const std::string& input) {
    AudioInputTuile* newTuile = new AudioInputTuile();
    TuilesManager::getInstance()->addLeaf(newTuile);
    return newTuile;
}

AudioOutputTuile* AudioManager::addAudioOutputTuile(const std::string& output) {
    AudioOutputTuile* newTuile = new AudioOutputTuile();
    TuilesManager::getInstance()->addLeaf(newTuile);
    return newTuile;
}

SeqTuile* AudioManager::addSeqTuile() {
    SeqTuile* newSeq = new SeqTuile();
    TuilesManager::getInstance()->addSeq(newSeq);
    return newSeq;
}

LoopTuile* AudioManager::addLoopTuile() {
    LoopTuile* newLoop = new LoopTuile();
    TuilesManager::getInstance()->addLoop(newLoop);
    return newLoop;
}

MidiOscMonitorTuile* AudioManager::addMidiOscMonitorTuile() {
    MidiOscMonitorTuile* newMonitor = new MidiOscMonitorTuile();
    TuilesManager::getInstance()->addMonitor((MonitorTuile*)newMonitor);
    return newMonitor;
}

MidiOscSwitchTuile* AudioManager::addMidiOscSwitchTuile() {
    MidiOscSwitchTuile* newSwitch = new MidiOscSwitchTuile();
    TuilesManager::getInstance()->addSwitch((SwitchTuile*)newSwitch);
    return newSwitch;
}

