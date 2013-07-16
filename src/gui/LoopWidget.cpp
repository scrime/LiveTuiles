/***************************************************************************
 *  LoopWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "LoopWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include <tuiles/LoopTuile.hpp>

using namespace std;
using namespace tuiles;

LoopWidget::LoopWidget(const std::string& name, 
                        LoopTuile* tuile): TuileWidget(name, tuile){
    m_canTakeInput=false;
}

LoopWidget::~LoopWidget() {}



