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

#ifndef CAMWIDGET_H
#define CAMWIDGET_H

#include <QWidget>
#include <QImage>
#include <QtOpenGL/QGLWidget>
#include <opencv2/core/core.hpp>

class CQtOpenCVViewerGl : public QGLWidget
{
    Q_OBJECT
public:
    explicit CQtOpenCVViewerGl(QWidget *parent = 0);

signals:
    void        imageSizeChanged( int outW, int outH ); /// Used to resize the image outside the widget

public slots:
    void        showImage( cv::Mat image ); /// Used to set the image to be viewed

protected:
    void        initializeGL(); /// OpenGL initialization
    void        paintGL(); /// OpenGL Rendering
    void        resizeGL(int width, int height);        /// Widget Resize Event

    void        updateScene();
    void        renderImage();

private:
    bool        mSceneChanged;          /// Indicates when OpenGL view is to be redrawn

    QImage      mRenderQtImg;           /// Qt image to be rendered
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown

    QColor      mBgColor;		/// Background color

    int         mOutH;                  /// Resized Image height
    int         mOutW;                  /// Resized Image width
    float       mImgRatio;             /// height/width ratio

    int         mPosX;                  /// Top left X position to render image in the center of widget
    int         mPosY;                  /// Top left Y position to render image in the center of widget

    cv::Mat m;

};


namespace Ui {
class CamWidget;
}

class CamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CamWidget(QWidget *parent = 0);

    CQtOpenCVViewerGl *getImageViewer() const;
    ~CamWidget();

    void setCheckTurnedOn(bool checked);
    void setCheckActive(bool active);
    void setThreshold(size_t threshold);

signals:
    void activeCam(bool active);

    void turnedOnCam(bool turnedOn);

    void thresholdCam(size_t threshold);

private slots:
    void on_ShowVideo_stateChanged(int arg1);

    void on_UseCamera_stateChanged(int arg1);

    void on_Threshold_valueChanged(int arg1);

private:
    Ui::CamWidget *ui;
};

#endif // CAMWIDGET_H
