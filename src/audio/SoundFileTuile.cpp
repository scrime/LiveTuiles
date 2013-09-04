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

#include <tuiles/CommandsHandler.hpp>

#include "commands/SetSoundFileProperties.hpp"

using namespace std;
using namespace tuiles;

SoundFileTuile::SoundFileTuile():   AudioTuile(),
                                    m_sfSpeed(1), 
                                    m_grainVolume(1),
                                    m_grainSize(4096),
                                    m_windowSize(2),
                                    m_windowStart(0),
                                    m_grainDistance(2048),
                                    m_grainDistanceCounter(0),
                                    m_procSfSpeed(1),
                                    m_elapsedFrames(64),
                                    m_setWindowStart(0),
                                    m_lastSetWindowStart(0) {
    m_type="SoundFile";
	m_envelopes[m_grainSize] = new jack_default_audio_sample_t[m_grainSize];
	for(unsigned int i=0; i<m_grainSize; ++i) {
		m_envelopes[m_grainSize][i]=0.5*(1.0 - cos((2.0*M_PI*float(i))
                                                        /(m_grainSize-1)));
	}
    m_protoSetSFProps = new SetSoundFileProperties();
    m_protoSetSFProps->createClones(m_nbCommands);
}

SoundFileTuile::~SoundFileTuile(){}

void SoundFileTuile::load(const std::string& fileName) {
	//get the file
	SF_INFO sfInfo;
	sfInfo.format=0;
	SNDFILE* sndFile = sf_open(fileName.c_str(), SFM_READ, &sfInfo);
    if(sndFile) {

        m_fileName=fileName;
        m_name=m_fileName;
        
        //test the Samplerate
        m_sampleRate = sfInfo.samplerate;
        
        //channels
        m_channelsCount = sfInfo.channels;
        m_internalBuffer.resize(m_channelsCount);
        
        //frames number
        m_framesCount = (long)(sfInfo.frames);
        m_length=m_framesCount;
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

        DEBUG("SoundFileTuile "<<m_name<<" loaded");
        m_loaded=true;
        updateLoaded();
    }
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
    updateLoaded();
}

void SoundFileTuile::processPos(const float& pos, const Voice& voice) {
    LeafTuile::processPos(pos, voice);
    //FIXME should be able to use activate/deactivate for voices
    if(m_voices.find(voice.getID())==m_voices.end()) {
        m_voices[voice.getID()]=SoundFileVoice();
        m_voices[voice.getID()].editProcPosition()=m_procPosition;
    }
}

//FIXME should be able to use activate/deactivate for voices 
void SoundFileTuile::activate(const Voice& voice) {
/*
    if(m_voices.find(voice.getID())==m_voices.end()) {
        cout<<"activate voice "<<voice.getID()<<endl;
        m_voices[voice.getID()] = SoundFileVoice();
    }
*/
}
void SoundFileTuile::deactivate(const Voice& voice) {
/*
    m_voices.erase(voice.getID());
*/
}

void SoundFileTuile::setLength(const long& length) {
    LeafTuile::setLength(length);
    m_speed = float(m_framesCount)/float(m_length);
    updateSoundFileProperties();
}

void SoundFileTuile::updateSoundFileProperties() {
    SetSoundFileProperties* com = static_cast<SetSoundFileProperties*>
                                                (m_protoSetSFProps->popClone());
    if(com) {
        com->setSoundFileTuile(this);
        com->setSpeed(m_sfSpeed);
        m_commandsToProc->runCommand(com);
    }
}

void SoundFileTuile::processBuffers(const int& nbFrames) {
    if(!m_computed && m_procLoaded) {
        for(unsigned int c=0; c<m_channelsCount; ++c) {
            m_internalBuffer[c].resize(nbFrames);
            m_internalBuffer[c].assign(m_internalBuffer[c].size(), 0);
        }
        m_computed=true;
/*
        if(m_procActive) {
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
                m_grains.push_back(Grain(m_grainVolume*m_procVolume, 
                                            m_grainSize, 
                                            rand()%m_windowSize
                                            +(m_procPosition/m_procLength)
                                            *float(m_framesCount),
                                            m_envelopes[m_grainSize],
                                            m_buffers, m_framesCount, 
                                            m_channelsCount));
                m_grainDistanceCounter=nbFrames;
            }
            else {
                m_grainDistanceCounter+=nbFrames;
            }
        }
*/

        if(m_grainDistanceCounter>=m_grainDistance) {
            if(m_procSfSpeed<1.0) {
                m_windowSize=1000.0*(1.0-m_speed)+1;
                m_grainDistance = m_grainSize/3.0;
            }
            else if(m_procSfSpeed>1.0) {
                m_windowSize=1;
                m_grainDistance = m_grainSize/3.0;
            }
            else {
                m_windowSize=1;
                m_grainDistance = m_grainSize/2.0;
            }
            map<string, SoundFileVoice>::iterator itVoice = m_voices.begin();
            for(; itVoice!=m_voices.end(); ++itVoice) {
                m_grains.push_back(Grain(m_grainVolume*m_procVolume, 
                                            m_grainSize, 
                                            rand()%m_windowSize
                                            +(itVoice->second.getProcPosition()
                                                / m_procLength)
                                            *float(m_framesCount),
                                            m_envelopes[m_grainSize],
                                            m_buffers, m_framesCount, 
                                            m_channelsCount));
            }
            m_grainDistanceCounter=nbFrames;
        }
        else {
            m_grainDistanceCounter+=nbFrames;
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

        //FIXME should be able to use activate/deactivate for voices
        m_voices.clear();
    }
}


xmlNodePtr SoundFileTuile::save(xmlNodePtr parentNode) {
    xmlNodePtr node = AudioTuile::save(parentNode);
    xmlNewProp(node, BAD_CAST "file", BAD_CAST m_fileName.c_str());
    return node;
}

void SoundFileTuile::load(xmlNodePtr node) {
    char* value=NULL;
    value=NULL;
    value = (char*)xmlGetProp(node,(xmlChar*)"file");
    if(value) {
        load(std::string(value));
    }
}

