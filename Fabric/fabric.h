#ifndef FABRIC_H
#define FABRIC_H

#include <QString>
#include <QObject>

#include "fixeditem.h"
#include "toweditem.h"
#include "streamer.h"
#include "connection_creator.h"

#include "streamerdialog.h"
#include "buoydialog.h"

#include "structures.h"

class Fabric : public QObject
{
// Q_OBJECT

public:
    Fabric();

    FixedItem* CreateFixedItem(FixedItemInfo info);
    TowedItem* CreateTowedItem(TowedItemInfo info);
    Buoy* CreateBuoyItem(BuoyInfo info);

    FixedItem* CreateItem(FixedItemInfo info, bool needConnect);
    TowedItem* CreateItem(TowedItemInfo info, bool needConnect);
    Streamer* CreateItem(StreamerInfo info);
    Buoy* CreateItem(BuoyInfo info);

    Connection* createConnection();


    int bindItemConnection(FixedItem* to, Connection* who);


    void setMyVault(ItemsStorage* MyVault);

private:
    Connection_creator* connectionCreator;
    ItemsStorage* MyVault;

private slots:
    void connectionCreated();
    void onRevieveStreamerData(const QString &data);
};

#endif // FABRIC_H
