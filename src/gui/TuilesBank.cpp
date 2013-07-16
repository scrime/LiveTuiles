/***************************************************************************
 *  TuilesBank.cpp
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
#include "TuilesBank.hpp"

#include <iostream>
#include <math.h>
#include <cassert>

#include "TreeWidget.hpp"
#include "TuileWidget.hpp"

using namespace std;

TuilesBank::TuilesBank(int x, int y, int w, int h):
								Fl_Group(x,y,w,h,"Drag Tuiles"),
                                m_dragging(false), m_offsetX(5), 
                                m_offsetY(15), m_topLine(1){

    align(FL_ALIGN_TOP|FL_ALIGN_LEFT);

    //back button
	m_prevButton = new Fl_Button(m_offsetX, y+5, 20, 10, "@$arrow");
	m_prevButton->callback(statPrevButton,this);
    this->add(m_prevButton);

    m_tuilesList.push_back(std::string("output"));
    m_tuilesList.push_back(std::string("input"));
    m_tuilesList.push_back(std::string("loop"));
    m_tuilesList.push_back(std::string("switch"));
    m_tuilesList.push_back(std::string("monitor"));
}

TuilesBank::~TuilesBank() {
	delete m_prevButton;
}

void TuilesBank::draw() {
    //draw label 
    draw_label();

	fl_draw_box(FL_DOWN_BOX, x(), y(), w(), h(), 
                fl_darker(FL_BACKGROUND2_COLOR));
    
    //draw button
    draw_children();

	fl_draw_box(FL_DOWN_BOX, x()+m_offsetX, y()+m_offsetY, 
                2*w()/3, h()-m_offsetY, FL_BACKGROUND_COLOR);

	fl_push_clip(x()+m_offsetX, y()+m_offsetY+2, 2*w()/3, h()-m_offsetY);
    
    fl_font(FL_HELVETICA, 14);

    //draw files from current directory
    fl_color(FL_FOREGROUND_COLOR);
    int posY=y()+fl_height()+m_offsetY;
    int firstLine = max<float>(m_topLine,0);
    for(unsigned int l=firstLine; l<m_filesList.size();++l,posY+=fl_height()) {
        if(fl_filename_match(m_filesList[l].c_str(), m_pattern.c_str())!=0) {
            fl_color(fl_lighter(FL_BACKGROUND_COLOR));
            if(m_selectedFile==int(l)) {
                fl_color(fl_lighter(FL_SELECTION_COLOR));
            }
            drawTuile(x()+m_offsetX*2, posY, 
                        fl_width(m_filesList[l].c_str())+2, 
                        fl_height(), m_filesList[l].c_str());
        }
        else {
            if(m_selectedFile==int(l)) {
                fl_color(fl_lighter(FL_SELECTION_COLOR));
                fl_rectf(x()+m_offsetX*2, posY-fl_height(), 
                        fl_width(m_filesList[l].c_str())+m_offsetX*2, 
                        fl_height());
            }
            fl_color(FL_FOREGROUND_COLOR);
            fl_draw(m_filesList[l].c_str(), x()+m_offsetX*2, posY-fl_descent());
        }
    }
	fl_pop_clip();

    //draw additional tuiles 
    posY=y()+fl_height()+m_offsetY;
    vector<string>::iterator itTui = m_tuilesList.begin();
    for(;itTui!=m_tuilesList.end(); ++itTui, posY+=fl_height()) {
        fl_color(fl_lighter(FL_BACKGROUND_COLOR));
        drawTuile(x()+2*w()/3+m_offsetX, posY, 
                    fl_width((*itTui).c_str())+4, fl_height(), 
                    (*itTui).c_str());

    }

    //draw dragged tuile
    if(m_dragging) {
        m_draggedTuile->drawComposition();
    }
}


void TuilesBank::drawTuile(const int& x, const int& y, 
                                const int& w, const int& h, 
                                const char* t) {
    fl_rectf(x, y-fl_height(), w, h);
    fl_color(fl_darker(FL_BACKGROUND_COLOR));
    fl_line(x, y+h/2-fl_height(), x+w, y+h/2-fl_height());
    fl_color(FL_FOREGROUND_COLOR);
    fl_rect(x, y-fl_height(), w, h);
    fl_font(FL_HELVETICA_ITALIC, 14);
    fl_draw(t, x, y-fl_descent());
    fl_font(FL_HELVETICA, 14);
}

int TuilesBank::handle(int event) {
    if(Fl_Group::handle(event)==0) {
    switch(event) { 
        case FL_DRAG: { 
            if(!m_dragging) {
                if(m_selectedFromBank.size()>0) {
                    m_dragging=true;
                    m_draggedTuile = new TuileWidget("");
                    m_draggedTuile->setName(m_selectedFromBank);
                    m_draggedTuile->position(Fl::event_x(), Fl::event_y());
                    window()->add(m_draggedTuile);
                }
                else if(m_selectedFile>=0 && m_selectedFromDir) {
                    if(fl_filename_match(m_filesList[m_selectedFile].c_str(), 
                                                m_pattern.c_str())!=0) {
                        m_dragging=true;
                        m_draggedTuile = new TuileWidget("");
                        m_draggedTuile->setName(m_filesList[m_selectedFile]);
                        m_draggedTuile->position(Fl::event_x(), Fl::event_y());
                        window()->add(m_draggedTuile);
                    }
                }
            }
            else {
                //get magnetized tuile and position from the tree widget
                int posX, posY;
                bool drop=false;
                m_treeWidget->getMagnetizedPositionAndTuile(Fl::event_x(), 
                                                    Fl::event_y(),
                                                    m_draggedTuile->w(),
                                                    posX, posY, drop, "");
                
                //move the tuilewidget associated with the cursor
                m_draggedTuile->position(posX,posY);
                window()->redraw();
                m_draggedTuile->redraw();
            }
            return 1;
        }break;
        case FL_PUSH: {
            m_selectedFromBank="";
            m_selectedFromDir=false;
            fl_font(FL_HELVETICA, 14);
            if(Fl::event_x()<x()+2*w()/3) { //files list
                //get selected line
                int selectedLine = (Fl::event_y()-y()-m_offsetY)/fl_height()
                                    +m_topLine;
                if(selectedLine>=0 && selectedLine<=int(m_filesList.size())) {
                    m_selectedFile=selectedLine;
                    m_selectedFromDir=true;
                    std::string absFn= m_currentDir+m_filesList[m_selectedFile];
                    //if it's a directory
                    if(fl_filename_isdir(absFn.c_str())) {
                        //if double-click
                        if(Fl::event_clicks()==1) {
                            //open this directory
                            m_currentDir=absFn;
                            displayCurrentDir();
                            Fl::event_clicks(0);
                            Fl::event_is_click(0);
                        }
                    }
                }
                else {
                    m_selectedFile=-1;
                }
            }
            else { //tuiles list
                unsigned int selectedLine = 
                            (Fl::event_y()-y()-m_offsetY)/fl_height();
                if(selectedLine<m_tuilesList.size()) {
                    m_selectedFromBank=m_tuilesList[selectedLine];
                }
            }
            return 1;
        }break;
        case FL_RELEASE: {
            if(m_dragging) {
                int posX, posY;
                bool drop=true;
                std::string tuileName;
                if(m_selectedFromBank.size()>0) {
                    tuileName=m_draggedTuile->getName();
                }
                else {
                    tuileName=m_currentDir+m_draggedTuile->getName();
                }
                //drop the tuile in the tree widget
                m_treeWidget->getMagnetizedPositionAndTuile(Fl::event_x(), 
                                                    Fl::event_y(),
                                                    m_draggedTuile->w(),
                                                    posX, posY, drop,
                                                    tuileName); 

                //remove the draggedtuile widget
                window()->remove(m_draggedTuile);
				delete m_draggedTuile;

                //clear selection
                drop=false;
                m_treeWidget->getMagnetizedPositionAndTuile(0, 0, 0, posX, 
                                                            posY, drop, "");
                m_dragging=false;
                window()->redraw();
            }
            return 1;
        }break;
        case FL_MOUSEWHEEL: {
            float newLine = m_topLine+float(Fl::event_dy())/10.0;
            if( newLine<=float(m_filesList.size()) && newLine>=0) {
                m_topLine=newLine;
            }
            return 1;
        }break;
    }
    }
    return 0;
}

void TuilesBank::cbPrevButton(Fl_Widget*) {
	//displayCurrentPath(-1);
    if(m_currentDir.compare("/")!=0) {
        if(m_currentDir[m_currentDir.size()-1]=='/') {
           m_currentDir.erase(m_currentDir.size()-1,1); 
        }
        size_t found = m_currentDir.find_last_of("/");
        if(found!=m_currentDir.npos) {
            std::string cut = m_currentDir.substr(0,found+1);
            m_currentDir=cut;
        }
    }
    displayCurrentDir();
}

void TuilesBank::open(std::string browserT, std::string fileX, 
												std::string previousFile) {

	//sets currentDir
	//if not null, go to the asked directory
	//remove the last / if any
	if(previousFile.size()>0 && previousFile[previousFile.size()-1]=='/') {
		previousFile = previousFile.substr(0,previousFile.size()-1);	
	}
	//find the previous /
	size_t found = previousFile.find_last_of("/");
	if(found!=previousFile.npos) {
		m_currentDir=previousFile.substr(0,found+1);
	}
	else { //current dir
		m_currentDir="~/";
	}

	//sets currentPattern
	m_pattern=fileX;
	m_currentPattern=m_pattern;
    
	if(displayCurrentDir()) {
		m_currentDir=getenv("HOME");
		m_currentDir+="/";
		displayCurrentDir();
	}
}

int TuilesBank::displayCurrentDir() {

    m_filesList.clear();
    m_topLine=0;

	dirent **files;
	int nbFiles = scandir(m_currentDir.c_str(),&files,NULL,alphasort);
	//if problem, retry with the user directory 
	if(nbFiles<0) {
		return -1;
	}
	else {
		for(int i=0;i<nbFiles;++i) {
			std::string fn = files[i]->d_name;
			//skip the current and parent dirs
			if(fn.compare(".")!=0 && fn.compare("..")!=0) {
				//hide hidden files/dirs
				if(fn[0]!='.') {
					//get the absolute name
					std::string absFn = m_currentDir+fn;
					//show dirs
					if(fl_filename_isdir(absFn.c_str())) {
						fn+="/";
						m_filesList.push_back(fn);
					}
					else { //for other files
						//match patterns
						if(fl_filename_match(fn.c_str(), 
                                m_currentPattern.c_str())!=0) {
							m_filesList.push_back(fn);
						}
					}
				}
			}
		}
	}
	this->redraw();

	return 0;	
}

