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

#include "HitPack.hpp"
#include "TreeWidget.hpp"
#include "TuilesBank.hpp"
#include "TuileParamGroup.hpp"

#include "../audio/AudioManager.hpp"

using namespace std;

MainWindow::MainWindow(): Fl_Double_Window(800, 600, "LiveTuiles") {
    Fl::scheme("gtk+");
	end();
}

MainWindow::~MainWindow() {}

MainWindow* MainWindow::getInstance() {
    static MainWindow instance;
    return &instance;
}

void MainWindow::init() {
	Fl::add_idle(idle, this);
    gettimeofday(&m_prevTime, NULL);

	//GUI
	Fl::set_color(FL_BACKGROUND_COLOR,240,240,240);
	Fl::set_color(FL_BACKGROUND2_COLOR,220,220,220);
	Fl::set_color(FL_FOREGROUND_COLOR,0,0,0);
	Fl::set_color(FL_INACTIVE_COLOR,40,40,40);
	Fl::set_color(FL_SELECTION_COLOR,150,150,150);
	m_controlHeight=20;
	m_editHeight=160;
	m_treeHeight=360;
	m_editScoreButtonsHeight=20;
	m_browserWidth=240;
    m_spacing=10;

    //CONTROL
    m_bpmInput = new Fl_Value_Input(m_spacing, m_spacing, 50, 30, "bpm");
    m_bpmInput->align(FL_ALIGN_RIGHT);
	m_bpmInput->clear_visible_focus();
    m_bpmInput->value(120);
    m_bpmInput->step(0.1);
    m_bpmInput->soft(0);
	m_bpmInputTooltip = "Drag horizontally with right or left click";
    m_bpmInput->tooltip(m_bpmInputTooltip.c_str());
    m_bpmInput->bounds(30,400);
    m_bpmInput->callback(statBpm, this);
    m_loopStartButton = new Fl_Toggle_Button(20, m_spacing, 50, 30, "Play");
    m_loopStartButton->callback(statLoopStart, this);
	m_loopStartButton->clear_visible_focus();
    m_treeZoomSlider= new Fl_Slider(0,0,100,20,"Zoom");
    m_treeZoomSlider->align(FL_ALIGN_LEFT);
    m_treeZoomSlider->type(FL_HOR_NICE_SLIDER);
    m_treeZoomSlider->callback(statZoomTree, this);
	m_treeZoomSlider->clear_visible_focus();
	m_controlPart = new HitPack(20, 0, w()-40, m_controlHeight, "");
    m_controlPart->end();
	m_controlPart->type(HitPack::HORIZONTAL);
    m_controlPart->add(m_loopStartButton);
    m_controlPart->add(m_bpmInput);
    m_controlPart->add(new Fl_Group(0,0,w()-250,0,""));
    m_controlPart->add(m_treeZoomSlider);

    //TREE/SCORE
	m_tuilesTree = TreeWidget::getInstance();
    m_tuilesTree->resize(m_spacing, 0, w()- 20, m_treeHeight);
	m_tuilesPart = new HitPack(0, 0, w(), m_treeHeight, "");
    m_tuilesPart->end();
	m_tuilesPart->type(HitPack::HORIZONTAL);
    m_tuilesPart->add(m_tuilesTree);


    //EDITION
    m_clearTreeButton = new Fl_Button(0, 20, 
            fl_width(std::string(" Clear ").c_str()), 20, "Clear");
    m_clearTreeButton->callback(statTreeButtons, this);
    m_saveTreeButton = new Fl_Button(0, 20, 
            fl_width(std::string(" Save ").c_str()), 20, "Save");
    m_saveTreeButton->callback(statTreeButtons, this);
    m_editScoreButtonsPart = new HitPack(0, 0, 80, m_editHeight, "Edit Score");
    m_editScoreButtonsPart->end();
    m_editScoreButtonsPart->color(fl_darker(FL_BACKGROUND2_COLOR));
    m_editScoreButtonsPart->box(FL_DOWN_BOX);
    m_editScoreButtonsPart->align(FL_ALIGN_LEFT|FL_ALIGN_TOP);
	m_editScoreButtonsPart->type(HitPack::VERTICAL);
	m_editScoreButtonsPart->externalSpacing(5);
	m_editScoreButtonsPart->internalSpacing(5);
	m_editScoreButtonsPart->add(m_saveTreeButton);
	m_editScoreButtonsPart->add(m_clearTreeButton);
	
	m_tuilesBank = new TuilesBank(0, 0, m_browserWidth, m_editHeight);
    m_tuilesBank->end();
	m_tuilesBank->open("","{*.wav,*.dsp,*.tui}",".");
    m_tuilesBank->setTreeWidget(m_tuilesTree);

    m_tuileParamGroup= new TuileParamGroup(0,15,w()-m_browserWidth-130, 
                                               m_editHeight);
    m_tuileParamGroup->end();
    m_tuileParamGroup->color(fl_darker(FL_BACKGROUND2_COLOR));
    m_tuileParamGroup->box(FL_DOWN_BOX);
    m_tuilesTree->setParamGroup(m_tuileParamGroup);

	m_editPart = new HitPack(0, 0, w(), m_editHeight, "");
    m_editPart->end();
	m_editPart->type(HitPack::HORIZONTAL);
	m_editPart->add(m_editScoreButtonsPart);
	m_editPart->add(m_tuilesBank);
    m_editPart->add(m_tuileParamGroup);

    //MAIN
	m_pack = new HitPack(0,0,w(),h(),"");
    m_pack->externalSpacing(m_spacing);
    m_pack->internalSpacing(m_spacing*2);
    m_pack->end();
	m_pack->type(HitPack::VERTICAL);
	m_pack->spacing(m_spacing);
	m_pack->add(m_controlPart);
	m_pack->add(m_tuilesPart);
	m_pack->add(m_editPart);

	this->add(m_pack);

    this->resizable(m_pack);
    //m_pack->resizable(m_tuilesPart);
    //m_tuilesPart->resizable(m_tuilesTree);
    m_controlPart->resizable(0);
    m_editPart->resizable(0);

    //clear to add first leaf and loop tuile 
    clearAll();

	//show window
	this->show();
}

void MainWindow::idle(void* pnt) {
	((MainWindow*)pnt)->update();
    AudioManager::getInstance()->update();
}
    
void MainWindow::update() {
	m_tuilesTree->update();
    redraw();
	usleep(1000);
}

void MainWindow::cbBpm(Fl_Widget*) {
	AudioManager::getInstance()->setBpm(m_bpmInput->value());
}

void MainWindow::cbLoopStart(Fl_Widget*) {
    togglePlay();
}

void MainWindow::togglePlay() {
    AudioManager* man = AudioManager::getInstance();
    if(man->isPlaying()) {
		m_loopStartButton->label("Play");
		man->stop();
        m_loopStartButton->value(0);
    }
    else {
		m_loopStartButton->label("Stop");
		man->start();
        m_loopStartButton->value(1);
	}
}

void MainWindow::cbTreeButtons(Fl_Widget* w) {
    if(m_clearTreeButton->contains(w)) {
        clearAll();
    }
    else if(m_saveTreeButton->contains(w)) {
/*
        if(m_tuiles->getNbTuileProps()>0) {
            std::string fileName = getenv("HOME");
            fileName+="/test.tui";
            xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
            xmlNodePtr rootNode = 
                xmlNewNode(NULL, BAD_CAST "MainWindow");
            xmlDocSetRootElement(doc, rootNode);
            //save from after the first fork
            m_tuiles->saveTrees(rootNode);
            //save all processes
            vector<unsigned int> ids;
            m_processes->save(rootNode);
            xmlSaveFormatFileEnc(fileName.c_str(), doc, "UTF-8", 1);
            xmlFreeDoc(doc);
            xmlCleanupParser(); 
        }
        else {
            cout<<"Nothing to save"<<endl;
        }
*/
    }
}

void MainWindow::clearAll() {
    m_loopStartButton->value(0);
    m_loopStartButton->label("Play");
/*
    m_tuiles->stop();
    m_tuiles->clear();
    m_processes->clear();
    m_tuilesTree->clear();
    m_tuileParamGroup->clear();
*/

/*
    //add the first leaf and loop tuile that will be used for looping 
    //default to 16 beats
    m_tuiles->addLeafTuile(16, m_firstLeafID);
    m_tuiles->addLoop(m_firstLeafID, m_mainLoopID);
    m_tuiles->setRoot(m_mainLoopID);
    m_tuilesTree->setMainLoopID(m_mainLoopID);
    m_tuilesTree->setLoopLeafID(m_firstLeafID);
    m_tuiles->refreshTree();
    m_tuilesTree->refreshTuiles();
*/
}

void MainWindow::removeTuile(const unsigned int& id) {
/*
    m_tuiles->removeTuile(id);
    m_processes->removeProcess(id);
*/
    m_tuilesTree->removeTuileWidget(id);
}

void MainWindow::cbZoomTree(Fl_Widget*) {
    m_tuilesTree->zoom(m_treeZoomSlider->value());
}

