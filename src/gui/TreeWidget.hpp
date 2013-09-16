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
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>

#include "TuileWidget.hpp";

class TuileParamGroup;
class TuileWidget;
class SeqWidget;
class AudioTuileWidget;
class LeafTuileWidget;
class ConnectionWidget;

class TreeWidget : public TuileWidget, public Fl_Group {
    public:
        static TreeWidget* getInstance();
        ~TreeWidget();
        inline void notifyDelete(){}

        void save(const std::string& fileName);
        TuileWidget* load(const std::string& fileName);
        void loadConnection(xmlNodePtr, const unsigned int&);

        void resize(int x, int y, int w, int h) {
            Fl_Group::resize(x, y, w, h);
            m_zoom=float(w)/m_totalWidth;
            updateZoom();
        }
        void update();
		void notifyUpdate();
        void updateChildren();
        void updateChildrenPositions();
        void updateZoom();
        void zoom(const float&);
        void scroll(const float& scrollX, const float& scrollY);
        inline const float& getZoom(){return m_zoom;}
        inline const float& getScrollX(){return m_scrollX;}
        inline const float& getScrollY(){return m_scrollY;}
        inline const float& getTotalWidth(){return m_totalWidth;}
        inline const float& getTotalHeight(){return m_totalHeight;}

        void draw();

        int handle(int);

        virtual bool testMagnetWithTuile(const int& inX, const int& inY,
                                            int& outX, int& outY, 
                                            const std::string& tuileName,
                                            const bool& drop);
        TuileWidget* createTuileWidget(const std::string& tuileName);
        SeqWidget* createSeqWidget(TuileWidget*, TuileWidget*);
        void addTuileWidget(TuileWidget*);
        void markWidgetForRemoval(TuileWidget*);
        TuileWidget* getTuileWidget(const unsigned int& id);
        inline const std::map<unsigned int, TuileWidget*>& getTuileWidgets() {
            return m_tuileWidgets;
        }

        void selectTuileWidget(TuileWidget* selected);
        void deselectAllTuileWidgets();
        inline void setParamGroup(TuileParamGroup* paramGroup){ 
            m_paramGroup=paramGroup;
        }

        void markConnectionForRemoval(ConnectionWidget*);
        void testConnection(AudioTuileWidget*, const int& x, 
                            const int& y, bool drop=false);

        void clear();

    private:
        TreeWidget();

    private:
        int m_cursorX;
        float m_totalWidth, m_minTotalWidth; 
        float m_totalHeight, m_minTotalHeight;
        float m_scrollX, m_scrollY;
        float m_zoom;
        float m_panelWidth;
        int m_magnetSize;
        int m_zeroPosX;

        std::map<unsigned int, AudioTuileWidget*> m_audioTuileWidgets;
        std::map<unsigned int, TuileWidget*> m_tuileWidgets;
        TuileWidget* m_selectedTuile;
        std::vector<TuileWidget*> m_removingWidgets;

        std::vector<TuileWidget*> m_panelWidget;

        unsigned int m_connectionIDCounter;
        std::map<unsigned int, ConnectionWidget*> m_connections;
        std::vector<ConnectionWidget*> m_removingConnections;

        TuileParamGroup* m_paramGroup;
};

#endif
