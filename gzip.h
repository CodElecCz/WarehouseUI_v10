#ifndef GZIP_H
#define GZIP_H

#include <QtZlib/zlib.h>
#include <QByteArray>

#define GZIP_WINDOWS_BIT 15 + 16
#define GZIP_CHUNK_SIZE 32 * 1024

class gzip
{
public:
    static bool compress(QByteArray input, QByteArray &output, int level = -1);
    static bool decompress(QByteArray input, QByteArray &output);
};

#endif // GZIP_H
