/***************************************************************************
 *  SoundFileWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "SoundFileWidget.hpp"
#include "../audio/SoundFileTuile.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

using namespace std;
using namespace tuiles;

SoundFileWidget::SoundFileWidget(const std::string& name, 
                                SoundFileTuile* tuile): 
                                AudioTuileWidget(name, tuile) {
    m_canTakeInput=false;
}

SoundFileWidget::~SoundFileWidget() {}


