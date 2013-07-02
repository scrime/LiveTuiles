/***************************************************************************
 *  AddAudioTuile.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef AddAudioTuile_h
#define AddAudioTuile_h

#include <tuiles/Command.hpp>
#include "../AudioManager.hpp"

class AddAudioTuile: public Command {
	public:
		inline AddAudioTuile(){}
		inline virtual ~AddAudioTuile(){};
		virtual void run() {
            m_audioManager->m_audioTuiles.push_back(m_audioTuile);
        }
        virtual void createClones(const unsigned int& nbClones) {
            for(unsigned int c=0; c<nbClones; ++c) {
                m_clones.push_back(new AddAudioTuile());
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

