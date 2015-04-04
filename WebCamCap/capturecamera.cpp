/*
 *
 * Copyright (C) 2014  Miroslav Krajicek, Faculty of Informatics Masaryk University (https://github.com/kaajo).
 * All Rights Reserved.
 *
 * This file is part of WebCamCap.
 *
 * WebCamCap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WebCamCap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU LGPL version 3
 * along with WebCamCap. If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */

#include "capturecamera.h"

#include <QVariant>
#include <QVariantMap>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QMatrix4x4>

using namespace cv;
using glm::vec2;
using glm::vec3;

cv::Mat CaptureCamera::distortionCoeffs() const
{
    return m_distortionCoeffs;
}

void CaptureCamera::setDistortionCoeffs(const cv::Mat &distortionCoeffs)
{
    m_distortionCoeffs = distortionCoeffs;
}

cv::Mat CaptureCamera::cameraMatrix() const
{
    return m_IntrinsicMatrix;
}

void CaptureCamera::setCameraMatrix(const cv::Mat &cameraMatrix)
{
    cameraMatrix.convertTo(m_IntrinsicMatrix, CV_32F);

    m_projectionMatrix = m_IntrinsicMatrix * m_CameraMatrix;
}

cv::Mat CaptureCamera::cameraProjectionMatrix() const
{
    return m_projectionMatrix;
}

void CaptureCamera::setCameraProjectionMatrix(const cv::Mat &cameraProjectionMatrix)
{
    m_projectionMatrix = cameraProjectionMatrix;
}

cv::Mat CaptureCamera::IntrinsicMatrix() const
{
    return m_IntrinsicMatrix;
}

void CaptureCamera::setIntrinsicMatrix(const cv::Mat &IntrinsicMatrix)
{
    m_IntrinsicMatrix = IntrinsicMatrix;
}



CaptureCamera::CaptureCamera()
{

}

CaptureCamera::CaptureCamera(vec2 resolution, vec3 pos, vec3 roomDimensions, QString name, int ID, float angle, bool backgroudSubstractor)
{
    m_videoUsbId = ID;
    m_name = name;
    m_globalPosition = pos;
    m_fov = angle;
    m_roomDimensions = roomDimensions;
    m_resolution = resolution;

    computeNewParameters();

    std::cout << "Vector to middle: " << m_directionVectorToCenter << std::endl;

    contourColor = Scalar(0, 0, 255);

    backgroundExtractor = new BackgroundSubtractorMOG(50, 10, 0.3, 0.4);
    useBackgroundSub = backgroudSubstractor;

    m_QtWidgetViewer = new CamWidget;
    CQtOpenCVViewerGl *t = m_QtWidgetViewer->getImageViewer();
    connect(this, SIGNAL(imageRead(cv::Mat)), t, SLOT(showImage(cv::Mat)));
    connect(m_QtWidgetViewer, SIGNAL(activeCam(bool)), this, SLOT(activeCam(bool)));
    connect(m_QtWidgetViewer, SIGNAL(turnedOnCam(bool)), this, SLOT(turnedOnCam(bool)));
    connect(m_QtWidgetViewer, SIGNAL(thresholdCam(size_t)), this, SLOT(thresholdCam(size_t)));
}

CaptureCamera::~CaptureCamera()
{
    Hide();
    TurnOff();
    delete m_QtWidgetViewer;
    delete backgroundExtractor;
}

QVector<Line> CaptureCamera::RecordNextFrame()
{
    lines.clear();

    if(!m_turnedOn)
    {
        QVector<Line> blank;
        return blank;
    }

    camera >> frame;

    UseFilter();


    MiddleOfContours();
    CreateLines();

    circle(frame, cv::Point(frame.cols/2, frame.rows/2), 1, CV_RGB(0,255,0), 2);

    if(m_showWindow)
    {
        emit imageRead(frame);
    }

    return lines;
}

#include <QTime>

std::vector<vec2> CaptureCamera::RecordNextFrame2D()
{
    if(!m_turnedOn)
    {
        std::vector<vec2> blank;
        return blank;
    }

    QTime timer;
    timer.start();

    camera >> frame;

    UseFilter();
    MiddleOfContours();

    circle(frame, cv::Point(frame.cols/2, frame.rows/2), 1, CV_RGB(0,255,0), 2);

    if(m_showWindow)
    {
        emit imageRead(frame);
    }

    NormalizeContours();

    return centerOfContour;
}

void CaptureCamera::UseFilter()
{
    if(!m_distortionCoeffs.empty() && !m_IntrinsicMatrix.empty())
    {
        GetUndisortedPosition();
    }

    if(ROI)
    {
        frame.copyTo(ROIMask, ROIMask);
        ROIMask.copyTo(frame);
    }

    if(useBackgroundSub)
    {
        backgroundExtractor->operator ()(frame, MOGMask);
        frame.copyTo(frameTemp,MOGMask);
    }
    else
    {
        frame.copyTo(frameTemp);
        absdiff(frameTemp,frameBackground, frameTemp);

        frameTemp = myColorThreshold(frameTemp, 20, 255);

        frame.copyTo(frameTemp, frameTemp);
    }

    cvtColor(frameTemp, frameTemp, COLOR_BGR2GRAY);
    medianBlur(frameTemp, frameTemp, 3);

    threshold(frameTemp,frameTemp, m_thresholdValue, 255, THRESH_BINARY);

    morphologyEx(frameTemp, frameTemp, MORPH_OPEN , dilateKernel);

    findContours(frameTemp, contours , RETR_EXTERNAL, CHAIN_APPROX_NONE);

    for(size_t i = 0; i < contours.size(); i++)
    {
        double contArea = contourArea(contours[i]);

        if(contArea > 500 || contArea <= 10)
        {
            contours.erase(contours.begin()+i);
        }
    }

    drawContours(frame, contours, -1, contourColor , CV_FILLED);
}

void CaptureCamera::GetUndisortedPosition()
{
    Mat framein;

    frame.copyTo(framein);

    cv::undistort(framein, frame, m_IntrinsicMatrix, m_distortionCoeffs);
}

void CaptureCamera::MiddleOfContours()
{
    centerOfContour.clear();

    for(size_t i = 0; i < contours.size(); i++)
    {
        centerMoment = moments(contours[i]);
        centerTemp = vec2(centerMoment.m10/centerMoment.m00, centerMoment.m01/centerMoment.m00);

        if(!isnan(centerTemp.x) && !isnan(centerTemp.y))
        {
            centerOfContour.push_back(centerTemp);

            circle(frame, cv::Point(centerTemp.x, centerTemp.y), 1, CV_RGB(0,0,255), 2);
        }
    }
}

void CaptureCamera::CreateLines()
{
    lines.clear();

    for(size_t i = 0; i < centerOfContour.size(); i++)
    {
        lines.push_back({m_globalPosition,m_pixelLines[centerOfContour[i].x][centerOfContour[i].y]});

/*

        //vypocitam stred contury vzhľadom ku stredu obrazovky
        //centerRelativeTemp = vec2(centerOfContour[i].x - frame.cols/2,centerOfContour[i].y - frame.rows/2);

        centerRelativeTemp = cv::Point2f(centerOfContour[i].x - frame.cols/2,centerOfContour[i].y - frame.rows/2);


        cv::Mat m_invertedRotationMatrix =  m_rotationMatrix.inv();


        QMatrix4x4 rotCamMatrix;
        QMatrix4x4 rotCamInvertedMatrix;

        for(size_t i = 0; i < 4; i++)
            for(size_t j = 0; j < 4; j++)
            {
                rotCamMatrix(i,j) = m_rotationMatrix.at<float>(i,j);
                rotCamInvertedMatrix(i,j) = m_invertedRotationMatrix.at<float>(i,j);
            }

        QMatrix4x4  rotMatrix;
        rotMatrix.rotate((-centerRelativeTemp.y * m_anglePerPixel), 1,0,0);

        QMatrix4x4 rotMatrix2;
        rotMatrix2.rotate((-centerRelativeTemp.x * m_anglePerPixel), 0, 1, 0);

        QVector4D vector(m_directionVectorToCenter.x, m_directionVectorToCenter.y, m_directionVectorToCenter.z, 0);

        QVector4D result = rotCamInvertedMatrix* rotMatrix2 * rotMatrix * rotCamMatrix * vector;

        vec3 final = vec3(result.x(), result.y(), result.z());

        lines.push_back({m_globalPosition,final});
*/
    }

}

void CaptureCamera::ComputeDirVector()
{
    m_directionVectorToCenter = vec3(m_roomDimensions.x/2 - m_globalPosition.x , m_roomDimensions.y/2 - m_globalPosition.y , m_roomDimensions.z/2 - m_globalPosition.z);
}

void CaptureCamera::NormalizeContours()
{
    for(size_t i = 0; i < centerOfContour.size(); i++)
    {
         centerOfContour[i] *= vec2(1.0/(float) frame.cols, 1.0f / (float) frame.rows);
    }
}

void CaptureCamera::createExtrinsicMatrix()
{
    m_rotationMatrix = cv::Mat::eye(4, 4, CV_32F);

    glm::vec3 normDirVector = glm::normalize(m_directionVectorToCenter); //L

    m_rotationMatrix.at<float>(2,0) = -normDirVector.x;
    m_rotationMatrix.at<float>(2,1) = -normDirVector.y;
    m_rotationMatrix.at<float>(2,2) = -normDirVector.z;

    glm::vec3 sVector = glm::cross(normDirVector, glm::vec3(0,0,1));
    glm::vec3 normSVector = glm::normalize(sVector);

    m_rotationMatrix.at<float>(0,0) = normSVector.x;
    m_rotationMatrix.at<float>(0,1) = normSVector.y;
    m_rotationMatrix.at<float>(0,2) = normSVector.z;

    glm::vec3 uVector = glm::cross(normSVector, normDirVector);

    m_rotationMatrix.at<float>(1,0) = uVector.x;
    m_rotationMatrix.at<float>(1,1) = uVector.y;
    m_rotationMatrix.at<float>(1,2) = uVector.z;


    cv::Mat tMatrix;
    tMatrix = cv::Mat::eye(4,4, CV_32F);

    tMatrix.at<float>(0,3) = -m_globalPosition.x;
    tMatrix.at<float>(1,3) = -m_globalPosition.y;
    tMatrix.at<float>(2,3) = -m_globalPosition.z;

    //float myAngle = Line::LineAngle(Line(glm::vec3(0,0,0), glm::vec3(1, 0, 0)), Line(glm::vec3(0,0,0), directionVectorToMiddle));

    cv::Mat temp = m_rotationMatrix * tMatrix;
    m_CameraMatrix = temp(Rect(0,0, 4, 3));

    std::cout << m_CameraMatrix << std::endl;
}

void CaptureCamera::computeAllDirections()
{
    if(m_anglePerPixel == 0)
    {
        m_anglePerPixel = ( (double)  m_fov ) / glm::sqrt( (m_resolution.x * m_resolution.x + m_resolution.y * m_resolution.y));
    }

    std::cout << m_anglePerPixel << std::endl;

    m_pixelLines.clear();

    m_pixelLines.reserve(m_resolution.x);
    cv::Mat m_invertedRotationMatrix =  m_rotationMatrix.inv();

    QMatrix4x4 rotCamMatrix;
    QMatrix4x4 rotCamInvertedMatrix;

    for(size_t i = 0; i < 4; i++)
        for(size_t j = 0; j < 4; j++)
        {
            rotCamMatrix(i,j) = m_rotationMatrix.at<float>(i,j);
            rotCamInvertedMatrix(i,j) = m_invertedRotationMatrix.at<float>(i,j);
        }

    QVector4D vector(m_directionVectorToCenter.x, m_directionVectorToCenter.y, m_directionVectorToCenter.z, 0);

    for(size_t i = 0; i < m_resolution.x; i++)
    {
        QVector<glm::vec3> vecTemp;
        vecTemp.reserve(m_resolution.y);

        for(size_t j = 0; j < m_resolution.y ; j++)
        {
            vec2 center = vec2(i,j);

            cv::Point2f centerTemp = cv::Point2f(center.x - m_resolution.x/2,center.y - m_resolution.y/2);

            QMatrix4x4  rotMatrix;
            rotMatrix.rotate((-centerTemp.y * m_anglePerPixel), 1,0,0);

            QMatrix4x4 rotMatrix2;
            rotMatrix2.rotate((-centerTemp.x * m_anglePerPixel), 0, 1, 0);

            QVector4D result = rotCamInvertedMatrix* rotMatrix2 * rotMatrix * rotCamMatrix * vector;

            vec3 final = vec3(result.x(), result.y(), result.z());

            vecTemp.append(final);
        }

        m_pixelLines.append(vecTemp);
    }
}

void CaptureCamera::computeNewParameters()
{
    ComputeDirVector();

    createExtrinsicMatrix();

    computeAllDirections();
}

cv::Mat CaptureCamera::myColorThreshold(cv::Mat input , int thresholdValue, int maxValue)
{
/*
    std::vector<Mat> ChannelsFrameTemp;

    split(input, ChannelsFrameTemp);

    for(size_t i = 0; i < 3; i++)
    {
        threshold(ChannelsFrameTemp[i], ChannelsFrameTemp[i], thresholdValue, maxValue, THRESH_BINARY);
    }

    bitwise_and(ChannelsFrameTemp[0], ChannelsFrameTemp[1], input);
    bitwise_and(input, ChannelsFrameTemp[2], input);

    morphologyEx(input, input, MORPH_OPEN , dilateKernel);// , Point(-1,-1),  5);*/

    cvtColor(input, input, COLOR_BGR2GRAY);

    threshold(input, input, thresholdValue, maxValue, THRESH_BINARY);

    return input;
}

void CaptureCamera::activeCam(bool active)
{
    if(active)
    {
        Show();
    }
    else
    {
        Hide();
    }
}

void CaptureCamera::turnedOnCam(bool turnedOn)
{
    if(turnedOn)
    {
        TurnOn();
    }
    else
    {
        TurnOff();
    }
}

void CaptureCamera::thresholdCam(size_t threshold)
{
    setThreshold(threshold);
}

void CaptureCamera::TurnOn()
{
    if(m_turnedOn)
        return;

    if(camera.open(m_videoUsbId))
    {
        //QtWidgetViewer->setCheckTurnedOn(true);
        m_turnedOn = true;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("");
        msgBox.warning(nullptr , "", "No project opened");
        msgBox.setFixedSize(200,100);

        //QtWidgetViewer->setCheckTurnedOn(false);
        m_turnedOn = false;
    }

        if(m_resolution.x != 0 && m_resolution.y !=0)
        {
            camera.set(CV_CAP_PROP_FRAME_WIDTH, m_resolution.x);
            camera.set(CV_CAP_PROP_FRAME_HEIGHT, m_resolution.y);
        }

}

void CaptureCamera::TurnOff()
{
    if(m_turnedOn)
    {
        m_turnedOn = false;
        m_QtWidgetViewer->setCheckTurnedOn(false);
        camera.release();
    }

}

void CaptureCamera::Show()
{
    if(!m_showWindow)
    {
        m_QtWidgetViewer->setCheckActive(true);
        m_showWindow = true;

    }
}

void CaptureCamera::Hide()
{
    if(m_showWindow)
    {
        m_QtWidgetViewer->setCheckActive(false);
        m_showWindow = false;
    }
}

void CaptureCamera::CalibNoMarkers()
{
    if(m_turnedOn)
    {
        int i = 0, maxIters = 10;
        Scalar meanValue, lastMeanValue;

        camera >> frameBackground;
        waitKey(33);

        lastMeanValue = mean(frameBackground);

        camera >> frameBackground;
        waitKey(33);

        meanValue = mean(frameBackground);

        while(i < maxIters && ( abs( lastMeanValue.val[0] - meanValue.val[0] ) > 1 || abs( lastMeanValue.val[1] - meanValue.val[1] ) > 1 || abs( lastMeanValue.val[2] - meanValue.val[2] ) > 1 ) )
        {
            camera >> frameBackground;
            lastMeanValue = meanValue;
            meanValue  = mean(frameBackground);
            ++i;
            waitKey(66);
        }

        std::cout << m_name.toStdString() << " calibrated in " << i << " iterations" << std::endl;

        Mat temp;

        for(size_t i = 0; i < 50; i++)
        {
            camera >> temp;
            backgroundExtractor->operator ()(frame, MOGMask);

            if(i < 15)
            {
                for(int i = 0; i < temp.rows; i++)
                {
                    for(int j = 0; j < temp.cols; j++)
                    {
                        if(temp.at<Vec3b>(i,j)[0] > frameBackground.at<Vec3b>(i,j)[0] || temp.at<Vec3b>(i,j)[1] > frameBackground.at<Vec3b>(i,j)[1] || temp.at<Vec3b>(i,j)[2] > frameBackground.at<Vec3b>(i,j)[2])
                        {
                            frameBackground.at<Vec3b>(i,j) = temp.at<Vec3b>(i,j);
                        }
                    }
                }
            }

            waitKey(20);
        }
    }
}

int CaptureCamera::CalibWithMarkers(int numOfMarkers)
{
    m_thresholdValue = 255;

    if(m_turnedOn)
    {

        size_t thresholdUp, thresholdLow;

        for(size_t i = 0; i < 15; i++)
        {
            camera >> frame;
            waitKey(10);
        }

        size_t nLines;

        //step 1, find first value which gives some Lines
        while(m_thresholdValue > 20)
        {
            UseFilter();
            MiddleOfContours();
            CreateLines();

            if(lines.size() == 0)
            {
                --m_thresholdValue;
                continue;
            }
            else
            {
                if(numOfMarkers == 0)
                {
                    break;
                }
                else
                {
                    if(lines.size() == numOfMarkers)
                    {
                        break;
                    }
                }
            }
        }

        //some difference in light intensity (rotation of LED)
        m_thresholdValue -= 10;

        UseFilter();
        MiddleOfContours();
        CreateLines();

        nLines = lines.size();

        thresholdUp = m_thresholdValue;
        thresholdLow = 0;
        std::cout << "calibrated upper value" << thresholdUp << std::endl;

        //step 2 , find threshold where num of lines is starting to grow
        while(m_thresholdValue > 0)
        {
            --m_thresholdValue;

            UseFilter();
            MiddleOfContours();
            CreateLines();

            if(nLines < lines.size())
            {
                thresholdLow = m_thresholdValue;
                std::cout << "distance: " << "calibrated lower value" << thresholdLow << std::endl;
                break;
            }
        }

        m_thresholdValue = thresholdLow + (thresholdUp + thresholdLow)/8;

        m_QtWidgetViewer->setThreshold(m_thresholdValue);
    }

    return m_thresholdValue;
}

const QString usbIdKey("USB");
const QString cameraNameKey("name");
const QString turnedOnKey("turneOn");
const QString globalPositionKeyX("globalpositionX");
const QString globalPositionKeyY("globalpositionY");
const QString globalPositionKeyZ("globalpositionZ");
const QString roomDimensionsKeyX("roomDimX");
const QString roomDimensionsKeyY("roomDimY");
const QString roomDimensionsKeyZ("roomDimZ");
const QString fovKey("fov");
const QString thresholdKey("threshold");
const QString showWindowKey("showWindow");
const QString roineededKey("roineeded");
// roi to variant
const QString useBackgroundSubstractorKey("backgroundSubstractor");
const QString resolutionKeyX("resolutionX");
const QString resolutionKeyY("resolutionY");

QVariantMap CaptureCamera::toVariantMap()
{
    QVariantMap retVal;

    retVal[usbIdKey] = m_videoUsbId;
    retVal[cameraNameKey] = m_name;
    retVal[turnedOnKey] = m_turnedOn;
    retVal[globalPositionKeyX] = m_globalPosition.x;
    retVal[globalPositionKeyY] = m_globalPosition.y;
    retVal[globalPositionKeyZ] = m_globalPosition.z;
    retVal[roomDimensionsKeyX] = m_roomDimensions.x;
    retVal[roomDimensionsKeyY] = m_roomDimensions.y;
    retVal[roomDimensionsKeyZ] = m_roomDimensions.z;
    retVal[fovKey] = m_fov;
    retVal[thresholdKey] = (int) m_thresholdValue;
    retVal[showWindowKey] = m_showWindow;
    retVal[roineededKey] = ROI;
    ////!!! need ROI man
    retVal[useBackgroundSubstractorKey] = useBackgroundSub;
    retVal[resolutionKeyX] = m_resolution.x;
    retVal[resolutionKeyY] = m_resolution.y;

    return retVal;
}

void CaptureCamera::fromVariantMap(QVariantMap varMap)
{
    m_videoUsbId = varMap[usbIdKey].toInt();
    m_name = varMap[cameraNameKey].toString();
    m_globalPosition = vec3(varMap[globalPositionKeyX].toFloat(),varMap[globalPositionKeyY].toFloat(),varMap[globalPositionKeyZ].toFloat());
    m_fov = varMap[fovKey].toFloat();
    m_roomDimensions = vec3(varMap[roomDimensionsKeyX].toFloat(), varMap[roomDimensionsKeyY].toFloat(), varMap[roomDimensionsKeyZ].toFloat());
    m_resolution =  vec2(varMap[resolutionKeyX].toFloat(), varMap[resolutionKeyY].toFloat());

    computeNewParameters();

    std::cout << "Vector to middle: " << m_directionVectorToCenter << std::endl;

    contourColor = Scalar(0, 0, 255);

    backgroundExtractor = new BackgroundSubtractorMOG(50, 10, 0.3, 0.4);
    useBackgroundSub = varMap[useBackgroundSubstractorKey].toFloat();

    m_QtWidgetViewer = new CamWidget;
    CQtOpenCVViewerGl *t = m_QtWidgetViewer->getImageViewer();
    connect(this, SIGNAL(imageRead(cv::Mat)), t, SLOT(showImage(cv::Mat)));
    connect(m_QtWidgetViewer, SIGNAL(activeCam(bool)), this, SLOT(activeCam(bool)));
    connect(m_QtWidgetViewer, SIGNAL(turnedOnCam(bool)), this, SLOT(turnedOnCam(bool)));
    connect(m_QtWidgetViewer, SIGNAL(thresholdCam(size_t)), this, SLOT(thresholdCam(size_t)));

    if(varMap[turnedOnKey].toBool())
    {
        TurnOn();
    }
}
