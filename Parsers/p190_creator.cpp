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
    // Check if the directory already exists
    // if (!dir.exists()) {
    //     // If the directory does not exist, create it
    //     if (dir.mkpath(dirPath)) {
    //         qDebug() << "Directory created successfully.";
    //     } else {
    //         qDebug() << "Failed to create directory.";
    //     }
    // } else {
    //     qDebug() << "Directory already exists.";
    // }
    outputFile = new QFile(dir.absolutePath() + "/" + fileName);
    qDebug() <<outputFile->fileName();

    outputFile->open(QIODevice::Append);

    QStringList list = createHeader();
    for (auto l:list) {
        // *outputStream << QByteArray(l.toStdString().c_str()) << "\n";
        outputFile->write(QByteArray(l.toUtf8()));
        outputFile->write("\n");
        qDebug() << l <<"wrote to" << outputFile->fileName();
    }
    outputFile->close();
};


void P190_creator::writeToFile(QStringList data) {
    if (outputFile->open(QIODevice::Append)){
        for (QString str: data) {
            if (str.length() > 82 ){    //
                // qDebug() << "too big string" << str;
            } else {
                // qDebug() << "not too big string" << data;
                outputFile->write(QByteArray(str.toUtf8()));
                outputFile->write("\n");
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
    // qDebug() << "P190";
    // for (QString i: res) {
    //     qDebug() << i ;
    // }
    return res;
}




QStringList P190_creator::createMainInfoBlock() {
    QStringList res;
    QString tmp;
    static uint pointNumber = 1;
    for (FixedItem* item: MyVault->ItemsVault) {
        if (QString(item->metaObject()->className()) == "Source") {
            ;
        } else if(QString(item->metaObject()->className()) == "FixedItem") {
            if (item->x == 0 && item->y == 0 && item->z == 0) {
                QDateTime dt = item->lastGGAData.dateTime;
                tmp = "V" +  lineName + " " + QString::number(pointNumber) +
                    QString::number(item->lastGGAData.coordinate.latitude())+"N"+
                    QString::number(item->lastGGAData.coordinate.longitude())+"E "+
                    floatToQString(item->x_coor, 7,2) + floatToQString(item->y_coor, 8,2) +
                    " " + floatToQString(item->z, 2,1) + QString::number(dt.date().toJulianDay()) +
                    QString::number(dt.time().hour())+QString::number(dt.time().minute()) +
                      QString::number(dt.time().second());
                res.append(tmp);
            }

        } else if(QString(item->metaObject()->className()) == "Buoy"){
            QDateTime dt = item->lastGGAData.dateTime;
            tmp = "T" +  lineName + " " + QString::number(pointNumber) +
                  QString::number(item->lastGGAData.coordinate.latitude())+"N"+
                  QString::number(item->lastGGAData.coordinate.longitude())+"E "+
                  floatToQString(item->x_coor, 7,2) + floatToQString(item->y_coor, 8,2) +
                  " " + floatToQString(item->z, 2,1) + QString::number(dt.date().toJulianDay()) +
                  QString::number(dt.time().hour())+QString::number(dt.time().minute()) +
                  QString::number(dt.time().second());
            res.append(tmp);
        }
    }
    ++pointNumber;
    // qDebug() << "P190";
    // for (QString i: res) {
    //     qDebug() << i ;
    // }
    return res;
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



