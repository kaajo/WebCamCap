#ifndef CAMERASETTINGS_H
#define CAMERASETTINGS_H

#include "webcamcapprimitives_global.h"

#include <opencv2/opencv.hpp>

#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <QObject>
#include <QVariantMap>

class WEBCAMCAPPRIMITIVESSHARED_EXPORT CameraSettings : public QObject
{
    Q_OBJECT

    typedef QVector<cv::Point> Contour;

    //BASIC PARAMETERS
    QString m_name = "Camera";
    int m_videoUsbId = -1;
    float m_diagonalFov = 0.0f;
    QVector3D m_globalPosition;
    QVector2D m_resolution;
    cv::UMat m_distortionCoeffs;
    QVector3D m_roomDimensions;

    bool m_turnedOn = false;
    bool useBackgroundSub = false;
    bool m_showWindow = true;
    bool ROI = false;
    bool m_saved = false;
    cv::Mat ROIMask;

    cv::Mat m_rotationMatrix;
    cv::Mat m_projectionMatrix;
    cv::Mat m_CameraMatrix;
    cv::Mat m_IntrinsicMatrix;

    QVector<QVector<QVector3D>> m_pixelLines;

public:

    cv::Scalar contourColor = cv::Scalar(0, 0, 255);
    cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3));

    enum class CameraSettingsType{
        NAME,
        VIDEOUSBID,
        FOV,
        RESOLUTION,
        POSITION,
        DISTORTION,
        TURNEDON,
        USEBACKGROUNDSUBSTRACTOR,
        SHOWIMAGEGUI,
        ROI,
        ROIMASK
    };

    explicit CameraSettings(QString m_name, int m_videoUsbId , float m_diagonalFov , QVector3D m_globalPosition , QVector3D m_roomDimensions, QObject *parent = 0);

    QVariantMap toVariantMap();
    void fromVariantMap(QVariantMap map);

    QString name() const;
    void setName(const QString &name);

    int videoUsbId() const;
    void setVideoUsbId(int videoUsbId);

    float diagonalFov() const;
    void setDiagonalFov(float diagonalFov);

    QVector3D globalPosition() const;
    void setGlobalPosition(const QVector3D &globalPosition);

    cv::UMat distortionCoeffs() const;
    void setDistortionCoeffs(const cv::UMat &distortionCoeffs);

    QVector3D roomDimensions() const;
    void setRoomDimensions(const QVector3D &roomDimensions);

    bool turnedOn() const;
    void setTurnedOn(bool turnedOn);

    bool UseBackgroundSub() const;
    void setUseBackgroundSub(bool value);

    bool showWindow() const;
    void setShowWindow(bool showWindow);

    bool getROI() const;
    void setROI(bool value);

    cv::Mat getROIMask() const;
    void setROIMask(const cv::Mat &value);

    QVector2D resolution() const;
    void setResolution(const QVector2D &resolution);

    void save();

    QVector3D pixelLineDirectionVector(int x, int y);

signals:
    void changed(CameraSettingsType type);

private slots:
    void setSave(CameraSettingsType type);

private:
    void computeParameters();

};

#endif // CAMERASETTINGS_H
