#include "postprocessor.h"

PostProcessor::PostProcessor() {
    p190Creator = new P190_creator();
}

int PostProcessor::mainProcess()
{
    QFile* jsonFile = new QFile(jsonSchemeFile);
    jsonFile->open(QIODevice::ReadOnly);
    items = ItemsLoader::readFromJSON(jsonFile);
    for (auto i: items) {
        Vault.SaveItem(i);
    }
    p190Creator->setMyVault(&Vault);

    return 0;
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
    QTime nmeaTime;
    nmeaFile->seek(currentPosition - 100);  // Устанавливаем текущую позицию файла

    while (!nmeaFile->atEnd()) {
        line = nmeaFile->readLine().trimmed();  // Считываем строку и удаляем пробелы по краям

        if (line.mid(3, 3) == "GGA") {
            nmeaTime = nmeaParser.getTimeFromNmeaGGA(line);
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

    // Если мы нашли верхнюю строку (higherNmea), но нижняя не была обновлена,
    // нужно вернуть предшествующую строку, которая может быть lowerNmea
    if (higherNmea.isEmpty() && !lowerNmea.isEmpty()) {
        // Перемещаемся на одну строку назад для получения GGA, если higherNmea не найден
        nmeaFile->seek(currentPosition - line.length());  // Возвращаемся на длину предыдущей строки
        if (!nmeaFile->atEnd()) {
            line = nmeaFile->readLine().trimmed();  // Считываем предыдущую строку
            if (line.mid(3, 3) == "GGA") {
                higherNmea = line;  // Устанавливаем строку как higherNmea
            }
        }
    }

    *pos = currentPosition;  // Обновляем позицию файла
    res.push_back(lowerNmea);
    res.push_back(higherNmea);
    if (!RMCData.isEmpty()) res.push_back(RMCData);

    // qDebug() << lowerNmea << higherNmea << pair.time << __FUNCTION__;

    return res;
}


NmeaParser::CoordinateData PostProcessor::calcTruePosition(NmeaParser::CoordinateData first,
    NmeaParser::CoordinateData second, QTime trueTime, QTime firstTime, QTime secondTime)
{
    double c1, c2;
    NmeaParser::CoordinateData res;
    if (!firstTime.isValid()) return second;
    if (!secondTime.isValid()) return first;
    if (secondTime == firstTime) return second;
    c1 = 1-((float)(trueTime.msecsSinceStartOfDay() - firstTime.msecsSinceStartOfDay())/
              (float)(secondTime.msecsSinceStartOfDay() - firstTime.msecsSinceStartOfDay()));
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
    p190Creator = newP190;
}

QVector<SegYReader::Pair> PostProcessor::preparePairs(QVector<SegYReader::Pair> in)
{
    QVector<SegYReader::Pair> toInsert;
    QVector<SegYReader::Pair> out;
    float timeStep = 0;
    float dt;

    for (QVector<SegYReader::Pair>::Iterator it = in.begin()+1; it!= in.end()-1; ++it) {
        QVector<SegYReader::Pair>::iterator prev_it = std::prev(it);
        QVector<SegYReader::Pair>::iterator next_it = std::next(it);
        timeStep = it->time.msecsSinceStartOfDay() - prev_it->time.msecsSinceStartOfDay();
        if (timeStep > 2000) {
            toInsert.clear();
            dt = next_it->time.msecsSinceStartOfDay() - it->time.msecsSinceStartOfDay();
            // qDebug() << prev_it->time <<it->time <<timeStep/1000<< __FUNCTION__;
            for (int j = -3; j < 0; j++) {
                SegYReader::Pair newPair(-1, QTime(it->time.addMSecs(j*dt)), it->julianDay);
                out.push_back(newPair);
            }
        }
        out.push_back(*it);
    }
    // for (QVector<SegYReader::Pair>::Iterator it = out.begin()+1; it!= out.end()-1; ++it) {
    //     qDebug() << it->ffid<<it->time;
    // }

    return out;
}




void PostProcessor::setMyVault(ItemsStorage *newMyVault)
{
    MyVault = newMyVault;
}

void PostProcessor::setNmeaStorage(const QDir &newNmeaStorage)
{
    nmeaStorage = newNmeaStorage;
}

// int PostProcessor::runPP() {
//     // int nmeaFilesFound;
//     int* trash = 0;
//     int res = 1;
//     p190->createP190File();


//     getDataFromSegy(); //getting pairs
//     // logmsg("Pairs generated");
//     fillItemsVectors(); //getting items (with and without connection)
//     // findNmeaFiles(); //getting list of files for items with con

//     QStringList nmeaValues;
//     for (SegYReader::Pair pair :pairs) {
//         float commonDummyAzimuth = -1;
//         for (FixedItem* item: vectorWithCon) {
//             nmeaValues = findNmeaForSegy(pair, item->connection->file, trash);
//             NmeaParser::NmeaGGAData first = nmeaParser.parseNmeaGGA(nmeaValues[0]);
//             NmeaParser::NmeaGGAData second = nmeaParser.parseNmeaGGA(nmeaValues[1]);
//             if (nmeaValues.size() == 3) {
//                 if (!nmeaValues[2].isEmpty()){
//                     NmeaParser::NmeaRMCData rmc = nmeaParser.parseNmeaRMC(nmeaValues[2]);
//                     item->lastRMCData = rmc;
//                     commonDummyAzimuth = rmc.azimuth;
//                 }
//             }
//             NmeaParser::NmeaGGAData truePosition = this->calcTruePosition(first, second,
//                                                                           pair.time, nmeaParser.getTimeFromNmeaGGA(nmeaValues[0]),
//                                                                           nmeaParser.getTimeFromNmeaGGA(nmeaValues[1]));
//             item->lastGGAData = truePosition;
//             item->azimuthOfMovement = commonDummyAzimuth;
//         }
//         if (commonDummyAzimuth == -1) {
//             qDebug() << "we have no azimuth" <<  __FUNCTION__;
//         }
//         for (FixedItem* item: vectorNoCon) {
//             item->azimuthOfMovement = commonDummyAzimuth;
//         }
//         for (FixedItem* item: vectorWithCon) {
//             item->calcItemCoordinates();
//         }
//         for (FixedItem* item: vectorNoCon) {
//             item->calcItemCoordinates();
//         }
//         p190->createMainInfoBlock(pair.ffid);
//         logmsg("P190 block with ffid " + QString::number(pair.ffid) + " created");
//     }

//     return res;
// }

