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
#include "logger.h"
#include "CircularBuffer.h"


class FixedItem : public QObject
{
Q_OBJECT
private:
    friend class Fabric;
    friend class Coordinator;
    FixedItem(double x,double y,double z, QString name);
    void calcIfConnected();
    virtual void calcIFNotConnected();

public:
    FixedItem();
    ~FixedItem();

    void calcItemCoordinates();
    ///for scheme
    double x;
    double y;
    double z;
    ///for scheme

    /// for real coordinates
    double x_coor;
    double y_coor;
    double latitude;
    double longitude;
    double height;
    double azimuthOfMovement;
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

    void setItemForCalculations(FixedItem *newItemForCalculations);
    static double realAzimuthOfTowingRadians;


    bool showAzDistToObj(FixedItem* to);
public slots:

    void newNmeaArived(QString msg);
protected:
    bool amIItemForCalculating = false;
    FixedItem* ItemForCalculations;
    static CircularBuffer sharedCircularBuffer;
};




#endif // FIXEDITEM_H
