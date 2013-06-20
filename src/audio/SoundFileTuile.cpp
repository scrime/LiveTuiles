/***************************************************************************
 *  SoundFileTuile.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "SoundFileTuile.hpp"

#include <sndfile.h>
#include <samplerate.h>
#include <math.h>
#include <stdexcept>

using namespace std;

SoundFileTuile::SoundFileTuile():   AudioTuile(),
                                    m_grainVolume(1),
                                    m_grainSize(4096),
                                    m_windowSize(2),
                                    m_windowStart(0),
                                    m_grainDistance(2048),
                                    m_grainDistanceCounter(0),
                                    m_elapsedFrames(64),
                                    m_speed(1), 
                                    m_setWindowStart(0),
                                    m_lastSetWindowStart(0) {

	m_envelopes[m_grainSize] = new jack_default_audio_sample_t[m_grainSize];
	for(unsigned int i=0; i<m_grainSize; ++i) {
		m_envelopes[m_grainSize][i]=0.5*(1.0 - cos((2.0*M_PI*float(i))
                                                        /(m_grainSize-1)));
	}
}

SoundFileTuile::~SoundFileTuile(){}

void SoundFileTuile::load(const std::string& fileName) {

	m_fileName=fileName;

	//get the file
	SF_INFO sfInfo;
	sfInfo.format=0;
	SNDFILE* sndFile = sf_open(m_fileName.c_str(), SFM_READ, &sfInfo);

	if(!sndFile) {
		throw logic_error("Unable to open sound file");
	}
	
	//test the Samplerate
	m_sampleRate = sfInfo.samplerate;
	
	//channels
	m_channelsCount = sfInfo.channels;
    m_internalBuffer.resize(m_channelsCount);
	
	//frames number
	m_framesCount = (long)(sfInfo.frames);
    m_lengthInMs = float(m_framesCount)/(float(m_sampleRate)/1000.0);

	//create the buffers
	m_buffers = new jack_default_audio_sample_t*[m_channelsCount];
	for(unsigned int i=0;i<m_channelsCount;++i) {
		m_buffers[i] = new jack_default_audio_sample_t[m_framesCount];
	}

	int maxNbFramesToRead=1000;
	float cf[m_channelsCount * maxNbFramesToRead];
	unsigned int nbFrames;
	unsigned int pos = 0;
	while ((nbFrames = sf_readf_float(sndFile,cf,maxNbFramesToRead)) > 0) {
		for (unsigned int i = 0; i < nbFrames*m_channelsCount; ++pos) {
			for(unsigned int j=0; j<m_channelsCount; ++j) {
				m_buffers[j][pos] = jack_default_audio_sample_t(cf[i]);
				++i;
			}
		}
	}

	//close the file
	if(sf_close(sndFile)) {
		throw logic_error("Unable to close sound file");
	}

	m_loaded=true;
}

void SoundFileTuile::setSampleRate(const unsigned int& sr) {
	/*
	m_sampleRate=sr;
	if(sr != m_sampleRate && m_loaded) {
		m_loaded=false;
		long newFramesCount = (long)((double)m_framesCount / (double) m_sampleRate * (double)sr); 
		float** convs = new float*[m_channelsCount];
		SRC_DATA *datas = new SRC_DATA[m_channelsCount];
		for(int i=0;i<m_channelsCount;++i) {
			convs[i] = new float[newFramesCount];
			datas[i].data_in = m_buffers[i];
			datas[i].data_out = convs[i];
			datas[i].input_frames = m_framesCount;
			datas[i].output_frames = newFramesCount;
			datas[i].src_ratio = (double)sr / (double)m_sampleRate;
			src_simple(datas+i, 0, 1);
		}

		for(int i=0;i<m_channelsCount;++i) {
			delete [] m_buffers[i];	
			m_buffers[i] = new float[datas[i].output_frames_gen];
			for(int j=0;j<datas[i].output_frames_gen;++j) {
				m_buffers[i][j] = convs[i][j];
			}
		}

		m_framesCount = newFramesCount;
		m_floatFramesCount = float(m_framesCount);
		m_sampleRate = sr;

		for(int i=0;i<m_channelsCount;++i) {
			delete [] convs[i];
		}
		delete [] convs;
		delete [] datas;
		
		m_loaded=true;
	}
	*/
}

void SoundFileTuile::unload() {
	//unload the file
	if(m_buffers) {
		for(unsigned int c=0; c<m_channelsCount; ++c) {
			delete [] m_buffers[c];
		}
		delete [] m_buffers;
	}
	m_loaded=false;
}

void SoundFileTuile::activate() {
    //TODO compute position in file from position and speed

}

void SoundFileTuile::deactivate() {
}

void SoundFileTuile::setLength(const long& length) {
    m_length=length;
    m_speed= float(m_framesCount)/float(m_length);
}

void SoundFileTuile::processBuffers(const int& nbFrames) {
    if(!m_computed) {
        for(unsigned int c=0; c<m_channelsCount; ++c) {
            m_internalBuffer[c].resize(nbFrames);
            m_internalBuffer[c].assign(m_internalBuffer[c].size(), 0);
        }
        if(m_active) {
            //start new grain ?
            if(m_grainDistanceCounter>=m_grainDistance) {
                if(m_speed<1.0) {
                    m_windowSize=1000.0*(1.0-m_speed)+1;
                    m_grainDistance = m_grainSize/3.0;
                }
                else if(m_speed>1.0) {
                    m_windowSize=1;
                    m_grainDistance = m_grainSize/3.0;
                }
                else {
                    m_windowSize=1;
                    m_grainDistance = m_grainSize/2.0;
                }
                m_grains.push_back(Grain(m_grainVolume*m_volume, m_grainSize, 
                                            rand()%m_windowSize+m_position,
                                            m_envelopes[m_grainSize],
                                            m_buffers, m_framesCount, 
                                            m_channelsCount));
                m_grainDistanceCounter=nbFrames;
            }
            else {
                m_grainDistanceCounter+=nbFrames;
            }
            m_filePosition+=(float)nbFrames * m_speed;
        }
        //process grains
        list<Grain>::iterator itGrain; 
        for(itGrain=m_grains.begin(); itGrain!=m_grains.end();) {
            (*itGrain).process(nbFrames, m_internalBuffer);
            if((*itGrain).isDone()) {
                itGrain=m_grains.erase(itGrain);
            }
            else {
                ++itGrain;
            }
        }
        m_computed=true;
    }
}




