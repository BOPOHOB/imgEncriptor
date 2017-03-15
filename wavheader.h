#ifndef WAVHEADER_H
#define WAVHEADER_H

#include <QtGlobal>

// Структура, описывающая заголовок WAV файла.
struct WAVHeader
{
    bool isValid() const { return numChannels > 0 && sampleRate > 0
                && chunkSize - subchunk2Size == 36
                && subchunk1Size == 16
                && byteRate == sampleRate * blockAlign
                && blockAlign == numChannels * bitsPerSample >> 3; }

    // WAV-формат начинается с RIFF-заголовка:

    // Содержит символы "RIFF" в ASCII кодировке
    quint8 chunkId[4];

    // 36 + subchunk2Size, или более точно:
    // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
    // Это оставшийся размер цепочки, начиная с этой позиции.
    // Иначе говоря, это размер файла - 8, то есть,
    // исключены поля chunkId и chunkSize.
    quint32 chunkSize;

    // Содержит символы "WAVE"
    // (0x57415645 в big-endian представлении)
    quint8 format[4];

    // Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
    // Подцепочка "fmt " описывает формат звуковых данных:

    // Содержит символы "fmt "
    quint8 subchunk1Id[4];

    // 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    quint32 subchunk1Size;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    quint16 audioFormat;

    // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    quint16 numChannels;

    // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    quint32 sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    quint32 byteRate;

    // numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    quint16 blockAlign;

    // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    quint16 bitsPerSample;

    // Подцепочка "data" содержит аудио-данные и их размер.

    // Содержит символы "data"
    quint8 subchunk2Id[4];

    // numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    quint32 subchunk2Size;

    quint32 getNumSamples() const {
        return subchunk2Size / numChannels / getSampleSize();
    }

    quint32 getSampleSize() const {
        return bitsPerSample >> 3;
    }

    // Далее следуют непосредственно Wav данные.
};

class QDebug;

QDebug operator<<(QDebug out, WAVHeader data);

#endif // WAVHEADER_H
