#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <zlib.h>

class Compressor
{
public:
        Compressor();
        ~Compressor();

        uLong compress(const Bytef* inputarr, uInt inputsz, Bytef* outputarr, uInt outputsz);
        uLong decompress(const Bytef* inputarr, uInt inputsz, Bytef* outputarr, uInt outputsz);
};

#endif // COMPRESSOR_H
