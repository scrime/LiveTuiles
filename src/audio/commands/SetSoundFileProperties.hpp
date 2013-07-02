/***************************************************************************
 *  SetSoundFileProperties.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef SetSoundFileProperties_h
#define SetSoundFileProperties_h

#include "Command.hpp"

class SetSoundFileProperties: public Command {
	public:
		inline SetSoundFileProperties(){}
		inline virtual ~SetSoundFileProperties(){};
		virtual void run() {
            m_tuile->m_procVolume=m_volume;
        }
        virtual void createClones(const unsigned int& nbClones) {
            for(unsigned int c=0; c<nbClones; ++c) {
                m_clones.push_back(new SetSoundFileProperties());
                m_clones.back()->setModel(this);
            }
        }
        inline void setSoundFileTuile(Tuile* tuile){m_tuile=tuile;}

	protected:
        SoundFileTuile* m_tuile; 
        float m_volume
};

#endif

