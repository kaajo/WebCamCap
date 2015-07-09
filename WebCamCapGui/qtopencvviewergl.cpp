#include "qtopencvviewergl.h"

#include <QGLWidget>

#include <opencv2/imgproc.hpp>

QtOpenCVViewerGl::QtOpenCVViewerGl(QWidget *parent) :
    QOpenGLWidget(parent)
{
}

void QtOpenCVViewerGl::initializeGL()
{
    mSceneChanged = true;

    showImage(cv::Mat::zeros(480, 640, 16));

    updateScene();
}

void QtOpenCVViewerGl::resizeGL(int width, int height)
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

void QtOpenCVViewerGl::updateScene()
{
    if( mSceneChanged && this->isVisible() )
    {
        update();
    }
}

void QtOpenCVViewerGl::paintGL()
{
    makeCurrent();

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);

    if(mSceneChanged)
        renderImage();

    mSceneChanged = false;
}

void QtOpenCVViewerGl::renderImage()
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

void QtOpenCVViewerGl::showImage( cv::Mat image )
{
    cv::circle(image, cv::Point(image.cols/2, image.rows/2), 1, CV_RGB(0,255,0), 2);

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

