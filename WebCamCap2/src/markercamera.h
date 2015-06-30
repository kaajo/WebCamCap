#ifndef MARKERCAMERA_H

#define MARKERCAMERA_H

#include "icamera.h"

#include <QVector2D>
#include <opencv2/core.hpp>

typedef std::vector<cv::Point> Contour;

class MarkerCamera : public ICamera
{
    Q_OBJECT

    cv::VideoCapture m_camera;
    cv::UMat m_actualFrame, m_filteredFrame;

    size_t m_thresholdValue = 255;
    cv::UMat m_background;

    QVector<Line> m_lines;


    std::vector<Contour> m_contours;

public:
    explicit MarkerCamera(CameraSettings *settings, QObject *parent = 0);

    virtual QVector<Line> nextFrame() override;
    virtual QVector<QVector3D> nextFrame2D() override;
    virtual void calibrate(CameraCalibrationType type) override;

    virtual void showPreviewInWindow(bool show) override;
    virtual bool turnOn(bool turnOn) override;
    virtual void setThreshold(size_t threshold) override;

    virtual QVariantMap toVariantMap() const override;
    virtual void fromVariantMap(QVariantMap varMap) override;
private:
    virtual void settingsChanged(CameraSettings::CameraSettingsType type) override;

    void calibBackground();
    void calibThreshold();

    ///computation

    void applyFilters();
    void createLineForComponents();

    inline static Line qtConcurrentpickLine(MarkerCamera *camera, const Contour &contour);

};

#endif // MARKERCAMERA_H
