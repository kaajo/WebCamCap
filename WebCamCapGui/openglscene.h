#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include "webcamcapgui_global.h"

#include "frame.h"

#include <QColor>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QGLWidget>
#include <GL/glu.h>

class WEBCAMCAPGUISHARED_EXPORT OpenGlScene : public QGLWidget
{
    static OpenGlScene *m_scene;

    QVector3D m_roomDims = QVector3D(100,100,100);

    //paint props
    QVector<QColor> randomColors;
    bool mdrawJoints = true;
    bool mdrawLines = true;

    QVector3D camRot = QVector3D(0,0,0);
    float zoom = 1.0;

    //mouse tracking
    bool leftButton = false;
    QVector2D lastMousePos, currentMousePos;

    Frame m_actualFrame;

    //some glu mess
    GLUquadricObj *m_quadric = gluNewQuadric();
public:
    explicit OpenGlScene(QWidget *parent = 0);
    void setRoomDimensions(QVector3D &m_roomDims);

    static OpenGlScene *getInstance();

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

public slots:
    void setFrame(Frame &frame);

private:
    void paintScene();
    void paintFrame();

};

#endif // OPENGLSCENE_H
