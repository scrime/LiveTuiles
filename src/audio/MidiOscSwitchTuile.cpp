/***************************************************************************
 *  MidiOscSwitchTuile.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "MidiOscSwitchTuile.hpp"

#include <sstream>
#include <fstream>
#include <stdexcept>

#include "AudioManager.hpp"

using namespace std;
using namespace tuiles;

MidiOscSwitchTuile::MidiOscSwitchTuile(): SwitchTuile() {
    m_type="MidiOscSwitch";
}

MidiOscSwitchTuile::~MidiOscSwitchTuile() {}

void MidiOscSwitchTuile::load(const std::string& fileName) {
/*
	m_fileName=fileName;
    m_loaded=true;
*/
}

void MidiOscSwitchTuile::unload() {
/*
	m_loaded=false;
*/
}

void MidiOscSwitchTuile::activate() {
}

void MidiOscSwitchTuile::deactivate() {
}

void MidiOscSwitchTuile::processBuffers(const int& nbFrames) {
}


