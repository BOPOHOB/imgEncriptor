#include "wavheader.h"

#include <QString>
#include <QDebug>

QDebug operator<<(QDebug out, WAVHeader data) {
    if (!data.isValid()) {
        return out << "invalid wav header" << (data.chunkSize - data.subchunk2Size);
    }
    return out << QString("WAVHeader {\n\
    RIFF {\n\
        chunkSize : %1\n\
    }\n\
    WAVE {\n\
        fmt {\n\
            subchunk1Size : %2\n\
            audioFormat : %3\n\
            numChannels : %4\n\
            sampleRate : %5\n\
            byteRate : %6\n\
            blockAlign : %7\n\
            bitsPerSample : %8\n\
        }\n\
        data {\n\
            subchunk2Size : %9\n\
        }\n\
    }\n}").arg(QString::number(data.chunkSize),
               QString::number(data.subchunk1Size),
               QString::number(data.audioFormat),
               QString::number(data.numChannels),
               QString::number(data.sampleRate),
               QString::number(data.byteRate),
               QString::number(data.blockAlign),
               QString::number(data.bitsPerSample),
               QString::number(data.subchunk2Size)).toLocal8Bit().data();
}
