#include "postprocessor.h"

PostProcessor::PostProcessor() {}

int PostProcessor::runPP() {
    int nmeaFilesFound;
    int* trash = 0;
    int res = 1;

    getDataFromSegy(); //getting pairs
    logmsg("Pairs generated");
    fillItemsVectors(); //getting items (with and without connection)
    // findNmeaFiles(); //getting list of files for items with con

    QStringList nmeaValues;
    for (SegYReader::Pair pair :pairs) {
        for (FixedItem* item: vectorWithCon) {
            nmeaValues = findNmaeForSegy(pair, item->connection->file, trash);
            NmeaParser::NmeaGGAData first = nmeaParser.parseNmeaGGA(nmeaValues[0]);
            NmeaParser::NmeaGGAData second = nmeaParser.parseNmeaGGA(nmeaValues[1]);
            NmeaParser::NmeaGGAData truePosition = this->calcTruePosition(first, second,
                pair.time, nmeaParser.getTimeFromNmeaGGA(nmeaValues[0]),
                nmeaParser.getTimeFromNmeaGGA(nmeaValues[1]));
            item->lastGGAData = truePosition;
        }
        logmsg("Show with ffid " + QString::number(pair.ffid) + " created");
    }
    return res;
}

void PostProcessor::getDataFromSegy()
{

    SegYReader sr;
    sr.readPathWithSegy(this->segyStorage);
    logmsg("Seg-Y reading started");
    for (int i = 0; i < sr.times.size(); i++) {
        pairs.push_back(sr.pairs.at(i));
        // logmsg("read from sgy. FFID - " + QString::number(pairs.at(i).ffid) + " time - " +
        //        pairs.at(i).time.toString("hh:mm:ss.zzz"));
    }
    logmsg("Seg-Y reading finished");
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

QStringList PostProcessor::findNmaeForSegy(SegYReader::Pair pair, QFile* nmeaFile, int* pos) {
    QTime time = pair.time;
    QString lowerNmea;
    QString higherNmea;
    int currentPosition = *pos;

    // Move the file pointer to the last found position
    // nmeaFile.seek(currentPosition);

    // Create a text stream to read the file
    QTextStream in(nmeaFile);
    QString line;

    while (in.readLineInto(&line)) {
        if (line[3] == 'G' && line[4] == 'G' && line[5] == 'A') {
            // Assuming you have a function that extracts QTime from NMEA string
            QTime nmeaTime = nmeaParser.getTimeFromNmeaGGA(line);
            // Check if the NMEA time is lower or higher than the specified time
            if (nmeaTime < time) {
                lowerNmea = line; // Update the lower NMEA string
                currentPosition = nmeaFile->pos(); // Update current position
            } else if (nmeaTime > time && higherNmea.isEmpty()) {
                higherNmea = line; // Get the first higher NMEA string
                currentPosition = nmeaFile->pos(); // Update current position
                break; // Stop searching once we find the first higher string
            }
        } else if (line[3] == 'R' && line[4] == 'M' && line[5] == 'C') {

        }
    }
    logmsg(lowerNmea+ " " + higherNmea + " time " + time.toString("hh:mm:ss.zzz"));
    // Update the position
    *pos = currentPosition;

    // Return the result as a QStringList
    return QStringList() << lowerNmea << higherNmea;
}

NmeaParser::NmeaGGAData PostProcessor::calcTruePosition(NmeaParser::NmeaGGAData first,
    NmeaParser::NmeaGGAData second, QTime trueTime, QTime firstTime, QTime secondTime)
{
    float c1, c2;
    NmeaParser::NmeaGGAData res;
    if (!firstTime.isValid()) return second;
    if (!secondTime.isValid()) return first;

    c1 = 1-(trueTime.msecsSinceStartOfDay() - firstTime.msecsSinceStartOfDay())/
                 (secondTime.msecsSinceStartOfDay() - firstTime.msecsSinceStartOfDay());
    c2 = 1-c1;
    trueTime.setHMS(trueTime.hour(),trueTime.minute(),trueTime.second());
    res.dateTime = QDateTime(first.dateTime.date(), trueTime);
    res.height = first.height*c1 + second.height*c2;
    res.coorUTM.rx() = first.coorUTM.rx()*c1 + second.coorUTM.rx()*c2;
    res.coorUTM.ry() = first.coorUTM.ry()*c1 + second.coorUTM.ry()*c2;
    res.coordinate.setLatitude(first.coordinate.altitude()*c1 + second.coordinate.altitude()*c2);
    res.coordinate.setLongitude(first.coordinate.longitude()*c1 + second.coordinate.longitude()*c2);
    return res;
}




void PostProcessor::setMyVault(ItemsStorage *newMyVault)
{
    MyVault = newMyVault;
}

void PostProcessor::setNmeaStorage(const QDir &newNmeaStorage)
{
    nmeaStorage = newNmeaStorage;
}


