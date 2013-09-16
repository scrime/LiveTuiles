/***************************************************************************
 *  MainWindow.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef _MainWindow_h
#define _MainWindow_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <list>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Counter.H>

class HitPack;
class TreeWidget;
class TuilesBank;
class TuileParamGroup;
class ScrollZoomWidget;

class MainWindow : public Fl_Double_Window {
	public:
		static MainWindow* getInstance();
		~MainWindow();

		void init();

		static void idle(void* pnt);
        int handle(int);
        void update();

		static void statPlayPause(Fl_Widget* w,void* f) { 
			MainWindow *tmpf = static_cast<MainWindow *>(f);
			tmpf->cbPlayPause(w);
		}	
		void cbPlayPause(Fl_Widget*);
        void togglePlayPause();
        void togglePlayStop();

		static void statStop(Fl_Widget* w,void* f) { 
			MainWindow *tmpf = static_cast<MainWindow *>(f);
			tmpf->cbStop(w);
		}	
		void cbStop(Fl_Widget*);

		static void statBpm(Fl_Widget* w,void* f) { 
			MainWindow *tmpf = static_cast<MainWindow *>(f);
			tmpf->cbBpm(w);
		}	
		void cbBpm(Fl_Widget*);
		static void statCursor(Fl_Widget* w,void* f) { 
			MainWindow *tmpf = static_cast<MainWindow *>(f);
			tmpf->cbCursor(w);
		}	
		void cbCursor(Fl_Widget*);
		static void statTreeButtons(Fl_Widget* w,void* f) { 
			MainWindow *tmpf = static_cast<MainWindow *>(f);
			tmpf->cbTreeButtons(w);
		}	
		void cbTreeButtons(Fl_Widget*);
        void clearAll();

        inline ScrollZoomWidget* getScrollZoomWidget(){return m_treeZoomSlider;}

	private:
		MainWindow();

		HitPack* m_pack;
        std::string m_currentFileName;

		int m_controlHeight;
        int m_controlWidth;
        int m_scrollZoomWidth;
		HitPack* m_controlPart;
		Fl_Toggle_Button* m_playPauseButton;
		Fl_Button* m_stopButton;
		Fl_Toggle_Button* m_cursorToggle;
		std::string m_cursorToggleTooltip;
		Fl_Value_Input *m_bpmInput;
		std::string m_bpmInputTooltip;
        ScrollZoomWidget* m_treeZoomSlider;

		TreeWidget* m_tuilesTree;

		int m_editHeight;
		int m_treeHeight;
		int m_browserWidth;
        int m_spacing;
		int m_editScoreButtonsHeight;
        int m_paramWidth;
		HitPack* m_editPart;
        HitPack* m_loadSavePart;
		TuilesBank* m_tuilesBank;
        HitPack* m_editScoreButtonsPart;
        Fl_Button* m_clearTreeButton;
        Fl_Button* m_saveTreeButton;
        
        TuileParamGroup* m_tuileParamGroup;
};

#endif

