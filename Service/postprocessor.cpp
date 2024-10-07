#include "postprocessor.h"

PostProcessor::PostProcessor() {
    p190 = new P190_creator();
}

int PostProcessor::runPP() {
    // int nmeaFilesFound;
    int* trash = 0;
    int res = 1;
    p190->createP190File();


    getDataFromSegy(); //getting pairs
    // logmsg("Pairs generated");
    fillItemsVectors(); //getting items (with and without connection)
    // findNmeaFiles(); //getting list of files for items with con

    QStringList nmeaValues;
    for (SegYReader::Pair pair :pairs) {
        float commonDummyAzimuth = -1;
        for (FixedItem* item: vectorWithCon) {
            nmeaValues = findNmeaForSegy(pair, item->connection->file, trash);
            NmeaParser::NmeaGGAData first = nmeaParser.parseNmeaGGA(nmeaValues[0]);
            NmeaParser::NmeaGGAData second = nmeaParser.parseNmeaGGA(nmeaValues[1]);
            if (nmeaValues.size() == 3) {
                if (!nmeaValues[2].isEmpty()){
                    NmeaParser::NmeaRMCData rmc = nmeaParser.parseNmeaRMC(nmeaValues[2]);
                    item->lastRMCData = rmc;
                    commonDummyAzimuth = rmc.azimuth;
                }
            }
            NmeaParser::NmeaGGAData truePosition = this->calcTruePosition(first, second,
                pair.time, nmeaParser.getTimeFromNmeaGGA(nmeaValues[0]),
                nmeaParser.getTimeFromNmeaGGA(nmeaValues[1]));
            item->lastGGAData = truePosition;
            item->azimuthOfMovement = commonDummyAzimuth;
        }
        if (commonDummyAzimuth == -1) {
            qDebug() << "we have no azimuth" <<  __FUNCTION__;
        }
        for (FixedItem* item: vectorNoCon) {
            item->azimuthOfMovement = commonDummyAzimuth;
        }
        for (FixedItem* item: vectorWithCon) {
            item->calcItemCoordinates();
        }
        for (FixedItem* item: vectorNoCon) {
            item->calcItemCoordinates();
        }
        p190->createMainInfoBlock(pair.ffid);
        logmsg("P190 block with ffid " + QString::number(pair.ffid) + " created");
    }

    return res;
}

void PostProcessor::getDataFromSegy()
{

    SegYReader sr;
    sr.readPathWithSegy(this->segyStorage);
    // logmsg("Seg-Y reading started");
    for (int i = 0; i < sr.times.size(); i++) {
        pairs.push_back(sr.pairs.at(i));
        // logmsg("read from sgy. FFID - " + QString::number(pairs.at(i).ffid) + " time - " +
        //        pairs.at(i).time.toString("hh:mm:ss.zzz"));
        // qDebug() << pairs[i].ffid << pairs[i].time;
    }
    // logmsg("Seg-Y reading finished");
}

void PostProcessor::setSegyStorage(const QDir &newSegyStorage)
{
    segyStorage = newSegyStorage;
}

void PostProcessor::fillItemsVectors()
{
    for (FixedItem* i:MyVault->ItemsVault) {
        if (i->hasConnection) {
            if (i->connection) {
                vectorWithCon.push_front(i);
            } else {
                Logger::instance().logMessage(Logger::LogLevel::Error, "no connection");
            }
        } else {
            vectorWithCon.push_front(i);
        }
    }
}

int PostProcessor::findNmeaFiles()
{
    for (FixedItem* i: this->vectorWithCon) {
        QFile* file = i->connection->file;
        this->NmeaFiles.push_front(file);
    }
    return 1;
}

/*QStringList PostProcessor::findNmeaForSegy(SegYReader::Pair pair, QFile* nmeaFile, int* pos) {
    QTime time = pair.time;
    QString lowerNmea;
    QString higherNmea;
    int currentPosition = *pos;

    QStringList res;
    // nmeaFile->open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(nmeaFile);
    // in.read((currentPosition));
    QString line;

    QString RMCData = "";
    while (in.readLineInto(&line)) {
        // qDebug() << line << __LINE__;

        if (line[3] == 'G' && line[4] == 'G' && line[5] == 'A') {
            // Assuming you have a function that extracts QTime from NMEA string
            QTime nmeaTime = nmeaParser.getTimeFromNmeaGGA(line);
            // Check if the NMEA time is lower or higher than the specified time
            if (nmeaTime.msecsSinceStartOfDay() < time.msecsSinceStartOfDay()) {
                lowerNmea = line; // Update the lower NMEA string
                currentPosition = in.pos(); // Update current position
                continue;
            } else if (nmeaTime.msecsSinceStartOfDay() > time.msecsSinceStartOfDay() && higherNmea.isEmpty()) {
                qDebug() << nmeaTime.msecsSinceStartOfDay() << time.msecsSinceStartOfDay() << __LINE__;
                higherNmea = line; // Get the first higher NMEA string
                // currentPosition = nmeaFile->pos(); // Update current position
                break; // Stop searching once we find the first higher string
            }
        } else if (line[3] == 'R' && line[4] == 'M' && line[5] == 'C') {
            RMCData = line;
        } else if (line[3] == 'H' && line[4] == 'D' && line[5] == 'T') {
            RMCData = line;
        }
    }

    // *pos = currentPosition;
    // qDebug() <<"POS" << currentPosition;
    // Return the result as a QStringList
    // qDebug() << lowerNmea << higherNmea << RMCData;

    res.push_back(lowerNmea);
    res.push_back(higherNmea);
    if (!RMCData.isEmpty()) res.push_back(RMCData);
    return res;
}*/
/*
QStringList PostProcessor::findNmeaForSegy(SegYReader::Pair pair, QFile* nmeaFile, int* pos) {
    QTime time = pair.time;
    QString lowerNmea;
    QString higherNmea;
    int currentPosition = *pos;

    QStringList res;
    QTextStream in(nmeaFile);
    QString line;
    QString RMCData = "";

    while (in.readLineInto(&line)) {
        // Print debug information for troubleshooting
        // qDebug() << "Processing Line:" << line;

        if (line.mid(3, 3) == "GGA") {
            QTime nmeaTime = nmeaParser.getTimeFromNmeaGGA(line);
            if (nmeaTime.hour() < time.hour() && nmeaTime.minute() < time.minute()) continue;

            if (nmeaTime.msecsSinceStartOfDay() < time.msecsSinceStartOfDay()) {
                // qDebug() << "NMEA Time:" << nmeaTime << "Pair Time:" << time << "Line:" << line;
                lowerNmea = line;  // Update lower NMEA
                currentPosition = in.pos();  // Update current position
            } else if (nmeaTime.msecsSinceStartOfDay() > time.msecsSinceStartOfDay()) {
                qDebug() << "NMEA Time:" << nmeaTime << "Pair Time:" << time << "Line:" << line;
                higherNmea = line;  // Capture first higher NMEA
                break;  // Exit loop after finding higher NMEA
            }
        } else if (line.mid(3, 3) == "RMC" || line.mid(3, 3) == "HDT") {
            RMCData = line;  // Capture RMC or HDT data
        }
    }

    *pos = currentPosition;  // Update file position
    // qDebug() << *pos;
    qDebug()<< res;
    res.push_back(lowerNmea);
    res.push_back(higherNmea);
    qDebug()<< res;
    if (!RMCData.isEmpty()) res.push_back(RMCData);
    // qDebug() << res;
    // nmeaFile->close();
    // nmeaFile->open(QIODevice::ReadOnly | QIODevice::Text);
    return res;
}
*/

QStringList PostProcessor::findNmeaForSegy(SegYReader::Pair pair, QFile* nmeaFile, int* pos) {
    QTime time = pair.time;
    QString lowerNmea;
    QString higherNmea;
    int currentPosition = *pos;

    QStringList res;
    QString line;
    QString RMCData = "";

    if (!nmeaFile->isOpen()) {
        nmeaFile->open(QIODevice::ReadOnly | QIODevice::Text);
    }

    nmeaFile->seek(currentPosition-100);  // Устанавливаем текущую позицию файла

    while (!nmeaFile->atEnd()) {
        line = nmeaFile->readLine().trimmed();  // Считываем строку и удаляем пробелы по краям

        if (line.mid(3, 3) == "GGA") {
            QTime nmeaTime = nmeaParser.getTimeFromNmeaGGA(line);
            // qDebug() << "NMEA Time:" << nmeaTime << "Pair Time:" << time << "Line:" << line;

            if (nmeaTime.msecsSinceStartOfDay() < time.msecsSinceStartOfDay()) {
                lowerNmea = line;  // Обновляем нижнюю NMEA строку
                currentPosition = nmeaFile->pos();  // Обновляем текущую позицию
            } else if (nmeaTime.msecsSinceStartOfDay() > time.msecsSinceStartOfDay() && higherNmea.isEmpty()) {
                higherNmea = line;  // Получаем первую строку с временем больше указанного
                break;  // Прекращаем поиск после нахождения первой строки с большим временем
            }
        } else if (line.mid(3, 3) == "RMC" || line.mid(3, 3) == "HDT") {
            RMCData = line;  // Сохраняем RMC или HDT данные
        }
    }

    *pos = currentPosition;  // Обновляем позицию файла
    res.push_back(lowerNmea);
    res.push_back(higherNmea);
    if (!RMCData.isEmpty()) res.push_back(RMCData);

    return res;
}
NmeaParser::NmeaGGAData PostProcessor::calcTruePosition(NmeaParser::NmeaGGAData first,
    NmeaParser::NmeaGGAData second, QTime trueTime, QTime firstTime, QTime secondTime)
{
    float c1, c2;
    NmeaParser::NmeaGGAData res;
    if (!firstTime.isValid()) return second;
    if (!secondTime.isValid()) return first;
    if (secondTime == firstTime) return second;
    c1 = 1-(trueTime.msecsSinceStartOfDay() - firstTime.msecsSinceStartOfDay())/
                 (secondTime.msecsSinceStartOfDay() - firstTime.msecsSinceStartOfDay());
    c2 = 1-c1;
    trueTime.setHMS(trueTime.hour(),trueTime.minute(),trueTime.second());
    res.dateTime = QDateTime(first.dateTime.date(), trueTime);
    res.height = first.height*c1 + second.height*c2;
    res.coorUTM.rx() = first.coorUTM.rx()*c1 + second.coorUTM.rx()*c2;
    res.coorUTM.ry() = first.coorUTM.ry()*c1 + second.coorUTM.ry()*c2;

    res.coordinate.setLatitude(first.coordinate.latitude()*c1 + second.coordinate.latitude()*c2);
    res.coordinate.setLongitude(first.coordinate.longitude()*c1 + second.coordinate.longitude()*c2);

    return res;
}

void PostProcessor::setP190(P190_creator *newP190)
{
    p190 = newP190;
}




void PostProcessor::setMyVault(ItemsStorage *newMyVault)
{
    MyVault = newMyVault;
}

void PostProcessor::setNmeaStorage(const QDir &newNmeaStorage)
{
    nmeaStorage = newNmeaStorage;
}


