/***************************************************************************
 *  AudioManager.hpp
 *  2012 Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/


#ifndef AudioManager_h
#define AudioManager_h

#include <string>
#include <vector>

#include <jack/jack.h>
#include <jack/types.h>
#include <jack/midiport.h>

class AudioTuile;
class FaustTuile;
class SoundFileTuile;
class AudioInputTuile;
class AudioOutputTuile;

class AudioManager {	
	public:
		static AudioManager* getInstance();
		~AudioManager();

		void init();
		void update();
        void clear();
        void start();
        void stop();
        void togglePlay();
        void setBpm(const float&);

		friend int jackCallback(jack_nframes_t , void *);
        void process(const int&);

        inline const unsigned int& getSampleRate(){return m_sampleRate;}
        inline const unsigned int& getBufferSize(){return m_bufferSize;}
        inline jack_client_t* getJackClient(){return m_jackClient;}

        FaustTuile* addFaustTuile(const std::string& fileName);
        SoundFileTuile* addSoundFileTuile(const std::string& fileName);
        AudioInputTuile* addAudioInputTuile(const std::string& input);
        AudioOutputTuile* addAudioOutputTuile(const std::string& output);

        bool isPlaying();
        const float& getPlayPosition();
        const float& getPlayPositionInBeats();
        const float& getBpm(){return m_bpm;}

    protected:

	private:
		AudioManager();

		jack_client_t *m_jackClient;
		unsigned int m_sampleRate;	
		unsigned int m_bufferSize;	
        float m_bpm;
        float m_playPosInBeats;

        std::vector<AudioTuile*> m_procAudioTuiles;
        float m_procStep;

		static const char MIDI_NOTE_ON=0x90;
		static const char MIDI_NOTE_OFF=0x80;
		static const char MIDI_PROGRAM_CHANGE=0xC0;
		static const char MIDI_CONTROLLER=0xB0;
};

#endif

