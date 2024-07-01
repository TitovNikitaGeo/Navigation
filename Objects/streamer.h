#ifndef STREAMER_H
#define STREAMER_H

#include "toweditem.h"
#include "fixeditem.h"
#include "buoy.h"

#include <QObject>

class Streamer : public TowedItem
{
    Q_OBJECT
    class Channel;
private:
    friend class Fabric;
    Streamer(QString name, FixedItem *towingPoint, float angleToWired, float wireLength, uint NumChannels, QVector<float> chans);


public:
    explicit Streamer(QObject *parent = nullptr);
    void calcChansCoors();

    void printChansCoor();
    void printPos();
    int checkStreamerCoordinates();

    Channel* getChan(uint number);
    uint getChanCount();
    QVector<float> getChans() const;

    void setTotalLength(float newTotalLength);

private:
    uint NumChanels;
    QVector<float> chans;
    QVector<Channel*> ChannelsVector;
    float totalLength;

    float endDepth;
    Buoy* endBuoy;

    float l;
    float h;
    float dh;


    ///inner class for channels
    class Channel : public TowedItem
    {
        // Q_OBJECT
    public:
        Channel(uint myNumber);

        QString getUTMPos();


    private:
        uint myNumber;
    };



};



#endif // STREAMER_H
