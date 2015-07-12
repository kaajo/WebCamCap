#include "markercamera.h"

#include <tr1/functional>

#include <QDebug>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
#include <QtConcurrent/QtConcurrentMap>
#include <QMutexLocker>

MarkerCamera::MarkerCamera(CameraSettings *settings, QObject *parent) :
    ICamera(settings, parent)
{
    if(m_settings->turnedOn())
    {
        turnOn(true);
    }
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
        emit imageShow(m_actualFrame);
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

        return true;
    }
}

void MarkerCamera::setThreshold(size_t threshold)
{
    m_thresholdValue = threshold;
}

QVariantMap MarkerCamera::toVariantMap() const
{

}

void MarkerCamera::fromVariantMap(QVariantMap varMap)
{

}

void MarkerCamera::settingsChanged(CameraSettings::CameraSettingsType type)
{
    switch (type) {
    case CameraSettings::CameraSettingsType::TURNEDON:
        if(m_settings->turnedOn())
        {
            turnOn(true);
        }
        else
        {
            turnOn(false);
        }
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

            cv::waitKey(20);
        }

        qDebug() << m_settings->name() << " calibrated in " << i << " iterations";
    }
}

void MarkerCamera::calibThreshold()
{
    m_thresholdValue = 255;

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
        while(m_thresholdValue > 20)
        {
            applyFilters();
            createLineForComponents();

            if(m_lines.size() == 0)
            {
                --m_thresholdValue;
                continue;
            }
            else
            {
                break;
            }
        }

        //some difference in light intensity (rotation of LED)
        m_thresholdValue -= 10;

        applyFilters();
        createLineForComponents();

        nLines = m_lines.size();

        thresholdUp = m_thresholdValue;
        thresholdLow = 0;
        qDebug() << "calibrated upper value" << thresholdUp;

        //step 2 , find threshold where num of lines is starting to grow
        while(m_thresholdValue > 0)
        {
            --m_thresholdValue;

            applyFilters();
            createLineForComponents();

            if(nLines < m_lines.size())
            {
                thresholdLow = m_thresholdValue;
                //std::cout << "distance: " << "calibrated lower value" << thresholdLow << std::endl;
                break;
            }
        }

        m_thresholdValue = (thresholdLow + thresholdUp)/2;

        qDebug() << "final threshold: " << m_thresholdValue;
    }
}

Line MarkerCamera::qtConcurrentpickLine(MarkerCamera *camera, const Contour &contour)
{
    auto centerMoment = cv::moments(contour);

    QVector2D center(centerMoment.m10/centerMoment.m00, centerMoment.m01/centerMoment.m00);

    cv::circle(camera->m_actualFrame, cv::Point(center.x(), center.y()), 1, CV_RGB(0,0,255), 2);

    return Line(camera->m_settings->globalPosition(),camera->m_settings->pixelLineDirectionVector(center.x(),center.y()));
}

void MarkerCamera::applyFilters()
{
    m_actualFrame.copyTo(m_filteredFrame);

    cv::cvtColor(m_filteredFrame, m_filteredFrame, cv::COLOR_BGR2GRAY);
    cv::medianBlur(m_filteredFrame, m_filteredFrame, 3);

    cv::threshold(m_filteredFrame, m_filteredFrame, m_thresholdValue, 255, cv::THRESH_BINARY);

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
