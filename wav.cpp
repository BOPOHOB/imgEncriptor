#include "wav.h"

#include <cassert>

#include <QDataStream>
#include <QFile>
#include <QDebug>

#include "encriptor.h"

const int WAV::ACCURACY = 4;

WAV::WAV(const QString& fname)
    : filename(fname)
{
    struct File : QFile {
        File(const QString& fname)
            : QFile(fname) {}
        qint64 readData(char *data, qint64 maxlen) final
            { return QFileDevice::readData(data, maxlen); }
    } file(fname);
    if (!file.open(QFile::ReadOnly)) {
        return;
    }
    if (file.readData(static_cast<char*>(static_cast<void*>(&meta)), sizeof(WAVHeader)) != sizeof(WAVHeader)) {
        return;
    }
    data.resize(meta.getNumSamples() * meta.getSampleSize());
    assert(meta.blockAlign / meta.numChannels <= ACCURACY);

    std::vector<quint8> rawData(meta.subchunk2Size);
    file.readData(static_cast<char*>(static_cast<void*>(rawData.data())), meta.subchunk2Size);

    const quint8* it(rawData.data());
    const quint8* const end(rawData.data() + rawData.size());
    quint8* jt(static_cast<quint8*>(static_cast<void*>(data.data())));
    quint8* const jtEnd(static_cast<quint8*>(static_cast<void*>(data.data() + data.size())));
    const int unvaluble(ACCURACY - meta.getSampleSize());
    while (it != end) {
        for (int i(unvaluble); i; --i) {
            *jt = 0;
            ++jt;
        }
        for (int i(ACCURACY); i; --i) {
            *jt = *it;
            ++it;
            ++jt;
        }
    }
    assert(jt == jtEnd);
}

void WAV::save(const QString& fname) const
{
    if (QFile::exists(fname)) {
        if (!QFile::remove(fname)) {
            qFatal("ran't delete file");
        }
    }
    QFile f(fname);
    f.open(QFile::WriteOnly);
    f.write(static_cast<const char*>(static_cast<const void*>(&meta)), sizeof(meta));
    std::vector<quint8> rawData(meta.subchunk2Size);

    const quint8* src(static_cast<const quint8*>(static_cast<const void*>(data.data())));
    quint8* dst(rawData.data());
    const quint8* const end(rawData.data() + rawData.size());
    const quint8* const srcEnd(static_cast<const quint8*>(static_cast<const void*>(data.data() + data.size())));

    const int unvaluble(ACCURACY - meta.getSampleSize());
    while (dst != end) {
        for (int i(unvaluble); i; --i) {
            ++src;
        }
        for (int i(ACCURACY); i; --i) {
            *dst = *src;
            ++src;
            ++dst;
        }
    }
    assert(srcEnd == src);
    f.write(static_cast<const char*>(static_cast<const void*>(rawData.data())), rawData.size());
}

void WAV::encript(const QString& script)
{
    data = Encriptor::encript(data, script);
}
