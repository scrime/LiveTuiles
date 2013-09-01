/***************************************************************************
 *  AudioTuileWidget.h
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/
 
 
#ifndef _AudioTuileWidget_H
#define _AudioTuileWidget_H

#include <map>

#include "LeafTuileWidget.hpp"

class AudioTuile;
class ConnectionWidget;

class AudioTuileWidget: public LeafTuileWidget {
  public:
	AudioTuileWidget(const std::string& name, AudioTuile*);
	virtual ~AudioTuileWidget();

    virtual void load();
    void notifyDelete();

    virtual void drawComposition();
    virtual int handle(int event);

    bool canTakeInput(){return m_canTakeInput;}
    inline AudioTuile* getAudioTuile(){return m_audioTuile;}

    void addConnection(ConnectionWidget*);
    void removeConnection(ConnectionWidget*);
    
  protected:
    AudioTuile* m_audioTuile;
    bool m_canTakeInput;
    bool m_connecting;
    std::map<unsigned int, ConnectionWidget*> m_connections;
    
};

#endif

