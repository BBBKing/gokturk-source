#include "microphone.h"

Record::Record(unsigned int quality, snd_pcm_uframes_t frame, size_t frameCount)
    :m_FrameBuffer(nullptr),
     m_TotalBuffer(nullptr),
     m_Handle(nullptr),
     m_Params(nullptr),
     m_Frame(frame),
     m_Quality(quality),
     m_FrameCount(frameCount)
{

}

Record::~Record()
{

}

int Record::openDevice(const char * name)
{
    snd_pcm_open(&m_Handle, name, SND_PCM_STREAM_CAPTURE, 0);
    snd_pcm_hw_params_alloca(&m_Params);
    snd_pcm_hw_params_any(m_Handle, m_Params);
    snd_pcm_hw_params_set_access(m_Handle, m_Params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(m_Handle, m_Params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(m_Handle, m_Params, 2);
    snd_pcm_hw_params_set_rate_near(m_Handle, m_Params, &m_Quality, &m_Directory);
    snd_pcm_hw_params_set_period_size_near(m_Handle, m_Params, &m_Frame, &m_Directory);
    snd_pcm_hw_params(m_Handle, m_Params);
    snd_pcm_hw_params_get_period_size(m_Params,&m_Frame, &m_Directory);
    snd_pcm_hw_params_get_period_time(m_Params, &m_Quality, &m_Directory);
    m_FrameSize   = m_Frame * 4;
    m_BufferSize  = m_FrameSize * m_FrameCount;
    m_FrameBuffer = new char [m_FrameSize];
    m_TotalBuffer = new char [m_BufferSize];
    return 0;
}

int Record::closeDevice()
{
    delete [] m_FrameBuffer;
    delete [] m_TotalBuffer;
    snd_pcm_drain(m_Handle);
    snd_pcm_close(m_Handle);
    m_Handle = nullptr;
    return 0;
}

const char * Record::getRecord()
{
    for(size_t i=0; i < m_FrameCount; ++i)
    {
        snd_pcm_readi(m_Handle, m_FrameBuffer, m_Frame);
        std::memcpy(m_TotalBuffer+(i*(m_FrameSize)), m_FrameBuffer, m_FrameSize);
    }

    return m_TotalBuffer;
}

size_t Record::getBufferSize()
{
    return m_BufferSize;
}

bool Record::isActivate()
{
    if(m_Handle != nullptr)
    {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////
Player::Player(unsigned int quality, snd_pcm_uframes_t frame, size_t frameCount)
    :m_Handle(nullptr),
     m_Params(nullptr),
     m_Frame(frame),
     m_Quality(quality),
     m_FrameCount(frameCount)
{

}

Player::~Player()
{

}

int Player::openDevice(const char * name)
{
    snd_pcm_open(&m_Handle, name, SND_PCM_STREAM_PLAYBACK, 0);
    snd_pcm_hw_params_alloca(&m_Params);
    snd_pcm_hw_params_any(m_Handle, m_Params);
    snd_pcm_hw_params_set_access(m_Handle, m_Params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(m_Handle, m_Params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(m_Handle, m_Params, 2);
    snd_pcm_hw_params_set_rate_near(m_Handle, m_Params, &m_Quality, &m_Directory);
    snd_pcm_hw_params_set_period_size_near(m_Handle, m_Params, &m_Frame, &m_Directory);
    snd_pcm_hw_params(m_Handle, m_Params);
    snd_pcm_hw_params_get_period_size(m_Params,&m_Frame, &m_Directory);
    snd_pcm_hw_params_get_period_time(m_Params, &m_Quality, &m_Directory);
    m_FrameSize   = m_Frame * 4;
    m_BufferSize  = m_FrameSize * m_FrameCount;
    return 0;
}

int Player::closeDevice()
{
    snd_pcm_drain(m_Handle);
    snd_pcm_close(m_Handle);
    return 0;
}

int Player::playRecord(const char * buffer)
{

    for(size_t i=0; i< m_FrameCount; ++i)
    {
        snd_pcm_writei(m_Handle, buffer+(i*m_FrameSize), m_Frame);
    }
    return 0;
}

size_t Player::getBufferSize()
{
    return m_BufferSize;
}


