/***************************************************************************
 *  TuileParamGroup.h
 *  2012-2013 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef _TuileParamGroup_H
#define _TuileParamGroup_H
 
#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>

class TuileParamWidget;

class TuileParamGroup : public Fl_Scroll {
  public:
    static TuileParamGroup* getInstance();
	~TuileParamGroup();
    void setWidget(TuileParamWidget*);
    void resize(int x, int y, int w, int h);

  protected:
	TuileParamGroup();
    TuileParamWidget* m_childWidget;

};

#endif

