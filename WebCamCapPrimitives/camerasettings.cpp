#include "camerasettings.h"

#include <QVariantMap>

CameraSettings::CameraSettings(QObject *parent) : QObject(parent)
{

}

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

cv::Mat CameraSettings::distortionCoeffs() const
{
    return m_distortionCoeffs;
}

void CameraSettings::setDistortionCoeffs(const cv::Mat &distortionCoeffs)
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

QVector4D CameraSettings::getDirectionVector() const
{
    return m_directionVector;
}

int CameraSettings::getThresholdValue() const
{
    return m_thresholdValue;
}

void CameraSettings::setThresholdValue(int thresholdValue)
{
    m_thresholdValue = thresholdValue;
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
}

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

//###################################################################################

QString nameKey("name");
QString videoUsbIdKey("usbId");
QString diagonalFovKey("diagonalFov");
QString globalPositionKey("globalPosition");
QString resolutionKey("resolution");
QString roomDimensionsKey("roomDims");
QString distortionCoeffsKey("distortionCoeffs");


QVariantMap CameraSettings::toVariantMap()
{
    QVariantMap retVal;

    retVal[nameKey] = m_name;
    retVal[videoUsbIdKey] = m_videoUsbId;
    retVal[diagonalFovKey] = m_diagonalFov;
    retVal[globalPositionKey] = m_globalPosition;
    retVal[resolutionKey] = m_resolution;
    retVal[roomDimensionsKey] = m_roomDimensions;

    if(! m_distortionCoeffs.empty())
    {
        QVariantMap distortionCoeffsMap;

        distortionCoeffsMap["x"] = m_distortionCoeffs.cols;
        distortionCoeffsMap["y"] = m_distortionCoeffs.rows;

        for(int x = 0; x < m_distortionCoeffs.cols; ++x)
            for(int y = 0; y < m_distortionCoeffs.rows; ++y)
            {
                auto xstring = QString::number(x);
                auto ystring = QString::number(y);

                distortionCoeffsMap[xstring+ystring] = m_distortionCoeffs.at<double>(x,y);
            }

        retVal[distortionCoeffsKey] = distortionCoeffsMap;
    }

    return retVal;
}

bool CameraSettings::fromVariantMap(QVariantMap map)
{
    if(! map.contains(nameKey) || ! map.contains(videoUsbIdKey) || ! map.contains(diagonalFovKey) ||
       ! map.contains(globalPositionKey) || ! map.contains(resolutionKey))
    {
        return false;
    }

    m_name = map[nameKey].toString();
    m_videoUsbId = map[videoUsbIdKey].toInt();
    m_diagonalFov = map[diagonalFovKey].toFloat();
    m_globalPosition = map[globalPositionKey].value<QVector3D>();
    m_resolution = map[resolutionKey].value<QVector2D>();
    m_roomDimensions = map[roomDimensionsKey].value<QVector3D>();

    if(map.contains(distortionCoeffsKey))
    {
        QVariantMap distortionCoeffsMap = map[distortionCoeffsKey].toMap();

        cv::Mat distortionMat;

        int x = distortionCoeffsMap["x"].toInt();
        int y = distortionCoeffsMap["y"].toInt();

        if(!(x == 0 || y == 0))
        {
            distortionMat.create(y,x, CV_64F);

            for(int i = 0; i < x; ++i)
                for(int j = 0; j < y; ++j)
                {
                    QString xstring = QString::number(i);
                    QString ystring = QString::number(j);

                    distortionMat.at<double>(i,j) = distortionCoeffsMap[xstring+ystring].toDouble();
                }

            m_distortionCoeffs = distortionMat;
        }
    }

    computeAllParameters();

    return true;
}
