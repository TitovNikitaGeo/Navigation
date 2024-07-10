#ifndef FIXEDITEM_H
#define FIXEDITEM_H

#include <QString>
#include <QDebug>
#include <QVector>
#include <QDebug>
#include <QObject>
#include <QScopedPointer>

#include "nmeaparser.h"
#include "connection.h"

class FixedItem : public QObject
{
Q_OBJECT
private:
    friend class Fabric;
    friend class Coordinator;
    FixedItem(float x,float y,float z, QString name);

public:
    FixedItem();
    ~FixedItem();

    ///for scheme
    float x;
    float y;
    float z;
    ///for scheme

    /// for real coordinates
    float x_coor;
    float y_coor;
    float x_proj;
    float y_proj;
    float height;
    double azimuthOfMovement;
    void calcItemCoordinates();
    NmeaParser::NmeaGGAData lastGGAData = {{0,0}, {0,0}, QDateTime(),0};
    NmeaParser::NmeaRMCData lastRMCData = {0,0};


    void printPos();
    /// for real coordinates

    QString name;
    QString itemType;
    int number; // not used

    ///funcs for working with connected items
    void deletingWires();
    void lostWire();

    ///funcs for working with connections
    QString getLastGGA();
    QString getLastRMC();

    bool hasConnection = false;

    Connection* connection;
    NmeaParser parser;
    QVector<FixedItem*> vectorOfConnected;

public slots:

    void newNmeaArived(QString msg);
protected:
    FixedItem* ItemForCalculations;
};

#endif // FIXEDITEM_H
