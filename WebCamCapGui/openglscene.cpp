#include "openglscene.h"

OpenGlScene::OpenGlScene(QWidget *parent) : QGLWidget(parent)
{
}

OpenGlScene *OpenGlScene::getInstance()
{
    if(!m_scene)
    {
        m_scene = new OpenGlScene();
        return m_scene;
    }
    else
    {
        return m_scene;
    }
}

void OpenGlScene::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glShadeModel(GL_SMOOTH);
    glRenderMode(GL_RENDER);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
}

void OpenGlScene::paintGL()
{
    /*
    glClearColor(0.15f, 0.15f, 0.15f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(
                -roomDims.x*zoom, 200*zoom, roomDims.y*zoom,
                 roomDims.x/2.0f, roomDims.z/2.0f , -roomDims.y/2.0f,
                 0, 1, 0
                    );

    glTranslatef(roomDims.x/2.0f, roomDims.z/2.0f, -roomDims.y/2.0f);
    glRotatef(camRot.x, 1, 0, 0);
    glRotatef(camRot.y, 0, 1, 0);
    glTranslatef(-roomDims.x/2.0f, -roomDims.z/2.0f, roomDims.y/2.0f);

    if(mdrawJoints)
    {
        drawJoints();
    }

    if(mdrawLines)
    {
        drawLines();
    }

    drawFloor();
    */
}

void OpenGlScene::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(60, (float) w/ (float) h, 0.1, 10000);

    glMatrixMode(GL_MODELVIEW);
}
