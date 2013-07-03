/***************************************************************************
 *  MidiOscMonitorTuile.hpp
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef MidiOscMonitorTuile_h
#define MidiOscMonitorTuile_h

#include "AudioTuile.hpp"

class MidiOscMonitorTuile : public AudioTuile{	
	public:
		MidiOscMonitorTuile();
		~MidiOscMonitorTuile();

		virtual void load(const std::string&);
		void unload();
	
		void activate();
		void deactivate();

        virtual void processBuffers(const int& nbFrames);

	private:

};

#endif

