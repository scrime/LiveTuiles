/***************************************************************************
 *  SeqWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _SeqWidget_H
#define _SeqWidget_H

#include "TuileWidget.hpp"

namespace tuiles{class SeqTuile;}

class SeqWidget: public TuileWidget {
  public:
	SeqWidget(const std::string& name, tuiles::SeqTuile*);
	~SeqWidget();

    void draw();
    void drawExec(const float&);

    void setFirstChildWidget(TuileWidgetNode*);
    void setSecondChildWidget(TuileWidgetNode*);

  private:
    tuiles::SeqTuile* m_seqTuile;
    
};

#endif

