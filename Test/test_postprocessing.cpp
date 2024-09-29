#include "test_postprocessing.h"

testPostprocessing::testPostprocessing() {
    testSearchingNmea();
}

int testPostprocessing::testSearchingNmea()
{
    QVector<FixedItem*> items = createItems();
    QString sdir = "C:/Users/sabrahar/Desktop/FINAL/20240923_UHRS_48/RAW";
    QDir segydir(sdir);
    QString ndir = "C:/Users/sabrahar/Desktop/FINAL";
    QDir nmeadir(ndir);

    PostProcessor pr;
    // qDebug() << segydir.entryList();
    // qDebug() << nmeadir.entryList();

    pr.setSegyStorage(segydir);
    pr.getDataFromSegy();
    QVector<SegYReader::Pair> pair(pr.pairs);

    int posbouy = 0;
    int posSTP = 0;

    QFile* endBuoyFile = new QFile("C:/Users/sabrahar/Desktop/FINAL/buoy_1.nmea");
    if (!endBuoyFile->isOpen()) endBuoyFile->open(QIODevice::ReadOnly | QIODevice::Text);

    QFile* STPFile = new QFile("C:/Users/sabrahar/Desktop/FINAL/streamerTP.nmea");
    if (!STPFile->isOpen()) STPFile->open(QIODevice::ReadOnly | QIODevice::Text);

    // qDebug() << QTime::currentTime();
    for (SegYReader::Pair i: pr.pairs) {
        // qDebug() << i.time<< pr.findNmeaForSegy(i, endBuoyFile, &pos);
        // qDebug() << i.time << i.ffid;
        // endBuoyFile->close();
        QStringList res_buoy = pr.findNmeaForSegy(i, endBuoyFile, &posbouy);
        QStringList res_STP = pr.findNmeaForSegy(i, STPFile, &posSTP);
        qDebug() <<res_buoy << "res_buoy";
        qDebug() <<res_STP << "res_STP";
        // break;
        // NmeaParser::NmeaGGAData GGAData = pr
        // qDebug() << QTime::currentTime();
    }
    delete endBuoyFile;
    return 1;
}

QVector<FixedItem*> testPostprocessing::createItems()
{
    QVector<float> chans;
    for (int i = 0; i < 24; i++) {
        chans.append(i+1);
    }
    for (int i = 0; i < 24; i++) {
        chans.append(26 + i*2);
    }
    QVector<FixedItem*> res;
    Fabric fb;
    FixedItem* shipBuoy = fb.CreateItem(FixedItemInfo(0,0,0,"ship buoy"), false);
    shipBuoy->hasConnection = true;
    Connection_com* conLead = new Connection_com();
    conLead->filename = "ship buoy.nmea";
    shipBuoy->connection = conLead;


    FixedItem* StreamerTP = fb.CreateItem(FixedItemInfo(-5,2,0,"STP"), false);
    FixedItem* BoomerTP = fb.CreateItem(FixedItemInfo(-5,-2,0,"BTP"), false);
    Source* boomer = fb.CreateItem(SourceInfo(TowedItemInfo(BoomerTP, 75, 0, "boomer")), false);
    Buoy* leadBuoy = fb.CreateItem(BuoyInfo(TowedItemInfo(StreamerTP, 55, 0, "leadbuoy"), 1, 0.5), false);
    Streamer* streamer = fb.CreateItem(StreamerInfo(TowedItemInfo(leadBuoy, 40, 0, "boomer"), 48, chans));

    Buoy* endBuoy = fb.CreateItem(BuoyInfo(TowedItemInfo(StreamerTP, 55, 0, "leadbuoy"), 1, 0.5), false);
    endBuoy->hasConnection = true;
    Connection_com* conEnd = new Connection_com();
    conEnd->filename = "ship buoy.nmea";
    endBuoy->connection = conEnd;

    res.push_back(shipBuoy);
    res.push_back(endBuoy );
    res.push_back(StreamerTP);
    res.push_back(BoomerTP);
    res.push_back(boomer);
    res.push_back(leadBuoy);
    res.push_back(streamer);

    // for (auto i: res) {
    //     qDebug() << i->metaObject()->className();
    // }

    return res;

}
