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
    friend class ItemsLoader;
    Streamer(QString name, FixedItem *towingPoint, double angleToWired, double wireLength, uint NumChannels, QVector<double> chans);
    Streamer(QString name, FixedItem *towingPoint, double angleToWired, double wireLength, uint NumChannels, QString chanStep);

public:

    explicit Streamer(QObject *parent = nullptr);
    void calcChansCoors();
    // void calcIFConnected();
    // void calcIFNotConnected();
    void printChansCoor();
    void printPos();
    int checkStreamerCoordinates();

    Channel* getChan(uint number);
    uint getChanCount();
    QVector<double> getChans() const;

    void setTotalLength(double newTotalLength);

    void setEndBuoy(Buoy *newEndBuoy);

    double getTotalLength();

    double depth;
    void calcStreamerDepth();
    double totalLength;
    Buoy* endBuoy = nullptr;
    double getDistanceCalcCoef() const;

    QString channelStep = "1";
    static double defaultChannelDepth;
private:
    uint NumChanels;


    QVector<double> chans;
    QVector<Channel*> ChannelsVector;


    double endDepth;

    double realLen;
    double heightDifference;
    double dh;

    double distanceCalcCoef;



    ///inner class for channels
    class Channel : public TowedItem
    {
        // Q_OBJECT
    public:
        Channel(uint myNumber);

        QString getUTMPos();
        double depth;

    private:
        uint myNumber;
    };



};



#endif // STREAMER_H
