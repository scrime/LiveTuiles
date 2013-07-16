/***************************************************************************
 *  FaustWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "FaustWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "../audio/FaustTuile.hpp"

using namespace std;

FaustWidget::FaustWidget(const std::string& name, 
                            FaustTuile* tuile): AudioTuileWidget(name, tuile){}

FaustWidget::~FaustWidget() {}



