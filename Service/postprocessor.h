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


    void setP190(P190_creator *newP190);
    QVector<SegYReader::Pair> pairs;

    QStringList findNmeaForSegy(SegYReader::Pair pair, QFile* nmeaFile, int* pos);
    NmeaParser::NmeaGGAData calcTruePosition(NmeaParser::NmeaGGAData first,
        NmeaParser::NmeaGGAData second, QTime trueTime, QTime firstTime, QTime secondTime);

    ItemsStorage* MyVault;
    QVector<FixedItem*> items;
    NmeaParser nmeaParser;

    P190_creator* p190;
private:
};

#endif // POSTPROCESSOR_H
