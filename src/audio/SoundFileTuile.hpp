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

#include <tuiles/Voice.hpp>
#include "AudioTuile.hpp"
#include "Grain.hpp"

class SoundFileVoice {
    public:
        float& editProcPosition(){return m_procPosition;}
        const float& getProcPosition(){return m_procPosition;}
    private:
        float m_procPosition;
};

class SetSoundFileProperties;

class SoundFileTuile : public AudioTuile {	
	public:
		SoundFileTuile();
		~SoundFileTuile();

		virtual void load(const std::string&);
		void unload();

        xmlNodePtr save(xmlNodePtr parentNode);
        void load(xmlNodePtr);

		void setSampleRate(const unsigned int&);

        virtual void processPos(const float& pos, const tuiles::Voice&);
		void activate(const tuiles::Voice&);
		void deactivate(const tuiles::Voice&);

        void setLength(const long&);
        inline virtual void setPositionRatio(const float& ratio){
            m_position=ratio*(float)m_framesCount;
        }

		virtual void processBuffers(const int& nbFrames);

    protected:
        friend class SetSoundFileProperties;
        SetSoundFileProperties* m_protoSetSFProps;
    
    private:
        void updateSoundFileProperties();

	private:
		jack_default_audio_sample_t** m_buffers;
		unsigned int m_channelsCount;
		unsigned int m_sampleRate;
		unsigned long m_framesCount; 
		float m_floatFramesCount;
		float m_sfSpeed;

        std::map<std::string, SoundFileVoice> m_voices;
		std::list<Grain> m_grains;

		float m_grainVolume;
		unsigned long m_grainSize;
		unsigned long m_windowSize;
		unsigned long m_windowStart;
		unsigned long m_grainDistance;
		unsigned long m_grainDistanceCounter;
        float m_procSfSpeed;
		unsigned long m_elapsedFrames;
		unsigned long m_setWindowStart;
		unsigned long m_lastSetWindowStart;

		std::map<unsigned int, jack_default_audio_sample_t*> m_envelopes;
		jack_default_audio_sample_t* m_env;
};

#endif

