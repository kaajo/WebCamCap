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


std::shared_ptr<CameraSettings> AddCamera::cameraSettings() const
{
    return m_cameraSettings;
}

void AddCamera::setCameraSettings(std::shared_ptr<CameraSettings> cameraSettings)
{
    if(!cameraSettings)
    {
        return;
    }

    m_cameraSettings = cameraSettings;

    m_ui->name->setText(m_cameraSettings.get()->name());
    m_ui->usbId->setText(QString::number(m_cameraSettings.get()->videoUsbId()));
    m_ui->diagonalFov->setText(QString::number(m_cameraSettings.get()->diagonalFov()));

    QVector3D globalPosition = m_cameraSettings.get()->globalPosition();

    m_ui->X->setText(QString::number(globalPosition.x()));
    m_ui->Y->setText(QString::number(globalPosition.y()));
    m_ui->Z->setText(QString::number(globalPosition.z()));

    QVector2D frameResolution = m_cameraSettings.get()->resolution();

    m_ui->resolution->setText(QString::number(frameResolution.y()) + "X" + QString::number(frameResolution.x()));
}

AddCamera::AddCamera(QVector3D roomDims, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AddCamera)
{
    m_ui->setupUi(this);
    this->setWindowTitle("Add new Camera");

    m_roomDims = roomDims;
}

AddCamera::~AddCamera()
{
    if(m_calibApplication)
    {
        delete m_calibApplication;
    }

    delete m_ui;
}

void AddCamera::readYaml(int )
{
    readConfigFile(m_ui->name->text());
}

void AddCamera::on_buttonBox_accepted()
{    
    QVector3D globalPosition = QVector3D(m_ui->X->text().toFloat(), m_ui->Y->text().toFloat(), m_ui->Z->text().toFloat());
    QVector2D resolution = QVector2D(m_frame.rows, m_frame.cols);

    if(!m_cameraSettings)
    {
        m_cameraSettings = std::make_shared<CameraSettings>(m_ui->name->text(), m_ui->usbId->text().toInt(),
                                              m_ui->diagonalFov->text().toFloat(), globalPosition, m_roomDims);
    }
    else
    {
        m_cameraSettings.get()->setName(m_ui->name->text());
        m_cameraSettings.get()->setVideoUsbId(m_ui->usbId->text().toInt());
        m_cameraSettings.get()->setDiagonalFov(m_ui->diagonalFov->text().toFloat());
        m_cameraSettings.get()->setGlobalPosition(globalPosition);
        m_cameraSettings.get()->setRoomDimensions(m_roomDims);
    }

    m_cameraSettings.get()->setResolution(resolution);

    if(!m_coefficients.empty())
    {
        m_cameraSettings.get()->setDistortionCoeffs(m_coefficients);
    }

    if(!m_cameraMatrix.empty())
    {
        //m_cameraSettings.setCameraMatrix(m_cameraMatrix);
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
            cv::Mat ex = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(20,20));
            cv::morphologyEx(m_mask, m_mask, cv::MORPH_DILATE, ex);
            m_cameraSettings->setuseRoi(true);
            m_cameraSettings->setRoiMask(255 - m_mask);
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
        bool isNumber = false;

        size_t usbId = m_ui->usbId->text().toInt(&isNumber);

        if(m_ui->usbId->text().isEmpty() || !isNumber)
        {
            QMessageBox::warning(this, "No device ID specified",
                                       "Couldn't turn on camera, because no device ID is specified");
            m_ui->Play_ID->setChecked(false);
            m_ui->usbId->setEnabled(true);
            return;
        }

        m_cameraRecording = true;
        m_videoCaptureTemp.open(usbId);

        if(! m_videoCaptureTemp.isOpened())
        {
            QMessageBox::warning(this, "Wrong device ID",
                                       "Specified device ID is wrong, camera could not be opened");

            m_ui->Play_ID->setChecked(false);
            m_ui->usbId->setEnabled(true);
            m_cameraRecording = false;
            return;
        }

        recording();
    }
    else
    {
        endRecording();
    }
}

void AddCamera::recording()
{
    while(m_cameraRecording)
    {
        m_tooHighValueWarning = false;

        m_videoCaptureTemp >> m_frame;

        m_ui->resolution->setText(QString::number(m_frame.cols) + "X" + QString::number(m_frame.rows));

        /*
        cv::threshold(m_frame, m_mask, 220, 255, cv::THRESH_BINARY);

        for(int i = 0; i < m_frame.rows; i++)
        {
            for(int j = 0; j < m_frame.cols; j++)
            {
                if(m_mask.at<uchar>(i,j) != 0)
                {
                    m_frame.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,255);
                    m_tooHighValueWarning = true;
                    goto endOfLoop;
                }
            }
        }

        endOfLoop:

        if(m_tooHighValueWarning)
        {
            m_ui->warning->setText("Some areas are too bright");
        }
        else
        {
            m_ui->warning->setText("");
        }
        */

        m_ui->glImage->showImage(m_frame);

        cv::waitKey(10);
        QCoreApplication::processEvents();
    }
}

void AddCamera::endRecording()
{
    if(m_cameraRecording)
    {
        m_cameraRecording = false;
        //m_ui->warning->setText("");
        m_videoCaptureTemp.release();
    }
}

void AddCamera::readConfigFile(QString path)
{
    cv::FileStorage file(path.toStdString(), cv::FileStorage::READ);

    file["camera_matrix"] >> m_cameraMatrix;
    file["distortion_coefficients"] >> m_coefficients;
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

void AddCamera::on_calibButton_clicked()
{
    QStringList arguments;

    QString usbid = m_ui->usbId->text();
    if(usbid == "")
    {
        usbid = "0";
    }

    arguments << usbid;
    arguments << "-w" << "7" << "-h" << "5" << "-pt" << "chessboard" << "-n" << "25";
    arguments << "-o" << m_ui->name->text() + ".yaml";

    QString path = QDir::currentPath() + "/Calib/Calib";

    QFile file(path);

    if(!file.exists())
    {
        qDebug() << "calib program does not exist";
    }

    m_calibApplication = new QProcess(this);
    m_calibApplication->start(path,  arguments);

    connect(m_calibApplication, SIGNAL(finished(int)), this, SLOT(readYaml(int)));
}
