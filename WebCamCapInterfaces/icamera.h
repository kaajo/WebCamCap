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

#ifndef ICAMERA_H
#define ICAMERA_H

#include "webcamcapinterfaces_global.h"

#include "line.h"
#include "camerasettings.h"

#include <opencv2/core/core.hpp>

#include <QVector>
#include <QVariantMap>
#include <QObject>

class WEBCAMCAPINTERFACESSHARED_EXPORT ICamera : public QObject
{
    Q_OBJECT

protected:
    CameraSettings *m_settings = nullptr;

public:
    explicit ICamera(CameraSettings *settings, QObject *parent = 0);
    virtual ~ICamera();

    enum class CameraCalibrationType {
        BACKGROUND,
        THRESHOLD
    };

    virtual QVector<Line> nextFrame() = 0;
    virtual QVector<QVector3D> nextFrame2D() = 0;
    virtual void calibrate(CameraCalibrationType type) = 0;


    CameraSettings *settings() const;
    void setSettings(CameraSettings *settings);

    virtual QVariantMap toVariantMap() const = 0;
    virtual void fromVariantMap(QVariantMap varMap) = 0;
signals:
    void imageShow(cv::UMat image);

public slots:
    virtual void showPreviewInWindow(bool show) = 0;
    virtual bool turnOn(bool turnOn) = 0;
    virtual void setThreshold(size_t threshold) = 0;

private slots:
    virtual void settingsChanged(CameraSettings::CameraSettingsType type) = 0;
};

#endif // ICAMERA_H
