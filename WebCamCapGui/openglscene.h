#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include <QColor>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QGLWidget>

class OpenGlScene : public QGLWidget
{
    static OpenGlScene *m_scene;

    //paint props
    QVector<QColor> randomColors;
    bool mdrawJoints = true;
    bool mdrawLines = true;

    QVector3D camRot;
    float zoom = 1.0;

    //mouse tracking
    bool leftButton = false;
    QVector2D lastMousePos, currentMousePos;

public:
    explicit OpenGlScene(QWidget *parent = 0);

    static OpenGlScene *getInstance();

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
};

#endif // OPENGLSCENE_H
