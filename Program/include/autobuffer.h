#ifndef AUTOBUFFER_H
#define AUTOBUFFER_H

#include <cstring>


class AutoBuffer
{
public:
    AutoBuffer(size_t bufferSize);
    ~AutoBuffer();

    void copyBuffer(char * buffer, size_t bufferSize);
    void clearBuffer();
    char * getPointer();
    size_t getSize();
    char& operator[] (size_t index);

private:
    char * m_Buffer;
    size_t m_BufferSize;
};



#endif // AUTOBUFFER_H
