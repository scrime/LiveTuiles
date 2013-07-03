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

TreeWidget::TreeWidget():   Fl_Scroll(0, 0, 10, 10, ""), 
                            TuileWidgetNode(NULL),
                            m_cursorX(0), m_pixelsPerBeat(10),
                            m_offsetX(10), m_offsetY(10), m_magnetSize(5),
                            m_zeroPosX(0), m_startLoopPosX(0), m_loopW(0),
                            m_selectedTuileWidget(NULL),
                            m_overWidget(NULL), m_overWidgetPart(0),
                            m_draggedWidget(NULL), m_draggedWidgetPart(0), 
                            m_nbInternalOutputs(0), m_nbExternalOutputs(0), 
                            m_nbExternalInputs(0) {

    m_id=-1;
    type(Fl_Scroll::BOTH);

	end();
}

TreeWidget::~TreeWidget() {}

TreeWidget* TreeWidget::getInstance() {
    static TreeWidget instance;
    return &instance;
}

void TreeWidget::update() {
    //update cursor
    m_cursorX = m_zeroPosX + 
                    (AudioManager::getInstance()->getPlayPositionInBeats()) 
                    *m_pixelsPerBeat;
/*
    //check for score modifications
    if(m_tuilesManager->checkTuilesPropsUpdate()) {
        refreshTuiles();
    }
    //check for processes modifications
    if(m_processesManager->checkProcessesUpdate()) {
        vector<string> names;
        vector<int> ids;
        m_processesManager->getProcessesWithAudioOutputs(names, ids);        
        list<TuileWidget*>::const_iterator itWidget=m_tuileWidgets.begin();
        for(; itWidget!=m_tuileWidgets.end(); ++itWidget) {
            (*itWidget)->getParamWidget()->setInputList(names, ids);
        }
        
    }
*/
}

void TreeWidget::zoom(const float& zoom) {
    m_pixelsPerBeat=10.0+zoom*100;
    refreshTuiles();
}

void TreeWidget::draw() {
    Fl_Scroll::draw();

	//box
	fl_draw_box(FL_DOWN_BOX, x(), y(), w(), h(), FL_BACKGROUND2_COLOR);
	fl_push_clip(x()+2, y()+2, w()-4, h()-4);

	//beat lines
	fl_color(FL_WHITE);
	for(int i=x()+m_offsetX; i<x()+w(); i+=m_pixelsPerBeat) {
		fl_line(i, y()+m_offsetY, i, y()+h()-m_offsetY);
	}

	//children
	//draw_children();
    vector<TuileWidgetNode*>::iterator itWidget=m_childrenWidgets.begin();
    for(; itWidget!=m_childrenWidgets.end(); ++itWidget) {
        //(*itWidget)->drawConnections();
        (*itWidget)->draw();
    }


/*
    //connections between tuiles 
    list<TuileWidget*>::const_iterator itWidget=m_tuileWidgets.begin();
    for(; itWidget!=m_tuileWidgets.end(); ++itWidget) {
        (*itWidget)->drawConnections();
    }
*/
    //cursor
	fl_color(FL_RED);
    fl_line(m_offsetX+m_cursorX, y()+m_offsetY, 
            m_offsetX+m_cursorX, y()+h()-m_offsetY);

	fl_pop_clip();
}

void TreeWidget::refreshTuiles() {
    //update all widgets 
    m_zeroPosX=0;
    m_startLoopPosX=0;
    m_loopW=0;

/*
    //update tuiles positions 
    int yPos=1;
    list<TuileWidget*>::const_iterator itWidget=m_tuileWidgets.begin();
    for(;itWidget!=m_tuileWidgets.end(); ++itWidget, yPos+=2) {
        (*itWidget)->refresh(-m_zeroPosX+x()+m_offsetX, y(), 
                             m_pixelsPerBeat, yPos);
    }
*/
}

int TreeWidget::handle(int event) {
    //bool needRefresh=false;

/*
    switch(event) { 
        case FL_DRAG: { 
            if(m_draggedWidget) {
                m_draggedWidget->drag(Fl::event_x(), Fl::event_y());
                switch(m_draggedWidgetPart) {
                    case 0: {
                        float draggedOff=float(Fl::event_x()-m_draggingStartX)
                                            /float(m_pixelsPerBeat);
                        float newOffset = 
                                (m_draggedTuileInitLeftOffset+draggedOff);
                        m_tuilesManager->setTuileLeftOffset(
                                         m_draggedWidget->getID(), newOffset);
                        m_draggedWidget->highlightSyncInLine();
                        needRefresh=true;
                    }break;
                    case 2: {
                        float draggedOff=-float(Fl::event_x()-m_draggingStartX)
                                            /float(m_pixelsPerBeat);
                        float newOffset = 
                                (m_draggedTuileInitRightOffset+draggedOff);
                        m_tuilesManager->setTuileRightOffset(
                                         m_draggedWidget->getID(), newOffset);
                        needRefresh=true;
                    }break;
                    default: {
                        float draggedOff=float(m_draggingStartX - Fl::event_x())
                                            /float(m_pixelsPerBeat);
                        float newOffset = 
                                (m_draggedTuileInitLeftOffset+draggedOff);
                        m_tuilesManager->setTuileLeftOffset(
                                         m_draggedWidget->getID(), newOffset);
                        m_draggedWidget->highlightSyncInLine();
                        float draggedOffY=float(m_draggingStartY-Fl::event_y())
                                            /float(m_pixelsPerBeat);
                        float newLength = m_draggedTuileInitLength+draggedOffY;
                        m_tuilesManager->setTuileLength(m_overWidget->getID(), 
                                                        newLength);
                        refreshTuiles();
                    }
                }
            }
            else {
               return Fl_Scroll::handle(event); 
            }
            return 1;
        }break;
        case FL_MOUSEWHEEL: {
           return Fl_Scroll::handle(event); 
        }break;
        case FL_MOVE: {
            m_overWidget=NULL;
            //get the clicked tuile widget
            list<TuileWidget*>::const_iterator itWidget=
                m_tuileWidgets.begin();
            for(;itWidget!=m_tuileWidgets.end(); ++itWidget) {
                if(Fl::event_y()>(*itWidget)->y() - (*itWidget)->h()/2 
                    && Fl::event_y()<
                        (*itWidget)->y()+3*(*itWidget)->h()/2) {

                    if(Fl::event_x() 
                            >(*itWidget)->getSyncIn()-m_magnetSize 
                        && Fl::event_x() 
                            < (*itWidget)->getSyncIn()+m_magnetSize) {

                        m_overWidget=*itWidget;
                        m_overWidgetPart=0;
                        (*itWidget)->highlightSyncInLine();
                    }
                    else if(Fl::event_x()
                                >(*itWidget)->getSyncOut()-m_magnetSize
                            && Fl::event_x()
                                <(*itWidget)->getSyncOut()+m_magnetSize) {
                        m_overWidget=*itWidget;
                        m_overWidgetPart=2;
                        (*itWidget)->highlightSyncOutLine();
                    }
                    else if(Fl::event_x()>(*itWidget)->x()
                            && Fl::event_x()
                                <(*itWidget)->x()+(*itWidget)->w()) {
                        m_overWidget=*itWidget;
                        m_overWidgetPart=1;
                        (*itWidget)->highlightReal();
                    }
                }
                else {
                    (*itWidget)->resetHighlight();
                }
            }
            if(m_overWidget==NULL) {
               return Fl_Scroll::handle(event); 
            }
            return 1;
        }break;
        case FL_PUSH: {
            if(m_overWidget) {
                if(m_selectedTuileWidget) {
                    m_selectedTuileWidget->deselect();
                }
                m_selectedTuileWidget=m_overWidget;
                m_selectedTuileWidget->select();
                m_draggedWidget=m_overWidget;
                m_draggedWidgetPart=m_overWidgetPart;
                m_draggingStartX=Fl::event_x();
                m_draggingStartY=Fl::event_y();
                TuileProps *props = 
                    m_tuilesManager->getTuileProps(m_draggedWidget->getID());
                m_draggedTuileInitLeftOffset=props->getLeftOffset();
                m_draggedTuileInitRightOffset=props->getRightOffset();
                m_draggedTuileInitLength=props->getLength();
                switch(m_draggedWidgetPart) {
                    case 0: {
                        m_draggedWidget->highlightSyncInLine();
                    }break;
                    case 1: {
                        m_draggedWidget->highlightReal();
                    }break;
                    case 2: {
                        m_draggedWidget->highlightSyncOutLine();
                    }break;
                }
            }
            else {
                    return Fl_Scroll::handle(event); 
            }
            return Fl_Scroll::handle(event); 
        }break;
        case FL_RELEASE: {
            m_movingLoop=false;
            m_overWidget=NULL;
            m_draggedWidget=NULL;
            return Fl_Scroll::handle(event); 
        }break;
        case FL_KEYUP: {
            switch(Fl::event_key()) {
                case FL_Control_R: 
                case FL_Control_L: {
                    m_connectingTuiles=false;
                }break;
                default:break;
            }
        }break;
        case FL_KEYDOWN: {
            switch(Fl::event_key()) {
                case FL_BackSpace: {
                    //TODO remove selectedTuile
                    return 1;
                }break;
                case FL_Control_R: 
                case FL_Control_L: {
                    m_connectingTuiles=true;
                    return 1;
                }break;
                case 32: {
                    MainWindow::getInstance()->togglePlay();
                }break;
                default:break;
            }
            return Fl_Scroll::handle(event); 
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
*/
    switch(event) { 
        case FL_KEYDOWN: {
            switch(Fl::event_key()) {
                case FL_BackSpace: {
                    //TODO remove selectedTuile
                    return 1;
                }break;
                case 32: {
                    MainWindow::getInstance()->togglePlay();
                }break;
                default:break;
            }
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


void TreeWidget::getMagnetizedPositionAndTuile( const int& inX, const int& inY, 
                                                const int& inW, 
                                                int& outX, int& outY, 
                                                bool& drop,
                                                const std::string& tuileName) {
    //default values
    outX=inX;
    outY=inY;
    bool outDrop=false;

    //check against widgets: syncIn and syncOut
	list<TuileWidget*>::const_iterator itWidget=m_tuileWidgets.begin();
	for(;itWidget!=m_tuileWidgets.end(); ++itWidget) {
        (*itWidget)->resetHighlight(); 

        if(inY>(*itWidget)->y() - (*itWidget)->h()/2 
                && inY<(*itWidget)->y() + 3*(*itWidget)->h()/2) {
            //fork
            if(fabs((*itWidget)->getSyncIn()-inX)<m_magnetSize) {
                outX=(*itWidget)->getSyncIn();
                (*itWidget)->highlightSyncInLine(); 
                if(drop) {
                    (*itWidget)->tryForkWithTuile(tuileName);
                    outDrop=true;
                }
            }
            //seq existing-new
            else if(fabs((*itWidget)->getSyncOut()-inX)<m_magnetSize) {
                outX=(*itWidget)->getSyncOut();
                (*itWidget)->highlightSyncOutLine(); 
                if(drop) {
					(*itWidget)->trySeqWithTuile(tuileName);
                    outDrop=true;
                }
            } 
            //seq new-existing
            else if(fabs((*itWidget)->getSyncIn()-(inX+inW))<m_magnetSize) {
                outX=(*itWidget)->getSyncIn()-inW;
                (*itWidget)->highlightSyncInLine(); 
                if(drop) {
					(*itWidget)->tryLeftSeqWithTuile(tuileName);
                    outDrop=true;
                }
            } 
            //join
            else if(fabs((*itWidget)->getSyncOut()-(inX+inW))<m_magnetSize) {
                outX=(*itWidget)->getSyncOut()-inW;
                (*itWidget)->highlightSyncOutLine(); 
                if(drop) {
					(*itWidget)->tryJoinWithTuile(tuileName);
                    outDrop=true;
                }
            } 
            else if(inX>(*itWidget)->x() 
                        && inX<(*itWidget)->x()+(*itWidget)->w()) {
                (*itWidget)->highlightReal(); 
                if(drop) {
					(*itWidget)->tryAddTuileChild(tuileName);
                    outDrop=true;
                }
            } 
        }
    }

	//drop without link to other tuiles
    if(inX>x() && inY<x()+w() && inY>y() && inY<y()+h()) {
        if(drop && !outDrop) {
            TuileWidget* newWidget = createTuileWidget(tuileName);
            if(newWidget) {
                addTuileWidget(newWidget);
            }
        }
    }
}

void TreeWidget::addTuileWidget(TuileWidget* newWidget) {
    m_tuileWidgets.push_back(newWidget);
    this->add(m_tuileWidgets.back());
    

/*
    if(m_tuilesManager->getTuileProps(id)) {
        std::string shortName = m_tuilesManager->getTuileProps(id)->getName();
        TuileParamWidget* newParamWidget 
            = m_paramGroup->createParamWidget(id, shortName);
        begin();
            TuileWidget* newWidget = new TuileWidget(id,shortName);
        end();
        newWidget->setParamWidgetAndGroup(newParamWidget, m_paramGroup);
        m_tuileWidgets.push_back(newWidget);
        if(m_selectedTuileWidget) {
            m_selectedTuileWidget->deselect();
        }
        m_selectedTuileWidget=newWidget;
        newWidget->select();
    }
*/
}

void TreeWidget::removeTuileWidget(TuileWidget* widget) {
    removeTuileWidget(widget->getID());
}

void TreeWidget::removeTuileWidget(const unsigned int& id) {
    TuileWidget* erasedWidget=NULL;
    list<TuileWidget*>::iterator itWidget=m_tuileWidgets.begin();
    for(;itWidget!=m_tuileWidgets.end();) {
        if((*itWidget)->getID()==id) {
            erasedWidget = (*itWidget);
            itWidget=m_tuileWidgets.erase(itWidget);
        }
        else {
            itWidget++;
        }
    }
    if(erasedWidget) {
        remove(erasedWidget);
        delete erasedWidget;
        m_paramGroup->clear();
    }
    if(m_tuileWidgets.size()==0) {
        AudioManager::getInstance()->clear();
    }
}

SeqWidget* TreeWidget::createSeqWidget() {
    AudioManager* audioMan=AudioManager::getInstance();
    SeqTuile* newSTuile = audioMan->addSeqTuile();
    SeqWidget* newWidget = new SeqWidget("seq", newSTuile);
    m_childrenWidgets.push_back(newWidget);
    newWidget->setParent(this);
    DEBUG("TreeWidget: created a new SeqWidget");
    return newWidget;
}


TuileWidget* TreeWidget::createTuileWidget(const std::string& tuileName) {
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
        newWidget = new AudioOutputWidget(tuileName, newAOTuile);
    }
    else if(tuileName.compare("input")==0) {
        AudioInputTuile* newAITuile = audioMan->addAudioInputTuile(tuileName);
        newTuile = (Tuile*)newAITuile;
        newWidget = new AudioInputWidget(tuileName, newAITuile);
    }
    else if(tuileName.find(".wav")!=string::npos) {
        SoundFileTuile* newSFTuile = audioMan->addSoundFileTuile(tuileName);
        newTuile = (Tuile*)newSFTuile;
        newWidget = new SoundFileWidget(tuileName, newSFTuile);
    }
    else if(tuileName.find(".dsp")!=string::npos) {
        FaustTuile* newFTuile = audioMan->addFaustTuile(tuileName);
        newTuile = (Tuile*)newFTuile;
        newWidget = new FaustWidget(tuileName, newFTuile);
    }

    if(newWidget) {
        m_childrenWidgets.push_back(newWidget);
        newWidget->setParent(this);
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



