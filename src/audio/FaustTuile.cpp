/***************************************************************************
 *  FaustTuile.cpp
 *  2012- Florent Berthaut
 *  ANR INEDIT Project
 *  This file is part of LiveTuiles
 ****************************************************************************/

#include "FaustTuile.hpp"

#include <sstream>
#include <fstream>
#include <stdexcept>

#include "AudioManager.hpp"

using namespace std;

FaustTuile::FaustTuile():   AudioTuile(), 
                            m_processing(false),
                            m_bufferSize(4096) {}

FaustTuile::~FaustTuile() {}

void FaustTuile::load(const std::string& fileName) {
	m_fileName=fileName;
    m_name=m_fileName;

    //get string with content of file
    ifstream fileStream(m_fileName.c_str());
    string fileContent, line;
    if (!fileStream.is_open()) {
        throw std::logic_error("Error opening file "+fileName);
    }
    while (fileStream.good() ) {
        getline (fileStream,line);
        fileContent+=line+'\n';
    }
    fileStream.close();

    char errorMsg[256];
    m_dspFactory = createDSPFactory(0, NULL, "", "", m_fileName.c_str(), 
                                        fileContent.c_str(), "", errorMsg, 3);
    if(!m_dspFactory) {
        throw std::logic_error("Error compiling file "+fileName+" "+errorMsg);
    }
    m_dsp = createDSPInstance(m_dspFactory);
    if(!m_dsp) {
        throw std::logic_error("Error compiling file "+fileName+" "+errorMsg);
    }
    m_dsp->init(AudioManager::getInstance()->getSampleRate());
    m_bufferSize = AudioManager::getInstance()->getBufferSize();
    m_inputChannels = m_dsp->getNumInputs();
    m_dspInputBuffer = new float*[m_inputChannels];
    for(int c=0; c<m_inputChannels; ++c) {
        m_dspInputBuffer[c]=new float[m_bufferSize];
    }
    m_outputChannels = m_dsp->getNumOutputs();
    m_dspOutputBuffer = new float*[m_outputChannels];
    for(int c=0; c<m_outputChannels; ++c) {
        m_dspOutputBuffer[c]=new float[m_bufferSize];
    }

    m_internalBuffer.resize(m_outputChannels, vector<float>(m_bufferSize, 0));

    m_loaded=true;
}

void FaustTuile::unload() {
	m_loaded=false;
}

void FaustTuile::activate() {
    if(m_loaded) {
        m_procActive=true;
        m_activateAsked=true;
    }
}

void FaustTuile::deactivate() {
	m_active=false;
}

void FaustTuile::processBuffers(const int& nbFrames) {
    if(!m_computed) {
        for(unsigned int c=0; c<m_internalBuffer.size(); ++c) {
            m_internalBuffer[c].assign(nbFrames, 0);
            for(int f=0; f<nbFrames && f<m_bufferSize; ++f) {
                m_dspInputBuffer[c][f]=0;
            }
        }
        //set computed before to avoid infinite recursions
        m_computed=true;
        if(m_procActive && m_inputTuiles.size()>0) {
            //accumulate the buffers of all inputs
            vector<AudioTuile*>::iterator itInp=m_inputTuiles.begin();
            for(; itInp!=m_inputTuiles.end(); ++itInp) {
                (*itInp)->processBuffers(nbFrames);
                const vector<vector<float> >& inpBuf = (*itInp)->getBuffer(); 
                for(int c=0; c<m_inputChannels && c<(int)inpBuf.size(); ++c) {
                    for(int f=0;f<m_bufferSize && f<(int)inpBuf[c].size(); ++f){
                        m_dspInputBuffer[c][f]+=inpBuf[c][f];
                    }
                }
            }
            m_dsp->compute(nbFrames, m_dspInputBuffer, m_dspOutputBuffer);
            for(unsigned int c=0; c<m_internalBuffer.size(); ++c) {
                for(int f=0; f<nbFrames && f<m_bufferSize; ++f) {
                    m_internalBuffer[c][f]=m_dspOutputBuffer[c][f]*m_procVolume;
                }
            }
        }
    }
}


