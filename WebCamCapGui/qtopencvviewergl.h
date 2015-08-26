/*
 * Source Code from:
 * https://github.com/Myzhar/QtOpenCVViewerGl
 */

#ifndef QTOPENCVVIEWERGL_H
#define QTOPENCVVIEWERGL_H


#include <QWidget>
#include <QImage>
#include <QOpenGLWidget>
#include <opencv2/core/core.hpp>

/**
 * @author Walter Lucetti <a href="https://github.com/Myzhar">link GitHub</a>
 * @brief The QtOpenCVViewerGl class is effective widget for showing OpenCV::Mat/UMat
 */
class QtOpenCVViewerGl : public QOpenGLWidget
{
    Q_OBJECT
public:
    /**
     * @brief QtOpenCVViewerGl default constructor
     * @param parent
     */
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
