#ifndef STREAMER_H
#define STREAMER_H

#include "toweditem.h"
#include "fixeditem.h"
// #include "buoy.h"
#include <QObject>

class Streamer : public QObject, public TowedItem
{
    Q_OBJECT
    class Channel;
private:
    friend class Fabric;
    Streamer(QString name, FixedItem *towingPoint, float angleToWired, float wireLength, uint NumChannels, float dCh);


public:
    explicit Streamer(QObject *parent = nullptr);
    void calcChansCoors();

    void printChanCoor();
    int checkStreamerCoordinates();

private:
    uint NumChanels;
    float dCh;
    QVector<Channel*> ChannelsVector;


    float endDepth;

    // Buoy* endBuoy;

    ///inner class for channels
    class Channel : public QObject, public TowedItem
    {
        // Q_OBJECT
    public:
        Channel(uint myNumber);

        void setCoordinates(float x, float y, float z);


    private:
        uint myNumber;
    };



};



#endif // STREAMER_H
