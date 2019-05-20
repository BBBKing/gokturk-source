#include "../include/packet.h"

SumPacket::SumPacket()
    :m_Buffer(nullptr), m_Iterator(0), m_BufferSize(0)
{

}

SumPacket::~SumPacket()
{

}

void SumPacket::createBuffer(size_t bufferSize)
{
    m_Buffer = new char[bufferSize];
    m_BufferSize = bufferSize;
}

void SumPacket::deleteBuffer()
{
    if(m_Buffer != nullptr){
        delete [] m_Buffer;
        m_BufferSize = 0;
        m_Iterator   = 0;
        m_Buffer     = nullptr;
    }
}

void SumPacket::add(char * buffer, size_t bufferSize)
{
    std::memcpy(m_Buffer + m_Iterator, buffer, bufferSize);
    m_Iterator += bufferSize;
}

void SumPacket::output(char * buffer)
{
    std::memcpy(buffer ,m_Buffer , m_BufferSize);
}


bool SumPacket::copleted()
{

    if(m_Iterator == m_BufferSize && m_BufferSize != 0){
        return true;
    }else{
        return false;
    }
}

size_t SumPacket::getSize()
{
    return m_BufferSize;
}

const char* SumPacket::getBuffer(){

 return m_Buffer;
}


DivPacket::DivPacket(char * buffer, size_t bufferSize, size_t packetSize)
    : m_Remainder(0),
      m_PacketSize(packetSize),
      m_BufferSize(bufferSize),
      m_Buffer(buffer)

{
    m_TotalPacket = m_BufferSize / m_PacketSize;
    m_Remainder   = m_BufferSize % m_PacketSize;
    if(m_Remainder !=0){
        m_TotalPacket++;
    }
}

DivPacket::~DivPacket()
{

}

char* DivPacket::getPtr(size_t iter)
{
        return (m_Buffer + (iter*m_PacketSize) );
}


size_t DivPacket::getSize(size_t iter)
{
    if(m_Remainder != 0)
    {
        if( iter < m_TotalPacket -1)
        {
            return m_PacketSize;
        }
        else
        {
            return m_Remainder;
        }

    }
    else
    {
        return m_PacketSize;
    }
}



size_t DivPacket::getIter()
{
    return m_TotalPacket;
}

