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

#ifndef ADDCAMERA_H
#define ADDCAMERA_H

#include <QDialog>
#include <QProcess>

#include <opencv2/highgui/highgui.hpp>

#include "../capturecamera.h"

namespace Ui {
class AddCamera;
}

class AddCamera : public QDialog
{
    Q_OBJECT

    CaptureCamera *m_camera = nullptr;
    cv::VideoCapture m_videoCaptureTemp;
    glm::vec3 m_roomDimensions;
    cv::Mat m_frame, m_mask;
    bool m_cameraRecording = false;
    bool m_tooHighValueWarning = false;

    QProcess *m_calibApplication = nullptr;

    cv::Mat m_cameraMatrix;
    cv::Mat m_coefficients;

public:
    explicit AddCamera(QWidget *parent = 0, glm::vec3 m_roomDimensions = glm::vec3(0.0f,0.0f, 0.0f));
    ~AddCamera();

    CaptureCamera* getCam() const {return m_camera;}

private slots:

    void readYaml(int);

    void on_buttonBox_accepted();

    void on_Play_ID_clicked(bool checked);

    void on_buttonBox_rejected();

    void on_FrameCols_editingFinished();

    void on_FrameRows_editingFinished();

    void on_readYAML_clicked();

    void on_calibButton_clicked();

private:
    Ui::AddCamera *ui;
    void recording();
    void endRecording();

    void readConfigFile(QString path);
};

#endif // ADDCAMERA_H
