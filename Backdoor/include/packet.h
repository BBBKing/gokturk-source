#ifndef PACKET_H
#define PACKET_H

#include <cstring>

class SumPacket
{
    public:
        SumPacket();
        ~SumPacket();
        void createBuffer(size_t bufferSize);
        void deleteBuffer();

        void add(char * buffer, size_t bufferSize);
        void output(char * buffer);
        bool copleted();
        size_t getSize();
        const char* getBuffer();

    private:
        char * m_Buffer;
        size_t m_Iterator;
        size_t m_BufferSize;

};


class DivPacket
{
    public:
        DivPacket(char * buffer, size_t bufferSize, size_t packetSize);
        ~DivPacket();
        char* getPtr(size_t iter);
        size_t getSize(size_t iter);
        size_t getIter();
    private:
        size_t m_Remainder;
        size_t m_TotalPacket;
        size_t m_PacketSize;
        size_t m_BufferSize;
        char * m_Buffer;
};


enum PackTypes{
    NULLTP,

    INFSCR,
    PCKSCR,
    OFFSCR,

    INFCAM,
    PCKCAM,
    OFFCAM,

    INFAUD,
    PCKAUD,
    OFFAUD,

    INFCMD,
    PCKCMD,
    OFFCMD,

    DWNFLE,
    UPLFLE,
    PCKFLE,
    OFFFLE
};

struct ScreenInfo{
    PackTypes packetType;
    int bufferSize;
    int compressSize;
    int width;
    int height;
};

struct ScreenPacket{
    PackTypes packetType;
    int packetSize;
    char packetBuffer[50000];
};

struct CameraInfo{
    PackTypes packetType;
    int bufferSize;
};

struct CameraPacket{
    PackTypes packetType;
    int packetSize;
    char packetBuffer[50000];
};

struct MicroInfo{
    PackTypes packetType;
};

struct MicroPacket{
    PackTypes packetType;
    char packetBuffer[49152];
};

struct CmdInfo{
    PackTypes packetType;
    char packetBuffer[50000];
};

struct CmdPacket{
    PackTypes packetType;
    char packetBuffer[200000];
};


struct FileInfo{
    PackTypes packetType;
    char filePath[4096];
    size_t bufferSize;
};

struct FilePacket{
    PackTypes packetType;
    int packetSize;
    char packetBuffer[50000];
};


#endif
