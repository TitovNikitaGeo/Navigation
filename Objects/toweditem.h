#ifndef TOWEDITEM_H
#define TOWEDITEM_H

#include "fixeditem.h"
#include <QtMath>

class TowedItem : public FixedItem
{
Q_OBJECT
private:
    friend class Fabric;
    friend class Streamer;
    friend class Buoy;
    friend class Source;
    TowedItem(QString name, FixedItem* towingPoint,
              float angleToWired, float wireLength);
public:
    TowedItem();
    ~TowedItem();


    // void calcItemCoordinates();
    void calcIFNotConnected();

    void printSelfInfo();

    FixedItem* towingPoint;
    float angle; //Angle to ship Dir
    float wireLength;


    ///funcs for working with connections
    QString getLastGGA();
    QString getLastRMC();

    QVector<FixedItem*> vectorOfConnections; // not used for now
    // QString name;
    // QString itemType;
    void setBoardHeight(float newBoardHeight);

    void printPos();


protected:
    float boardHeight = 0;


};

#endif // TOWEDITEM_H
