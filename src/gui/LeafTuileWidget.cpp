/***************************************************************************
 *  LeafTuileWidget.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "LeafTuileWidget.hpp"

#include <iostream>
#include <math.h>
#include <cassert>
#include <sstream>

#include <Fl/fl_draw.H>
#include <FL/filename.H>

#include "TuileParamWidget.hpp"
#include "TuileParamGroup.hpp"
#include "TreeWidget.hpp"
#include "SeqWidget.hpp"

#include <tuiles/SeqTuile.hpp>
#include "../audio/AudioManager.hpp"

using namespace std;
using namespace tuiles;

LeafTuileWidget::LeafTuileWidget(const std::string& name, 
                                Tuile* tuile): TuileWidget(tuile),
                                                Fl_Widget(0, 0, 100, 20),
                                                m_name(name) {
    if(tuile) {
        tuile->setSubdivisions(4);
        tuile->setSubEpsilon(10000);
    }
    m_sync2X=w();
    m_real2X=w();
}

LeafTuileWidget::~LeafTuileWidget() {
    if(m_paramWidget) {
        delete m_paramWidget;
    }
}

void LeafTuileWidget::update() {

}

void LeafTuileWidget::resetHighlight() {
    m_sync1Color=FL_FOREGROUND_COLOR;
    m_sync2Color=FL_FOREGROUND_COLOR;
    m_syncLColor=FL_FOREGROUND_COLOR;
    highlightReal(false);
}

void LeafTuileWidget::highlightSyncInLine() {
    m_sync1Color=fl_lighter(FL_BACKGROUND_COLOR);
    m_syncLColor=fl_lighter(FL_BACKGROUND_COLOR);
    highlightReal(true);
}

void LeafTuileWidget::highlightSyncOutLine() {
    m_sync2Color=fl_lighter(FL_BACKGROUND_COLOR);
    m_syncLColor=fl_lighter(FL_BACKGROUND_COLOR);
    highlightReal(false);
}

void LeafTuileWidget::highlightReal(bool high) {
    if(!m_muted) {
        m_rectColor=FL_FOREGROUND_COLOR;
        if(m_selected) {
            m_realColor=fl_darker(FL_DARK_GREEN);
        }
        else {
            m_realColor=fl_darker(fl_darker(FL_BACKGROUND_COLOR));
        }
    }
    else {
        m_realColor=FL_BACKGROUND2_COLOR;
        if(m_selected) {
            m_rectColor=fl_darker(FL_DARK_GREEN);
        }
        else {
            m_rectColor=fl_darker(FL_FOREGROUND_COLOR);
        }
    }
    if(high) {
        if(!m_muted) {
            m_realColor=fl_lighter(m_realColor);
        }
        else {
            m_rectColor=fl_lighter(m_rectColor);
        }
    }
    TreeWidget::getInstance()->redraw();
}

void LeafTuileWidget::drawComposition() {
    //real frame
	fl_color(FL_FOREGROUND_COLOR);
	fl_rect(x(), y(), w(), h());

    //stretch line
	fl_color(fl_darker(m_realColor));
    fl_line_style(FL_DOT, h()/3);
	fl_line(x(), y()+3*h()/4, x()+w(), y()+3*h()/4);

	//syncwindow
    fl_line_style(0);
	fl_color(m_syncLColor);
	fl_line(x()+m_sync1X, y()+h()/2, x()+w(), y()+h()/2);
	fl_line(x()+m_sync2X, y()+h()/2, x(), y()+h()/2);
	fl_color(m_sync1Color);
	fl_line(x()+m_sync1X, y()+h()/2+m_sync1Y, 
            x()+m_sync1X, y()+h()/2);
    fl_arc(x()+m_sync1X-m_syncWidth/2, y()+h()/2+m_sync1Y-m_syncHeight/2, 
            m_syncWidth, m_syncHeight, -90, 90); 
	fl_line(x()+m_sync1X, y()+h()/2+m_sync1Y-m_syncHeight/2, 
            x()+m_sync1X, y()+h()/2+m_sync1Y+m_syncHeight/2);
	fl_color(m_sync2Color);
	fl_line(x()+m_sync2X, y()+h()/2+m_sync2Y, 
            x()+m_sync2X, y()+h()/2);
    fl_arc(x()+m_sync2X-m_syncWidth/2, y()+h()/2+m_sync2Y-m_syncHeight/2, 
            m_syncWidth, m_syncHeight, 90, 270); 
	fl_line(x()+m_sync2X, y()+h()/2+m_sync2Y-m_syncHeight/2, 
            x()+m_sync2X, y()+h()/2+m_sync2Y+m_syncHeight/2);

	//name
    fl_color(FL_WHITE);
	fl_font(FL_HELVETICA_ITALIC, 10);
	fl_draw(m_name.c_str(), x()+2, y()+h()/2-fl_descent());
}

void LeafTuileWidget::drawExecution(const int& offset) {
	//realwindow
	fl_color(m_realColor);
	fl_rectf(x()+offset, y(), w(), h());

    //name background
	fl_color(m_realColor);
	fl_rectf(x()+2+offset, y(), 
            fl_width(m_name.c_str())+2, fl_height());
	//name
    fl_color(fl_darker(m_realColor));
	fl_font(FL_HELVETICA_ITALIC, 10);
	fl_draw(m_name.c_str(), x()+2+offset, y()+h()/2-fl_descent());

    //realization frame
	fl_color(fl_darker(m_realColor));
	fl_rect(x()+offset, y(), w(), h());
}

int LeafTuileWidget::handle(int event) {
    switch(event) { 
        case FL_MOVE: { 
            resetHighlight();
            if(Fl::event_y()>y() && Fl::event_y()<y()+h()) {
                if(abs(Fl::event_x()-(x()+m_sync2X))<m_magnetSize) {
                    fl_cursor(FL_CURSOR_NONE);
                    m_overPart=TUILE_SYNCOUT;
                    m_dragFrame=m_tuile->getRightOffset();
                    resetHighlight();
                    highlightSyncOutLine();
                    return 1;
                }
                else if(Fl::event_x()>x() && Fl::event_x()<x()+w()) {
                    if(Fl::event_y()>y()+h()/2) {
                        m_overPart=TUILE_LENGTH;
                        m_dragFrame=m_tuile->getLength();
                        fl_cursor(FL_CURSOR_WE);
                        resetHighlight();
                        highlightReal();
                    }
                    else {
                        fl_cursor(FL_CURSOR_MOVE);
                        m_overPart=TUILE_SYNCIN;
                        m_dragFrame=m_tuile->getLeftOffset();
                        resetHighlight();
                        highlightSyncInLine();
                        highlightReal();
                    }
                    return 1;
                }
            }
            fl_cursor(FL_CURSOR_DEFAULT);
            m_overPart=TUILE_NONE;
            m_dragging=false;
            return 0;
        }break;
        case FL_DRAG: {
            if(m_dragging) {
                float diffX = Fl::event_x()-m_dragPosX;
                float pixPerFrame=
                        TreeWidget::getInstance()->getPixelsPerFrame();
                switch(m_overPart) {
                    case TUILE_SYNCOUT: {
                        m_tuile->setRightOffset(m_dragFrame-diffX/pixPerFrame);
                    }break;
                    case TUILE_SYNCIN: {
                        m_tuile->setLeftOffset(m_dragFrame-diffX/pixPerFrame);
                        position(x(), Fl::event_y());
                        highlightReal();
                    }break;
                    case TUILE_LENGTH: {
                        m_tuile->setLength(m_dragFrame+diffX/pixPerFrame);
                        highlightReal();
                    }
                    default:break;
                }
                return 1;
            }
            return 0;
        }break;
        case FL_PUSH: {
            if(Fl::event_y()>y() && Fl::event_y()<y()+h() && 
                    (abs(Fl::event_x()-(x()+m_sync2X))<m_magnetSize || 
                        (Fl::event_x()>x() && Fl::event_x()<x()+w())) ) {
                m_dragging=true;
                m_dragPosX=Fl::event_x();
                m_dragPosY=Fl::event_y();
                TreeWidget::getInstance()->deselectAllTuileWidgets();
                TreeWidget::getInstance()->selectTuileWidget(this);
                select();
                return 1;
            }
            else {
                m_dragging=false;
                resetHighlight();
                return 0;
            }
        }break;
        case FL_RELEASE: {
            if(m_dragging && 
                    Fl::event_y()>y() && Fl::event_y()<y()+h() && 
                    (abs(Fl::event_x()-(x()+m_sync2X))<m_magnetSize || 
                        (Fl::event_x()>x() && Fl::event_x()<x()+w())) ) {
                m_dragging=false;
                return 1;
            }
            return 0;
        }break;
        default:break;
    }
    return 0;
}

void LeafTuileWidget::notifyUpdate() {
    TuileWidget::notifyUpdate();
    m_name=fl_filename_name(m_tuile->getName().c_str());
    resize(x(), y(), m_width, h());
}

void LeafTuileWidget::notifyDelete() {
    TreeWidget::getInstance()->markWidgetForRemoval(this);
}

void LeafTuileWidget::extract() {
    AudioManager::getInstance()->extractTuile(m_tuile);
}

void LeafTuileWidget::select() {
    m_selected=true;
    highlightReal();
    if(m_paramWidget) {
        TuileParamGroup::getInstance()->setWidget(m_paramWidget);
    }
}

void LeafTuileWidget::deselect() {
    m_selected=false;
    resetHighlight();
}

bool LeafTuileWidget::testMagnetWithTuile(const int& inX, const int& inY,
                                            int& outX, int& outY,
                                            const std::string& tuileName,
                                            const bool& drop) {
    if(inY>y()-m_magnetSize && inY<y()+h()+m_magnetSize) {
        if(abs(inX-(x()+m_sync2X))<m_magnetSize) {
            outX=x()+m_sync2X;
            outY=y();
            if(drop) {
                TreeWidget* tree = TreeWidget::getInstance();
                TuileWidget* newWidget = tree->createTuileWidget(tuileName);
                if(newWidget) {
                    newWidget->getTuile()->setName(tuileName);
                    newWidget->load();
                    newWidget->getWidget()->position(x(), y());
                    SeqWidget* newSeqWidget = 
                        tree->createSeqWidget(this, newWidget);
                    newSeqWidget->notifyUpdate();
                }
                tree->refreshTuiles();

            }
            return true;
        }
    }

    return false;
}

void LeafTuileWidget::save(xmlNodePtr parentNode) {
    xmlNodePtr node = m_tuile->save(parentNode);
    ostringstream oss;
    oss<<y();
    xmlNewProp(node, BAD_CAST "widget_y_offset", BAD_CAST oss.str().c_str());
}

void LeafTuileWidget::load(xmlNodePtr node) {
    TuileWidget::load(node);
    char* value=NULL;
    value=NULL;
    value = (char*)xmlGetProp(node,(xmlChar*)"widget_y_offset");
    if(value) {
        position(x(), atoi(value));
    }
}

void LeafTuileWidget::load() {
    m_name = m_tuile->getName();
}

