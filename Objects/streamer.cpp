#include "streamer.h"
#include "functions.h"

double Streamer::defaultChannelDepth = 0.5;

Streamer::Streamer(QObject *parent)
{}

Streamer::Streamer(QString Name, FixedItem *towingPoint,
    double angleToWired, double wireLength, uint NumChanels, QVector<double> chans)
    : TowedItem(Name, towingPoint, angleToWired, wireLength)
{
    this->NumChanels = NumChanels;
    this->chans = chans;
    itemType = "Streamer";

    if (NumChanels == 0 || chans.empty()) {
        qDebug() << "Wrong streamer configuration";
        return;
    }

    for (uint i = 1; i <= NumChanels; i++) {
        Channel* chan = new Channel(i);
        ChannelsVector.append(chan);
    }
    totalLength =  chans.at(chans.size() - 1);
    // calcChansCoors();
    // printSelfInfo();
}

Streamer::Streamer(QString name, FixedItem *towingPoint, double angleToWired,
                   double wireLength, uint NumChannels, QString chanStep) : TowedItem(name, towingPoint, angleToWired, wireLength), NumChanels(NumChannels)
{
    bool stepIsStable = false;
    float dChan = 0;
    itemType = "Streamer";

    channelStep = chanStep;

    chanStep.toDouble(&stepIsStable);
    if (stepIsStable) {
        dChan = chanStep.toDouble();
        for (uint i = 0; i < NumChannels; i++) {
            chans.append(i*dChan);
        }
    } else { //1&2 48 chans
        if (chanStep == "1&2") {
            for (int i = 0; i < 24; i++) {
                chans.append(i);
                // qDebug() << chans[i];
            }
            for (int i = 0; i < 24; i++) {
                chans.append(chans[23] + 2 + i*2);
                // qDebug() << chans[24+i];
            }
        }
    }
    for (uint i = 1; i <= NumChanels; i++) {
        Channel* chan = new Channel(i);
        ChannelsVector.append(chan);
    }
    totalLength =  chans.at(chans.size() - 1);
}

void Streamer::calcChansCoors()
{
    // double azRad = azimuthOfMovement*M_PI/180;

    if (endBuoy != nullptr) {

        realLen = pow(((x_coor - getChan(getChanCount()-1)->x_coor)*(x_coor - getChan(getChanCount()-1)->x_coor) +
                       (y_coor - getChan(getChanCount()-1)->y_coor)*(y_coor - getChan(getChanCount()-1)->y_coor)), 0.5); //real length of streamer


        distanceCalcCoef = totalLength >= realLen? 1.0: realLen/totalLength;
        ChannelsVector[0]->x_coor = this->x_coor;
        ChannelsVector[0]->y_coor = this->y_coor;

        // qDebug() << x_coor << y_coor << towingPoint->x_coor << towingPoint->y_coor << __FUNCTION__;

        QPointF pTowingPoint(towingPoint->x_coor, towingPoint->y_coor);
        QPointF pEndBuoy(endBuoy->x_coor, endBuoy->y_coor);
        double deltaX = pTowingPoint.x() - pEndBuoy.x();
        double deltaY = pTowingPoint.y() - pEndBuoy.y();
        realAzimuthOfTowingRadians = M_PI + qAtan2(deltaX, deltaY);

        qDebug() << pTowingPoint <<pEndBuoy;
        qDebug() << __FUNCTION__ << qRadiansToDegrees(realAzimuthOfTowingRadians);


        for(int i = 1; i < ChannelsVector.size(); i++) {
            // ChannelsVector[i]->height = this->height + chans[i]* dh;
            ChannelsVector[i]->x_coor = x_coor + (1)*(chans[i]) *qSin(realAzimuthOfTowingRadians);
            ChannelsVector[i]->y_coor = y_coor + (1)*(chans[i]) *qCos(realAzimuthOfTowingRadians);
        }
    } else {

        ChannelsVector[0]->x_coor = this->x_coor;
        ChannelsVector[0]->y_coor = this->y_coor;
        // qDebug() << this->x_coor << this->y_coor;


        // qWarning() << name <<" has azimuth of towing from calculated by vessel azimuth of moving";
        for(int i = 1; i < ChannelsVector.size(); i++) {
            ChannelsVector[i]->height = this->height + chans[i]* dh;
            ChannelsVector[i]->x_coor = x_coor + (1)*(chans[i]) *qSin(sharedCircularBuffer.calculateAzimuth() + M_PI);
            ChannelsVector[i]->y_coor = y_coor + (1)*(chans[i]) *qCos(sharedCircularBuffer.calculateAzimuth() + M_PI);
        } //бля
        //когда мы не знаем тру азимут, берем повернутый на 180 градусов азимут движения судна
    }
    calcStreamerDepth();

}

// void Streamer::calcIFConnected()
// {
//     NmeaParser parser;
//     double azRad = -1;

//     Streamer* strm = this;
//     realAzimuthOfTowingRadians = M_PI - atan2(towingPoint->x_coor - endBuoy->x_coor,
//                                                     towingPoint->y_coor - endBuoy->y_coor);

//     QGeoCoordinate towingGeoPos = parser.UTMtoGeo(QPointF(towingPoint->x_coor,strm->towingPoint->y_coor));
//     QGeoCoordinate endBuoyGeoPos = parser.UTMtoGeo(QPointF(endBuoy->x_coor,strm->endBuoy->y_coor));

//     // qDebug() << towingGeoPos << endBuoyGeoPos;
//     // qDebug() << qDegreesToRadians(towingGeoPos.azimuthTo(endBuoyGeoPos)) << strm->realAzimuthOfTowingRadians <<__FUNCTION__;
//     realAzimuthOfTowingRadians = qDegreesToRadians(towingGeoPos.azimuthTo(endBuoyGeoPos));




//     if (realAzimuthOfTowingRadians < 0) {
//         realAzimuthOfTowingRadians += 2*M_PI;
//     } else if (realAzimuthOfTowingRadians >= 2 * M_PI) {
//         realAzimuthOfTowingRadians -= 2 * M_PI;
//     }

//     realAzimuthOfTowingRadians = realAzimuthOfTowingRadians;
//     azRad = realAzimuthOfTowingRadians;
//     calcChansCoors();
//     return;
// }

// void Streamer::calcIFNotConnected()
// {
//     //calculating coors for streamer without endBuoy Position
//     double fakeAzOfTowing = (sharedCircularBuffer.calculateAzimuth());
//     if (fakeAzOfTowing > 2*M_PI) fakeAzOfTowing -=2*M_PI;
//     if (fakeAzOfTowing < 0) fakeAzOfTowing +=2*M_PI;

//     x_coor = towingPoint->x_coor + (x-towingPoint->x)*qSin(fakeAzOfTowing) + (y-towingPoint->y)*qCos(fakeAzOfTowing);
//     y_coor = towingPoint->y_coor + (x-towingPoint->x)*qCos(fakeAzOfTowing) - (y-towingPoint->y)*qSin(fakeAzOfTowing);

//     qDebug() << qRadiansToDegrees(fakeAzOfTowing)<< qRadiansToDegrees(sharedCircularBuffer.calculateAzimuth()) << __LINE__;
//     this->showAzDistToObj(towingPoint);

//     return;
// }

void Streamer::printChansCoor() //функция для дебага. выводит координаты ка
{
    int i = 0;
    // QDebug deb = qDebug();
    for (Channel* ch: ChannelsVector) {
        qDebug() <<qSetRealNumberPrecision(10)<< "Chan "
            <<i<<"X "<<ch->x_coor<<"Y "<<ch->y_coor<<"Depth"<<ch->depth;
        i++;
    }
}

void Streamer::printPos()
{
    logmsg(name + "\nHeight " + QString::number(height) +
           "\nUTM East:" + QString::number(x_coor) +
           "\nUTM North:" + QString::number(y_coor) +
           "\nGeo East:" + QString::number(this->longitude) +
           "\nGeo Notrh:" + QString::number(this->latitude) +
           "Azimuth of movement" + QString::number(azimuthOfMovement));
    printChansCoor();
}


///TODO
int Streamer::checkStreamerCoordinates()
{
    return 666;
}

Streamer::Channel* Streamer::getChan(uint number)
{
    if (number < 1){
        qDebug() << number<<" is out of channels range";
        return nullptr;
    } else if(number > ChannelsVector.size()) {
        return ChannelsVector.at(ChannelsVector.size() - 1);
    } else {
        return ChannelsVector.at(number-1);
    }
}

uint Streamer::getChanCount()
{
    return this->NumChanels;
}

QVector<double> Streamer::getChans() const
{
    return chans;
}

void Streamer::setTotalLength(double newTotalLength)
{
    totalLength = newTotalLength;
}

void Streamer::setEndBuoy(Buoy *newEndBuoy)
{
    endBuoy = newEndBuoy;
}

double Streamer::getTotalLength()
{
    return totalLength;
}

void Streamer::calcStreamerDepth()
{
    if (QString(towingPoint->metaObject()->className()) == "Buoy") {
        Buoy* leadBuoy = dynamic_cast<Buoy*>(towingPoint);
        double leadEndDepthDif = 0;
        if (this->endBuoy != nullptr) {
            leadEndDepthDif = leadBuoy->towingDepth - this->endBuoy->towingDepth;
        } else {
            leadEndDepthDif = 0;
        }
        for (int i = 0; i < NumChanels; i++) {
            ChannelsVector[i]->depth = leadBuoy->towingDepth + leadEndDepthDif*(chans[i]/realLen);
        }
    } else {
        if (endBuoy != nullptr) {
            for (int i = 0; i < NumChanels; i++) {
                ChannelsVector[i]->depth = endBuoy->towingDepth;
            }
        } else {
            for (int i = 0; i < NumChanels; i++) {
                ChannelsVector[i]->depth = defaultChannelDepth;
            }
            // qDebug() << "Streamer hasn't depth information";
        }
    }
}

double Streamer::getDistanceCalcCoef() const
{
    return distanceCalcCoef;
}

Streamer::Channel::Channel(uint myNumber)
{
    this->myNumber = myNumber;
}



QString Streamer::Channel::getUTMPos()
{
    // QLocale locale(QLocale::C);
    QString res(22, ' ');
    res.replace(4, 9,floatToQString(x_coor, 9,1));
    res.replace(13, 8,floatToQString(y_coor, 9,1));
    // qDebug() <<locale.toString(y_coor, 'f', 10)<<doubleToQString(y_coor, 10,2) << __FUNCTION__;

    res.replace(22, 4,floatToQString(depth, 4, 1));

    for (int i = 0; i < 4; i++) {
        res[i] = floatToQString(myNumber, 4, -1)[i];
    }
    // qDebug() << res;
    return res;
}







