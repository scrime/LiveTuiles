/***************************************************************************
 *  SetProcVolume.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef SetProcVolume_h
#define SetProcVolume_h

#include <tuiles/Command.hpp>
#include "../SoundFileTuile.hpp"

class SetProcVolume: public tuiles::Command {
	public:
		inline SetProcVolume(){m_name="SetProcVolume";}
		inline virtual ~SetProcVolume(){};
		virtual void run() {
            m_tuile->procSetVolume(m_volume);
        }
        virtual void createClones(const unsigned int& nbClones) {
            for(unsigned int c=0; c<nbClones; ++c) {
                m_clones.push_back(new SetProcVolume());
                m_clones.back()->setModel(this);
            }
        }
        inline void setAudioTuile(AudioTuile* tuile){m_tuile=tuile;}
        inline void setVolume(const float& vol){m_volume=vol;}

	protected:
        AudioTuile* m_tuile; 
        float m_volume;
};

#endif

