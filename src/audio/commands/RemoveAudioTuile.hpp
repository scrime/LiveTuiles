/***************************************************************************
 *  RemoveAudioTuile.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef RemoveAudioTuile_h
#define RemoveAudioTuile_h

#include <tuiles/Command.hpp>
#include "../AudioManager.hpp"

class RemoveAudioTuile: public tuiles::Command {
	public:
		inline RemoveAudioTuile(){m_name="RemoveAudioTuile";}
		inline virtual ~RemoveAudioTuile(){};
		virtual void run() {
            m_audioManager->procRemoveAudioTuile(m_audioTuile);
        }
        virtual void createClones(const unsigned int& nbClones) {
            for(unsigned int c=0; c<nbClones; ++c) {
                m_clones.push_back(new RemoveAudioTuile());
                m_clones.back()->setModel(this);
            }
        }
        inline void setAudioTuile(AudioTuile* tuile){m_audioTuile=tuile;}
        inline void setAudioManager(AudioManager* man){m_audioManager=man;}

	protected:
        AudioTuile* m_audioTuile;
        AudioManager* m_audioManager;
        
};

#endif

