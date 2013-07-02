/***************************************************************************
 *  FaustTuile.hpp
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef FaustTuile_h
#define FaustTuile_h

#include <faust/llvm-dsp.h>
#include <faust/gui/UI.h>

#include "AudioTuile.hpp"

class FaustTuile : public AudioTuile{	
	public:
		FaustTuile();
		~FaustTuile();

		virtual void load(const std::string&);
		void unload();
	
		void activate();
		void deactivate();

        virtual void processBuffers(const int& nbFrames);

        inline llvm_dsp* getDSP(){return m_dsp;}

	private:
        llvm_dsp* m_dsp;
        llvm_dsp_factory* m_dspFactory;
        bool m_processing;
        unsigned int m_inputChannels;
        unsigned int m_outputChannels;
        unsigned int m_bufferSize;
        float **m_dspInputBuffer;
        float **m_dspOutputBuffer;
        bool m_activateAsked;
};

#endif

