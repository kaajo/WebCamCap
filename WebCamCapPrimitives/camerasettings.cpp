#include "camerasettings.h"

CameraSettings::CameraSettings(QString name, int videoUsbId , float diagonalFov ,
                               QVector3D globalPosition , QVector3D roomDimensions,  QObject *parent)
    : QObject(parent)
{
    m_name = name;
    m_videoUsbId = videoUsbId;
    m_diagonalFov = diagonalFov;
    m_globalPosition = globalPosition;
    m_roomDimensions = roomDimensions;

    computeAllParameters();
}

QVariantMap CameraSettings::toVariantMap()
{
    QVariantMap retVal;

    return retVal;
}

void CameraSettings::fromVariantMap(QVariantMap map)
{

}

QVector2D CameraSettings::resolution() const
{
    return m_resolution;
}

void CameraSettings::setResolution(const QVector2D &resolution)
{
    m_resolution = resolution;

    computeAllParameters();

    emit changed(CameraSettingsType::RESOLUTION);
}

void CameraSettings::save()
{
    m_saved = true;
}

QVector3D CameraSettings::pixelLineDirectionVector(int x, int y)
{
    return m_pixelLines[x][y];
}

bool CameraSettings::turnedOn() const
{
    return m_turnedOn;
}

void CameraSettings::setTurnedOn(bool turnedOn)
{
    m_turnedOn = turnedOn;

    emit changed(CameraSettingsType::TURNEDON);
}

bool CameraSettings::UseBackgroundSub() const
{
    return useBackgroundSub;
}

void CameraSettings::setUseBackgroundSub(bool value)
{
    useBackgroundSub = value;

    emit changed(CameraSettingsType::USEBACKGROUNDSUBSTRACTOR);
}

bool CameraSettings::showWindow() const
{
    return m_showWindow;
}

void CameraSettings::setShowWindow(bool showWindow)
{
    m_showWindow = showWindow;

    emit changed(CameraSettingsType::SHOWIMAGEGUI);
}

bool CameraSettings::getuseRoi() const
{
    return m_useRoi;
}

void CameraSettings::setuseRoi(bool value)
{
    m_useRoi = value;

    emit changed(CameraSettingsType::ROI);
}

cv::Mat CameraSettings::getRoiMask() const
{
    return m_roiMask;
}

void CameraSettings::setRoiMask(const cv::Mat &value)
{
    m_roiMask = value;

    emit changed(CameraSettingsType::ROIMASK);
}

QString CameraSettings::name() const
{
    return m_name;
}

void CameraSettings::setName(const QString &name)
{
    m_name = name;

    emit changed(CameraSettingsType::NAME);
}

int CameraSettings::videoUsbId() const
{
    return m_videoUsbId;
}

void CameraSettings::setVideoUsbId(int videoUsbId)
{
    m_videoUsbId = videoUsbId;

    emit changed(CameraSettingsType::VIDEOUSBID);
}

float CameraSettings::diagonalFov() const
{
    return m_diagonalFov;
}

void CameraSettings::setDiagonalFov(float diagonalFov)
{
    m_diagonalFov = diagonalFov;

    computeAllParameters();

    emit changed(CameraSettingsType::FOV);
}

QVector3D CameraSettings::globalPosition() const
{
    return m_globalPosition;
}

void CameraSettings::setGlobalPosition(const QVector3D &globalPosition)
{
    m_globalPosition = globalPosition;

    computeAllParameters();

    emit changed(CameraSettingsType::POSITION);
}

cv::UMat CameraSettings::distortionCoeffs() const
{
    return m_distortionCoeffs;
}

void CameraSettings::setDistortionCoeffs(const cv::UMat &distortionCoeffs)
{
    m_distortionCoeffs = distortionCoeffs;

    emit changed(CameraSettingsType::DISTORTION);
}

QVector3D CameraSettings::roomDimensions() const
{
    return m_roomDimensions;
}

void CameraSettings::setRoomDimensions(const QVector3D &roomDimensions)
{
    m_roomDimensions = roomDimensions;

    computeAllParameters();
}

void CameraSettings::setSave(CameraSettings::CameraSettingsType /*type*/)
{
    m_saved = false;
}

void CameraSettings::computeDirectionVector()
{
    m_directionVector = QVector3D(m_roomDimensions.x()/2 - m_globalPosition.x(),
                                  m_roomDimensions.y()/2 - m_globalPosition.y(),
                                  m_roomDimensions.z()/2 - m_globalPosition.z()
                                  );
}

void CameraSettings::computeAnglePerPixel()
{
    m_anglePerPixel = ((double) m_diagonalFov ) / sqrt( (m_resolution.x() * m_resolution.x() + m_resolution.y() * m_resolution.y()));
}

void CameraSettings::computeMatrices()
{
    //rotation matrix
    auto normDirVector = m_directionVector.normalized(); //L

    m_rotationMatrix.setRow(2, -normDirVector);

    auto normSVector = QVector4D(QVector3D::crossProduct(normDirVector.toVector3D(), QVector3D(0,0,1)).normalized());

    m_rotationMatrix.setRow(0, normSVector);

    auto uVector = QVector4D(QVector3D::crossProduct(normSVector.toVector3D(), normDirVector.toVector3D()));

    m_rotationMatrix.setRow(1, uVector);
    m_rotationMatrix.setRow(3, QVector4D(0,0,0,1));

    /*
    //extrinsic matrix
    cv::Mat tMatrix;
    tMatrix = cv::Mat::eye(4,4, CV_32F);

    tMatrix.at<float>(0,3) = -m_globalPosition.x;
    tMatrix.at<float>(1,3) = -m_globalPosition.y;
    tMatrix.at<float>(2,3) = -m_globalPosition.z;

    //float myAngle = Line::LineAngle(Line(glm::vec3(0,0,0), glm::vec3(1, 0, 0)), Line(glm::vec3(0,0,0), directionVectorToMiddle));

    cv::Mat temp = m_rotationMatrix * tMatrix;
    m_CameraMatrix = temp(Rect(0,0, 4, 3));*/

    /*
    //projection matrix

    m_projectionMatrix = m_IntrinsicMatrix * m_CameraMatrix;

    */
}

#include <QTime>
#include <QTimer>

void CameraSettings::computePixelLines()
{
    m_pixelLines.clear();

    QMatrix4x4 rotCamInvertedMatrix = m_rotationMatrix.inverted();

    QVector2D distanceFromCenter;

    m_pixelLines.reserve(m_resolution.x());
    for(size_t i = 0; i < m_resolution.x(); i++)
    {
        QVector<QVector3D> vecTemp;
        vecTemp.reserve(m_resolution.y());

        for(size_t j = 0; j < m_resolution.y() ; j++)
        {
            distanceFromCenter = QVector2D(i - m_resolution.x()/2,j - m_resolution.y()/2);

            QMatrix4x4 rotMatrix;
            rotMatrix.rotate((-distanceFromCenter.y() * m_anglePerPixel), 1,0,0);

            QMatrix4x4 rotMatrix2;
            rotMatrix2.rotate((-distanceFromCenter.x() * m_anglePerPixel), 0, 1, 0);

            vecTemp.append((rotCamInvertedMatrix* rotMatrix2 * rotMatrix * m_rotationMatrix * m_directionVector).toVector3D());
        }

        m_pixelLines.append(vecTemp);
    }
}

void CameraSettings::computeAllParameters()
{
    computeDirectionVector();
    computeAnglePerPixel();
    computeMatrices();
    computePixelLines();
}
