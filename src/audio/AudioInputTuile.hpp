/***************************************************************************
 *  AudioInputTuile.hpp
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef AudioInputTuile_h
#define AudioInputTuile_h

#include "AudioTuile.hpp"
#include <jack/jack.h>

class AudioInputTuile : public AudioTuile {	
	public:
		AudioInputTuile();
		~AudioInputTuile();

		virtual void load(const std::string&);
		void unload();
        virtual void load(xmlNodePtr node);

        virtual void processBuffers(const int& nbFrames);

	private:
		jack_port_t* m_inputPortLeft;
		jack_port_t* m_inputPortRight;
};

#endif

