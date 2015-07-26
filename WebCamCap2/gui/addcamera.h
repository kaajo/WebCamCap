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

#ifndef ADDCAMERA_H
#define ADDCAMERA_H

#include "qtopencvviewergl.h"
#include "camerasettings.h"

#include <QDialog>
#include <QProcess>
#include <QVector3D>

#include <opencv2/highgui/highgui.hpp>

#include <memory>

namespace Ui {
class AddCamera;
}

class AddCamera : public QDialog
{
    Q_OBJECT

    std::shared_ptr<CameraSettings> m_cameraSettings = nullptr;
    cv::VideoCapture m_videoCaptureTemp;
    cv::Mat m_frame, m_mask;
    bool m_cameraRecording = false;
    bool m_tooHighValueWarning = false;
    QVector3D m_roomDims;
    QProcess *m_calibApplication = nullptr;

    cv::Mat m_cameraMatrix;
    cv::Mat m_coefficients;

public:
    explicit AddCamera(QVector3D m_roomDimensions, QWidget *parent = 0);
    ~AddCamera();

    std::shared_ptr<CameraSettings> cameraSettings() const;
    void setCameraSettings(std::shared_ptr<CameraSettings> cameraSettings);

private slots:

    void readYaml(int);

    void on_buttonBox_accepted();

    void on_Play_ID_clicked(bool checked);

    void on_buttonBox_rejected();

    void on_FrameX_editingFinished();

    void on_FrameY_editingFinished();

    void on_readYAML_clicked();

    void on_calibButton_clicked();

private:
    Ui::AddCamera *m_ui;
    void recording();
    void endRecording();

    void readConfigFile(QString path);
};

#endif // ADDCAMERA_H
