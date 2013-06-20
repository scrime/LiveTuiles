/***************************************************************************
 *  SoundFileWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "SoundFileWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

using namespace std;

SoundFileWidget::SoundFileWidget(const std::string& name, 
                                SoundFileTuile* tuile): 
                                TuileWidget(name, (Tuile*)tuile){}

SoundFileWidget::~SoundFileWidget() {}


