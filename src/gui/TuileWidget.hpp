/***************************************************************************
 *  TuileWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
#ifndef _TuileWidget_H
#define _TuileWidget_H

#include <string>

#include "TuileWidgetNode.hpp"

class TuileParamWidget;
class TuileParamGroup;
class AudioTuile;

class TuileWidget: public TuileWidgetNode {
  public:
	TuileWidget(const std::string& name, tuiles::Tuile* tuile=NULL);
	~TuileWidget();

    inline const std::string& getName(){return m_name;}
	inline void setName(const std::string& name){m_name=name;}

    void update();

    virtual int handle(int event);
    virtual void drawComposition();
    virtual void drawExecution(const float&);
    virtual void drawConnections();
    virtual void drag(const int&, const int&);

    void select();
    void deselect();

    virtual void notify();

    inline int getSyncIn() {return x()+m_sync1X;}
    inline int getSyncOut() {return x()+m_sync2X;}
    inline int getRealIn() {return x()+m_real1X;}
    inline int getRealOut() {return x()+m_real2X;}

    void resetHighlight();
    void highlightSyncInLine();
    void highlightSyncOutLine();
    void highlightReal(bool high=true);

    bool canTakeInput(){return m_canTakeInput;}
    virtual void connectToWidget(TuileWidget*){}

    inline TuileParamWidget* getParamWidget(){return m_paramWidget;}

    virtual void tryForkWithTuile(const std::string& tuileName);
    virtual void trySeqWithTuile(const std::string& tuileName);
    virtual void tryJoinWithTuile(const std::string& tuileName);
    virtual void tryLeftSeqWithTuile(const std::string& tuileName);
    inline virtual void tryAddTuileChild(const std::string& tuileName){}

  protected:
	std::string m_name;
    bool m_muted;
    bool m_canTakeInput;

	float m_height;

	int m_sync1X;
	int m_sync2X;
	int m_real1X;
	int m_real2X;
    Fl_Color m_sync1Color;
    Fl_Color m_sync2Color;
    Fl_Color m_syncLColor;
    Fl_Color m_rectColor;
    Fl_Color m_realColor;

	bool m_overSyncIn;
	bool m_overSyncOut;
	bool m_draggingSyncIn;
	bool m_draggingSyncOut;
    bool m_dragging;

	int m_dragPosX;
	int m_dragPosY;
	int m_magnetSize;
	int m_measureDiv;
    bool m_connecting;

	unsigned int m_state;
	Fl_Color m_backgroundColor;
    bool m_selected;

    std::vector<TuileWidget*> m_inputWidgets;

    TuileParamWidget* m_paramWidget; 
    TuileParamGroup* m_paramGroup;
    
};

#endif

