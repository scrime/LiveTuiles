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

#include <tuiles/TuilesManager.hpp>

class AudioTuile;
class FaustTuile;
class SoundFileTuile;
class AudioInputTuile;
class AudioOutputTuile;
class MidiOscSwitchTuile;
class MidiOscMonitorTuile;

class AddAudioTuile;
class RemoveAudioTuile;
class SetProcStep;

class AudioManager : public tuiles::TuilesManager {
	public:
		static AudioManager* getInstance();
		~AudioManager();

		void init();
        void close();

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
        tuiles::SeqTuile* addSeqTuile();
        tuiles::LoopTuile* addLoopTuile();
        MidiOscMonitorTuile* addMidiOscMonitorTuile();
        MidiOscSwitchTuile* addMidiOscSwitchTuile();
        void deleteAudioTuile(AudioTuile*);
        void extractAudioTuile(AudioTuile*);

        const float& getPlayPositionInBeats();
        const float& getBpm(){return m_bpm;}
        const float& getFramesPerBeat(){return m_framesPerBeat;}
        void framesToBeats(const float& frames, float& beats);

    protected:
        void procClearTrees();

    protected:
        friend class AddAudioTuile;
        AddAudioTuile* m_protoAddAudioTuile;
        friend class RemoveAudioTuile;
        RemoveAudioTuile* m_protoRemoveAudioTuile;
        friend class SetProcStep;
        SetProcStep* m_protoSetProcStep;

        void procRemoveAudioTuile(AudioTuile*);

    private:
        void internalAddAudioTuile(AudioTuile*);

	private:
		AudioManager();

		jack_client_t *m_jackClient;
		unsigned int m_sampleRate;	
		unsigned int m_bufferSize;	
        float m_bpm;
        float m_framesPerBeat;
        float m_playPosInBeats;
        float m_defaultLength;

        std::vector<AudioTuile*> m_procAudioTuiles;
        float m_procStep;

		jack_port_t *m_midiInputProc;
		static const char MIDI_NOTE_ON=0x90;
		static const char MIDI_NOTE_OFF=0x80;
		static const char MIDI_PROGRAM_CHANGE=0xC0;
		static const char MIDI_CONTROLLER=0xB0;
};

#endif

