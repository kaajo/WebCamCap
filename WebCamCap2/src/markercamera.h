/*
 *
 * Copyright (C) 2015  Miroslav Krajicek (https://github.com/kaajo).
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

#ifndef MARKERCAMERA_H
#define MARKERCAMERA_H

#include "icamera.h"

#include <QVector2D>
#include <QMutex>
#include <opencv2/core.hpp>

typedef std::vector<cv::Point> Contour;

class MarkerCamera : public ICamera
{
    Q_OBJECT

    cv::VideoCapture m_camera;
    cv::UMat m_actualFrame, m_filteredFrame;

    size_t m_thresholdValue = 255;
    cv::Mat m_background;

    QVector<Line> m_lines;


    std::vector<Contour> m_contours;

    QMutex m_mutex;

    bool m_running = false;

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

    virtual void stopWork() override;
    virtual void startWork() override;

    void doWork();

    void calibBackground();
    void calibThreshold();

    ///computation

    void applyFilters();
    void createLineForComponents();

    inline static Line qtConcurrentpickLine(MarkerCamera *camera, const Contour &contour);

};

#endif // MARKERCAMERA_H
