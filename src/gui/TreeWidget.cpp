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

#include "MainWindow.hpp"
#include "TuileWidget.hpp"
#include "SoundFileWidget.hpp"
#include "FaustWidget.hpp"
#include "AudioInputWidget.hpp"
#include "AudioOutputWidget.hpp"

#include "TuileParamGroup.hpp"

using namespace std;

TreeWidget::TreeWidget(int x, int y, int w, int h):
								Fl_Scroll(x, y, w, h, ""), 
								m_cursorX(0), m_pixelsPerBeat(10),
                                m_offsetX(10), m_offsetY(10), m_magnetSize(5),
                                m_zeroPosX(0), m_startLoopPosX(0), m_loopW(0),
                                m_selectedTuileWidget(NULL),
                                m_overWidget(NULL), m_overWidgetPart(0),
                                m_draggedWidget(NULL), m_draggedWidgetPart(0), 
                                m_movingLoop(false),
                                m_nbInternalOutputs(0), m_nbExternalOutputs(0), 
                                m_nbExternalInputs(0) {


    type(Fl_Scroll::BOTH);

	end();
}

TreeWidget::~TreeWidget() {}

void TreeWidget::update() {
/*
    //update cursor
    m_cursorX = m_zeroPosX + (m_tuilesManager->getPlayPosition()) 
                                *m_pixelsPerBeat;
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
	draw_children();
	fl_pop_clip();
}

void TreeWidget::refreshTuiles() {
    //update all widgets 
    m_zeroPosX=0;
    m_startLoopPosX=0;
    m_loopW=0;

    //update tuiles positions 
    int yPos=1;
    list<TuileWidget*>::const_iterator itWidget=m_tuileWidgets.begin();
    for(;itWidget!=m_tuileWidgets.end(); ++itWidget, yPos+=2) {
        (*itWidget)->refresh(-m_zeroPosX+x()+m_offsetX, y(), 
                             m_pixelsPerBeat, yPos);
    }
}

int TreeWidget::handle(int event) {
    //bool needRefresh=false;

    switch(event) { 
        case FL_DRAG: { 
            if(m_draggedWidget) {
                m_draggedWidget->drag(Fl::event_x(), Fl::event_y());
/*
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
*/
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
/*
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
*/
            return Fl_Scroll::handle(event); 
            return 1;
        }break;
        case FL_RELEASE: {
            m_movingLoop=false;
            m_overWidget=NULL;
            m_draggedWidget=NULL;
            return Fl_Scroll::handle(event); 
        }break;
        case FL_KEYDOWN: {
            switch(Fl::event_key()) {
                case FL_BackSpace: {
                    //TODO remove selectedTuile
                    return 1;
                }break;
                case 32: {
                    AudioManager::getInstance()->togglePlay();
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
    return Fl_Scroll::handle(event);
}


void TreeWidget::getMagnetizedPositionAndTuile(const int& inX, const int& inY, 
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
					dropTuile(tuileName, (*itWidget)->getID(),"fork");
                    outDrop=true;
                }
            }
            //seq existing-new
            else if(fabs((*itWidget)->getSyncOut()-inX)<m_magnetSize) {
                outX=(*itWidget)->getSyncOut();
                (*itWidget)->highlightSyncOutLine(); 
                if(drop) {
					dropTuile(tuileName, (*itWidget)->getID(),"seq");
                    outDrop=true;
                }
            } 
            //seq new-existing
            else if(fabs((*itWidget)->getSyncIn()-(inX+inW))<m_magnetSize) {
                outX=(*itWidget)->getSyncIn()-inW;
                (*itWidget)->highlightSyncInLine(); 
                if(drop) {
					dropTuile(tuileName, (*itWidget)->getID(),"leftseq");
                    outDrop=true;
                }
            } 
            //join
            else if(fabs((*itWidget)->getSyncOut()-(inX+inW))<m_magnetSize) {
                outX=(*itWidget)->getSyncOut()-inW;
                (*itWidget)->highlightSyncOutLine(); 
                if(drop) {
					dropTuile(tuileName, (*itWidget)->getID(),"join");
                    outDrop=true;
                }
            } 
        }
    }

	//drop without link to other tuiles
    if(inX>x() && inY<x()+w() && inY>y() && inY<y()+h()) {
        if(drop && !outDrop) {
            dropTuile(tuileName, 0, "");
        }
    }
}

void TreeWidget::addTuileWidget(const unsigned int& id) {
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
/*
    if(m_tuileWidgets.size()==0) {
        SimpleTuilesLooper::getInstance()->clearAll();
    }
*/
}

void TreeWidget::dropTuile(const std::string& tuileName, 
							const unsigned int& id, const std::string& op) {
    AudioManager* audioMan=AudioManager::getInstance();
    if(tuileName.find(".tui")!=string::npos) {
    }
    else if(tuileName.compare("loop")==0) {
    }
    else if(tuileName.compare("monitor")==0) {
    }
    else if(tuileName.compare("switch")==0) {
    }
    else if(tuileName.compare("output")==0) {
        m_tuileWidgets.push_back(
            new AudioOutputWidget(tuileName, 
                                audioMan->addAudioOutputTuile(tuileName)));
        this->add(m_tuileWidgets.back());
    }
    else if(tuileName.compare("input")==0) {
        m_tuileWidgets.push_back(
            new AudioInputWidget(tuileName, 
                                audioMan->addAudioInputTuile(tuileName)));
        this->add(m_tuileWidgets.back());
    }
    else if(tuileName.find(".wav")!=string::npos) {
        m_tuileWidgets.push_back(
            new SoundFileWidget(tuileName, 
                                audioMan->addSoundFileTuile(tuileName)));
        this->add(m_tuileWidgets.back());
    }
    else if(tuileName.find(".dsp")!=string::npos) {
        m_tuileWidgets.push_back(
            new FaustWidget(tuileName, 
                            audioMan->addFaustTuile(tuileName)));
        this->add(m_tuileWidgets.back());
    }

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



