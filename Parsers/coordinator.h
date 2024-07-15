#ifndef COORDINATOR_H
#define COORDINATOR_H

#include "itemsstorage.h"

#include <QObject>

class Coordinator : public QObject
{
    Q_OBJECT
public:
    explicit Coordinator(QObject *parent = nullptr);
    int calcCoors();
    int printCoors();
    ItemsStorage* Vault;
    bool wireFixedItems();

signals:

public slots:
    double commonDummyAzimuth; //if end buoy doesn't send az
    void boardDepthChanged(double boardHeight);
};

#endif // COORDINATOR_H
