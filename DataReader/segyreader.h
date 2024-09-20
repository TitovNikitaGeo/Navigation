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

class SegYReader
{
    // Q_OBJECT
public:
    struct Pair{
        int ffid;
        QTime time;
        Pair(int ffid, QTime time) :ffid(ffid), time(time){};
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
