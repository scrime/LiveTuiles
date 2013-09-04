/***************************************************************************
 *  SetSoundFileProperties.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef SetSoundFileProperties_h
#define SetSoundFileProperties_h

#include <tuiles/Command.hpp>
#include "../SoundFileTuile.hpp"

class SetSoundFileProperties: public tuiles::Command {
	public:
		inline SetSoundFileProperties(){m_name="SetSoundFileProperties";}
		inline virtual ~SetSoundFileProperties(){};
		virtual void run() {
            m_tuile->m_procSfSpeed=m_speed;
        }
        virtual void createClones(const unsigned int& nbClones) {
            for(unsigned int c=0; c<nbClones; ++c) {
                m_clones.push_back(new SetSoundFileProperties());
                m_clones.back()->setModel(this);
            }
        }
        inline void setSoundFileTuile(SoundFileTuile* tuile){m_tuile=tuile;}
        inline void setSpeed(const float& speed){m_speed=speed;}

	protected:
        SoundFileTuile* m_tuile; 
        float m_speed;
};

#endif

