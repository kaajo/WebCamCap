#ifndef POLYGONCAMERATOPOLOGY_H
#define POLYGONCAMERATOPOLOGY_H

#include "icameratopology.h"

#include "pointchecker.h"

#include <QWaitCondition>
#include <QTime>

class TopologyEdge
{
public:
    TopologyEdge(){}
    TopologyEdge(ICamera *cam1, ICamera *cam2)
    {
        m_camera1 = cam1;
        m_camera2 = cam2;
    }

    ICamera *m_camera1 = nullptr;
    ICamera *m_camera2 = nullptr;
};

class PolygonCameraTopology : public ICameraTopology
{
    Q_OBJECT

    size_t m_turnedOnCamerasCounter = 0;
    size_t m_resultsCounter = 0;

    QVector<QThread*> m_cameraThreads;
    QVector<TopologyEdge> m_topology;

    static QMap<ICamera*, QVector<Line>> m_resultLines;

    QWaitCondition * m_waitCondition = nullptr;

    QTime m_frameTimer;

    PointChecker m_pointChecker;

public:

    PolygonCameraTopology(QVector3D roomDims, double maxError, QObject *parent = 0);

    virtual QVariantMap toVariantMap() override;
    virtual void fromVariantMap(QVariantMap varMap) override;

    virtual void addCameras(QVector<ICamera *> cameras) override;
    virtual void removeCamera(ICamera *camera) override;

    virtual void record(bool start) override;

    static QVector<QVector3D> intersection(TopologyEdge edge);

signals:
    void startRecording();
    void stopRecording();
    void startRecording2D();

private:
    void resolveEdges();

    void intersections();

    void normaliseCoords(QVector<Marker> &points, QVector3D roomSize);

private slots:
    void handleCameraSettingsChange(CameraSettings::CameraSettingsType type);
    void handleCameraResults(QVector<Line> lines);

};

#endif // POLYGONCAMERATOPOLOGY_H
