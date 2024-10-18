#include "streamer.h"
#include "functions.h"

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
        for (int i = 0; i < NumChannels; i++) {
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
    double azRad = azimuthOfMovement*M_PI/180;

    if (endBuoy != nullptr) {

        realLen = pow(((x_coor - getChan(getChanCount()-1)->x_coor)*(x_coor - getChan(getChanCount()-1)->x_coor) +
                       (y_coor - getChan(getChanCount()-1)->y_coor)*(y_coor - getChan(getChanCount()-1)->y_coor)), 0.5) - endBuoy->wireLength; //real length of streamer
        heightDifference = (endBuoy->height - endBuoy->AnthenaHeight - endBuoy->towingDepth) -  height; //difference of height
        //of streamer begin and streamer end
        dh = heightDifference/realLen;

        distanceCalcCoef = totalLength >= realLen? 1.0: realLen/totalLength;
        ChannelsVector[0]->x_coor = this->x_coor;
        ChannelsVector[0]->y_coor = this->y_coor;

        // qDebug() << x_coor << y_coor << towingPoint->x_coor << towingPoint->y_coor << __FUNCTION__;

        for(int i = 1; i < ChannelsVector.size(); i++) {
            ChannelsVector[i]->height = this->height + chans[i]* dh;
            ChannelsVector[i]->x_coor = x_coor + (1)*(chans[i]) *qSin(realAzimuthOfTowingRadians);
            ChannelsVector[i]->y_coor = y_coor + (1)*(chans[i]) *qCos(realAzimuthOfTowingRadians);
        }
    }
    calcStreamerDepth();

}

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
        double leadEndDepthDif = leadBuoy->towingDepth - this->endBuoy->towingDepth;
        for (int i = 0; i < NumChanels; i++) {
            ChannelsVector[i]->depth = leadBuoy->towingDepth + leadEndDepthDif*(chans[i]/realLen);
        }
    } else {
        for (int i = 0; i < NumChanels; i++) {
            ChannelsVector[i]->depth = endBuoy->towingDepth;
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







