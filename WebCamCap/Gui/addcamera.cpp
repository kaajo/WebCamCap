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


#include "addcamera.h"
#include "ui_addcamera.h"

#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

using glm::vec2;
using glm::vec3;
using namespace cv;

AddCamera::AddCamera(QWidget *parent, vec3 roomDims) :
    QDialog(parent),
    ui(new Ui::AddCamera)
{
    ui->setupUi(this);
    this->setWindowTitle("Add new Camera");

    m_roomDimensions = roomDims;
}

AddCamera::~AddCamera()
{
    if(m_calibApplication)
    {
        delete m_calibApplication;
    }

    delete ui;
}

void AddCamera::readYaml(int )
{
    readConfigFile(ui->Name->text());
}

void AddCamera::on_buttonBox_accepted()
{
    if(m_camera)
    {
        delete m_camera;
    }

    m_camera = new CaptureCamera(vec2(ui->FrameCols->text().toInt(), ui->FrameRows->text().toInt()), vec3(ui->X->text().toFloat(),
                                 ui->Y->text().toFloat(),
                                 ui->Z->text().toFloat()),
                            m_roomDimensions,
                            ui->Name->text(),
                            ui->deviceUSB_ID->text().toInt(),
                            ui->zorny_uhol->text().toFloat(), ui->useBackgroundSub->isChecked());


    if(!m_coefficients.empty())
    {
        m_camera->setDistortionCoeffs(m_coefficients);
    }

    if(!m_cameraMatrix.empty())
    {
        m_camera->setCameraMatrix(m_cameraMatrix);
    }

    if(m_tooHighValueWarning)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(" ");
        msgBox.setText("Scene contains bright areas!");
        msgBox.setInformativeText("Do you want to ignore them in capture?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);

        int status = msgBox.exec();

        if(status == QMessageBox::Yes)
        {
            Mat ex = getStructuringElement(MORPH_ELLIPSE, Size(20,20));
            morphologyEx(m_mask, m_mask, MORPH_DILATE, ex);
            m_camera->setROI(255 - m_mask);
        }
        else if(status == QMessageBox::Cancel)
        {
            return;
        }
    }

    endRecording();

    accept();
}

void AddCamera::on_buttonBox_rejected()
{
    endRecording();
}

void AddCamera::on_Play_ID_clicked(bool checked)
{
    if(checked)
    {
        m_cameraRecording = true;

        if(!m_videoCaptureTemp.open(ui->deviceUSB_ID->text().toInt()))
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("");
            msgBox.warning(this, "", "Wrong device ID");
            msgBox.setFixedSize(200,100);

            ui->Play_ID->setChecked(false);
            ui->deviceUSB_ID->setEnabled(true);
            m_cameraRecording = false;
            return;
        }
        ui->FrameCols->setText(QString::number(m_videoCaptureTemp.get(CV_CAP_PROP_FRAME_WIDTH)));
        ui->FrameRows->setText(QString::number(m_videoCaptureTemp.get(CV_CAP_PROP_FRAME_HEIGHT)));

        recording();
    }
    else
    {
        endRecording();
    }
}

void AddCamera::recording()
{
    m_tooHighValueWarning = false;

    while(m_cameraRecording)
    {
        QCoreApplication::processEvents();

        m_videoCaptureTemp >> m_frame;
        m_mask = CaptureCamera::myColorThreshold(m_frame, 220, 255);

        for(int i = 0; i < m_frame.rows; i++)
        {
            for(int j = 0; j < m_frame.cols; j++)
            {
                if(m_mask.at<uchar>(i,j) != 0)
                {
                    m_frame.at<Vec3b>(i,j) = Vec3b(0,0,255);
                    m_tooHighValueWarning = true;
                }
            }
        }

        if(m_tooHighValueWarning)
        {
            ui->warning->setText("Some areas are too bright");
        }
        else
        {
            ui->warning->setText("");
        }

        ui->glImage->showImage(m_frame);

        cv::waitKey(10);
    }
}

void AddCamera::endRecording()
{
    if(m_cameraRecording)
    {
        m_cameraRecording = false;
        ui->warning->setText("");
        m_videoCaptureTemp.release();
    }
}

void AddCamera::readConfigFile(QString path)
{
    cv::FileStorage file(path.toStdString(), FileStorage::READ);

    file["camera_matrix"] >> m_cameraMatrix;
    file["distortion_coefficients"] >> m_coefficients;
}


void AddCamera::on_FrameCols_editingFinished()
{
    m_videoCaptureTemp.set(CV_CAP_PROP_FRAME_WIDTH, ui->FrameCols->text().toInt());
}

void AddCamera::on_FrameRows_editingFinished()
{
    m_videoCaptureTemp.set(CV_CAP_PROP_FRAME_HEIGHT, ui->FrameRows->text().toInt());
}

void AddCamera::on_pushButton_clicked()
{
    QStringList arguments;

    QString usbid = ui->deviceUSB_ID->text();
    if(usbid == "")
    {
        usbid = "0";
    }

    arguments << usbid;
    arguments << "-w" << "7" << "-h" << "5" << "-pt" << "chessboard" << "-n" << "50";
    arguments << "-o" << ui->Name->text() + ".yaml";

    m_calibApplication = new QProcess(this);
    m_calibApplication->start(QDir::currentPath() + "/Calib",  arguments);

    connect(m_calibApplication, SIGNAL(finished(int)), this, SLOT(readYaml(int)));
}

void AddCamera::on_readYAML_clicked()
{

    QString filename = QFileDialog::getOpenFileName(this,tr("Load Distortion Coefficients"), ".", tr(".yaml Files (*.yaml)"));

    if(filename != "")
    {
        std::cout << filename.toStdString() << std::endl;

        readConfigFile(filename);
    }
}
