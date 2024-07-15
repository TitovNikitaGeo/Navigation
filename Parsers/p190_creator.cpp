#include "p190_creator.h"


P190_creator::P190_creator(QObject *parent)
    : QObject{parent}
{
}

P190_creator::~P190_creator()
{
    outputFile->close();
    outputStream->flush();
}

QStringList P190_creator::createShotBlock()
{
    QStringList res;
    static int shotNum = 0; //номер выстрела
    QStringList mainBlock = createMainInfoBlock();
    QStringList streamerBlock = createStreamerBlock();
    qDebug() << "P190";
    // if (!outputFile->isOpen()) outputFile->open(QIODevice::Append);
    for (QString l: mainBlock) {
        qDebug() << l;
    }
    for (QString l: streamerBlock) {
        qDebug() << l;
    }
    res.append(mainBlock);
    res.append(streamerBlock);
    writeToFile(res);
    return res;
}



///STUB. DO NOT OPEN
QStringList P190_creator::createHeader() {
    return QStringList {"H0100 AREA NAME                ALMATY, KAZAKHSTAN                              ",
                       "H0101 JOB DESCRIPTION           2D HIGH RESOLUTION SURVEY                       ",
                       "H0102 VESSEL DETAILS            GEOHIMIK                   1                    ",
                       "H0103 SOURCE DETAILS            COS                        1   1                ",
                       "H0104 STREAMER DETAILS          GPS                        1       1   1        ",
                       "H0105 OTHER DETAILS             SIMRAD EA600:SIMRADEA600   1               1    ",
                       "H0105 OTHER DETAILS             CMP                        1               2    ",
                       "H0200 DATE OF SURVEY            23.06.1997                                      ",
                       "H0201 DATE OF TAPE              23.06.1997                                      ",
                       "H0202 TAPE VERSION ID           CBR                                             ",
                       "H0203 LINE PREFIX               AA16A                                           ",
                       "H0300 CLIENT NAME               GEODEVICE                                       ",
                       "H0400 GEOPHYSICAL CONTRACTOR    GEODEVICE KAZAKHSTAN                            ",
                       "H0500 POSITION CONTRACTOR       C-NAV                                           ",
                       "H0600 POS. PROC. CONTRACTOR     GEODEVICE                                       ",
                       "H0700 POS./ COMPUTER SYSTEM     C-NAV                                           ",
                       "H0800 SHOTPOINT POSITION        EIVA NAVIPAC 3.10.5                             "};
}


void P190_creator::createP190File() {
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir dir(dirPath + "/Ship_logs");
    // qDebug() << dir;
    fileName = createFileName();

    if (!dir.exists()) {
        // If the directory does not exist, create it
        if (dir.mkpath(dirPath)) {
            qDebug() << "Directory created successfully.";
        } else {
            qDebug() << "Failed to create directory.";
        }
    } else {
        // qDebug() << "Directory already exists.";
    }
    outputFile = new QFile(dir.absolutePath() + "/" + fileName);
    qDebug() <<outputFile->fileName();

    outputFile->open(QIODevice::Append);
    // writeToFile(createHeader());
    QStringList list = createHeader();
    for (QString l:list) {
        // *outputStream << QByteArray(l.toStdString().c_str()) << "\n";
        outputFile->write(QByteArray(l.toUtf8()));
        outputFile->write("\n");
        // qDebug() << l <<"wrote to" << outputFile->fileName();
    }
    outputFile->close();
};


void P190_creator::writeToFile(QStringList data) {
    if (outputFile->open(QIODevice::Append)){
        for (QString str: data) {
            if (data.isEmpty()) continue;
            if (str.length() > 82 ){    //
            } else {
                outputFile->write(QByteArray(str.toUtf8()) + "\n");
                // outputFile->write("\n");
            }
        }
        outputFile->close();
    }
}

QStringList P190_creator::createStreamerBlock() {
    QStringList res;
    QString tmp;
    for (FixedItem* item: MyVault->ItemsVault) {
        if (QString(item->metaObject()->className()) == "Streamer") {
            Streamer* strm = dynamic_cast<Streamer*>(item);
            for (uint i = 0; i < strm->getChanCount(); ++i) {
                if (i % 3 == 0) {
                    if (!tmp.isEmpty()) {
                        res.append(tmp.append("1"));
                    }
                    tmp = "R  ";
                }
                tmp += strm->getChan(i+1)->getUTMPos(); // в массиве от 0 до 23
                tmp += " "; //каналы от 1 до 24
            }
            if (!tmp.isEmpty()) {
                res.append(tmp + "1");
            }
        }
    }
    return res;
}




QStringList P190_creator::createMainInfoBlock() {
    QStringList res;
    QString tmp;
    static int pointNumber = 1;
    for (FixedItem* item: MyVault->ItemsVault) {
        if (QString(item->metaObject()->className()) == "Source") {
            continue;
        }
        tmp = createMainRow(item, pointNumber);
        if (tmp.isEmpty()) continue;
        res.append(tmp);
        // qDebug() <<tmp << "createMainInfoBlock() ";
    }
    ++pointNumber;

    return res;
}

QString P190_creator::createMainRow(FixedItem *item, int pointNumber) {
    QString tmp;
    QChar type = 'Z';
    QChar VesselID = '1';
    QChar SourceID = '1';
    QChar TailBuoyID = ' ';
    QDateTime dt = item->lastGGAData.dateTime;

    if(QString(item->metaObject()->className()) == "FixedItem") {
        if (item->x == 0 && item->y == 0 && item->z == 0) {
            type = 'V';
            SourceID = ' ';
            if (item->lastGGAData.dateTime.isValid()) curDateTime = item->lastGGAData.dateTime;
        }
    } else if(QString(item->metaObject()->className()) == "Buoy") {
        type = 'T';
        TailBuoyID = '1';
    } else if (QString(item->metaObject()->className()) == "Streamer") {
        type = 'S';
    }
    // qDebug() <<item->lastGGAData.coordinate.toString();

    // if (type == 'Z') return QString("");
    if (!dt.isValid()) {
        dt = this->curDateTime;
        // qDebug() << QString("%1").arg(dt.date().day(), 3, 10, QChar('0'))
        //                 + QString("%1").arg(dt.time().hour(), 2, 10, QChar('0'))
        //                 + QString("%1").arg(dt.time().minute(), 2, 10, QChar('0'))
        //                 + QString("%1").arg(dt.time().second(), 2, 10, QChar('0'));
    }
    // + convertCoordinates(item->lastGGAData.coordinate.toString())

    tmp = type + lineName  + QString(12-lineName.length(), ' ') + "   " +
          VesselID + SourceID + TailBuoyID
          + QString(6-QString::number(pointNumber).length(), ' ') + QString::number(pointNumber)
          + QGeoCoordinate(item->latitude, item->longitude).toString(QGeoCoordinate::CoordinateFormat::DegreesWithHemisphere).remove("°").remove(' ')
          + floatToQString(item->x_coor, 8,2)
          + floatToQString(item->y_coor, 9,2)
          + " " + floatToQString(item->height, 3,2)
          + QString("%1").arg(dt.date().day(), 3, 10, QChar('0'))
          + QString("%1").arg(dt.time().hour(), 2, 10, QChar('0'))
          + QString("%1").arg(dt.time().minute(), 2, 10, QChar('0'))
           + QString("%1").arg(dt.time().second(), 2, 10, QChar('0'));
    return tmp;
}

void P190_creator::setItemStoragePtr(ItemsStorage *Vault)
{
    this->MyVault = Vault;
}

void P190_creator::setLineName(const QString &newLineName)
{
    lineName = newLineName;
}

QString P190_creator::createFileName()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString fileName = QString("%1_%2_%3_%4.p190")
                           .arg(currentDateTime.time().hour())
                           .arg(currentDateTime.time().minute(), 2, 10, QChar('0'))  // Добавляем ведущий ноль для минут
                           .arg(currentDateTime.date().day(),2, 10, QChar('0'))
                           .arg(currentDateTime.date().month(),2, 10, QChar('0'));
    this->fileName = fileName;
    return fileName;
}

QString P190_creator::convertCoordinates(const QString &input)
{
    QRegExp rx("([0-9]+)° ([0-9]+)' ([0-9]+\\.[0-9]+)\" ([NSEW]), ([0-9]+)° ([0-9]+)' ([0-9]+\\.[0-9]+)\" ([NSEW])");
    QString result;

    if (rx.indexIn(input) != -1) {
        QString latDeg = rx.cap(1);
        QString latMin = rx.cap(2);
        QString latSec = rx.cap(3);
        QString latDir = rx.cap(4);
        QString lonDeg = rx.cap(5);
        QString lonMin = rx.cap(6);
        QString lonSec = rx.cap(7);
        QString lonDir = rx.cap(8);

        result = latDeg + latMin + latSec + latDir + lonDeg + lonMin + lonSec + lonDir;
    }

    return result;
}



