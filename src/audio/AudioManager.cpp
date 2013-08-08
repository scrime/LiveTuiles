/***************************************************************************
 *  AudioManager.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "AudioManager.hpp"

#include <iostream>
#include <sstream>

#include <tuiles/SeqTuile.hpp>
#include <tuiles/LoopTuile.hpp>

#include "FaustTuile.hpp"
#include "SoundFileTuile.hpp"
#include "AudioInputTuile.hpp"
#include "AudioOutputTuile.hpp"
#include "MidiOscSwitchTuile.hpp"
#include "MidiOscMonitorTuile.hpp"

#include <tuiles/CommandsHandler.hpp>
#include "commands/AddAudioTuile.hpp"
#include "commands/RemoveAudioTuile.hpp"
#include "commands/SetProcStep.hpp"

using namespace std;
using namespace tuiles;

AudioManager::AudioManager() {
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
    AudioManager* aman = AudioManager::getInstance();
    //progress in the trees
    aman->processTrees(float(nbFrames)*aman->m_procStep);
    //process the buffers
    aman->process(nbFrames);
	return 0;
}

void AudioManager::init() {
	//create jack client
	m_jackClient = jack_client_open("LiveTuiles", JackNullOption, NULL);	
	m_sampleRate = jack_get_sample_rate(m_jackClient);	
	m_bufferSize = jack_get_buffer_size(m_jackClient);	
    setBpm(60);

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

void AudioManager::setBpm(const float& bpm) {
    m_bpm=bpm;
    m_framesPerBeat = float(m_sampleRate)/(m_bpm/60.0);
    SetProcStep* com = static_cast<SetProcStep*>
                                    (m_protoSetProcStep->popClone());
    if(com) {
        com->setAudioManager(this);
        com->setStep(m_bpm/60.0);
        m_commandsToProc->runCommand(com);
    }
    m_defaultLength=4*float(m_sampleRate)/(m_bpm/60.0);
}

void AudioManager::framesToBeats(const float& frames, float& beats) {
    beats = frames/float(m_sampleRate)*(m_bpm/60.0);
}

void AudioManager::togglePlay() {
    if(isPlaying()) {
        stopTrees();
    }
    else {
        startTrees();
    }
}

const float& AudioManager::getPlayPositionInBeats() {
    m_playPosInBeats = m_playingPos/float(m_sampleRate)*(m_bpm/60.0);
    return m_playPosInBeats;
}

void AudioManager::process(const int& nbFrames) {
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

void AudioManager::internalAddAudioTuile(AudioTuile* tuile) {
    AddAudioTuile* com = 
        static_cast<AddAudioTuile*>(m_protoAddAudioTuile->popClone());
    if(com) {
        com->setAudioTuile(tuile); 
        com->setAudioManager(this);
        m_commandsToProc->runCommand(com);
    }
}

void AudioManager::deleteTuile(Tuile* tuile) {
    TuilesManager::deleteTuile(tuile);

}

FaustTuile* AudioManager::addFaustTuile(const std::string& fileName) {
    FaustTuile* newTuile = new FaustTuile();
    addLeaf(newTuile);
    newTuile->load(fileName);
    internalAddAudioTuile(newTuile);
    newTuile->setLength(m_defaultLength);
    return newTuile;
}

SoundFileTuile* AudioManager::addSoundFileTuile(const std::string& fileName) {
    SoundFileTuile* newTuile = new SoundFileTuile();
    addLeaf(newTuile);
    newTuile->load(fileName);
    internalAddAudioTuile(newTuile);
    return newTuile;
}

AudioInputTuile* AudioManager::addAudioInputTuile(const std::string& input) {
    AudioInputTuile* newTuile = new AudioInputTuile();
    addLeaf(newTuile);
    ostringstream oss;
    oss<<newTuile->getID();
    newTuile->load("input"+oss.str());
    internalAddAudioTuile(newTuile);
    newTuile->setLength(m_defaultLength);
    return newTuile;
}

AudioOutputTuile* AudioManager::addAudioOutputTuile(const std::string& output) {
    AudioOutputTuile* newTuile = new AudioOutputTuile();
    addLeaf(newTuile);
    ostringstream oss;
    oss<<newTuile->getID();
    newTuile->load("output"+oss.str());
    internalAddAudioTuile(newTuile);
    newTuile->setLength(m_defaultLength);
    return newTuile;
}

SeqTuile* AudioManager::addSeqTuile() {
    SeqTuile* newSeq = new SeqTuile();
    addSeq(newSeq);
    return newSeq;
}

LoopTuile* AudioManager::addLoopTuile() {
    LoopTuile* newLoop = new LoopTuile();
    addLoop(newLoop);
    newLoop->setLength(m_defaultLength);
    return newLoop;
}

MidiOscMonitorTuile* AudioManager::addMidiOscMonitorTuile() {
    MidiOscMonitorTuile* newMonitor = new MidiOscMonitorTuile();
    addMonitor((MonitorTuile*)newMonitor);
    //FIXME
    //internalAddAudioTuile(newMonitor);
    newMonitor->setLength(m_defaultLength);
    return newMonitor;
}

MidiOscSwitchTuile* AudioManager::addMidiOscSwitchTuile() {
    MidiOscSwitchTuile* newSwitch = new MidiOscSwitchTuile();
    addSwitch((SwitchTuile*)newSwitch);
    //FIXME
    //internalAddAudioTuile(newSwitch);
    newSwitch->setLength(m_defaultLength);
    return newSwitch;
}

void AudioManager::procClearTrees() {
    m_procAudioTuiles.clear();
    TuilesManager::procClearTrees();
}

