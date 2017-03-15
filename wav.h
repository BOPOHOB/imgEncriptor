#ifndef WAV_H
#define WAV_H
#include "wavheader.h"

#include <QString>
#include <vector>

class WAV
{
    WAVHeader meta;
    typedef std::vector<quint32> Array;
    Array data;
    QString filename;
    static const int ACCURACY;
public:
    WAV(const QString& fname);

    const WAVHeader& getMeta() const { return meta; }

    bool isValid() const { return meta.isValid(); }

    void save(const QString& fname) const;

    void encript(const QString& script);
};

#endif // WAV_H
