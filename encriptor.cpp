#include "encriptor.h"
#include <QDebug>
#include <QColor>

Encriptor::Encriptor()
{

}

quint32 Encriptor::setBit(const quint8 &byte, const bool value, const int position)
{
    return (quint8(value) << position) | (byte & ~(quint8(1) << position));
}

QBitArray Encriptor::toBitArray(const QByteArray& bytes)
{
    QBitArray bits(bytes.count()*8);

    // Convert from QByteArray to QBitArray
    for(int i=0; i<bytes.count(); ++i) {
        for(int b=0; b<8;b++) {
            bits.setBit( i*8+b, bytes.at(i)&(1<<(7-b)) );
        }
    }
    return bits;
}

QByteArray Encriptor::toByteArray(const QBitArray& bits)
{
    // Resulting byte array
    QByteArray bytes;
    bytes.resize(bits.size() >> 3);
    bytes.fill(0);
    if (bits.size() % 8) {
        bytes.push_back(char(0));
    }

    // Convert from QBitArray to QByteArray
    for(int b=0; b<bits.count();++b) {
        bytes[b/8] = (bytes.at(b/8) | ((bits[b]?1:0)<<(7-(b%8))));
    }
    return bytes;
}

QImage Encriptor::encript(const QImage& img, const QString& script)
{
    QImage result(img);
    const QByteArray data(script.toLocal8Bit());
    quint32 length(data.length());
    QByteArray lengthArray;
    lengthArray.push_back(static_cast<quint8>(length       % 0x100));
    lengthArray.push_back(static_cast<quint8>(length >>  8 % 0x100));
    lengthArray.push_back(static_cast<quint8>(length >> 16 % 0x100));
    lengthArray.append(data);
    QBitArray mask(toBitArray(lengthArray));
    int curPixel(0);
    if (mask.size() % 3) {
        mask.resize(mask.size() / 3 * 3 + 3);
    }
    for (int i(0); mask.size() > i; i += 3, ++curPixel) {
        const QPoint pixPoint(curPixel % result.width(), curPixel / result.width());
        QRgb cur(result.pixel(pixPoint));
        cur = qRgb(setBit(  qRed(cur), mask[i    ], 0),
                   setBit(qGreen(cur), mask[i + 1], 0),
                   setBit( qBlue(cur), mask[i + 2], 0));
        result.setPixel(pixPoint, cur);
    }
    return result;
}

QString Encriptor::decript(const QImage& img)
{
    QBitArray length(3 * 8);
    int curPixel = 0;
    for (int i(0); length.size() != i; ++curPixel, i += 3) {
        const QPoint pixPoint(curPixel % img.width(), curPixel / img.width());
        QRgb cur(img.pixel(pixPoint));
        length.setBit(i    ,   qRed(cur) & 1);
        length.setBit(i + 1, qGreen(cur) & 1);
        length.setBit(i + 2,  qBlue(cur) & 1);
    }
    QByteArray lengthBytes(toByteArray(length));
    int l = static_cast<unsigned char>(lengthBytes[0]) + static_cast<unsigned char>(lengthBytes[1]) * 0x100 + static_cast<unsigned char>(lengthBytes[2]) * 0x10000;
    QBitArray data;
    data.resize(l << 3);
    for (int i(0); data.size() > i; i += 3, ++curPixel) {
        const QPoint pixPoint(curPixel % img.width(), curPixel / img.width());
        QRgb cur(img.pixel(pixPoint));
        data.setBit(i    ,   qRed(cur) & 1);
        if (i + 1 < data.size()) {
            data.setBit(i + 1, qGreen(cur) & 1);
        }
        if (i + 2 < data.size()) {
            data.setBit(i + 2,  qBlue(cur) & 1);
        }
    }
    return QString::fromLocal8Bit(toByteArray(data));
}
