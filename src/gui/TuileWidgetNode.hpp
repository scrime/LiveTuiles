/***************************************************************************
 *  TuileWidgetNode.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
#ifndef _TuileWidgetNode_H
#define _TuileWidgetNode_H

#include <vector>

namespace tuiles{class Tuile;}

class TuileWidgetNode {
  public:
	TuileWidgetNode(tuiles::Tuile*);
	virtual ~TuileWidgetNode();

    inline virtual void draw(){}

    void removeChildWidget(TuileWidgetNode*);
    void replaceChildWidget(TuileWidgetNode*, TuileWidgetNode*);

    inline void setParent(TuileWidgetNode* parent){m_parent=parent;}
    inline tuiles::Tuile* getTuile(){return m_tuile;}

  protected:
    int m_id;
    std::vector<TuileWidgetNode*> m_childrenWidgets;
    TuileWidgetNode* m_parent;
    tuiles::Tuile* m_tuile;
    
};

#endif

