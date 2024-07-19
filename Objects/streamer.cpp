#include "streamer.h"
#include "functions.h"

Streamer::Streamer(QObject *parent)
{}

Streamer::Streamer(QString Name, FixedItem *towingPoint,
                   float angleToWired, float wireLength, uint NumChanels, QVector<float> chans)
    : TowedItem(Name, towingPoint, angleToWired, wireLength)
{
    this->NumChanels = NumChanels;
    this->chans = chans;
    itemType = "Streamer";

    if (NumChanels == 0 || chans.empty()) {
        qDebug() << "Wrong streaner configuration";
        return;
    }

    for (uint i = 1; i <= NumChanels; i++) {
        Channel* chan = new Channel(i);
        ChannelsVector.append(chan);
    }
    // calcChansCoors();
    // printSelfInfo();
}

void Streamer::calcChansCoors()
{
    float azRad = azimuthOfMovement*M_PI/180;
    if (endBuoy != nullptr) {

        realLen = pow(((x_coor - endBuoy->x_coor)*(x_coor - endBuoy->x_coor) +
                 (y_coor - endBuoy->y_coor)*(y_coor - endBuoy->y_coor)), 0.5); //real length of streamer
        heightDifference = (endBuoy->height - endBuoy->AnthenaHeight - endBuoy->towingDepth) -  height; //difference of height
        //of streamer begin and streamer end
        dh = heightDifference/realLen;

        float realAz = qAtan((x_coor - endBuoy->x_coor)/(y_coor - endBuoy->y_coor)); //азимут от начала косы до буя
        float distanceCalcCoef = realLen >= totalLength? 1.0: realLen/endBuoy->wireLength;
        for(int i = 0; i < ChannelsVector.size(); i++) {
            ChannelsVector[i]->height = this->height + chans[i]* dh;
            ChannelsVector[i]->x_coor = this->x_coor - (distanceCalcCoef)*chans[i]*qSin(realAz);
            ChannelsVector[i]->y_coor = this->y_coor - (distanceCalcCoef)*chans[i]*qCos(realAz);
        }
    } else {
        for(int i = 0; i < ChannelsVector.size(); i++) {
            ChannelsVector[i]->x_coor = this->x_coor - chans[i]*qSin(azRad);
            ChannelsVector[i]->y_coor = this->y_coor - chans[i]*qCos(azRad);
            ChannelsVector[i]->height = height;
        }

    }

}

void Streamer::printChansCoor() //функция для дебага. выводит координаты ка
{
    int i = 0;
    // QDebug deb = qDebug();
    for (Channel* ch: ChannelsVector) {
        qDebug() <<qSetRealNumberPrecision(10)<< "Chan "<<i<<"X "<<ch->x_coor<<"Y "<<ch->y_coor<<"Depth"<<ch->height;
        i++;
    }
}

void Streamer::printPos()
{
    qDebug() <<"-----------------------";
    qDebug() <<name;
    qDebug() << "Высота:" << qSetRealNumberPrecision(3) << height  ;
    // QPointF utmCoordinates = GeoToUTM(data.coordinate);
    qDebug() << "UTM Восток:" << qSetRealNumberPrecision(10)<< x_coor ;
    qDebug() << "UTM Север:" << qSetRealNumberPrecision(10) << y_coor;
    // qDebug() << "Высота над уровнем моря"<< data.height;
    qDebug() << "Азимут движения" << azimuthOfMovement;
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
    } else if(number > NumChanels) {
        return ChannelsVector.at(ChannelsVector.size() - 1);
    } else {
        return ChannelsVector.at(number-1);
    }
}

uint Streamer::getChanCount()
{
    return this->NumChanels;
}

QVector<float> Streamer::getChans() const
{
    return chans;
}

void Streamer::setTotalLength(float newTotalLength)
{
    totalLength = newTotalLength;
}

void Streamer::setEndBuoy(Buoy *newEndBuoy)
{
    endBuoy = newEndBuoy;
}

float Streamer::getTotalLength() const
{
    return totalLength;
}

Streamer::Channel::Channel(uint myNumber)
{
    this->myNumber = myNumber;
}



QString Streamer::Channel::getUTMPos()
{
    QString res(29, ' ');
    // res +=
    res.replace(4, 10,floatToQString(x_coor, 10,2));
    res.replace(14, 10,floatToQString(y_coor, 10,2));
    res.replace(24, 5,floatToQString(height, 5,2));

    for (int i = 0; i < 4; i++) {
        res[i] = floatToQString(myNumber, 4, 0)[i];
        // res[i] = '~';
    }
    // res.remove(' ');
    // qDebug() << res.replace(0, 4,floatToQString(myNumber, 4, 0));
    // QString res;
    // res += floatToQString(myNumber, 2, 0) + " " + floatToQString(x_coor, 7,1)
           // + floatToQString(y_coor, 7,1) + floatToQString(height, 5,2);
    // qDebug() << res;
    return res;
}







