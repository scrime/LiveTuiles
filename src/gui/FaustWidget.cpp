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
#include "FaustParamWidget.hpp"

using namespace std;

FaustWidget::FaustWidget(const std::string& name, 
                            FaustTuile* tuile): 
                            AudioTuileWidget(name, tuile),
                            m_faustTuile(tuile) {
    m_faustParamWidget=new FaustParamWidget(tuile);
    m_paramWidget=m_faustParamWidget;
}

FaustWidget::~FaustWidget() {}

void FaustWidget::load(xmlNodePtr node) {
    LeafTuileWidget::load(node);
    load();
}

void FaustWidget::load() {
    LeafTuileWidget::load();
    m_faustTuile->load(m_name);
    m_faustParamWidget->load();
}


