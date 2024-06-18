#include "streamer.h"

Streamer::Streamer(QObject *parent)
    : QObject{parent}
{}

Streamer::Streamer(QString Name, FixedItem *towingPoint,
    float angleToWired, float wireLength, uint NumChanels, float dCh)
{
    this->name = Name;
    this->towingPoint = towingPoint;
    this->angle = angleToWired;
    this->wireLength = wireLength;
    this->NumChanels = NumChanels;
    this->dCh = dCh;
    for (uint i = 1; i <= NumChanels; i++) {
        Channel* chan = new Channel(i);
        ChannelsVector.append(chan);
    }
}

void Streamer::calcChansCoors()
{
    int i = 0;
    for (Channel* ch: ChannelsVector) {
        ch->x_coor = this->x_coor + wireLength*sin(angle)*i;
        ch->y_coor = this->y_coor + wireLength*sin(angle)*i;
        i++;
    }
    ///depth calculation
    // for (Channel* ch: ChannelsVector) {
    //     ch->depth = this->depth + ((endBuoy->towingPointDepth - this->depth) / NumChanels)*i;
    //     i++;
    // }
    ///add depth sensorss
}

void Streamer::printChanCoor() //функция для дебага. выводит координаты ка
{
    int i = 0;
    for (Channel* ch: ChannelsVector) {
        qDebug() << "Chan "<<i<<"X "<<ch->x_coor<<"Y "<<ch->y_coor<<"Depth"<<ch->depth;
        i++;
    }
}


///TODO
int Streamer::checkStreamerCoordinates()
{
    return 666;
}





Streamer::Channel::Channel(uint myNumber)
{
    this->myNumber = myNumber;
}
