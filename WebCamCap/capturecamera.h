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

#ifndef CAPTURECAMERA_H
#define CAPTURECAMERA_H

#include "line.h"
#include "Gui/camwidget.h"

#include <fstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

class CaptureCamera: public QObject
{
    Q_OBJECT

    typedef std::vector<cv::Point> Contour;

    //BASIC PARAMETERS
    int m_videoUsbId = 0;
    QString m_name = "Camera";
    bool m_turnedOn = false;
    float m_fov;
    size_t m_thresholdValue = 255;


    //width, length
    glm::vec3 m_roomDimensions;
    glm::vec3 m_globalPosition;
    glm::vec3 m_directionVectorToCenter;

    //triangulation
    double m_anglePerPixel = 0;
    cv::Mat m_rotationMatrix;
    cv::Mat m_distortionCoeffs;

    //widget
    bool m_showWindow = true;
    CamWidget * m_QtWidgetViewer = nullptr;

    //ADVANCED for camera
    cv::VideoCapture camera;
    bool ROI = false;
    cv::Mat ROIMask;
    cv::Mat frameBackground ,frame, frameTemp;

    //background substract
    bool useBackgroundSub;
    cv::Mat MOGMask;
    cv::BackgroundSubtractorMOG* backgroundExtractor;

    //ADVANCED for image process
    cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3,3));
    cv::Scalar contourColor;
    std::vector <Contour> contours;
    cv::Moments centerMoment;
    glm::vec2 centerTemp;
    cv::Point2f  centerRelativeTemp;
    std::vector<glm::vec2> centerOfContour;

    glm::vec3 directionTemp;
    QVector<Line> lines;

    //all matrices
    cv::Mat m_projectionMatrix;
    cv::Mat m_CameraMatrix;
    cv::Mat m_IntrinsicMatrix;

    QVector<QVector<glm::vec3>> m_pixelLines; // reserver , squeeze

public:
    //public parameters
    glm::vec2 m_resolution = glm::vec2(0,0);

    //public functions
    CaptureCamera();
    CaptureCamera(glm::vec2 m_resolution, glm::vec3 pos, glm::vec3 m_roomDimensions, QString m_name, int ID, float angle, bool backgroudSubstractor = false);

    ~CaptureCamera();

    QVector<Line> RecordNextFrame();
    std::vector<glm::vec2> RecordNextFrame2D();
    void TurnOn();
    void TurnOff();
    void Show();
    void Hide();
    void Save(std::ofstream &outputFile);
    void CalibNoMarkers();
    int CalibWithMarkers(int numOfMarkers);
    void setROI(cv::Mat roi){ROIMask = roi; ROI = true;}

    void setPosition(glm::vec3 position){m_globalPosition = position; computeNewParameters();}
    void setDimensions(glm::vec3 roomDim){m_roomDimensions = roomDim; computeNewParameters();}
    void setThreshold(size_t Threshold){m_thresholdValue = Threshold;}
    void setAngle(float Angle){m_fov = Angle; m_anglePerPixel = 0;}
    void setName(QString name){m_name = name;}


    QString getName() const {return m_name;}
    glm::vec3 getPosition() const {return m_globalPosition;}
    glm::vec3 getDirVector() const {return m_directionVectorToCenter;}
    int getID() const {return m_videoUsbId;}
    float getAngle() const {return m_fov;}
    bool getTurnedOn() const {return m_turnedOn;}
    CamWidget *getWidget() const {return m_QtWidgetViewer;}

    static cv::Mat myColorThreshold(cv::Mat input, int m_thresholdValue, int maxValue);

    cv::Mat distortionCoeffs() const;
    void setDistortionCoeffs(const cv::Mat &distortionCoeffs);

    cv::Mat cameraMatrix() const;
    void setCameraMatrix(const cv::Mat &cameraMatrix);

    cv::Mat cameraProjectionMatrix() const;
    void setCameraProjectionMatrix(const cv::Mat &cameraProjectionMatrix);

    cv::Mat IntrinsicMatrix() const;
    void setIntrinsicMatrix(const cv::Mat &IntrinsicMatrix);

    QVariantMap toVariantMap();
    void fromVariantMap(QVariantMap varMap);

public slots:
    void activeCam(bool active);

    void turnedOnCam(bool m_turnedOn);

    void thresholdCam(size_t threshold);

private:
    
    void GetUndisortedPosition();
    void UseFilter();
    void MiddleOfContours();
    void CreateLines();
    void ComputeDirVector();
    void NormalizeContours();
    void createExtrinsicMatrix();

    void computeAllDirections();

    void computeNewParameters();

signals:
    void imageRead(cv::Mat image);

};

#endif // CAPTURECAMERA_H
