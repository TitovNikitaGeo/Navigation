#ifndef TEST_POSTPROCESSING_H
#define TEST_POSTPROCESSING_H

#include "postprocessor.h"
#include "fabric.h"
#include <QTime>

class testPostprocessing
{
public:
    testPostprocessing();
    int testSearchingNmea();
    QVector<FixedItem*> createItems();

    QVector<FixedItem*> testItems;
};

#endif // TEST_POSTPROCESSING_H
