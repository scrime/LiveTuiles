/***************************************************************************
 *  TuileParamGroup.cpp
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "TuileParamGroup.hpp"

#include "FaustParamWidget.hpp"
#include "SoundFileParamWidget.hpp"
#include "SwitchParamWidget.hpp"

using namespace std;

TuileParamGroup::TuileParamGroup(): Fl_Group(0,0,100,100,"Edit Tuile") { 
    align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    color(fl_darker(fl_darker(FL_GREEN)));
    end();
}

TuileParamGroup::~TuileParamGroup() {}

TuileParamGroup* TuileParamGroup::getInstance() {
    static TuileParamGroup instance;
    return &instance;
}

void TuileParamGroup::setWidget(TuileParamWidget* widget) {
    if(children()>0) {
        while(children()>0) {
            remove(child(0));
        }
    }
    if(widget) {
        add(widget);
        widget->resize(x(),y(),w(),h());
        widget->redraw();
    }
    redraw();
}


