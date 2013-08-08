/***************************************************************************
 *  TuilesBank.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef _TuilesBank_H
#define _TuilesBank_H
 
#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/fl_draw.H>
#include <FL/filename.H>

#include <string>
#include <vector>

class TreeWidget;
class LeafTuileWidget;

class TuilesBank : public Fl_Group {
    public:
        TuilesBank(int ,int , int, int);
        ~TuilesBank();

        void draw();
        void drawDraggedTuile();
        int handle(int);

        void open(std::string,std::string,std::string);
        int displayCurrentDir();
        inline void setPattern(const std::string& pattern) {m_pattern=pattern;}
        inline void setTreeWidget(TreeWidget* tree) { m_treeWidget=tree;}

		inline void setCurrentDir(const std::string& dir) { 
            m_currentDir=dir;
            m_topLine=1;
        }
        inline const std::string& getCurrentDir(){return m_currentDir;}

        static void statPrevButton(Fl_Widget* w,void* f) { 
            TuilesBank *tmpf = static_cast<TuilesBank *>(f);
            tmpf->cbPrevButton(w);
        }	
        void cbPrevButton(Fl_Widget*);

    private:
        void drawTuile(const int&, const int&, const int&, 
                        const int&, const char*);

    private:
        bool m_dragging;
        int m_offsetX, m_offsetY;
        std::string m_pattern;
        std::string m_currentPattern;
        LeafTuileWidget* m_draggedTuile;
        TreeWidget* m_treeWidget;
		std::string m_currentDir;
        Fl_Button *m_prevButton;

        float m_topLine;

        std::vector<std::string> m_filesList;
        int m_selectedFile;
        bool m_selectedFromDir;

        std::string m_selectedFromBank;
        std::vector<std::string> m_tuilesList;

};

#endif
