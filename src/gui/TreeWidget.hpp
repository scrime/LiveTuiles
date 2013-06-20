/***************************************************************************
 *  TreeWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef _TreeWidget_H
#define _TreeWidget_H
 
#include <FL/Fl.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Counter.H>
#include <iostream>
#include <string>
#include <vector>
#include <list>

class TuileWidget;
class TuileParamGroup;

class TreeWidget : public Fl_Scroll {
  public:
	TreeWidget(int ,int , int, int);
	~TreeWidget();

    void update();
    void refreshTuiles();
    void zoom(const float&);

	void draw();
    int handle(int);

    inline void setPixelsPerBeat(int ppb){m_pixelsPerBeat=ppb;}
    void getMagnetizedPositionAndTuile(const int&, const int&, const int&,
                                        int&, int&, bool&, const std::string&);

    void addTuileWidget(const unsigned int& id);
    void removeTuileWidget(TuileWidget*);
    void removeTuileWidget(const unsigned int&);
	void dropTuile(const std::string& tuileName, 
					const unsigned int& id, const std::string& op);

    inline void setParamGroup(TuileParamGroup* paramGroup){ 
        m_paramGroup=paramGroup;
    }

    inline void clear(){Fl_Scroll::clear();m_tuileWidgets.clear();}

  private:

	int m_cursorX;
	int m_pixelsPerBeat;
	int m_offsetX, m_offsetY;
    int m_magnetSize;
    
    int m_zeroPosX;
    int m_startLoopPosX;
    int m_loopW;
    unsigned int m_forkLeafID;

    std::list<TuileWidget*> m_tuileWidgets;
    TuileWidget* m_selectedTuileWidget;

    TuileWidget* m_overWidget;
    unsigned int m_overWidgetPart;

    TuileWidget* m_draggedWidget;
    unsigned int m_draggedWidgetPart;
    int m_draggingStartX, m_draggingStartY;
    float m_draggedTuileInitLeftOffset;
    float m_draggedTuileInitRightOffset;
    float m_draggedTuileInitLength;

    bool m_movingLoop;
    float m_loopInitLeftOffset;
    float m_loopInitRightOffset;
    float m_loopInitLength;

    TuileParamGroup* m_paramGroup;

    std::vector<std::string> m_inputBuffersStr;
    std::vector<std::string> m_outputBuffersStr;
    unsigned int m_nbInternalOutputs, m_nbExternalOutputs, m_nbExternalInputs;
};

#endif
