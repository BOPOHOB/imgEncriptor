#ifndef ENCRIPTOR_H
#define ENCRIPTOR_H

#include <QImage>
#include <QString>
#include <QByteArray>
#include <QBitArray>

class Encriptor
{
    static quint32 setBit(const quint8& byte, const bool value, const int position);

    static QBitArray toBitArray(const QByteArray&);
    static QByteArray toByteArray(const QBitArray&);

    Encriptor();
public:

    static QImage encript(const QImage&, const QString&);
    static QString decript(const QImage&);
};

#endif // ENCRIPTOR_H
