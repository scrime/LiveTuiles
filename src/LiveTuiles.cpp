/***************************************************************************
 *  LiveTuiles.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "LiveTuiles.hpp"

#include "gui/MainWindow.hpp"
#include "audio/AudioManager.hpp"

using namespace std;

LiveTuiles::LiveTuiles() {}

LiveTuiles::~LiveTuiles() {}

LiveTuiles* LiveTuiles::getInstance() {
    static LiveTuiles instance;
    return &instance;
}

void LiveTuiles::start() {
    //initialize the audio manager 
    AudioManager* man = AudioManager::getInstance();
    man->init();

    //initialize the main window and run fltk
    MainWindow* win = MainWindow::getInstance();
    win->init();
	Fl::run();
}

//-------------MAIN--------------------
int main(int argc, char** argv) {
	LiveTuiles* looper = LiveTuiles::getInstance();
	looper->start();
}

