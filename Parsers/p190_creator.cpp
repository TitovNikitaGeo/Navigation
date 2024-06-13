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

QStringList P190_creator::createRecieverData() {
    QStringList res;
    for (FixedItem* item: items) {
        if (item->itemType == "Streamer") { //fuck
            ;
        }
    }
    return res;
}

QStringList P190_creator::createMainInfoBlock() {
    QStringList res;
    for (FixedItem* item: items) {
        if (item->itemType == "Source") {
            ;
        } else if(item->itemType == "Ship Center") {
            ;
        } else if(item->itemType == "Buoy"){
            ;
        }
        continue;
    }
    return res;
}




void P190_creator::updateItemsList(FixedItem* newItem) {
    items.append(newItem);
    // connect(this, itemsGiveMeData(), newItem, SLOT());
}



