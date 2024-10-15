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
    if (-1 == FFID) return QStringList();
    QStringList res;
    QStringList mainBlock = createMainInfoBlock(this->FFID);
    // createMainInfoBlock()
    QStringList streamerBlock = createStreamerBlock();
    // qDebug() << "P190 createShotBlock()";
    // if (!outputFile->isOpen()) outputFile->open(QIODevice::Append);
    for (QString l: mainBlock) {
        // qDebug() << l;
    }
    for (QString l: streamerBlock) {
        // qDebug() << l;
    }
    res.append(mainBlock);
    res.append(streamerBlock);
    writeToFile(res);
    return res;
}




QStringList P190_creator::createHeader(int dayNumber) {
    QString filePath = ":/Data/SurveyInfo.txt";
    QFile file(filePath);
    QStringList header;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // Читаем строки из файла и добавляем их в QStringList
        while (!in.atEnd()) {
            QString line = in.readLine();
            header << line;
        }

        file.close();
    } else {
        // Обработка ошибки, если файл не удалось открыть
        qWarning("Cannot open file for reading: %s", qPrintable(file.errorString()));
    }
    header[7] = replaceDates(header[7]);
    header[8] = replaceDates(header[8]);


    return header;
}

QStringList P190_creator::createHeader() {
    QString filePath = ":/Data/SurveyInfo.txt";
    QFile file(filePath);
    QStringList header;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        // Читаем строки из файла и добавляем их в QStringList
        while (!in.atEnd()) {
            QString line = in.readLine();
            header << line;
        }

        file.close();
    } else {
        // Обработка ошибки, если файл не удалось открыть
        qWarning("Cannot open file for reading: %s", qPrintable(file.errorString()));
    }
    header[7] = replaceDates(header[7]);
    header[8] = replaceDates(header[8]);

    return header;
};



void P190_creator::createP190File() {
    if (headerIsWritten || dayOfSurvey == -1) {
        return;
    }
    QDir dir = path; //setted in MainWindow
    fileName = createFileName();
    outputFile = new QFile(dir.absolutePath() + "/" + fileName);
    outputFile->open(QIODevice::Append);
    QStringList list = createHeader();
    for (QString l:list) {
        outputFile->write(QByteArray(l.toUtf8()));
        outputFile->write("\n");
    }
    outputFile->close();
}

void P190_creator::createP190File(int dayNumber)
{
    if (!headerIsWritten) {
        this->dayOfSurvey = dayNumber;
        createP190File();
        headerIsWritten = true;
    } else {
        return;
    }
}




void P190_creator::writeToFile(QStringList data) {
    if (outputFile->open(QIODevice::Append)){
        for (QString str: data) {
            if (str.isEmpty() || str.length() < 20) continue;
            outputFile->write(QByteArray(str.toUtf8()) + "\n");
            // logmsg(str);
                // outputFile->write("\n");
        }
        outputFile->close();
    }
}

QStringList P190_creator::createStreamerBlock() {
    QStringList res;
    ///TODO: убрать костыли
    for (FixedItem* item: MyVault->ItemsVault) {
        if (QString(item->metaObject()->className()) == "Streamer") {
            Streamer* strm = dynamic_cast<Streamer*>(item);
            // qDebug() << "REAL AZIMUTH OF TOWING" << strm->realAzimuthOfTowingRadians;
            // QString tmp(80, ' ');
            QString tmp = "R";
            for (uint i = 0; i < strm->getChanCount(); ++i) {
                if (i % 3 == 0) {
                    if (!tmp.isEmpty()) {
                        // res.append(tmp.append("1"));
                        res.append(tmp);
                        tmp = "R";
                    } ///I dont fucking know how does it work

                }
                tmp += strm->getChan(i+1)->getUTMPos(); // в массиве от 0 до 23
                // tmp += " "; //каналы от 1 до 24
            }
            if (!tmp.isEmpty()) res.append(tmp);
        }
    }
    return res;
}




QStringList P190_creator::createMainInfoBlock() {
    this->currentBuoyNumber = 0;
    QStringList res;
    QString tmp;
    static int pointNumber = 1;
    for (FixedItem* item: MyVault->ItemsVault) {
        if (QString(item->metaObject()->className()) == "Streamer") {
            continue;
        }
        tmp = createMainRow__new(item, pointNumber, currentBuoyNumber);
        currentBuoyNumber++;
        if (tmp.isEmpty()) continue;
        res.append(tmp);
        // qDebug() <<tmp << "createMainInfoBlock() ";
    }
    ++pointNumber;

    return res;
}

QStringList P190_creator::createMainInfoBlock(int ffid) {
    currentBuoyNumber = 0;
    QStringList res;
    QString tmp;
    // int pointNumber = ffid;
    for (FixedItem* item: MyVault->ItemsVault) {
        if (QString(item->metaObject()->className()) == "Streamer") {
            continue;
        }
        tmp = createMainRow__new(item, ffid, currentBuoyNumber);
        currentBuoyNumber++;
        if (tmp.isEmpty()) continue;
        res.append(tmp);
        // qDebug() <<tmp << "createMainInfoBlock() ";
    }
    // ++pointNumber;

    return res;
}

QString P190_creator::createMainRow__new(FixedItem *item, int pointNumber, int tailBuoy)
{
    QString res(80, ' ');
    QChar type = 'Z';
    QChar VesselID = '1';
    QChar SourceID = '1';
    QChar TailBuoyID = QChar(tailBuoy);
    QDateTime dt = item->lastGGAData.dateTime;
    if (dt.isValid()) dummyTimeForItemsWithNoTime = dt;
    if(QString(item->metaObject()->className()) == "FixedItem") {
        if (item->x == 0 && item->y == 0 && item->z == 0) {
            type = 'V';
            SourceID = ' ';
            TailBuoyID = ' ';
            if (item->lastGGAData.dateTime.isValid()) curDateTime = item->lastGGAData.dateTime;
        }   else if (item->name == "BoomerTP") {
            type = 'Z';
            SourceID = '1';
            TailBuoyID = ' ';
            return "";
        }   else if (item->name == "StreamerTP") {
            type = 'X';
            SourceID = '1';
            TailBuoyID = ' ';
            return "";
        }   else if (item->name == "ship buoy") {
            type = 'X';
            SourceID = '1';
            TailBuoyID = ' ';
            return "";
        }   else {
            return "";
        }
    } else if(QString(item->metaObject()->className()) == "Buoy") {
        type = 'T';
        TailBuoyID = '1';
    } else if (QString(item->metaObject()->className()) == "Source") {
        type = 'S';
        TailBuoyID = ' ';
    }
    if (!dt.isValid()) {
        // dt = this->curDateTime;
        dt = dummyTimeForItemsWithNoTime;
    }
    // dt.date() = dateOfSurvey;
    res.replace(0, 1, type);
    res.replace(1, 12, lineName);
    res.replace(16, 1, VesselID);
    res.replace(17, 1, SourceID);
    res.replace(18, 1, TailBuoyID);
    res.replace(19, 6, QString(6-QString::number(pointNumber).length(), ' ') + QString::number(pointNumber));
    res.replace(25, 21, QGeoCoordinate(item->latitude, item->longitude).
        toString(QGeoCoordinate::CoordinateFormat::DegreesWithHemisphere).remove("°").remove(' '));
    res.replace(46, 9, floatToQString(item->x_coor, 8,2));
    res.replace(55, 9, floatToQString(item->y_coor, 9,2));
    res.replace(64, 5, floatToQString(item->height, 5,1));
    res.replace(70, 3, QString("%1").arg(dayOfSurvey, 3, 10, QChar('0')));
    res.replace(73, 2, QString("%1").arg(dt.time().hour(), 2, 10, QChar('0')));
    res.replace(75, 2, QString("%1").arg(dt.time().minute(), 2, 10, QChar('0')));
    res.replace(77, 2, QString("%1").arg(dt.time().second(), 2, 10, QChar('0')));
    return res;
}

void P190_creator::setPath(const QDir &newPath)
{
    path = newPath;
}

void P190_creator::setFFID(int newFFID)
{
    FFID = newFFID;
}

ItemsStorage *P190_creator::getMyVault() const
{
    return MyVault;
}

void P190_creator::setMyVault(ItemsStorage *newMyVault)
{
    MyVault = newMyVault;
}

void P190_creator::setDayOfSurvey(int newDayOfSurvey)
{
    dayOfSurvey = newDayOfSurvey;
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


QString P190_creator::replaceDates(QString& input) {
    // Define the current date in the desired format (dd.MM.yyyy)
    QDate date = QDate(2024, 1,1).addDays(this->dayOfSurvey-1);
    QString currentDate = date.toString("dd.MM.yyyy");
    QString modifiedInput = input;
    QString month = QString::number(date.month());
    QString day = QString::number(date.day());
    if (month.length() == 1) {
        month = "0" + month;
    }
    if (day.length() == 1) {
        day = "0" + day;
    }
    modifiedInput.replace("23", day);
    modifiedInput.replace("06", month);
    modifiedInput.replace("1997", QString::number(date.year()));

    return modifiedInput;
}

