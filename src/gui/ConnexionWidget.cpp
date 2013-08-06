/***************************************************************************
 *  ConnexionWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "ConnexionWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>
#include <Fl/fl_draw.H>

#include <tuiles/OpTuile.hpp>

#include "TreeWidget.hpp"

using namespace std;
using namespace tuiles;

ConnexionWidget::ConnexionWidget(LeafTuileWidget* from, 
                                LeafTuileWidget* to): Fl_Group(0, 0, 100, 20),
                                                      m_fromWidget(from),  
                                                      m_toWidget(to) {
}

ConnexionWidget::~ConnexionWidget() {}


void ConnexionWidget::draw() {


}

int ConnexionWidget::handle(int event) {


}


