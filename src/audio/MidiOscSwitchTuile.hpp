/***************************************************************************
 *  MidiOscSwitchTuile.hpp
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef MidiOscSwitchTuile_h
#define MidiOscSwitchTuile_h

#include <tuiles/SwitchTuile.hpp>

class MidiOscSwitchTuile : public tuiles::SwitchTuile {	
	public:
		MidiOscSwitchTuile();
		~MidiOscSwitchTuile();
		virtual void load(const std::string&);
		void unload();
		void activate();
		void deactivate();
        virtual void processBuffers(const int& nbFrames);

	private:
};

#endif

