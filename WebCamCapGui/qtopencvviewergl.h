/*
 * Source Code from:
 * http://www.robot-home.it/blog/en/software/tutorial-opencv-qt-opengl-widget-per-visualizzare-immagini-da-opencv-in-una-gui-con-qt/
 */

#ifndef QTOPENCVVIEWERGL_H
#define QTOPENCVVIEWERGL_H


#include <QWidget>
#include <QImage>
#include <QOpenGLWidget>
#include <opencv2/core/core.hpp>

class QtOpenCVViewerGl : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit QtOpenCVViewerGl(QWidget *parent = 0);

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
    bool        mSceneChanged = false;          /// Indicates when OpenGL view is to be redrawn

    QImage      mRenderQtImg;           /// Qt image to be rendered
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown

    QColor      mBgColor;		/// Background color

    int         mOutH = 0;                  /// Resized Image height
    int         mOutW = 0;                  /// Resized Image width
    float       mImgRatio = 4.0f/3.0f;             /// height/width ratio

    int         mPosX = 0;                  /// Top left X position to render image in the center of widget
    int         mPosY = 0;                  /// Top left Y position to render image in the center of widget

    cv::Mat m;

};

#endif // QTOPENCVVIEWERGL_H
