#ifndef MICROPHONE_H
#define MICROPHONE_H

#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>
#include <cstring>


class Record
{
    private:
        char * m_FrameBuffer;
        char * m_TotalBuffer;

        snd_pcm_t *m_Handle;
        snd_pcm_hw_params_t *m_Params;
        snd_pcm_uframes_t  m_Frame;
        unsigned int m_Quality;
        int m_Directory;
        size_t m_FrameSize;
        size_t m_FrameCount;
        size_t m_BufferSize;

    public:
        Record(unsigned int quality, snd_pcm_uframes_t frame, size_t frameCount);
        ~Record();
        int openDevice(const char * name);
        int closeDevice();

        const char* getRecord();
        size_t getBufferSize();
        bool isActivate();


};

class Player
{
    private:
        snd_pcm_t *m_Handle;
        snd_pcm_hw_params_t *m_Params;
        snd_pcm_uframes_t  m_Frame;
        unsigned int m_Quality;
        int m_Directory;
        size_t m_FrameSize;
        size_t m_FrameCount;
        size_t m_BufferSize;

    public:
        Player(unsigned int quality, snd_pcm_uframes_t frame, size_t frameCount);
        ~Player();
        int openDevice(const char * name);
        int closeDevice();
        int playRecord(const char * buffer);
        size_t getBufferSize();

};


#endif // MICROPHONE_H
