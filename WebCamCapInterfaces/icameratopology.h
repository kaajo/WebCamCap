#ifndef ICAMERATOPOLOGY_H
#define ICAMERATOPOLOGY_H

#include "webcamcapinterfaces_global.h"

#include "frame.h"
#include "icamera.h"

#include <QString>
#include <QVector>
#include <QVariantMap>
#include <QObject>

class WEBCAMCAPINTERFACESSHARED_EXPORT ICameraTopology : public QObject
{
    Q_OBJECT

protected:
    class TopologyEdge
    {
    public:
        ICamera *m_camera1 = nullptr;
        ICamera *m_camera2 = nullptr;
    };

    QVector<ICamera*> m_cameras;
    QVector<TopologyEdge> m_topology;

public:
    explicit ICameraTopology(QObject *parent = 0);
    virtual ~ICameraTopology();

    virtual void addCamera(ICamera *camera) = 0;
    virtual void removeCamera(ICamera *camera) = 0;

    ICamera *getCamera(QString name);

    virtual QVariantMap toVariantMap() = 0;
    virtual void fromVariantMap(QVariantMap varMap) = 0;
signals:
    void frameReady(Frame frame);

public slots:
    virtual void record(bool start) = 0;

};

#endif // ICAMERATOPOLOGY_H
