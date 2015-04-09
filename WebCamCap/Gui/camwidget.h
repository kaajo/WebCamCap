/*
 * Source Code from:
 * http://www.robot-home.it/blog/en/software/tutorial-opencv-qt-opengl-widget-per-visualizzare-immagini-da-opencv-in-una-gui-con-qt/
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
