#ifndef FIXEDITEM_H
#define FIXEDITEM_H

#include <QString>
#include <QDebug>
#include <QVector>
#include <QDebug>
#include <QObject>

#include "nmeaparser.h"
#include "connection.h"

class FixedItem
{

private:
    friend class Fabric;
    FixedItem(float x,float y,float z, QString name);

public:
    FixedItem();
    ~FixedItem();

    void set_coor();
    void set_x(float x);
    void set_y(float y);
    void set_z(float z);

    ///for scheme
    float x;
    float y;
    float z;
    ///for scheme

    /// for real coordinates
    float x_coor;
    float y_coor;
    float depth;
    /// for real coordinates

    QString name;
    QString itemType;
    int number; // not used

    ///funcs for working with connected items
    void deletingWires();
    void lostWire();

    ///funcs for working with connections
    QString getLastNmeaStr();
    bool hasConnection = false;

    Connection* connection = nullptr;
    NmeaParser parser;
    QVector<FixedItem*> vectorOfConnected;

public slots:

    void newNmeaArived(QString msg);
};

#endif // FIXEDITEM_H
