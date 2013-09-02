/***************************************************************************
 *  SwitchParamWidget.cpp
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include <iostream>
#include <math.h>
#include <cassert>

#include "SwitchParamWidget.hpp"

#include "HitPack.hpp"
#include "../audio/MidiOscSwitchTuile.hpp"

using namespace std;

SwitchParamWidget::SwitchParamWidget(TuileWidget* widget, 
                                        MidiOscSwitchTuile* switchTuile):
                                            TuileParamWidget(widget,
                                                             switchTuile),
                                            m_switchTuile(switchTuile) {

    m_selectInput = new Fl_Simple_Counter(0, 0, 50, 20, "Selected Child");
    m_selectInput->align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    m_selectInput->callback(statTuileSwitchInputs,this);
    m_selectInput->labelsize(12);
    m_selectInput->step(1);
    m_selectInput->bounds(0,10);
    Fl_Group *tmpGroup = new Fl_Group(0,0,80,20,"");
    tmpGroup->end();
    tmpGroup->add(m_selectInput);
    tmpGroup->resizable(false);
    add(tmpGroup);
    end();
}

SwitchParamWidget::~SwitchParamWidget() {}

void SwitchParamWidget::update() {

}

void SwitchParamWidget::cbTuileSwitchInputs(Fl_Widget* w) {
/*
    TuilesManager* tMan = TuilesManager::getInstance();
    tMan->setSwitchTuileSelection(m_id, m_selectInput->value());
*/
}

