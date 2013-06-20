/***************************************************************************
 *  Grain.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef Grain_h
#define Grain_h

#include <vector>
#include <jack/types.h>

class Grain {	
	public:
		Grain(const float& volume, 
				const unsigned long& size, const unsigned long& offset, 
				jack_default_audio_sample_t* envelope, 
				jack_default_audio_sample_t** buffers,
				const unsigned long& nbFrames, 
				const unsigned int& nbChannels);
		~Grain();
		
/*
		void process(const unsigned int& nbFrames, 
					jack_default_audio_sample_t* portLeft, 
					jack_default_audio_sample_t* portRight);
*/
		void process(const unsigned int& nbFrames, 
                        std::vector<std::vector<float> >&);


		inline bool isDone(){return m_counter>=m_size;}

	protected:
		float m_volume;
		unsigned long m_size;
		unsigned long m_offset;
		jack_default_audio_sample_t* m_envelope;
		jack_default_audio_sample_t** m_buffers;
		unsigned int m_nbChannels;
		unsigned long m_nbFrames;
		unsigned int m_firstBufferID;
		unsigned int m_secondBufferID;

		unsigned long m_counter;

};

#endif

