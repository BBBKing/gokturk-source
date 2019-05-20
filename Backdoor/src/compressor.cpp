#include "compressor.h"
uLong Compressor::compress(const Bytef* inputarr, uInt inputsz, Bytef* outputarr, uInt outputsz){
    z_stream compstream;
    compstream.zalloc = Z_NULL;
    compstream.zfree = Z_NULL;
    compstream.opaque = Z_NULL;

    compstream.avail_in = inputsz;
    compstream.next_in = (Bytef*)inputarr;

    compstream.avail_out = outputsz;
    compstream.next_out = outputarr;

    deflateInit(&compstream, Z_BEST_COMPRESSION);
    deflate(&compstream, Z_FINISH);
    deflateEnd(&compstream);

    return compstream.total_out;
}
uLong Compressor::decompress(const Bytef* inputarr, uInt inputsz, Bytef* outputarr, uInt outputsz){
    z_stream decompstream;
    decompstream.zalloc = Z_NULL;
    decompstream.zfree = Z_NULL;
    decompstream.opaque = Z_NULL;

    decompstream.avail_in = inputsz;
    decompstream.next_in = (Bytef*)inputarr;

    decompstream.avail_out = outputsz;
    decompstream.next_out = outputarr;

    inflateInit(&decompstream);
    inflate(&decompstream, Z_NO_FLUSH);
    inflateEnd(&decompstream);

    return decompstream.total_out;
}

Compressor::Compressor()
{
    //ctor
}

Compressor::~Compressor()
{
    //dtor
}
