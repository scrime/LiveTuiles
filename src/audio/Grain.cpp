/***************************************************************************
 *  Grain.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "Grain.hpp"

using namespace std;

Grain::Grain(const float& volume, const unsigned long& size, 
		const unsigned long& offset, 
		jack_default_audio_sample_t* envelope, 
		jack_default_audio_sample_t** buffers, 	
		const unsigned long& nbFrames, 
		const unsigned int& nbChannels):
											m_volume(volume), 
											m_size(size), m_offset(offset), 
											m_envelope(envelope), 
											m_buffers(buffers), 
											m_nbChannels(nbChannels),
											m_nbFrames(nbFrames),
											m_counter(0){
	m_firstBufferID=0;
	m_secondBufferID=(m_nbChannels>=2)?1:0;
}

Grain::~Grain() {}

void Grain::process(const unsigned int& nbFrames, 
                        vector< vector<float> >& buf) {
    for(unsigned long f=0; f<nbFrames && m_counter<m_size 
                    && (m_counter+m_offset)<m_nbFrames; ++f, ++m_counter) {
        for(unsigned int c=0;c<buf.size() && c<m_nbChannels; ++c) {
            buf[c][f]+=m_buffers[c][m_offset+m_counter]
                            *m_envelope[m_counter]*m_volume;
        }
    }
}

