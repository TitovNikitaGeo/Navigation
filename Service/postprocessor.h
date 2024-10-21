#ifndef POSTPROCESSOR_H
#define POSTPROCESSOR_H

#include <QObject>
#include <QFileDialog>
#include <QFile>

#include "DataReader/segyreader.h"
#include "coordinator.h"
#include "p190_creator.h"
#include "logger.h"
#include "itemsstorage.h"
#include "nmeaparser.h"
#include "itemsloader.h"

class PostProcessor
{
public:
    PostProcessor();

    int mainProcess();



    void getDataFromSegy();
    void setMyVault(ItemsStorage *newMyVault);

    QDir nmeaStorage;
    QDir segyStorage;
    void setNmeaStorage(const QDir &newNmeaStorage);
    void setSegyStorage(const QDir &newSegyStorage);

    QVector<QFile*> NmeaFiles;

    void fillItemsVectors();
    int findNmeaFiles();


    void setP190(P190_creator *newP190);
    QVector<SegYReader::Pair> pairs;

    QVector<SegYReader::Pair> preparePairs(QVector<SegYReader::Pair> in);

    QStringList findNmeaForSegy(SegYReader::Pair pair, QFile* nmeaFile, int* pos);
    NmeaParser::CoordinateData calcTruePosition(NmeaParser::CoordinateData first,
        NmeaParser::CoordinateData second, QTime trueTime, QTime firstTime, QTime secondTime);

    QVector<SegYReader::Pair> readFileAndGeneratePairs(const QString& fileName);


    QVector<FixedItem*> vectorWithCon;
    QVector<FixedItem*> vectorNoCon;

    ItemsStorage* MyVault;
    NmeaParser nmeaParser;
    P190_creator* p190Creator;


//взаимодействие с интерфейсом PostProcessorView
    ItemsStorage Vault;
    QVector<FixedItem*> items;
    int itemsWithConnection = 0;

    QFile* jsonSchemeFile;
    QVector<QFile*> nmeaFiles;
    QVector<QFile*> ppkFiles;
    QString ffidTimeSourceTxtFile;
    QString ffidTimeSourceDir;

    float percentageP190Creation = 0;
    QVector<QPair<QString, QString>> pairItemFile; // имя объекта - имя файла


private:
};

#endif // POSTPROCESSOR_H
