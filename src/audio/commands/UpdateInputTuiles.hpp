/***************************************************************************
 *  UpdateInputTuiles.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef UpdateInputTuiles_h
#define UpdateInputTuiles_h

#include <tuiles/Command.hpp>
#include "../AudioManager.hpp"

class UpdateInputTuiles: public tuiles::Command {
	public:
		inline UpdateInputTuiles(){m_name="UpdateInputTuiles";}
		inline virtual ~UpdateInputTuiles(){};
		virtual void run() {
            m_audioTuile->procUpdateInputTuiles(m_inputTuiles);
        }
        virtual void createClones(const unsigned int& nbClones) {
            for(unsigned int c=0; c<nbClones; ++c) {
                m_clones.push_back(new UpdateInputTuiles());
                m_clones.back()->setModel(this);
            }
        }
        inline void setAudioTuile(AudioTuile* tuile){m_audioTuile=tuile;}
        inline void setInputTuiles(const std::vector<AudioTuile*>& inp){
                        m_inputTuiles=inp;
        }

	protected:
        AudioTuile* m_audioTuile;
        std::vector<AudioTuile*> m_inputTuiles;
        
};

#endif

