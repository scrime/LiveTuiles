/***************************************************************************
 *  TuileWidgetNode.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
#ifndef _TuileWidgetNode_H
#define _TuileWidgetNode_H

#include <vector>
#include <FL/Fl_Scroll.H>
#include <tuiles/Observer.hpp>

namespace tuiles{class Tuile;}

class TuileWidgetNode : public Fl_Scroll, public tuiles::Observer {
  public:
	TuileWidgetNode(tuiles::Tuile*);
	virtual ~TuileWidgetNode();

    inline virtual void draw(){drawComposition();}
    inline virtual void drawComposition(){}
    inline virtual void drawExecution(const float& alpha=1){}
    inline tuiles::Tuile* getTuile(){return m_tuile;}
    inline virtual void refreshChildrenTuileWidgets(){}
    inline const int& getID(){return m_id;}

  protected:
    int m_id;
    std::vector<TuileWidgetNode*> m_childrenTuileWidgets;
    tuiles::Tuile* m_tuile;
};

#endif

