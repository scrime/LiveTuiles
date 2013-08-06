/***************************************************************************
 *  TreeWidget.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/


#include "TreeWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "../audio/AudioManager.hpp"
#include <tuiles/SeqTuile.hpp>
#include <tuiles/SwitchTuile.hpp>
#include <tuiles/MonitorTuile.hpp>
#include <tuiles/LoopTuile.hpp>

#include "MainWindow.hpp"
#include "TuileWidget.hpp"
#include "SoundFileWidget.hpp"
#include "FaustWidget.hpp"
#include "AudioInputWidget.hpp"
#include "AudioOutputWidget.hpp"
#include "SeqWidget.hpp"
#include "LoopWidget.hpp"
#include "MonitorWidget.hpp"
#include "SwitchWidget.hpp"

#include "TuileParamGroup.hpp"

using namespace std;
using namespace tuiles;

TreeWidget::TreeWidget():   TuileWidget(AudioManager::getInstance()),
                            Fl_Scroll(0,0,100,100,""),
                            m_cursorX(0), 
                            m_offsetX(10), m_offsetY(10), m_magnetSize(5),
                            m_zeroPosX(0) {
    m_tuile->addObserver(this);
	end();
    m_id=-1;
    type(Fl_Scroll::BOTH);
    zoom(0);
}

TreeWidget::~TreeWidget() {}

TreeWidget* TreeWidget::getInstance() {
    static TreeWidget instance;
    return &instance;
}

void TreeWidget::update() {
    //update cursor
    m_cursorX = (AudioManager::getInstance()->getPlayPositionInBeats()) 
                    *m_pixelsPerBeat;
    if(AudioManager::getInstance()->isPlaying()) {
        redraw();
    }
}

void TreeWidget::zoom(const float& zoom) {
    m_pixelsPerBeat=10.0+zoom*100;
    m_pixelsPerFrame=m_pixelsPerBeat
                        /AudioManager::getInstance()->getFramesPerBeat();
    DEBUG("Zoom : pixperbeat: "<<m_pixelsPerBeat
        <<" framesperbeat: "<<AudioManager::getInstance()->getFramesPerBeat()
        <<" pixperframe: "<<m_pixelsPerFrame);
    refreshTuiles();
}

void TreeWidget::draw() {
    Fl_Scroll::draw();

	//box
	fl_draw_box(FL_DOWN_BOX, x(), y(), w(), h(), FL_BACKGROUND_COLOR);
	fl_push_clip(x()+2, y()+2, w()-4, h()-4);

	//score and beat lines
	fl_rectf(x()+(m_zeroPosX-xposition()), y(), 
            w()-(m_zeroPosX-xposition()), h(), FL_BACKGROUND2_COLOR);
	fl_color(FL_WHITE);
	for(int i=x()+(m_zeroPosX-xposition()); i<x()+w(); i+=m_pixelsPerBeat) {
		fl_line(i, y(), i, y()+h());
	}
	//first draw background execution tuiles
    vector<TuileWidget*>::iterator itWidgetNode=
                                            m_childrenTuileWidgets.begin();
    for(; itWidgetNode!=m_childrenTuileWidgets.end(); ++itWidgetNode) {
        (*itWidgetNode)->drawExecution();
    }

    //then draw composition tuiles and scrollbar
	draw_children();

    //connections between tuiles 
    list<AudioTuileWidget*>::const_iterator itWidget
                                                =m_audioTuileWidgets.begin();
    for(; itWidget!=m_audioTuileWidgets.end(); ++itWidget) {
        (*itWidget)->drawConnections();
    }

    //cursor
	fl_color(FL_RED);
    fl_line(x()+m_cursorX+(m_zeroPosX-xposition()), y(), 
            x()+m_cursorX+(m_zeroPosX-xposition()), y()+h());
	fl_pop_clip();
}

void TreeWidget::refreshChildrenTuileWidgets() {
    //remove all children tuile widgets
    vector<TuileWidget*>::iterator itChWid=m_childrenTuileWidgets.begin();
    for(; itChWid!=m_childrenTuileWidgets.end(); ++itChWid) {
        remove((*itChWid)->getWidget());
    }
    m_childrenTuileWidgets.clear();
    //get new children tuile widgets
    AudioManager* man = AudioManager::getInstance();
    vector<Tuile*>::const_iterator itChild=man->getChildren().begin();
    for(; itChild!=man->getChildren().end(); ++itChild) {
        if(m_tuileWidgetMap.find((*itChild)->getID())!=m_tuileWidgetMap.end()){
            m_childrenTuileWidgets.push_back(
                                        m_tuileWidgetMap[(*itChild)->getID()]);
            add(m_tuileWidgetMap[(*itChild)->getID()]->getWidget());
        }
    }
    //update the children of all the tuilewidgets
    list<TuileWidget*>::iterator itWid=m_tuileWidgets.begin();
    for(; itWid!=m_tuileWidgets.end(); ++itWid) {
        (*itWid)->refreshChildrenTuileWidgets();
    }
    //print the trees
    man->printTrees();
    //refresh tuiles
    refreshTuiles();
}

void TreeWidget::refreshTuiles() {
    //notify
    notify();
}

void TreeWidget::notify() {
    //update tuiles positions 
    m_zeroPosX = m_tuile->getLeftOffset()*m_pixelsPerFrame;
    for(unsigned int i=0; i<m_childrenTuileWidgets.size(); ++i) {
        m_childrenTuileWidgets[i]->getWidget()->position(
            min<float>(m_tuile->getLeftOffset(), 
                        m_tuile->getLeftOffset()
                            -m_childrenTuileWidgets[i]
                                ->getTuile()->getLeftOffset())
                *m_pixelsPerFrame
                +x(), 
            m_childrenTuileWidgets[i]->getWidget()->y());
    }
    //notify all tuiles
    list<TuileWidget*>::const_iterator itWidget=m_tuileWidgets.begin();
    for(;itWidget!=m_tuileWidgets.end(); ++itWidget) {
        (*itWidget)->notify();
    }
    redraw();
}

int TreeWidget::handle(int event) {
    switch(event) { 
        case FL_MOUSEWHEEL: {
            if(Fl::event_state(FL_CTRL|FL_COMMAND)) {
                //TODO zoom in/out the tree
                return 1;
            }
            else {
                redraw();
                return Fl_Scroll::handle(event);
            }
        }
        break;
        case FL_KEYDOWN: {
            switch(Fl::event_key()) {
                case FL_BackSpace: {
                    //TODO remove selectedTuile
                    return 1;
                }break;
                case 32: {
                    if(Fl::event_state(FL_SHIFT)) {
                        MainWindow::getInstance()->togglePlayStop();
                    }
                    else {
                        MainWindow::getInstance()->togglePlayPause();
                    }
                    return 1;
                }break;
                case FL_Meta_L:
                case FL_Meta_R:
                case FL_Control_L: 
                case FL_Control_R: {
                    fl_cursor(FL_CURSOR_CROSS);
                    cout<<"changed cursor"<<endl;
                    return 1;
                }break;;
                default:break;
            }
        }break;
        case FL_KEYUP: {
            switch(Fl::event_key()) {
                case FL_Meta_L:
                case FL_Meta_R:
                case FL_Control_L: 
                case FL_Control_R: {
                    fl_cursor(FL_CURSOR_DEFAULT);
                    return 1;
                }break;;
            };
        }break;
        case FL_ENTER: {
            Fl::focus(this);
            return 1;
        }break;
        case FL_LEAVE: 
        case FL_FOCUS: 
        case FL_UNFOCUS: {
            return 1;
        }break;
        default:break;
    }
    return Fl_Scroll::handle(event);
}

void TreeWidget::deselectAllTuileWidgets() {
    m_paramGroup->setWidget(NULL);
	list<TuileWidget*>::const_iterator itWidget=m_tuileWidgets.begin();
	for(;itWidget!=m_tuileWidgets.end(); ++itWidget) {
        (*itWidget)->deselect();
    }
}

bool TreeWidget::testMagnetWithTuile(const int& inX, const int& inY,
                                    int& outX, int& outY, 
                                    const std::string& tuileName,
                                    const bool& drop) {
    bool magnetized=false;
    outX=inX;
    outY=inY;
    vector<TuileWidget*>::iterator itChWid=m_childrenTuileWidgets.begin();
    for(; itChWid!=m_childrenTuileWidgets.end() && !magnetized; ++itChWid) {
        if((*itChWid)->testMagnetWithTuile(inX, inY, outX, 
                                            outY, tuileName, drop)) {
            magnetized=true;
        }
    }

	//drop without link to other tuiles
    if(inX>x() && inY<x()+w() && inY>y() && inY<y()+h()) {
        if(drop && !magnetized) {
            TuileWidget* newWidget = createTuileWidget(tuileName);
            if(newWidget) {
                newWidget->getTuile()->setLeftOffset(-float(inX)
                                                        /m_pixelsPerFrame);
                newWidget->getWidget()->position(newWidget->getWidget()->x(), 
                                                inY);
            }
        }
    }

    return false;
}

void TreeWidget::testConnection(AudioTuileWidget* tuile, 
                                        const int& x, 
                                            const int& y, 
                                                bool drop) {
	list<AudioTuileWidget*>::const_iterator itWidget=
                                                m_audioTuileWidgets.begin();
	for(;itWidget!=m_audioTuileWidgets.end(); ++itWidget) {
        (*itWidget)->resetHighlight(); 
        if(y>(*itWidget)->y() - (*itWidget)->h()/2 
                && y<(*itWidget)->y() + 3*(*itWidget)->h()/2) {
            if(x>(*itWidget)->getRealIn() && x<(*itWidget)->getRealOut()) {
                if((*itWidget)->canTakeInput()) {
                    if(drop) {
                        DEBUG("Connecting tuile "<<tuile->getID()
                                <<" to "<<(*itWidget)->getID());
                        (*itWidget)->highlightReal(false);
                        (*itWidget)->connectToWidget(tuile);
                    }
                    else {
                        (*itWidget)->highlightReal();
                    }
                }
            }
        }
    }
}

void TreeWidget::addTuileWidget(TuileWidget* newWidget) {
    m_tuileWidgets.push_back(newWidget);
    m_tuileWidgetMap[newWidget->getID()]=newWidget;
    refreshChildrenTuileWidgets();
    deselectAllTuileWidgets();
    newWidget->select();
    DEBUG("TreeWidget: added the tuile widget to trees");
}

TuileWidget* TreeWidget::getTuileWidget(const unsigned int& id) {
    if(m_tuileWidgetMap.find(id)!=m_tuileWidgetMap.end()) {
        return m_tuileWidgetMap[id];
    }
    else {
        return NULL;
    }
}

void TreeWidget::removeTuileWidget(TuileWidget* erasedWidget) {
    m_tuileWidgetMap.erase(erasedWidget->getID());
    remove(erasedWidget->getWidget());
    delete erasedWidget;
    m_paramGroup->clear();
    if(m_tuileWidgets.size()==0) {
        AudioManager::getInstance()->clear();
    }
}

SeqWidget* TreeWidget::createSeqWidget(TuileWidget* t1, TuileWidget* t2) {
    AudioManager* audioMan=AudioManager::getInstance();
    SeqTuile* newSTuile = audioMan->addSeqTuile();
    audioMan->insertSeq(newSTuile, t1->getTuile(), t2->getTuile());
    SeqWidget* newWidget = new SeqWidget("seq", newSTuile);
    newWidget->getWidget()->position(t1->getWidget()->x(),t1->getWidget()->y());
    addTuileWidget(newWidget);
    DEBUG("TreeWidget: created a new SeqWidget");
    return newWidget;
}


TuileWidget* TreeWidget::createTuileWidget(const std::string& tuileName) {
    //TODO create the associated tuileparamwidget
    AudioManager* audioMan=AudioManager::getInstance();
    TuileWidget* newWidget=NULL;
    Tuile* newTuile=NULL;
    if(tuileName.find(".tui")!=string::npos) {
    }
    else if(tuileName.compare("loop")==0) {
        LoopTuile* newLTuile = audioMan->addLoopTuile();
        newTuile = (Tuile*)newLTuile;
        newWidget = new LoopWidget(tuileName, newLTuile);
    }
    else if(tuileName.compare("monitor")==0) {
        MidiOscMonitorTuile* newMTuile = audioMan->addMidiOscMonitorTuile();
        newTuile = (Tuile*)newMTuile;
        newWidget = new MonitorWidget(tuileName, newMTuile);
    }
    else if(tuileName.compare("switch")==0) {
        MidiOscSwitchTuile* newSwTuile = audioMan->addMidiOscSwitchTuile();
        newTuile = (Tuile*)newSwTuile;
        newWidget = new SwitchWidget(tuileName, newSwTuile);
    }
    else if(tuileName.compare("output")==0) {
        AudioOutputTuile* newAOTuile = audioMan->addAudioOutputTuile(tuileName);
        newTuile = (Tuile*)newAOTuile;
        AudioOutputWidget* newAOWidget = 
                                new AudioOutputWidget(tuileName, newAOTuile);
        newWidget=newAOWidget;
        m_audioTuileWidgets.push_back(newAOWidget);
    }
    else if(tuileName.compare("input")==0) {
        AudioInputTuile* newAITuile = audioMan->addAudioInputTuile(tuileName);
        newTuile = (Tuile*)newAITuile;
        AudioInputWidget* newAIWidget = 
                            new AudioInputWidget(tuileName, newAITuile);
        newWidget=newAIWidget;
        m_audioTuileWidgets.push_back(newAIWidget);
    }
    else if(tuileName.find(".wav")!=string::npos) {
        SoundFileTuile* newSFTuile = audioMan->addSoundFileTuile(tuileName);
        newTuile = (Tuile*)newSFTuile;
        SoundFileWidget* newSFWidget = 
                                new SoundFileWidget(tuileName, newSFTuile);
        newWidget=newSFWidget;
        m_audioTuileWidgets.push_back(newSFWidget);
    }
    else if(tuileName.find(".dsp")!=string::npos) {
        FaustTuile* newFTuile = audioMan->addFaustTuile(tuileName);
        newTuile = (Tuile*)newFTuile;
        FaustWidget* newFWidget = new FaustWidget(tuileName, newFTuile);
        newWidget=newFWidget;
        m_audioTuileWidgets.push_back(newFWidget);
    }

    if(newTuile && newWidget) {
        addTuileWidget(newWidget);
        DEBUG("TreeWidget: created the new TuileWidget "<<tuileName);
    }
    else {
        DEBUG("TreeWidget: could not create the new TuileWidget "<<tuileName);
    }
    return newWidget;

/*

    //LOAD tree file, or single leaf 
    unsigned int newRootID;
    vector<unsigned int> newIDs;
    if(tuileName.find(".tui")!=string::npos) {
        xmlDocPtr doc = xmlReadFile(tuileName.c_str(),NULL,0);
        if(doc) {
            xmlNodePtr rootNode = xmlDocGetRootElement(doc);
            if(rootNode && string((const char*)rootNode->name)
                                    .compare("SimpleTuilesLooper")==0) {
                map<unsigned int, unsigned int> idsMap;
                xmlNodePtr curNode;
                for (curNode = rootNode->children; curNode; 
                                    curNode = curNode->next) {
                    if(string((const char*)curNode->name).compare("Tuiles")==0){
                        m_tuilesManager->loadTree(curNode, newRootID, idsMap);
                        //add tuiles widgets
                        map<unsigned int, unsigned int>::iterator itIDs;
                        itIDs=idsMap.begin();
                        for(;itIDs!=idsMap.end();++itIDs) {
                            std::string type= m_tuilesManager
                                                ->getTuileProps(itIDs->second)
                                                    ->getType();
                            if(type.compare("leaf")==0 
                                    ||  type.compare("switch")==0) {
                                newIDs.push_back(itIDs->second);
                            }
                        }
                    }
                    else if(string((const char*)curNode->name)
                                        .compare("Processes")==0) {
                        m_processesManager->load(curNode, idsMap);
                    }
                }
            }
            xmlFreeDoc(doc);
        }
    }
    else if(tuileName.compare("loop")==0) {
        //create the new tuile
        m_tuilesManager->addLoop(newRootID);
        newIDs.push_back(newRootID);
        std::string shortName = fl_filename_name(tuileName.c_str());
        m_tuilesManager->getTuileProps(newRootID)->editName()=shortName;
        //create the associated process
        Process* newProcess = m_processesManager->addProcess(newRootID, 
                                                                tuileName);
        if(newProcess) {
            m_tuilesManager->setTuileLengthInMs(newRootID, 
                                                newProcess->getLengthInMs());
        }
        else {
            cout<<"Could not load process "<<tuileName<<endl;
        }
    }
    else if(tuileName.compare("switch")==0) {
        //create the new tuile
        m_tuilesManager->addSwitch(newRootID);
        newIDs.push_back(newRootID);
        std::string shortName = fl_filename_name(tuileName.c_str());
        m_tuilesManager->getTuileProps(newRootID)->editName()=shortName;
        //create the associated process
        Process* newProcess = m_processesManager->addProcess(newRootID, 
                                                                tuileName);
        if(newProcess) {
            m_tuilesManager->setTuileLengthInMs(newRootID, 
                                                newProcess->getLengthInMs());
        }
        else {
            cout<<"Could not load process "<<tuileName<<endl;
        }
    }
    else if(tuileName.compare("monitor")==0) {
        //create the new tuile
        m_tuilesManager->addSwitch(newRootID);
        newIDs.push_back(newRootID);
        std::string shortName = fl_filename_name(tuileName.c_str());
        m_tuilesManager->getTuileProps(newRootID)->editName()=shortName;
        //create the associated process
        Process* newProcess = m_processesManager->addProcess(newRootID, 
                                                                tuileName);
        if(newProcess) {
            m_tuilesManager->setTuileLengthInMs(newRootID, 
                                                newProcess->getLengthInMs());
        }
        else {
            cout<<"Could not load process "<<tuileName<<endl;
        }
    }
    else {
        //create the new tuile
        m_tuilesManager->addLeafTuile(4, newRootID);
        newIDs.push_back(newRootID);
        std::string shortName = fl_filename_name(tuileName.c_str());
        m_tuilesManager->getTuileProps(newRootID)->editName()=shortName;
        //create the associated process
        Process* newProcess = m_processesManager->addProcess(newRootID, 
                                                                tuileName);
        if(newProcess) {
            m_tuilesManager->setTuileLengthInMs(newRootID, 
                                                newProcess->getLengthInMs());
        }
        else {
            cout<<"Could not load process "<<tuileName<<endl;
        }
    }
    
    if(op.size()==0) {
        if(m_tuilesManager->getNbTuileProps()==1) {
            cout<<"first tuile"<<endl;
            m_forkLeafID=newRootID;
        }
        else {
            unsigned int forkID;
            m_tuilesManager->addForkComposition(m_forkLeafID, newRootID, forkID);
            m_forkLeafID=newRootID;
        }
    }
    else {
        TuileProps* props = m_tuilesManager->getTuileProps(id);
        if(props) {
            unsigned int opID;
            if(op.compare("fork")==0) {
                if(props->getType().compare("switch")==0) { 
                    m_tuilesManager->addChild(newRootID, id);
                }
                else if(props->getType().compare("loop")==0) { 
                    m_tuilesManager->addChild(newRootID, id);
                }
                else {
                    m_tuilesManager->addForkComposition(id, newRootID, opID);
                }
            }
            else if(op.compare("join")==0) {
                if(props->getType().compare("switch")==0) { 
                    m_tuilesManager->addChild(newRootID, id);
                }
                else {
                    m_tuilesManager->addJoinComposition(id, newRootID, opID);
                }
            }
            else if(op.compare("seq")==0) {
                m_tuilesManager->addSequentialComposition(id, 
                                                        newRootID, opID);
            }
            else if(op.compare("leftseq")==0) {
                m_tuilesManager->addLeftSeqComposition(id, newRootID, opID);
            }
        }
	}

    vector<unsigned int>::iterator itNewIDs=newIDs.begin();
    for(;itNewIDs!=newIDs.end(); ++itNewIDs) {
        addTuileWidget(*itNewIDs);
    }

    m_tuilesManager->refreshTree();
*/
}



