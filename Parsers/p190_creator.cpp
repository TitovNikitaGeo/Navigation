#include "p190_creator.h"


P190_creator::P190_creator(QObject *parent)
    : QObject{parent}
{}

void P190_creator::createP190File() {
    outputFile = new QFile(fileName);
    if (outputFile->open(QIODevice::Append)) {
        outputStream = new QTextStream(outputFile);
        writeToFile(createHeader());
    } else {
        QMessageBox box;
        box.critical(nullptr, "Error", "File not created");
    }
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
};


void P190_creator::writeToFile(QStringList data) {
    for (QString str: data) {
        if (str.length() > 82){    //
            qDebug() << "too big string";
        }
        (*outputStream) << str;
    }
}

void P190_creator::setFileName(QString fileName)
{
    this->fileName = fileName;
}

QStringList P190_creator::createStreamerData() {
    QStringList res;
    QString tmp;
    for (FixedItem* item: MyVault->ItemsVault) {
        if (QString(item->metaObject()->className()) == "Streamer") {
            Streamer* strm = dynamic_cast<Streamer*>(item);
            for (uint i = 0; i < strm->getChanCount(); ++i) {
                if (i % 3 == 0) {
                    if (!tmp.isEmpty()) {
                        res.append(tmp + "1");
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
    qDebug() << "P190";
    for (QString i: res) {
        qDebug() << i ;
    }
    return res;
}

QStringList P190_creator::createMainInfoBlock() {
    QStringList res;
    QString tmp;
    for (FixedItem* item: MyVault->ItemsVault) {
        if (QString(item->metaObject()->className()) == "Source") {
            ;
        } else if(QString(item->metaObject()->className()) == "FixedItem") {
            if (item->x == 0 && item->y == 0 && item->z == 0) {
                QDateTime dt = item->lastGGAData.dateTime;
                tmp = "V" +  lineName + "   " + QString::number(pointNumber) +
                    QString::number(item->lastGGAData.coordinate.latitude())+"N"+
                    QString::number(item->lastGGAData.coordinate.longitude())+"E "+
                    // floatToQString(item->x_coor, 7,1) + floatToQString(item->y_coor, 8,1) +
                    // " " + floatToQString(item->z, 2,1) + QString::number(dt.date().toJulianDay()) +
                    QString::number(dt.time().hour())+QString::number(dt.time().minute()) +
                      QString::number(dt.time().second());
            }
        } else if(item->itemType == "Buoy"){
            ;
        }
        continue;
    }
    qDebug() << "P190";
    for (QString i: res) {
        qDebug() << i ;
    }
    return res;
}



void P190_creator::setItemStoragePtr(ItemsStorage *Vault)
{
    this->MyVault = Vault;
}



