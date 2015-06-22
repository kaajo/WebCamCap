#ifndef ICAMERA_H
#define ICAMERA_H

#include "line.h"
#include "camerasettings.h"

#include <opencv2/core/core.hpp>

#include <QVector>
#include <QVariantMap>
#include <QObject>

class ICamera : public QObject
{
    Q_OBJECT

protected:
    CameraSettings *m_settings;

public:
    explicit ICamera(QObject *parent = 0);
    virtual ~ICamera();

    enum class CameraCalibrationType {
        BACKGROUND,
        MARKERS
    };

    virtual QVector<Line> nextFrame() = 0;
    virtual QVector<QVector3D> nextFrame2D() = 0;

    virtual QVariantMap toVariantMap() = 0;
    virtual void fromVariantMap(QVariantMap varMap) = 0;

    CameraSettings *settings() const;
    void setSettings(CameraSettings *settings);

signals:
    void imageShow(cv::Mat image);

public slots:
    virtual void showPreviewInWindow(bool show) = 0;
    virtual void turnOn(bool turnOn) = 0;
    virtual void setThreshold(size_t threshold) = 0;

private slots:
    virtual void settingsChanged() = 0;
};

#endif // ICAMERA_H
