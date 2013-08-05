/***************************************************************************
 *  TuileParamWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include <iostream>
#include <math.h>
#include <cassert>

#include "TuileParamWidget.hpp"

#include <tuiles/Tuile.hpp>

#include "HitPack.hpp"

using namespace std;
using namespace tuiles;

TuileParamWidget::TuileParamWidget(Tuile* tuile):Fl_Scroll(0, 0, 100, 100, ""), 
                                                  m_tuile(tuile) {
    box(FL_DOWN_BOX);
    color(fl_rgb_color(70,90,70));

    type(VERTICAL_ALWAYS);

    m_pack = new HitPack(0,0,w()-30,h(),"");
    m_pack->end();
    m_pack->type(Fl_Pack::VERTICAL);
    m_pack->internalSpacing(15);
    m_pack->externalSpacing(5);
    end();

    m_tuilePack = new HitPack(0, y(), w(), 40, "");
    m_tuilePack->end();
    m_tuilePack->type(Fl_Pack::HORIZONTAL);
    m_tuilePack->internalSpacing(10);
    m_tuilePack->externalSpacing(10);

    m_nameInput = new Fl_Input(0, 0, 120, 20, "Name");
    m_nameInput->labelsize(12);
    m_nameInput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    m_nameInput->callback(statTuileInputs,this);
    m_nameInput->deactivate();
    m_lengthInput = new Fl_Value_Input(0, 0, 50, 20, "Length");
    m_lengthInput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    m_lengthInput->callback(statTuileInputs,this);
    m_lengthInput->labelsize(12);
    m_lengthInput->step(0.1);
    m_lengthInput->bounds(0,100000);
    m_lengthInput->soft(0);
    m_lOffsetInput = new Fl_Value_Input(0, 0, 60, 20, "Left Offset");
    m_lOffsetInput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    m_lOffsetInput->callback(statTuileInputs,this);
    m_lOffsetInput->labelsize(12);
    m_lOffsetInput->step(0.1);
    m_lOffsetInput->soft(0);
    m_lOffsetInput->bounds(-100000,100000);
    m_rOffsetInput = new Fl_Value_Input(0, 0, 60, 20, "Right Offset");
    m_rOffsetInput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    m_rOffsetInput->callback(statTuileInputs,this);
    m_rOffsetInput->labelsize(12);
    m_rOffsetInput->step(0.1);
    m_rOffsetInput->soft(0);
    m_rOffsetInput->bounds(-100000,100000);
    m_removeButton = new Fl_Button(0,0,60,20,"Remove");
    m_removeButton->callback(statTuileInputs,this);
    m_tuilePack->add(m_nameInput);
    m_tuilePack->add(m_lengthInput);
    m_tuilePack->add(m_lOffsetInput);
    m_tuilePack->add(m_rOffsetInput);
    m_tuilePack->add(m_removeButton);

    m_pack->add(m_tuilePack);

    updateTuileProps();
}

TuileParamWidget::~TuileParamWidget() {}

void TuileParamWidget::update() {

}


void TuileParamWidget::cbTuileInputs(Fl_Widget* w) {
/*
    TuilesManager* tMan = TuilesManager::getInstance();
    if(m_nameInput->contains(w)) {
    }
    else if(m_lengthInput->contains(w)) {
        tMan->setTuileLength(m_id, m_lengthInput->value());
    }
    else if(m_lOffsetInput->contains(w)) {
        tMan->setTuileLeftOffset(m_id, m_lOffsetInput->value());
    }
    else if(m_rOffsetInput->contains(w)) {
        tMan->setTuileRightOffset(m_id, m_rOffsetInput->value());
    }
    else if(m_removeButton->contains(w)) {
        SimpleTuilesLooper::getInstance()->removeTuile(m_id);
    }
*/
}


/*
void TuileParamWidget::setBuffers(const vector<Buffer*>& intOutBufs, 
                                    const vector<Buffer*>& extInpBufs, 
                                    const vector<Buffer*>& extOutBufs) {
    m_inputBufferList->clear();
    m_outputBufferList->clear();
    vector<Buffer*>::const_iterator itBuf;
    for(itBuf=intOutBufs.begin(); itBuf!=intOutBufs.end(); ++itBuf) {
        m_inputBufferList->add(fl_filename_name((*itBuf)->getName().c_str()),
                                    0, NULL, (void*)*itBuf);
    }
    for(itBuf=extInpBufs.begin(); itBuf!=extInpBufs.end(); ++itBuf) {
        m_inputBufferList->add(fl_filename_name((*itBuf)->getName().c_str()),
                                    0, NULL, (void*)*itBuf);
    }
    m_outputBufferList->add("Internal Only", 0, NULL, NULL);
    for(itBuf=extOutBufs.begin(); itBuf!=extOutBufs.end(); ++itBuf) {
        m_outputBufferList->add(fl_filename_name((*itBuf)->getName().c_str()),
                                    0, NULL, (void*)*itBuf);
    }

    //set current values
    if(m_process->getInputBuffer()) {
        m_inputBufferList->value(
            m_inputBufferList->find_item(
            fl_filename_name(m_process->getInputBuffer()->getName().c_str())));
    }

    if(m_process->getOutputBuffer()) {
        m_outputBufferList->value(
            m_outputBufferList->find_item(
            fl_filename_name(m_process->getOutputBuffer()->getName().c_str())));
    }
    else {
        m_outputBufferList->value(
            m_outputBufferList->find_item("Internal Only"));
    }
}
*/

void TuileParamWidget::updateTuileProps() {
/*
    TuilesManager* tMan = TuilesManager::getInstance();
    TuileProps* tuileProps = tMan->getTuileProps(m_id);
	if(tuileProps) {
        m_nameInput->value(tuileProps->getName().c_str());
		m_lengthInput->value(tuileProps->getLength());
		m_lOffsetInput->value(tuileProps->getLeftOffset());
		m_rOffsetInput->value(tuileProps->getRightOffset());
    }
*/
}


