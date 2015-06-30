/*
 * Source Code from:
 * http://www.robot-home.it/blog/en/software/tutorial-opencv-qt-opengl-widget-per-visualizzare-immagini-da-opencv-in-una-gui-con-qt/
 */

#include <opencv2/imgcodecs.hpp>

#include "camwidget.h"
#include "ui_camwidget.h"

#include <iostream>
#include <QDir>

CQtOpenCVViewerGl::CQtOpenCVViewerGl(QWidget *parent) :
    QGLWidget(parent)
{
}

void CQtOpenCVViewerGl::initializeGL()
{
    mSceneChanged = true;

    showImage(cv::Mat::zeros(480, 640, 16));

    updateScene();
}

void CQtOpenCVViewerGl::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, 0, height, 0, 1);	// To Draw image in the center of the area

    glMatrixMode(GL_MODELVIEW);

    // ---> Scaled Image Sizes
    mOutH = width/mImgRatio;
    mOutW = width;

    if(mOutH>height)
    {
        mOutW = height*mImgRatio;
        mOutH = height;
    }

    emit imageSizeChanged( mOutW, mOutH );
    // <--- Scaled Image Sizes

    mPosX = (width-mOutW)/2;
    mPosY = (height-mOutH)/2;

    mSceneChanged = true;

    updateScene();
}

void CQtOpenCVViewerGl::updateScene()
{
    if( mSceneChanged && this->isVisible() )
    {
        updateGL();
    }
}

void CQtOpenCVViewerGl::paintGL()
{
    makeCurrent();

    qglClearColor(Qt::white);
    glClear(GL_COLOR_BUFFER_BIT);

    if(mSceneChanged)
        renderImage();

    mSceneChanged = false;
}

void CQtOpenCVViewerGl::renderImage()
{
    makeCurrent();

    if (!mRenderQtImg.isNull())
    {
        glLoadIdentity();

        QImage image; // the image rendered

        glPushMatrix();
        {
            int imW = mRenderQtImg.width();
            int imH = mRenderQtImg.height();

            // The image is to be resized to fit the widget?
            if( imW != this->size().width() &&
                    imH != this->size().height() )
            {

                image = mRenderQtImg.scaled( //this->size(),
                                             QSize(mOutW,mOutH),
                                             Qt::IgnoreAspectRatio,
                                             Qt::SmoothTransformation
                                             );

                //qDebug( QString( "Image size: (%1x%2)").arg(imW).arg(imH).toAscii() );
            }
            else
                image = mRenderQtImg;

            // ---> Centering image in draw area

            glRasterPos2i( mPosX, mPosY );
            // <--- Centering image in draw area

            image = image.mirrored(true,false);

            imW = image.width();
            imH = image.height();

            glDrawPixels( imW, imH, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
        }
        glPopMatrix();

        // end
        glFlush();
    }
}

void CQtOpenCVViewerGl::showImage( cv::Mat image )
{

    image.copyTo(mOrigImage);

    mImgRatio = (float)image.cols/(float)image.rows;

    if( mOrigImage.channels() == 3)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_RGB888).rgbSwapped();
    else if( mOrigImage.channels() == 1)
        mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                              mOrigImage.cols, mOrigImage.rows,
                              mOrigImage.step, QImage::Format_Indexed8);
    else
        return;

    mRenderQtImg = QGLWidget::convertToGLFormat(mRenderQtImg);

    mSceneChanged = true;

    updateScene();

    return;
}

CamWidget::CamWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CamWidget)
{
    ui->setupUi(this);
    ui->Threshold->setValue(255);
}

CQtOpenCVViewerGl *CamWidget::getImageViewer() const
{
    return ui->glimage;
}

CamWidget::~CamWidget()
{
    delete ui;
}

void CamWidget::setCheckTurnedOn(bool checked)
{
    if(checked)
    {
        ui->UseCamera->setCheckState(Qt::Checked);
    }
    else
    {
        ui->UseCamera->setCheckState(Qt::Unchecked);
    }
}

void CamWidget::setCheckActive(bool active)
{
    if(active)
    {
        ui->ShowVideo->setCheckState(Qt::Checked);
    }
    else
    {
        ui->ShowVideo->setCheckState(Qt::Unchecked);
    }
}

void CamWidget::setThreshold(size_t threshold)
{
    ui->Threshold->setValue(threshold);
}

void CamWidget::on_ShowVideo_stateChanged(int arg1)
{
    if(sender() != this)
        emit activeCam(arg1);
}

void CamWidget::on_UseCamera_stateChanged(int arg1)
{
    if(sender() != this)
        emit turnedOnCam(arg1);
}

void CamWidget::on_Threshold_valueChanged(int arg1)
{
    if(sender() != this)
        emit thresholdCam(arg1);
}
