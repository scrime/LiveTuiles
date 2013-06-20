/***************************************************************************
 *            TuileParamGroup.cpp
 *
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *
 *  This file is part of libTuiles
 ****************************************************************************/

#include "TuileParamGroup.hpp"

#include "FaustParamWidget.hpp"
#include "SoundFileParamWidget.hpp"
#include "SwitchParamWidget.hpp"

using namespace std;

TuileParamGroup::TuileParamGroup(int x, int y, int w, int h): 
                                Fl_Group(x,y,w,h,"Edit Tuile") { 
    align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    color(fl_darker(fl_darker(FL_GREEN)));
    end();
}

TuileParamGroup::~TuileParamGroup() {}


void TuileParamGroup::setWidget(TuileParamWidget* widget) {
    if(children()>0) {
        while(children()>0) {
            remove(child(0));
        }
    }
    begin();
    add(widget);
    end();
}

TuileParamWidget* TuileParamGroup::createParamWidget(const unsigned int& id, 
                                                     const std::string& name) {
    begin();
    TuileParamWidget* newParamWidget=NULL;
    if(name.find(".dsp")!=name.npos) {
        newParamWidget = new FaustParamWidget(x(),y(),w(),h(), id, name);
    }
    else if(name.find(".wav")!=name.npos) {
        newParamWidget = new SoundFileParamWidget(x(),y(),w(),h(), id, name);
    }
    else if(name.compare("osc")==0) {
        //newParamWidget = new SoundFileParamWidget(x(),y(),w(),h(), id, name);
        newParamWidget = new TuileParamWidget(x(),y(),w(),h(), id, name);
        cout<<"Create osc tuile param widget"<<endl;
    }
    else if(name.compare("switch")==0) {
        //newParamWidget = new SoundFileParamWidget(x(),y(),w(),h(), id, name);
        newParamWidget = new SwitchParamWidget(x(),y(),w(),h(), id, name);
        cout<<"Create switch tuile param widget"<<endl;
    }
    else if(name.compare("while")==0) {
        //newParamWidget = new SoundFileParamWidget(x(),y(),w(),h(), id, name);
        newParamWidget = new TuileParamWidget(x(),y(),w(),h(), id, name);
        cout<<"Create while tuile param widget"<<endl;
    }
    else {
        //newParamWidget = new SoundFileParamWidget(x(),y(),w(),h(), id, name);
        newParamWidget = new TuileParamWidget(x(),y(),w(),h(), id, name);
        cout<<"Create curve tuile param widget"<<endl;
    }
    end();
    if(newParamWidget) {
        setWidget(newParamWidget);
        newParamWidget->init();
    }
    return newParamWidget;
}


