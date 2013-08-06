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

using namespace std;
using namespace tuiles;

TuileParamWidget::TuileParamWidget(Tuile* tuile):
                                                HitPack(0, 0, 100, 100, ""), 
                                                m_tuile(tuile) {
    box(FL_FLAT_BOX);
    color(fl_rgb_color(70,90,70));
    type(Fl_Pack::VERTICAL);
    internalSpacing(15);
    externalSpacing(5);
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
    m_lengthInput->bounds(0, 100000);
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

    add(m_tuilePack);

    updateTuileProps();
}

TuileParamWidget::~TuileParamWidget() {}

void TuileParamWidget::update() {

}


void TuileParamWidget::cbTuileInputs(Fl_Widget* w) {
/*
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


