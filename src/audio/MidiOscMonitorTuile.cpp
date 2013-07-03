/***************************************************************************
 *  MidiOscMonitorTuile.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "MidiOscMonitorTuile.hpp"

#include <sstream>
#include <fstream>
#include <stdexcept>

#include "AudioManager.hpp"

using namespace std;

MidiOscMonitorTuile::MidiOscMonitorTuile(): AudioTuile() {}

MidiOscMonitorTuile::~MidiOscMonitorTuile() {}

void MidiOscMonitorTuile::load(const std::string& fileName) {
	m_fileName=fileName;
}

void MidiOscMonitorTuile::unload() {
}

void MidiOscMonitorTuile::activate() {
    if(m_loaded) {
    }
}

void MidiOscMonitorTuile::deactivate() {
}

void MidiOscMonitorTuile::processBuffers(const int& nbFrames) {

}


