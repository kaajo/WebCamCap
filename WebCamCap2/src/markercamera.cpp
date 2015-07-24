#include "markercamera.h"

#include <tr1/functional>

#include <QDebug>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentMap>
#include <QMutexLocker>

MarkerCamera::MarkerCamera(QVariantMap varMap, QWaitCondition *waitCondition, QObject *parent) :
    ICamera(parent)
{
    setWaitCondition(waitCondition);

    fromVariantMap(varMap);
}

MarkerCamera::MarkerCamera(CameraSettings *settings, QObject *parent) :
    ICamera(parent)
{
    setSettings(settings);

    if(m_settings->turnedOn())
    {
        turnOn(true);
    }
}

MarkerCamera::~MarkerCamera()
{
    m_settings->setTurnedOn(false);
}

QVector<Line> MarkerCamera::nextFrame()
{
    m_lines.clear();

    if(!m_settings->turnedOn())
    {
        QVector<Line> blank;
        return blank;
    }

    m_camera >> m_actualFrame;

    applyFilters();
    createLineForComponents();

    if(m_settings->showWindow())
    {
        m_actualFrame.copyTo(m_signalFrame);

        emit imageShow(m_signalFrame);
    }

    return m_lines;
}

QVector<QVector3D> MarkerCamera::nextFrame2D()
{
    QVector<QVector3D> blank;
    return blank;
}

void MarkerCamera::calibrate(ICamera::CameraCalibrationType type)
{
    switch (type) {
    case ICamera::CameraCalibrationType::BACKGROUND:
        calibBackground();
        break;
    case ICamera::CameraCalibrationType::THRESHOLD:
        calibThreshold();
        break;
    default:
        break;
    }
}

void MarkerCamera::showPreviewInWindow(bool show)
{
    if(show != m_settings->showWindow())
    {
        m_settings->setShowWindow(show);
    }
}

bool MarkerCamera::turnOn(bool turnOn)
{
    if(m_settings->turnedOn() == m_camera.isOpened())
    {
        return false;
    }

    if(turnOn)
    {
        if(m_camera.open(m_settings->videoUsbId()))
        {


            m_settings->setTurnedOn(turnOn);

            if(m_settings->resolution().x() != 0 && m_settings->resolution().y() != 0)
            {
                m_camera.set(CV_CAP_PROP_FRAME_WIDTH, m_settings->resolution().x());
                m_camera.set(CV_CAP_PROP_FRAME_HEIGHT, m_settings->resolution().y());
            }

            qDebug() << "turning on cam " << this;

            return true;
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("");
            msgBox.warning(nullptr , "", "Could not open device");
            msgBox.setFixedSize(200,100);

            return false;
        }
    }
    else
    {
        m_camera.release();
        m_settings->setTurnedOn(false);

        qDebug() << "turning off cam ";

        return true;
    }
}

void MarkerCamera::setThreshold(size_t threshold)
{
    m_settings->setThresholdValue(threshold);
}

void MarkerCamera::settingsChanged(CameraSettings::CameraSettingsType type)
{
    switch (type) {
    case CameraSettings::CameraSettingsType::TURNEDON:
            turnOn(m_settings->turnedOn());
        break;
    case CameraSettings::CameraSettingsType::ALL:
        turnOn(m_settings->turnedOn());
        break;
    default:
        break;
    }
}

void MarkerCamera::stopWork()
{
    QMutexLocker l(&m_mutex);
    m_running = false;
}

void MarkerCamera::startWork()
{
    m_running = true;
    doWork();
}

void MarkerCamera::doWork()
{
    forever
    {
        m_mutex.lock();
        if (!m_running)
        {
            m_mutex.unlock();
            break;
        }

        emit results(nextFrame());

        if(!m_waitCondition->wait(&m_mutex,1000))
        {
           qDebug() << "signal lost in thread: " << this->thread();
        }
        m_mutex.unlock();

        QCoreApplication::processEvents();
    }

    qDebug() << "Stopping capture thread";
}

void MarkerCamera::calibBackground()
{
    qDebug() << "calib";

    if(m_settings->turnedOn() && m_camera.isOpened())
    {
        int i = 0, maxIters = 10;
        cv::Scalar meanValue, lastMeanValue;

        m_camera >> m_background;
        cv::waitKey(33);

        lastMeanValue = cv::mean(m_background);

        m_camera >> m_background;
        cv::waitKey(33);

        meanValue = cv::mean(m_background);

        while(i < maxIters && ( abs( lastMeanValue.val[0] - meanValue.val[0] ) > 1 || abs( lastMeanValue.val[1] - meanValue.val[1] ) > 1 || abs( lastMeanValue.val[2] - meanValue.val[2] ) > 1 ) )
        {
            m_camera >> m_background;
            lastMeanValue = meanValue;
            meanValue  = cv::mean(m_background);
            ++i;
            cv::waitKey(1);
        }

        cv::Mat temp;

        for(size_t i = 0; i < 20; i++)
        {
            m_camera >> temp;

            if(m_settings->UseBackgroundSub())
            {
                //backgroundExtractor->operator ()(temp, MOGMask); !!!!!!!!!!!!!!!!!!!!!!
            }

            for(int i = 0; i < temp.rows; i++)
            {
                for(int j = 0; j < temp.cols; j++)
                {
                    if(temp.at<cv::Vec3b>(i,j)[0] > m_background.at<cv::Vec3b>(i,j)[0] || temp.at<cv::Vec3b>(i,j)[1] > m_background.at<cv::Vec3b>(i,j)[1] || temp.at<cv::Vec3b>(i,j)[2] > m_background.at<cv::Vec3b>(i,j)[2])
                    {
                        m_background.at<cv::Vec3b>(i,j) = temp.at<cv::Vec3b>(i,j);
                    }
                }
            }

            cv::waitKey(15);
        }

        qDebug() << m_settings->name() << " calibrated in " << i << " iterations";
    }
}

void MarkerCamera::calibThreshold()
{
    m_settings->setThresholdValue(255);

    if(m_settings->turnedOn())
    {
        size_t thresholdUp = 255, thresholdLow = 0;

        for(size_t i = 0; i < 15; i++)
        {
            m_camera >> m_actualFrame;
            cv::waitKey(10);
        }

        int nLines;

        //step 1, find first value which gives some Lines
        while(m_settings->getThresholdValue() > 20)
        {
            applyFilters();
            createLineForComponents();

            if(m_lines.size() == 0)
            {
                m_settings->setThresholdValue(m_settings->getThresholdValue() - 1);
                continue;
            }
            else
            {
                break;
            }
        }

        //some difference in light intensity (rotation of LED)
        m_settings->setThresholdValue(m_settings->getThresholdValue() - 10);

        applyFilters();
        createLineForComponents();

        nLines = m_lines.size();

        thresholdUp = m_settings->getThresholdValue();
        thresholdLow = 0;
        qDebug() << "calibrated upper value" << thresholdUp;

        //step 2 , find threshold where num of lines is starting to grow
        while(m_settings->getThresholdValue() > 0)
        {
            m_settings->setThresholdValue(m_settings->getThresholdValue() - 1);

            applyFilters();
            createLineForComponents();

            if(nLines < m_lines.size())
            {
                thresholdLow = m_settings->getThresholdValue();
                //std::cout << "distance: " << "calibrated lower value" << thresholdLow << std::endl;
                break;
            }
        }

        m_settings->setThresholdValue((thresholdLow + thresholdUp)/2);

        qDebug() << "final threshold: " << m_settings->getThresholdValue();
    }
}

Line MarkerCamera::qtConcurrentpickLine(MarkerCamera *camera, const Contour &contour)
{
    double m00 = contour.size(), m10 = 0.0 , m01 = 0.0;

    for(const cv::Point &pnt : contour)
    {
        m10 += pnt.x;
        m01 += pnt.y;
    }

    int x = m10/m00;
    int y = m01/m00;

    cv::circle(camera->m_actualFrame, cv::Point(x, y), 1, CV_RGB(0,0,255), 2);

    return Line(camera->m_settings->globalPosition(),camera->m_settings->pixelLineDirectionVector(x,y));
}

void MarkerCamera::applyFilters()
{
    m_actualFrame.copyTo(m_filteredFrame);

    cv::cvtColor(m_filteredFrame, m_filteredFrame, cv::COLOR_BGR2GRAY);
    cv::medianBlur(m_filteredFrame, m_filteredFrame, 3);

    cv::threshold(m_filteredFrame, m_filteredFrame, m_settings->getThresholdValue(), 255, cv::THRESH_BINARY);

    cv::morphologyEx(m_filteredFrame, m_filteredFrame, cv::MORPH_OPEN , m_settings->dilateKernel);

    cv::findContours(m_filteredFrame, m_contours , cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

    QtConcurrent::blockingFilter(m_contours, [](const Contour &contour)
    {
        double contArea = contour.size();
        return contArea < 500 && contArea > 5;
    });

    cv::drawContours(m_actualFrame, m_contours, -1, m_settings->contourColor , CV_FILLED);
}

void MarkerCamera::createLineForComponents()
{
    m_lines = QtConcurrent::blockingMapped<QVector<Line>>(m_contours,
              std::tr1::bind(&MarkerCamera::qtConcurrentpickLine, this, std::tr1::placeholders::_1));
}

//########################################################################################

const QString settingsKey("settings");

QVariantMap MarkerCamera::toVariantMap() const
{
    QVariantMap retVal;

    retVal[settingsKey] = m_settings->toVariantMap();

    return retVal;
}

bool MarkerCamera::fromVariantMap(QVariantMap varMap)
{
    auto settings = new CameraSettings;

    if(! settings->fromVariantMap(varMap[settingsKey].toMap()))
    {
        qDebug() << "wrong variantMap for CameraSettings";
        return false;
    }

    setSettings(settings);

    return true;
}
