/***************************************************************************
 *  OpWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _OpWidget_H
#define _OpWidget_H

#include "TuileWidget.hpp"

namespace tuiles{class OpTuile;}

class OpWidget: public TuileWidget {
  public:
	OpWidget(const std::string& name, tuiles::OpTuile*);
	~OpWidget();

    void refreshChildrenTuileWidgets();

  private:
    tuiles::OpTuile* m_opTuile;
};

#endif

