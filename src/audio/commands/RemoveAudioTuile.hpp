/***************************************************************************
 *  RemoveAudioTuile.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef RemoveAudioTuile_h
#define RemoveAudioTuile_h

#include "Command.hpp"
#include "../AudioManager.hpp"

class RemoveAudioTuile: public Command {
	public:
		inline RemoveAudioTuile(){}
		inline virtual ~RemoveAudioTuile(){};
		virtual void run() {
            vector<AudioTuile*>::iterator itTui
                    =m_audioManager->m_audioTuiles.begin();
            for(; itTui!=m_audioManager->m_audioTuiles.end(); ) {
                if((*itChild)->getID()==child->getID()) {
                    itChild=m_audioTuile->m_audioTuiles.erase(itChild);
                }
                else {
                    itChild++;
                }
            }
        }
        virtual void createClones(const unsigned int& nbClones) {
            for(unsigned int c=0; c<nbClones; ++c) {
                m_clones.push_back(new RemoveAudioTuile());
                m_clones.back()->setModel(this);
            }
        }
        inline void setTuile(Tuile* tuile){m_tuile=tuile;}

	protected:
        AudioTuile* m_audioTuile;
        AudioManager* m_audioManager;
};

#endif

