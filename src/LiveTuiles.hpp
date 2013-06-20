/***************************************************************************
 *  LiveTuiles.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef _LiveTuiles_h
#define _LiveTuiles_h

class LiveTuiles {
	public:
		static LiveTuiles* getInstance();
		~LiveTuiles();
		void start();

	private:
		LiveTuiles();
};

#endif

