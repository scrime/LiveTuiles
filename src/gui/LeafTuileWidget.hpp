/***************************************************************************
 *  LeafTuileWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
#ifndef _LeafTuileWidget_H
#define _LeafTuileWidget_H

#include <string>
#include <FL/Fl_Widget.H>

#include "TuileWidget.hpp"

class TuileParamWidget;
class TuileParamGroup;

class LeafTuileWidget: public TuileWidget, public Fl_Widget {
  public:
	LeafTuileWidget(const std::string& name, tuiles::Tuile* tuile=NULL);
	virtual ~LeafTuileWidget();

    inline const std::string& getName(){return m_name;}
	inline void setName(const std::string& name){m_name=name;}

    void update();
    virtual void notify();
    virtual void save(xmlNodePtr);

    virtual int handle(int event);
    virtual void draw(){drawComposition();}
    virtual void drawComposition();
    virtual void drawExecution(const int& offset=0);
    inline int getSyncIn() {return x()+m_sync1X;}
    inline int getSyncOut() {return x()+m_sync2X;}
    inline int getRealIn() {return x()+m_real1X;}
    inline int getRealOut() {return x()+m_real2X;}
    inline int getCenterReal(){return x()+m_real1X+(m_real2X-m_real1X)/2;}

    void select();
    void deselect();
    void resetHighlight();
    void highlightSyncInLine();
    void highlightSyncOutLine();
    void highlightReal(bool high=true);

    inline TuileParamWidget* getParamWidget(){return m_paramWidget;}

    virtual bool testMagnetWithTuile(const int& inX, const int& inY,
                                        int& outX, int& outY,     
                                        const std::string& tuileName,
                                        const bool& drop);

    inline virtual Fl_Widget* getWidget(){return this;}

  protected:

    std::string m_name;
    std::vector<LeafTuileWidget*> m_inputWidgets;

    
};

#endif

