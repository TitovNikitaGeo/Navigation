#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <QObject>
#include <QFileDialog>

#include "DataReader/segyreader.h"
#include "coordinator.h"
#include "p190_creator.h"
#include "logger.h"
#include "itemsstorage.h"
#include "nmeaparser.h"

class PostProcessor
{
public:
    PostProcessor();

    int runPP();

    void getDataFromSegy();
    void setMyVault(ItemsStorage *newMyVault);

    QDir nmeaStorage;
    QDir segyStorage;
    void setNmeaStorage(const QDir &newNmeaStorage);
    void setSegyStorage(const QDir &newSegyStorage);

    QVector<FixedItem*> vectorWithCon;
    QVector<FixedItem*> vectorNoCon;
    QVector<QFile*> NmeaFiles;


    void fillItemsVectors();
    int findNmeaFiles();


private:
    QStringList findNmaeForSegy(SegYReader::Pair pair, QFile* nmeaFile, int* pos);
    NmeaParser::NmeaGGAData calcTruePosition(NmeaParser::NmeaGGAData first,
        NmeaParser::NmeaGGAData second, QTime trueTime, QTime firstTime, QTime secondTime);

    ItemsStorage* MyVault;
    QVector<SegYReader::Pair> pairs;
    NmeaParser nmeaParser;
};

#endif // POSTPROCESSOR_H
