/***************************************************************************
 *  MonitorWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "MonitorWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "../audio/MidiOscMonitorTuile.hpp"

using namespace std;

MonitorWidget::MonitorWidget(const std::string& name, 
                        MidiOscMonitorTuile* tuile): TuileWidget(name, tuile){}

MonitorWidget::~MonitorWidget() {}



