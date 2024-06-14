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

    ///add depth sensors
}





Streamer::Channel::Channel(uint myNumber)
{
    this->myNumber = myNumber;
}
