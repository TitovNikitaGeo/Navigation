#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>
#include "toweditem.h"

class Source : public TowedItem
{
    friend class Fabric;
    Q_OBJECT
public:
    Source();
    Source(QString Name,
           FixedItem* towingPoint, float angle = 0, float wireLength = 5);
};

#endif // SOURCE_H
