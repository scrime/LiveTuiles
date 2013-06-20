/***************************************************************************
 *  AudioTuile.hpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#ifndef AudioTuile_h
#define AudioTuile_h

#include <vector>

#include <tuiles/LeafTuile.hpp>
#include <jack/types.h>

class AudioTuile: public LeafTuile {
    public:
        AudioTuile();
        virtual ~AudioTuile();
        virtual void processBuffers(const int& nbFrames)=0;
        void resetBuffers();

        void addInputTuile(AudioTuile*);
        void removeInputTuile(AudioTuile*);
        inline void clearInputTuiles(){m_inputTuiles.clear();}

        inline std::vector<std::vector<jack_default_audio_sample_t> >& 
                    getBuffer(){return m_internalBuffer;}

    protected: 
        std::string m_fileName;
        bool m_loaded;
        float m_lengthInMs;
        bool m_computed;
        float m_volume;

        std::vector<AudioTuile*> m_inputTuiles;
        std::vector<std::vector<jack_default_audio_sample_t> > m_internalBuffer;
};

#endif

