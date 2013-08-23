/***************************************************************************
 *  SetProcLoaded.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef SetProcLoaded_h
#define SetProcLoaded_h

#include <tuiles/Command.hpp>
#include "../SoundFileTuile.hpp"

class SetProcLoaded: public tuiles::Command {
	public:
		inline SetProcLoaded(){m_name="SetProcLoaded";}
		inline virtual ~SetProcLoaded(){};
		virtual void run() {
            m_tuile->procSetLoaded(m_loaded);
        }
        virtual void createClones(const unsigned int& nbClones) {
            for(unsigned int c=0; c<nbClones; ++c) {
                m_clones.push_back(new SetProcLoaded());
                m_clones.back()->setModel(this);
            }
        }
        inline void setAudioTuile(AudioTuile* tuile){m_tuile=tuile;}
        inline void setLoaded(const bool& load){m_loaded=load;}

	protected:
        AudioTuile* m_tuile; 
        bool m_loaded;
};

#endif

