/***************************************************************************
 *  TuileParamGroup.cpp
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "TuileParamGroup.hpp"
#include "TuileParamWidget.hpp"

using namespace std;

TuileParamGroup::TuileParamGroup(): Fl_Scroll(0, 0, 100, 100, "Edit Tuile"), 
                                    m_childWidget(NULL) { 
    align(FL_ALIGN_TOP|FL_ALIGN_LEFT);
    type(Fl_Scroll::VERTICAL_ALWAYS);
    resizable(this);
    end();
}

TuileParamGroup::~TuileParamGroup() {}

TuileParamGroup* TuileParamGroup::getInstance() {
    static TuileParamGroup instance;
    return &instance;
}

void TuileParamGroup::setWidget(TuileParamWidget* widget) {
    if(m_childWidget) {
        remove(m_childWidget);
    }
    m_childWidget=widget;
    if(widget) {
        add(m_childWidget);
        m_childWidget->resize(x(), y(), w(), m_childWidget->h());
    }
    redraw();
}

void TuileParamGroup::resize(int x, int y, int w, int h) {
    Fl_Group::resize(x, y, w, h);
    if(m_childWidget) {
        m_childWidget->resize(x, y, w, m_childWidget->h());
    }
}

