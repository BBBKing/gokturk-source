#include "autobuffer.h"

AutoBuffer::AutoBuffer(size_t bufferSize)
    : m_BufferSize(bufferSize)
{
        m_Buffer = new char[bufferSize];
}

AutoBuffer::~AutoBuffer()
{
    delete [] m_Buffer;
}

void AutoBuffer::copyBuffer(char * buffer, size_t bufferSize)
{
    std::memcpy(m_Buffer, buffer, bufferSize);
}

void AutoBuffer::clearBuffer()
{
    std::memset(m_Buffer, 0, m_BufferSize);
}

char * AutoBuffer::getPointer()
{
    return m_Buffer;
}

size_t AutoBuffer::getSize()
{
    return m_BufferSize;
}

char& AutoBuffer::operator[] (size_t index)
{
    return m_Buffer[index];
}
