/***************************************************************************
 *  SetProcStep.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of libTuiles
 ****************************************************************************/

#ifndef SetProcStep_h
#define SetProcStep_h

#include "Command.hpp"

class SetProcStep: public Command {
	public:
		inline SetProcStep(){}
		inline virtual ~SetProcStep(){};
		virtual void run() {
            m_audioManager->m_procStep=m_step;
        }
        virtual void createClones(const unsigned int& nbClones) {
            for(unsigned int c=0; c<nbClones; ++c) {
                m_clones.push_back(new SetProcStep());
                m_clones.back()->setModel(this);
            }
        }
        inline void setAudioManager(AudioManager* man){m_audioManager=man;}
        inline void setStep(const float& step){m_step=step;}

	protected:
        AudioManager* m_audioManager;
        float m_step;
};

#endif

