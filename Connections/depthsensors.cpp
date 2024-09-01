#include "depthsensors.h"

DepthSensors::DepthSensors() {}

DepthSensors::~DepthSensors()
{

}

void DepthSensors::createCalculationScheme()
{
    nS = new QVector<nearestSensors>;
    if (isLeadBuoy) {
        sensorsDepth.prepend(leadBuoyDepth);
        sensorsPos.prepend(0);
    }
    if (isEndBuoy) {
        sensorsDepth.append(leadBuoyDepth);
        sensorsPos.append(0);
    }
    for (int i = 0; i < chans.size(); i++) {
        nearestSensors a;
        if (chans[i])
    }

}

void DepthSensors::calcDepth()
{

}

void DepthSensors::setChans(const QVector<float> &newChans)
{
    chans = newChans;
}

QVector<float> DepthSensors::getChanDepth() const
{
    return chanDepth;
}
