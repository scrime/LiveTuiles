/***************************************************************************
 *  MainWindow.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "MainWindow.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <FL/fl_ask.H>

#include "HitPack.hpp"
#include "TreeWidget.hpp"
#include "TuilesBank.hpp"
#include "TuileParamGroup.hpp"
#include "ScrollZoomWidget.hpp"

#include "../audio/AudioManager.hpp"

using namespace std;

MainWindow::MainWindow(): Fl_Double_Window(800, 600, "LiveTuiles") {
    size_range(800, 600);
    Fl::scheme("gtk+");
	end();
}

MainWindow::~MainWindow() {
   clearAll(); 
   AudioManager::getInstance()->close();
}

MainWindow* MainWindow::getInstance() {
    static MainWindow instance;
    return &instance;
}

void MainWindow::init() {
	Fl::add_idle(idle, this);

	Fl::set_color(FL_BACKGROUND_COLOR,240,240,240);
	Fl::set_color(FL_BACKGROUND2_COLOR,220,220,220);
	Fl::set_color(FL_FOREGROUND_COLOR,0,0,0);
	Fl::set_color(FL_INACTIVE_COLOR,40,40,40);
	Fl::set_color(FL_SELECTION_COLOR,150,150,150);
	m_controlHeight=50;
	m_editHeight=160;
	m_treeHeight=390;
	m_editScoreButtonsHeight=20;
	m_browserWidth=240;
    m_paramWidth=360;
    m_spacing=10;

    //CONTROL
    m_bpmInput = new Fl_Value_Input(m_spacing, m_spacing, 50, 30, "bpm");
    m_bpmInput->align(FL_ALIGN_RIGHT);
	m_bpmInput->clear_visible_focus();
    m_bpmInput->value(60);
    m_bpmInput->step(0.1);
    m_bpmInput->soft(0);
	m_bpmInputTooltip = "Drag horizontally with right or left click";
    m_bpmInput->tooltip(m_bpmInputTooltip.c_str());
    m_bpmInput->bounds(30,400);
    m_bpmInput->callback(statBpm, this);
    m_playPauseButton = new Fl_Toggle_Button(20, m_spacing, 30, 30, "@>");
    m_playPauseButton->callback(statPlayPause, this);
	m_playPauseButton->clear_visible_focus();
    m_stopButton = new Fl_Button(30, m_spacing, 20, 30, "@|<");
    m_stopButton->callback(statStop, this);
	m_stopButton->clear_visible_focus();
    HitPack* controlButtonsPack = new HitPack(0, 0, 50, 20,"");
    controlButtonsPack->add(m_stopButton);
    controlButtonsPack->add(m_playPauseButton);
    controlButtonsPack->add(m_bpmInput);
    HitPack* controlLevelsPack = new HitPack(0, 0, 50, 20,"");
    controlLevelsPack->type(Fl_Pack::VERTICAL);
    Fl_Box* sep1 = new Fl_Box(0, 0, w()-270, 20, "");
    controlLevelsPack->add(controlButtonsPack);
    controlLevelsPack->add(sep1);
    m_treeZoomSlider = new ScrollZoomWidget(0, 0, 500, 20, "Zoom/Scroll");
	m_controlPart = new HitPack(20, 0, w()-40, m_controlHeight, "");
	m_controlPart->type(Fl_Pack::HORIZONTAL);
    Fl_Box* sep2 = new Fl_Box(0, 0, w()-270, 20, "");
    m_controlPart->add(controlLevelsPack);
    m_controlPart->add(sep2);
    m_controlPart->add(m_treeZoomSlider);
    m_controlPart->resizable(sep2);

    //TREE/SCORE
	m_tuilesTree = TreeWidget::getInstance();
    m_tuilesTree->resize(m_spacing, 0, w()- 20, m_treeHeight);

    //EDITION
    m_clearTreeButton = new Fl_Button(0, 20, 
            fl_width(std::string(" Clear ").c_str()), 20, "Clear");
    m_clearTreeButton->callback(statTreeButtons, this);
    m_saveTreeButton = new Fl_Button(0, 20, 
            fl_width(std::string(" Save ").c_str()), 20, "Save");
    m_saveTreeButton->callback(statTreeButtons, this);
    m_editScoreButtonsPart = new HitPack(0, 0, 80, m_editHeight, "Edit Score");
    m_editScoreButtonsPart->color(fl_darker(FL_BACKGROUND2_COLOR));
    m_editScoreButtonsPart->box(FL_DOWN_BOX);
    m_editScoreButtonsPart->align(FL_ALIGN_LEFT|FL_ALIGN_TOP);
	m_editScoreButtonsPart->type(Fl_Pack::VERTICAL);
	m_editScoreButtonsPart->externalYSpacing(5);
	m_editScoreButtonsPart->externalXSpacing(5);
	m_editScoreButtonsPart->internalSpacing(5);
	m_editScoreButtonsPart->add(m_saveTreeButton);
	m_editScoreButtonsPart->add(m_clearTreeButton);
	
	m_tuilesBank = new TuilesBank(0, 0, m_browserWidth, m_editHeight);
	m_tuilesBank->open("", "{*.wav,*.dsp,*.tui}", ".");
    m_tuilesBank->setTreeWidget(m_tuilesTree);

    m_tuileParamGroup= TuileParamGroup::getInstance();
    m_tuileParamGroup->resize(0, 0, m_paramWidth, m_editHeight);
    m_tuileParamGroup->color(fl_darker(FL_BACKGROUND2_COLOR));
    m_tuileParamGroup->box(FL_DOWN_BOX);
    m_tuilesTree->setParamGroup(m_tuileParamGroup);

	m_editPart = new HitPack(0, 0, w(), m_editHeight, "");
	m_editPart->type(Fl_Pack::HORIZONTAL);
	m_editPart->add(m_editScoreButtonsPart);
	m_editPart->add(m_tuilesBank);
    m_editPart->add(m_tuileParamGroup);
    m_editPart->internalSpacing(m_spacing);
    m_editPart->externalYSpacing(m_spacing);

    //MAIN
	m_pack = new HitPack(0, 0, w(), h(), "");
    m_pack->externalXSpacing(m_spacing);
    m_pack->externalYSpacing(m_spacing);
    m_pack->internalSpacing(m_spacing);
	m_pack->type(Fl_Pack::VERTICAL);
	m_pack->add(m_controlPart);
	m_pack->add(m_tuilesTree);
	m_pack->add(m_editPart);
	add(m_pack);

    resizable(m_pack);
    m_pack->resizable(m_tuilesTree);
    m_editPart->resizable(m_tuileParamGroup);

    m_pack->repositionWidgets();

	//show window
	show();
}

void MainWindow::idle(void* pnt) {
	((MainWindow*)pnt)->update();
    AudioManager::getInstance()->update();
}
    
void MainWindow::update() {
	m_tuilesTree->update();
    if(AudioManager::getInstance()->isPlaying()) {
        m_tuilesBank->drawDraggedTuile();
    }
	usleep(1000);
}

int MainWindow::handle(int event) {
    switch(event) {
        case FL_SHORTCUT:
        case FL_KEYDOWN: {
            switch(Fl::event_key()) {
                case FL_Escape: {
                    return 1;
                }break;
                default:break;
            }
        }break;
        default:break;
    }
    return Fl_Double_Window::handle(event);
}

void MainWindow::cbBpm(Fl_Widget*) {
	AudioManager::getInstance()->setBpm(m_bpmInput->value());
}

void MainWindow::cbPlayPause(Fl_Widget*) {
    togglePlayPause();
}

void MainWindow::togglePlayPause() {
    AudioManager* man = AudioManager::getInstance();
    if(man->isPlaying()) {
		m_playPauseButton->label("@>");
		man->pauseTrees();
        m_playPauseButton->value(0);
    }
    else {
		m_playPauseButton->label("@||");
		man->startTrees();
        m_playPauseButton->value(1);
	}
    m_tuilesTree->redraw();
}

void MainWindow::togglePlayStop() {
    AudioManager* man = AudioManager::getInstance();
    if(man->isPlaying()) {
		m_playPauseButton->label("@>");
		man->stopTrees();
        m_playPauseButton->value(0);
    }
    else {
		m_playPauseButton->label("@||");
		man->stopTrees();
		man->startTrees();
        m_playPauseButton->value(1);
	}
    m_tuilesTree->redraw();
}

void MainWindow::cbStop(Fl_Widget*) {
    AudioManager* man = AudioManager::getInstance();
    m_playPauseButton->label("@>");
    man->stopTrees();
    m_playPauseButton->value(0);
    m_tuilesTree->redraw();
}

void MainWindow::cbTreeButtons(Fl_Widget* w) {
    if(m_clearTreeButton->contains(w)) {
        clearAll();
    }
    else if(m_saveTreeButton->contains(w)) {
        const char* fileName = fl_input("Please enter a filename", 
                                                    m_currentFileName.c_str());
        if(fileName && std::string(fileName).compare("")!=0) {
            m_currentFileName=fileName;
            m_tuilesTree->save(
                    m_tuilesBank->getCurrentDir()+"/"+m_currentFileName+".tui");
        }
    }
}

void MainWindow::clearAll() {
    m_playPauseButton->value(0);
    m_playPauseButton->label("@|>");
    AudioManager::getInstance()->clearTrees();
    m_tuilesTree->clear();
    redraw();
}

