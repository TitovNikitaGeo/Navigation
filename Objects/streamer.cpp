#include "streamer.h"

Streamer::Streamer(QObject *parent)
{}

Streamer::Streamer(QString Name, FixedItem *towingPoint,
                   float angleToWired, float wireLength, uint NumChanels, float dCh)
    : TowedItem(Name, towingPoint, angleToWired, wireLength)
{
    // this->name = Name;
    // this->towingPoint = towingPoint;
    // this->angle = angleToWired;
    // this->wireLength = wireLength;
    this->NumChanels = NumChanels;
    this->dCh = dCh;
    itemType = "Streamer";


    for (uint i = 1; i <= NumChanels; i++) {
        Channel* chan = new Channel(i);
        ChannelsVector.append(chan);
    }
    calcChansCoors();
    printSelfInfo();
}

void Streamer::calcChansCoors()
{
    int i = 0;
    for (Channel* ch: ChannelsVector) {
        ch->x_coor = this->x_coor + wireLength*sin(angle)*i;
        ch->y_coor = this->y_coor + wireLength*sin(angle)*i;
        ch->height = this->height -0.1*i;
        i++;
    }
    ///depth calculation
    // for (Channel* ch: ChannelsVector) {
    //     ch->depth = this->depth + ((endBuoy->towingPointDepth - this->depth) / NumChanels)*i;
    //     i++;
    // }
    ///add depth sensorss
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
        return ChannelsVector.at(number);
    }
}

uint Streamer::getChanCount()
{
    return this->NumChanels;
}





Streamer::Channel::Channel(uint myNumber)
{
    this->myNumber = myNumber;
}
