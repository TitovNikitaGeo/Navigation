#ifndef SEGYREADER_H
#define SEGYREADER_H

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QString>
#include <iostream>
#include <QDebug>
#include <QDir>
#include <QTime>
#include "functions.h"

class SegYReader
{
    // Q_OBJECT
public:
    struct Pair{
        int ffid;
        QTime time;
        int julianDay;
        Pair(int ffid, QTime time, int julianDay) :ffid(ffid), time(time), julianDay(julianDay){};
    };

    QVector<int> ffids;
    QVector<QTime> times;
    QVector<Pair> pairs;

    SegYReader();
    SegYReader(const QString& filePath);
    bool readFile();
    void readPathWithSegy(QDir path);

    QString filePath;

    void readTraceHeader(QFile& file);
    void setFilePath(const QString &newFilePath);


};

#endif // SEGYREADER_H
