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
#include "TuileParamGroup.hpp"
#include "TuileWidget.hpp"

#include <tuiles/Tuile.hpp>
#include "../audio/AudioManager.hpp"

using namespace std;
using namespace tuiles;

TuileParamWidget::TuileParamWidget(TuileWidget* widget, 
                                    Tuile* tuile): 
                                            Fl_Group(0, 0, 100, 100, ""), 
                                            Observer(),
                                            m_tuile(tuile),
                                            m_tuileWidget(widget) {
    clip_children(1);
    box(FL_FLAT_BOX);
    color(fl_rgb_color(70, 90, 70));
    end();

    m_pack = new HitPack(0, 0, w(), h(), "");
    m_pack->type(Fl_Pack::VERTICAL);
    m_pack->internalSpacing(15);
    m_pack->externalXSpacing(10);
    m_pack->externalYSpacing(10);

    m_tuilePack = new HitPack(0, y(), w(), 30, "");
    m_tuilePack->end();
    m_tuilePack->type(Fl_Pack::HORIZONTAL);
    m_tuilePack->internalSpacing(10);
    m_tuilePack->externalXSpacing(5);
    m_tuilePack->externalYSpacing(5);

    m_nameInput = new Fl_Input(0, 0, 120, 20, "Name");
    m_nameInput->labelsize(12);
    m_nameInput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    m_nameInput->callback(statTuileInputs,this);
    m_nameInput->when(FL_WHEN_CHANGED);
    m_lengthInput = new Fl_Value_Input(0, 0, 50, 20, "Length");
    m_lengthInput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    m_lengthInput->callback(statTuileInputs,this);
    m_lengthInput->labelsize(12);
    m_lengthInput->step(0.001);
    m_lengthInput->bounds(0, 100000000);
    m_lengthInput->soft(0);
    m_lOffsetInput = new Fl_Value_Input(0, 0, 60, 20, "Left Offset");
    m_lOffsetInput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    m_lOffsetInput->callback(statTuileInputs,this);
    m_lOffsetInput->labelsize(12);
    m_lOffsetInput->step(0.001);
    m_lOffsetInput->soft(0);
    m_lOffsetInput->bounds(-100000000, 1000000000);
    m_rOffsetInput = new Fl_Value_Input(0, 0, 60, 20, "Right Offset");
    m_rOffsetInput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    m_rOffsetInput->callback(statTuileInputs,this);
    m_rOffsetInput->labelsize(12);
    m_rOffsetInput->step(0.001);
    m_rOffsetInput->soft(0);
    m_rOffsetInput->bounds(-100000000, 100000000);
    m_removeButton = new Fl_Button(0,0,60,20,"Remove");
    m_removeButton->callback(statTuileInputs,this);
    m_tuilePack->add(m_nameInput);
    m_tuilePack->add(m_lengthInput);
    m_tuilePack->add(m_lOffsetInput);
    m_tuilePack->add(m_rOffsetInput);
    m_tuilePack->add(m_removeButton);

    m_pack->add(m_tuilePack);
    add(m_pack);

    if(m_tuile) {
        m_tuile->addObserver(this);
    }
}

TuileParamWidget::~TuileParamWidget() {}

void TuileParamWidget::update() {}

void TuileParamWidget::cbTuileInputs(Fl_Widget* w) {
    if(m_tuile) {
        float fpb=AudioManager::getInstance()->getFramesPerBeat();
        if(m_nameInput->contains(w)) {
            m_tuile->setName(std::string(m_nameInput->value()));        
        }
        else if(m_lengthInput->contains(w)) {
            m_tuile->setLength(m_lengthInput->value()*fpb);
        }
        else if(m_lOffsetInput->contains(w)) {
            m_tuile->setLeftOffset(m_lOffsetInput->value()*fpb);
        }
        else if(m_rOffsetInput->contains(w)) {
            m_tuile->setRightOffset(m_rOffsetInput->value()*fpb);
        }
        else if(m_removeButton->contains(w)) {
            m_tuileWidget->extract();
        }
    }
    TuileParamGroup::getInstance()->redraw();
}

void TuileParamWidget::notifyUpdate() {
    if(m_tuile) {
        float fpb=AudioManager::getInstance()->getFramesPerBeat();
        m_nameInput->value(m_tuile->getName().c_str());
        m_lengthInput->value(m_tuile->getLength()/fpb);
        m_lOffsetInput->value(m_tuile->getLeftOffset()/fpb);
        m_rOffsetInput->value(m_tuile->getRightOffset()/fpb);
    }
    TuileParamGroup::getInstance()->redraw();
}



