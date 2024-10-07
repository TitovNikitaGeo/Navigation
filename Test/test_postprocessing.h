#ifndef TEST_POSTPROCESSING_H
#define TEST_POSTPROCESSING_H

#include "postprocessor.h"
#include "fabric.h"
#include "itemsstorage.h"
#include "segyreader.h"

#include <QTime>
#include <QThread>

class testPostprocessing
{
public:
    testPostprocessing();
    int testSearchingNmea23();
    int testSearchingNmea23WithOnboard();
    int testSearchingNmea22();
    QVector<FixedItem*> createItems2309();
    QVector<FixedItem*> createItems2309WithOnboard();
    QVector<FixedItem*> createItems2209();

    QVector<FixedItem*> testItems;
    QVector<SegYReader::Pair>readFileAndGeneratePairs(const QString& fileName);
};

#endif // TEST_POSTPROCESSING_H
