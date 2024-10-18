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
    friend class ItemsLoader;
    TowedItem(QString name, FixedItem* towingPoint,
              double angleToWired, double wireLength);
public:
    TowedItem();
    ~TowedItem();


    // void calcItemCoordinates();
    void calcIFNotConnected();

    void printSelfInfo();

    FixedItem* towingPoint;
    double angle; //Angle to ship Dir
    double wireLength;


    ///funcs for working with connections
    QString getLastGGA();
    QString getLastRMC();

    QVector<FixedItem*> vectorOfConnections; // not used for now
    // QString name;
    // QString itemType;
    void setBoardHeight(double newBoardHeight);

    void printPos();


protected:
    double boardHeight = 0;


};

#endif // TOWEDITEM_H
