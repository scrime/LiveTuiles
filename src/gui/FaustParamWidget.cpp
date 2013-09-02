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

FaustParamWidget::FaustParamWidget(TuileWidget* widget, FaustTuile* faustTuile):
                                AudioTuileParamWidget(widget, faustTuile),
                                m_faustTuile(faustTuile) {
    load();
    end();
}

FaustParamWidget::~FaustParamWidget() {}

void FaustParamWidget::load() {
    if(m_faustTuile->getDSP()) {
        m_faustTuile->getDSP()->buildUserInterface(this);
    }
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

