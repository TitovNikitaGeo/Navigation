#include "test_postprocessing.h"

testPostprocessing::testPostprocessing() {
    testSearchingNmea22();
    // testSearchingNmea23WithOnboard();
}

int testPostprocessing::testSearchingNmea23()
{
    Logger::instance();
    QVector<FixedItem*> items = createItems2309();
    PostProcessor pr;
    ItemsStorage* Vault = new ItemsStorage;

    for (auto i: items) {
        // qDebug() << i->name;
        pr.items.push_back(i);
        Vault->ItemsVault.push_back(i);
    }
    pr.p190->setMyVault(Vault);


    // for (auto i: pr.items) {
    //     qDebug() << i->name;
    //     // pr.items.push_back(i);
    // }

    // exit(0);
    QString sdir = "C:/Users/sabrahar/Desktop/FINAL/20240923_UHRS_48/RAW";
    QDir segydir(sdir);
    QString ndir = "C:/Users/sabrahar/Desktop/FINAL";
    QDir nmeadir(ndir);

    // qDebug() << segydir.entryList();
    // qDebug() << nmeadir.entryList();

    pr.setSegyStorage(segydir);
    pr.getDataFromSegy();
    pr.p190->createP190File();
    QVector<SegYReader::Pair> pair(pr.pairs);
    // pr.pairs.clear();
    // pr.pairs = readFileAndGeneratePairs("C:/Users/sabrahar/Desktop/FINAL/240922_geom.txt");

    NmeaParser parser;

    int posbouy = 0;
    int posSTP = 0;

    QFile* endBuoyFile = new QFile("C:/Users/sabrahar/Desktop/FINAL/23092024endBuoy.nmea");
    if (!endBuoyFile->isOpen()) endBuoyFile->open(QIODevice::ReadOnly | QIODevice::Text);

    QFile* STPFile = new QFile("C:/Users/sabrahar/Desktop/FINAL/streamerTP.nmea");
    if (!STPFile->isOpen()) STPFile->open(QIODevice::ReadOnly | QIODevice::Text);


    for (SegYReader::Pair i: pr.pairs) {
        NmeaParser::NmeaRMCData RMCSTP;

        QStringList res_buoy = pr.findNmeaForSegy(i, endBuoyFile, &posbouy);
        QStringList res_STP = pr.findNmeaForSegy(i, STPFile, &posSTP);
        NmeaParser::NmeaGGAData GGAbuoy_first = parser.parseNmeaGGA(res_buoy[0]);
        NmeaParser::NmeaGGAData GGASTP_first = parser.parseNmeaGGA(res_STP[0]);
        NmeaParser::NmeaGGAData GGAbuoy_second = parser.parseNmeaGGA(res_buoy[1]);
        NmeaParser::NmeaGGAData GGASTP_second= parser.parseNmeaGGA(res_STP[1]);
        if (res_STP.size() == 3) {
            RMCSTP= parser.parseNmeaRMC(res_STP[2]);
        }


        qDebug() << GGAbuoy_first.dateTime.time() << GGASTP_first.dateTime.time();

        NmeaParser::NmeaGGAData GGAbuoyTrue = pr.calcTruePosition(GGAbuoy_first,GGAbuoy_second,i.time, GGAbuoy_first.dateTime.time(),GGAbuoy_second.dateTime.time());
        NmeaParser::NmeaGGAData GGASTPTrue = pr.calcTruePosition(GGASTP_first,GGASTP_second,i.time, GGASTP_first.dateTime.time(),GGASTP_second.dateTime.time());




        qDebug() << GGAbuoy_first.coordinate;
        qDebug() << GGAbuoyTrue.coordinate;

        items[0]->lastGGAData = GGAbuoyTrue;
        items[0]->lastRMCData = RMCSTP; //fake one
        items[1]->lastGGAData = GGASTPTrue;
        items[1]->lastRMCData = RMCSTP;


        for (auto i: items) {
            i->calcItemCoordinates();
            // i->printPos();
            // break;
            // std::this_thread::sleep_for(3);
            // QThread::sleep(0.5);
        }
        // break;

        pr.p190->setFFID(i.ffid);
        pr.p190->createShotBlock();
        // break;
        // NmeaParser::NmeaGGAData GGAData = pr
    }
    delete endBuoyFile;
    return 1;
}

int testPostprocessing::testSearchingNmea23WithOnboard()
{
    Logger::instance();
    QVector<FixedItem*> items = createItems2209();
    PostProcessor pr;
    ItemsStorage* Vault = new ItemsStorage;

    for (auto i: items) {
        pr.items.push_back(i);
        Vault->ItemsVault.push_back(i);
    }
    pr.p190->setMyVault(Vault);

    QString sdir = "C:/Users/sabrahar/Desktop/FINAL/20240923_UHRS_48/RAW";
    QDir segydir(sdir);
    QString ndir = "C:/Users/sabrahar/Desktop/FINAL";
    QDir nmeadir(ndir);

    pr.p190->createP190File();
    pr.setSegyStorage(segydir);
    pr.getDataFromSegy();
    pr.p190->createP190File();
    QVector<SegYReader::Pair> pair(pr.pairs);
    // pr.pairs = readFileAndGeneratePairs("C:/Users/sabrahar/Desktop/FINAL/240922_geom.txt");

    NmeaParser parser;

    int posbouy = 0;
    int posSTP = 0;

    QFile* endBuoyFile = new QFile("C:/Users/sabrahar/Desktop/FINAL/PPK GNSS data/23092024endBuoy.nmea");
    if (!endBuoyFile->isOpen()) endBuoyFile->open(QIODevice::ReadOnly | QIODevice::Text);

    QFile* vesBuoyFile = new QFile("C:/Users/sabrahar/Desktop/FINAL/PPK GNSS data/23092024OnboardBuoy.nmea");
    if (!vesBuoyFile->isOpen()) vesBuoyFile->open(QIODevice::ReadOnly | QIODevice::Text);


    for (SegYReader::Pair pair: pr.pairs) {
        NmeaParser::NmeaRMCData RMCSTP;

        QStringList res_endbuoy = pr.findNmeaForSegy(pair, endBuoyFile, &posbouy);
        QStringList res_vesbuoy = pr.findNmeaForSegy(pair, vesBuoyFile, &posSTP);
        NmeaParser::NmeaGGAData GGAendbuoy_first = parser.parseNmeaGGA(res_endbuoy[0]);
        NmeaParser::NmeaGGAData GGAvesBuoy_first = parser.parseNmeaGGA(res_vesbuoy[0]);
        NmeaParser::NmeaGGAData GGAendbuoy_second = parser.parseNmeaGGA(res_endbuoy[1]);
        NmeaParser::NmeaGGAData GGAvesBuoy_second= parser.parseNmeaGGA(res_vesbuoy[1]);
        if (res_vesbuoy.size() == 3) {
            RMCSTP= parser.parseNmeaRMC(res_vesbuoy[2]);
        }


        // qDebug() << GGAbuoy_first.dateTime.time() << GGASTP_first.dateTime.time();
        NmeaParser::NmeaGGAData GGAendbuoyTrue = pr.calcTruePosition(GGAendbuoy_first,GGAendbuoy_second,pair.time, GGAendbuoy_first.dateTime.time(),GGAendbuoy_second.dateTime.time());
        NmeaParser::NmeaGGAData GGAvesBuoyTrue = pr.calcTruePosition(GGAvesBuoy_first,GGAvesBuoy_second,pair.time, GGAvesBuoy_first.dateTime.time(),GGAvesBuoy_second.dateTime.time());




        // qDebug() << GGAbuoy_first.coordinate;
        // qDebug() << GGAvesBuoyTrue.coorUTM << "VESSELBUOY";
        // qDebug() << GGAendbuoyTrue.coorUTM << "ENDBUOY";

        //расстояние между буями
        // qDebug() << GGAvesBuoyTrue.coordinate.distanceTo(GGAbuoyTrue.coordinate);
        // continue;


        items[0]->lastGGAData = GGAendbuoyTrue; //endBuoy
        items[1]->lastGGAData = GGAvesBuoyTrue; //shipBuoy

        qDebug() << "FAKE AZIMUTH"<<GGAendbuoyTrue.coordinate.azimuthTo(GGAvesBuoyTrue.coordinate);


        for (auto i: items) {
            i->calcItemCoordinates();
            if (QString(i->metaObject()->className()) == "Streamer") {
                Streamer* strm = dynamic_cast<Streamer*>(i);
                if (pair.ffid / 100 == 0 || true) {
                    qDebug() << "REAL AZIMUTH OF TOWING" << qRadiansToDegrees(strm->realAzimuthOfTowingRadians);
                    qDebug() <<"FFID" << pair.ffid;
                }
                // qDebug() << ;
            }
            // exit(666);
            // std::this_thread::sleep_for(3);
            // QThread::sleep(0.5);
        }
        // break;

        pr.p190->setFFID(pair.ffid);
        pr.p190->createShotBlock();
        // break;
        // NmeaParser::NmeaGGAData GGAData = pr
    }
    delete endBuoyFile;
    return 1;
}

int testPostprocessing::testSearchingNmea22()
{
    Logger::instance();
    QVector<FixedItem*> items = createItems2209();
    PostProcessor pr;
    ItemsStorage* Vault = new ItemsStorage;

    for (auto i: items) {
        pr.items.push_back(i);
        Vault->ItemsVault.push_back(i);
    }
    pr.p190->setMyVault(Vault);

    QString ndir = "C:/Users/sabrahar/Desktop/FINAL";
    QDir nmeadir(ndir);

    pr.p190->createP190File();
    pr.pairs = readFileAndGeneratePairs("C:/Users/sabrahar/Desktop/FINAL/240922_geom.txt");

    NmeaParser parser;

    int posbouy = 0;
    int posSTP = 0;

    QFile* endBuoyFile = new QFile("C:/Users/sabrahar/Desktop/FINAL/PPK GNSS data/22092024endBuoy.nmea");
    if (!endBuoyFile->isOpen()) endBuoyFile->open(QIODevice::ReadOnly | QIODevice::Text);

    QFile* vesBuoyFile = new QFile("C:/Users/sabrahar/Desktop/FINAL/PPK GNSS data/22092024OnboardBuoy.nmea");
    if (!vesBuoyFile->isOpen()) vesBuoyFile->open(QIODevice::ReadOnly | QIODevice::Text);


    for (SegYReader::Pair pair: pr.pairs) {
        NmeaParser::NmeaRMCData RMCSTP;

        QStringList res_endbuoy = pr.findNmeaForSegy(pair, endBuoyFile, &posbouy);
        QStringList res_vesbuoy = pr.findNmeaForSegy(pair, vesBuoyFile, &posSTP);
        NmeaParser::NmeaGGAData GGAendbuoy_first = parser.parseNmeaGGA(res_endbuoy[0]);
        NmeaParser::NmeaGGAData GGAvesBuoy_first = parser.parseNmeaGGA(res_vesbuoy[0]);
        NmeaParser::NmeaGGAData GGAendbuoy_second = parser.parseNmeaGGA(res_endbuoy[1]);
        NmeaParser::NmeaGGAData GGAvesBuoy_second= parser.parseNmeaGGA(res_vesbuoy[1]);
        if (res_vesbuoy.size() == 3) {
            RMCSTP= parser.parseNmeaRMC(res_vesbuoy[2]);
        }


        // qDebug() << GGAbuoy_first.dateTime.time() << GGASTP_first.dateTime.time();
        NmeaParser::NmeaGGAData GGAendbuoyTrue = pr.calcTruePosition(GGAendbuoy_first,GGAendbuoy_second,pair.time, GGAendbuoy_first.dateTime.time(),GGAendbuoy_second.dateTime.time());
        NmeaParser::NmeaGGAData GGAvesBuoyTrue = pr.calcTruePosition(GGAvesBuoy_first,GGAvesBuoy_second,pair.time, GGAvesBuoy_first.dateTime.time(),GGAvesBuoy_second.dateTime.time());




        // qDebug() << GGAbuoy_first.coordinate;
        // qDebug() << GGAvesBuoyTrue.coorUTM << "VESSELBUOY";
        // qDebug() << GGAendbuoyTrue.coorUTM << "ENDBUOY";

        //расстояние между буями
        // qDebug() << GGAvesBuoyTrue.coordinate.distanceTo(GGAbuoyTrue.coordinate);
        // continue;


        items[0]->lastGGAData = GGAendbuoyTrue; //endBuoy
        items[1]->lastGGAData = GGAvesBuoyTrue; //shipBuoy

        qDebug() << "FAKE AZIMUTH"<<GGAendbuoyTrue.coordinate.azimuthTo(GGAvesBuoyTrue.coordinate);


        for (auto i: items) {
            i->calcItemCoordinates();
            if (QString(i->metaObject()->className()) == "Streamer") {
                Streamer* strm = dynamic_cast<Streamer*>(i);
                if (pair.ffid / 100 == 0 || true) {
                    qDebug() << "REAL AZIMUTH OF TOWING" << qRadiansToDegrees(strm->realAzimuthOfTowingRadians);
                    qDebug() <<"FFID" << pair.ffid;
                }
                // qDebug() << ;
            }
            // exit(666);
            // std::this_thread::sleep_for(3);
            // QThread::sleep(0.5);
        }
        // break;

        pr.p190->setFFID(pair.ffid);
        pr.p190->createShotBlock();
        // break;
        // NmeaParser::NmeaGGAData GGAData = pr
    }
    delete endBuoyFile;
    return 1;
}

QVector<FixedItem*> testPostprocessing::createItems2309()
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

    FixedItem* StreamerTP = fb.CreateItem(FixedItemInfo(0.01,0,0,"STP"), false);
    StreamerTP->hasConnection = true;
    Connection_com* conSTP = new Connection_com();
    conSTP->filename = "streamerTP.nmea";
    StreamerTP->connection = conSTP;

    FixedItem* BoomerTP = fb.CreateItem(FixedItemInfo(0.45,-6.9,0,"BTP"), false);
    BoomerTP->setItemForCalculations(StreamerTP);

    Source* boomer = fb.CreateItem(SourceInfo(TowedItemInfo(BoomerTP, 75, 0, "boomer")), false);
    // Buoy* leadBuoy = fb.CreateItem(BuoyInfo(TowedItemInfo(StreamerTP, 55, 0, "leadbuoy"), 1, 0.5), false);
    Streamer* streamer = fb.CreateItem(StreamerInfo(TowedItemInfo(StreamerTP, 85.5, 0, "streamer"), 48, chans));

    Buoy* endBuoy = fb.CreateItem(BuoyInfo(TowedItemInfo(StreamerTP, 40.5, 0, "endbuoy"), 1, 0.5), false);
    endBuoy->hasConnection = true;

    streamer->setEndBuoy(endBuoy);
    Connection_com* conEnd = new Connection_com();
    conEnd->filename = "buoy_1.nmea";
    endBuoy->connection = conEnd;

    res.push_back(StreamerTP);
    res.push_back(endBuoy );
    res.push_back(streamer);
    res.push_back(BoomerTP);
    res.push_back(boomer);
    return res;

}

QVector<FixedItem *> testPostprocessing::createItems2309WithOnboard()
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
    FixedItem* shipBuoy = fb.CreateItem(FixedItemInfo(6.6,-5.4,0,"ship buoy"), false);
    shipBuoy->hasConnection = true;
    Connection_com* shipBuoyConnection = new Connection_com();
    shipBuoyConnection->filename = "ship buoy.nmea";
    shipBuoy->connection = shipBuoyConnection;


    FixedItem* StreamerTP = fb.CreateItem(FixedItemInfo(0.01,0,0,"STP"), false);
    StreamerTP->setItemForCalculations(shipBuoy);


    FixedItem* BoomerTP = fb.CreateItem(FixedItemInfo(0.45,-6.9,0,"BTP"), false);
    BoomerTP->setItemForCalculations(shipBuoy);

    Source* boomer = fb.CreateItem(SourceInfo(TowedItemInfo(BoomerTP, 75, 0, "boomer")), false);
    Streamer* streamer = fb.CreateItem(StreamerInfo(TowedItemInfo(StreamerTP, 85.5, 0, "streamer"), 48, chans));

    Buoy* endBuoy = fb.CreateItem(BuoyInfo(TowedItemInfo(StreamerTP, 40.4, 0, "endbuoy"), 1, 0.5), false);
    endBuoy->hasConnection = true;

    streamer->setEndBuoy(endBuoy);
    Connection_com* conEnd = new Connection_com();
    conEnd->filename = "buoy_1.nmea";
    endBuoy->connection = conEnd;



    res.push_back(endBuoy );
    res.push_back(shipBuoy);
    res.push_back(StreamerTP);
    res.push_back(streamer);
    res.push_back(BoomerTP);
    res.push_back(boomer);
    // res.push_back(leadBuoy);

    // for (auto i: res) {
    //     qDebug() << i->metaObject()->className();
    // }

    return res;
}


QVector<FixedItem*> testPostprocessing::createItems2209()
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
    FixedItem* shipBuoy = fb.CreateItem(FixedItemInfo(6.6,-5.4,0,"ship buoy"), false);
    shipBuoy->hasConnection = true;
    Connection_com* shipBuoyConnection = new Connection_com();
    shipBuoyConnection->filename = "ship buoy.nmea";
    shipBuoy->connection = shipBuoyConnection;


    FixedItem* StreamerTP = fb.CreateItem(FixedItemInfo(0.01,0,0,"STP"), false);
    StreamerTP->setItemForCalculations(shipBuoy);


    FixedItem* BoomerTP = fb.CreateItem(FixedItemInfo(0.45,-6.9,0,"BTP"), false);
    BoomerTP->setItemForCalculations(shipBuoy);

    Source* boomer = fb.CreateItem(SourceInfo(TowedItemInfo(BoomerTP, 75, 0, "boomer")), false);
    Streamer* streamer = fb.CreateItem(StreamerInfo(TowedItemInfo(StreamerTP, 85.5, 0, "streamer"), 48, chans));

    Buoy* endBuoy = fb.CreateItem(BuoyInfo(TowedItemInfo(StreamerTP, 30, 0, "endbuoy"), 1, 0.5), false);
    endBuoy->hasConnection = true;

    streamer->setEndBuoy(endBuoy);
    Connection_com* conEnd = new Connection_com();
    conEnd->filename = "buoy_1.nmea";
    endBuoy->connection = conEnd;



    res.push_back(endBuoy );
    res.push_back(shipBuoy);
    res.push_back(StreamerTP);
    res.push_back(streamer);
    res.push_back(BoomerTP);
    res.push_back(boomer);
    // res.push_back(leadBuoy);

    // for (auto i: res) {
    //     qDebug() << i->metaObject()->className();
    // }

    return res;

}


QVector<SegYReader::Pair> testPostprocessing::readFileAndGeneratePairs(const QString& fileName) {
    QVector<SegYReader::Pair> pairs;
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Cannot open file!" << std::endl;
        return pairs;
    }

    QTextStream in(&file);
    // Пропускаем первую строку с заголовками
    QString header = in.readLine();

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split('\t');  // Разделитель - табуляция

        if (fields.size() == 6) {
            int ffid = fields[0].toInt();
            int hour = fields[2].toInt();
            int minute = fields[3].toInt();
            int second = fields[4].toInt();
            int microsecond = fields[5].split('.').first().toInt();  // Отбрасываем дробную часть

            QTime time = QTime(hour, minute, second, microsecond / 1000);  // Микросекунды преобразуем в миллисекунды
            pairs.append(SegYReader::Pair(ffid, time));
        }
    }

    file.close();
    return pairs;
}
