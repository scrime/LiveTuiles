/***************************************************************************
 *  AudioOutputTuile.hpp
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef AudioOutputTuile_h
#define AudioOutputTuile_h

#include "AudioTuile.hpp"

class AudioOutputTuile : public AudioTuile {	
	public:
		AudioOutputTuile();
		~AudioOutputTuile();

		virtual void load(const std::string&);
		void unload();
	
        virtual void processBuffers(const int& nbFrames);

	private:
		jack_port_t* m_outputPortLeft;
		jack_port_t* m_outputPortRight;

};

#endif

