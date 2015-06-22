#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include <opencv2/opencv.hpp>

#include <QVector3D>
#include <QVector>
#include <QObject>

class CameraSettings : public QObject
{
    Q_OBJECT

    typedef QVector<cv::Point> Contour;

    //BASIC PARAMETERS
    QString m_name = "Camera";
    int m_videoUsbId = -1;
    float m_diagonalFov = 0.0f;
    QVector3D m_globalPosition;
    QVector3D m_roomDimensions;

    bool m_turnedOn = false;
    bool useBackgroundSub = false;
    bool m_showWindow = true;
    bool ROI = false;
    cv::Mat ROIMask;
    size_t m_thresholdValue = 255;

    cv::Mat m_rotationMatrix;
    cv::Mat m_distortionCoeffs;
    cv::Mat m_projectionMatrix;
    cv::Mat m_CameraMatrix;
    cv::Mat m_IntrinsicMatrix;

    cv::Scalar contourColor = cv::Scalar(0, 0, 255);

    QVector<QVector<QVector3D>> m_pixelLines;

public:
    explicit CameraSettings(QString m_name, int m_videoUsbId , float m_diagonalFov , QVector3D m_globalPosition , QVector3D m_roomDimensions, QObject *parent = 0);

signals:
    void changed();

public slots:

private:
    void computeParameters();

};

#endif // CAMERASETTINGS_H
