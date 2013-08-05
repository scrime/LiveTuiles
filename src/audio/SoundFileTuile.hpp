/***************************************************************************
 *  SoundFileTuile.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef SoundFileTuile_h
#define SoundFileTuile_h

#include <vector>
#include <list>
#include <map>

#include "AudioTuile.hpp"
#include "Grain.hpp"

class SetSoundFileProperties;

class SoundFileTuile : public AudioTuile {	
	public:
		SoundFileTuile();
		~SoundFileTuile();

		virtual void load(const std::string&);
		void unload();
		void setSampleRate(const unsigned int&);

		void activate();
		void deactivate();

        void setLength(const long&);
        inline virtual void setPositionRatio(const float& ratio){
            m_position=ratio*(float)m_framesCount;
        }
        const float& getVolume(){return m_volume;}
        void setVolume(const float& vol);

		virtual void processBuffers(const int& nbFrames);

    protected:
        friend class SetSoundFileProperties;
        SetSoundFileProperties* m_protoSetSFProps;
    
    private:
        void updateSoundFileProperties();

	private:
        float m_volume;

        float m_procVolume;
		jack_default_audio_sample_t** m_buffers;
		unsigned int m_channelsCount;
		unsigned int m_sampleRate;
		unsigned long m_framesCount; 
		float m_floatFramesCount;
        unsigned int m_filePosition;

		std::list<Grain> m_grains;

		float m_grainVolume;
		unsigned long m_grainSize;
		unsigned long m_windowSize;
		unsigned long m_windowStart;
		unsigned long m_grainDistance;
		unsigned long m_grainDistanceCounter;

		unsigned long m_elapsedFrames;
		float m_speed;
		unsigned long m_setWindowStart;
		unsigned long m_lastSetWindowStart;

		std::map<unsigned int, jack_default_audio_sample_t*> m_envelopes;
		jack_default_audio_sample_t* m_env;
};

#endif

