/***************************************************************************
 *  SwitchWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "SwitchWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "../audio/MidiOscSwitchTuile.hpp"

using namespace std;

SwitchWidget::SwitchWidget(const std::string& name, 
                        MidiOscSwitchTuile* tuile): TuileWidget(name, tuile){
    m_canTakeInput=false;
}

SwitchWidget::~SwitchWidget() {}



