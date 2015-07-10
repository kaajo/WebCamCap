#ifndef POLYGONCAMERATOPOLOGY_H
#define POLYGONCAMERATOPOLOGY_H

#include "icameratopology.h"

class PolygonCameraTopology : public ICameraTopology
{
public:
    PolygonCameraTopology(QObject *parent = 0);

    virtual QVariantMap toVariantMap() override;
    virtual void fromVariantMap(QVariantMap varMap) override;

    virtual void addCameras(QVector<ICamera *> cameras) override;
    virtual void removeCamera(ICamera *camera) override;

    virtual void record(bool start) override;

private:
    void resolveEdges();

    void nextFrame();

};

#endif // POLYGONCAMERATOPOLOGY_H
