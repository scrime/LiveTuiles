/***************************************************************************
 *  FaustParamWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "FaustParamWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "../audio/FaustTuile.hpp"

using namespace std;


FaustParamWidget::FaustParamWidget(FaustTuile* faustTuile):
                                TuileParamWidget(faustTuile),
                                m_faustTuile(faustTuile) {
    
    m_connectPack = new Fl_Pack(0,0,50,20,"");
    m_connectPack->type(Fl_Pack::HORIZONTAL);
    m_connectPack->end();
    m_outputChoice = new Fl_Choice(0, 0, 100, 20, "Output");
    m_outputChoice->align(FL_ALIGN_LEFT|FL_ALIGN_TOP);
    m_outputChoice->callback(statConnectChoice,this);
    m_outputChoice->add("Internal+External", "", NULL); 
    m_outputChoice->add("Internal only", "", NULL); 
    m_connectPack->add(m_outputChoice);

    m_inputChoice = new Fl_Choice(0, 0, 100, 20, "Input");
    m_inputChoice->align(FL_ALIGN_LEFT|FL_ALIGN_TOP);
    m_inputChoice->callback(statConnectChoice,this);
    m_inputChoice->add("External", "", NULL); 
    m_connectPack->add(m_inputChoice);

    m_pack->add(m_connectPack);

    end();
}

FaustParamWidget::~FaustParamWidget() {}

void FaustParamWidget::init() {
/*
    JackProcessesManager* procMan = JackProcessesManager::getInstance();
    FaustProcess* fp = static_cast<FaustProcess*>(procMan->getProcess(m_id));
    if(fp) {
        fp->getDSP()->buildUserInterface(this);
    }
    else {
        cout<<"Could not add Faust param widget"<<endl;
    }   
*/
}

void FaustParamWidget::cbParamVal(Fl_Valuator* val) {
    if(m_paramsMap.find(std::string(val->label()))!=m_paramsMap.end()) {
        *(m_paramsMap[std::string(val->label())]) = val->value();
    }
}

void FaustParamWidget::cbParamBut(Fl_Button* but) {
    if(m_paramsMap.find(std::string(but->label()))!=m_paramsMap.end()) {
        *(m_paramsMap[std::string(but->label())]) = but->value();
    }
}

void FaustParamWidget::cbConnectChoice(Fl_Widget* w) {
/*
    JackProcessesManager* pMan = JackProcessesManager::getInstance();
    if(m_inputChoice->contains(w)) {
        pMan->setProcessInput(m_id, m_inputIDs[m_inputChoice->value()]);
    }
    else if(m_outputChoice->contains(w)) {
        pMan->setProcessOutput(m_id, m_outputChoice->value());
        cout<<m_outputChoice->value()<<endl;
    }
*/
}

