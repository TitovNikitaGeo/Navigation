#ifndef FABRIC_H
#define FABRIC_H

#include <QString>
#include <QObject>

#include "fixeditem.h"
#include "toweditem.h"
#include "streamer.h"
#include "source.h"
#include "connection_creator.h"

#include "streamerdialog.h"
#include "buoydialog.h"

#include "structures.h"

class Fabric : public QObject
{
// Q_OBJECT

public:
    Fabric();


    FixedItem* CreateItem(FixedItemInfo info, bool needConnect);
    TowedItem* CreateItem(TowedItemInfo info, bool needConnect);
    Streamer* CreateItem(StreamerInfo info);
    Buoy* CreateItem(BuoyInfo info, bool needConnection);
    Source* CreateItem(SourceInfo info, bool needConnect);

    Connection* createConnection(QString itemName);


    int bindItemConnection(FixedItem* to, Connection* who);


    void setMyVault(ItemsStorage* MyVault);

    Connection_creator* connectionCreator;
private:
    ItemsStorage* MyVault;

private slots:
    void connectionCreated();
    void onRevieveStreamerData(const QString &data);
};

#endif // FABRIC_H
