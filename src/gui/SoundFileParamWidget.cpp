/***************************************************************************
 *  SoundFileParamWidget.cpp
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "SoundFileParamWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "HitPack.hpp"
#include "../audio/SoundFileTuile.hpp"

using namespace std;

SoundFileParamWidget::SoundFileParamWidget(SoundFileTuile* sfTuile):
                                AudioTuileParamWidget(sfTuile), 
                                m_sfTuile(sfTuile) {
    end();
}

SoundFileParamWidget::~SoundFileParamWidget() {}

void SoundFileParamWidget::update() {}


