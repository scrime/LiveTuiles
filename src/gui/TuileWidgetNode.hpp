/***************************************************************************
 *  TuileWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
#ifndef _TuileWidget_H
#define _TuileWidget_H

#include <FL/Fl_Widget.H>
#include <string>
#include <vector>

class TuileParamWidget;
class TuileParamGroup;
class Tuile;

class TuileWidget : public Fl_Widget {
  public:
	TuileWidget(const std::string& name, Tuile* tuile=NULL);
	~TuileWidget();

    void update();

    int handle(int event);
	void draw();
    void drawConnections();
    void drag(const int&, const int&);

    inline const std::string& getName(){return m_name;}
	inline void setName(const std::string& name){m_name=name;}

    void select();
    void deselect();

	void refresh(const int&, const int&, const float& zoom, const int& pos);

    inline int getSyncIn() {return x()+m_sync1X;}
    inline int getSyncOut() {return x()+w()-m_sync2X;}

    void resetHighlight();
    void highlightSyncInLine();
    void highlightSyncOutLine();
    void highlightReal(bool high=true);

    inline const unsigned int& getID(){return m_id;}

    inline TuileParamWidget* getParamWidget(){return m_paramWidget;}

    virtual void tryForkWithTuile(const std::string& tuileName);
    virtual void trySeqWithTuile(const std::string& tuileName);
    virtual void tryJoinWithTuile(const std::string& tuileName);
    virtual void tryLeftSeqWithTuile(const std::string& tuileName);
    inline virtual void tryAddTuileChild(const std::string& tuileName){}

  protected:
    Tuile* m_tuile;
    unsigned int m_id;
	std::string m_name;
    bool m_muted;

	float m_height;

	int m_sync1X;
	int m_sync2X;

    Fl_Color m_sync1Color;
    Fl_Color m_sync2Color;

    Fl_Color m_rectColor;
    Fl_Color m_realColor;

	bool m_clickedBegin;
	bool m_clickedEnd;
	bool m_clickedMiddle;

	int m_dragPosX;
	int m_dragPosY;
	int m_magnetSize;
	int m_measureDiv;
    bool m_tmpConnectionActive;

	unsigned int m_state;
	Fl_Color m_backgroundColor;
    bool m_selected;

    std::vector<TuileWidget*> m_inputWidgets;

    TuileParamWidget* m_paramWidget; 
    TuileParamGroup* m_paramGroup;
    
};

#endif

