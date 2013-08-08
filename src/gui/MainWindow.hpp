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
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Counter.H>

class HitPack;
class TreeWidget;
class TuilesBank;
class TuileParamGroup;

class MainWindow  : public Fl_Double_Window {
	public:
		static MainWindow* getInstance();
		~MainWindow();

		void init();

		static void idle(void* pnt);
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
		static void statTreeButtons(Fl_Widget* w,void* f) { 
			MainWindow *tmpf = static_cast<MainWindow *>(f);
			tmpf->cbTreeButtons(w);
		}	
		void cbTreeButtons(Fl_Widget*);
        void clearAll();

		static void statZoomTree(Fl_Widget* w,void* f) { 
			MainWindow *tmpf = static_cast<MainWindow *>(f);
			tmpf->cbZoomTree(w);
		}	
		void cbZoomTree(Fl_Widget*);

	private:
		MainWindow();

		HitPack* m_pack;
        std::string m_currentFileName;

		int m_controlHeight;
		HitPack* m_controlPart;
		Fl_Toggle_Button* m_playPauseButton;
		Fl_Button* m_stopButton;
		Fl_Value_Input *m_bpmInput;
		std::string m_bpmInputTooltip;
        Fl_Slider *m_treeZoomSlider;

		HitPack* m_tuilesPart;
		TreeWidget* m_tuilesTree;

		int m_editHeight;
		int m_treeHeight;
		int m_browserWidth;
        int m_spacing;
		int m_editScoreButtonsHeight;
		HitPack* m_editPart;
        HitPack* m_loadSavePart;
		TuilesBank* m_tuilesBank;
        HitPack* m_editScoreButtonsPart;
        Fl_Button* m_clearTreeButton;
        Fl_Button* m_saveTreeButton;
        
        TuileParamGroup* m_tuileParamGroup;
};

#endif

