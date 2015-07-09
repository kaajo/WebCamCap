#include "openglscene.h"

#include <GL/glu.h>
#include <QGL>

OpenGlScene* OpenGlScene::m_scene = nullptr;

OpenGlScene::OpenGlScene(QWidget *parent) : QOpenGLWidget(parent)
{
    update();
}

void OpenGlScene::setRoomDimensions(QVector3D &roomDims)
{
    m_roomDims = roomDims;

    update();
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

    glClearColor(0.15f, 0.15f, 0.15f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(
                -m_roomDims.x()*zoom, 200*zoom, m_roomDims.y()*zoom,
                 m_roomDims.x()/2.0f, m_roomDims.z()/2.0f , -m_roomDims.y()/2.0f,
                 0, 1, 0
             );

    glTranslatef(m_roomDims.x()/2.0f, m_roomDims.z()/2.0f, -m_roomDims.y()/2.0f);
    glRotatef(camRot.x(), 1, 0, 0);
    glRotatef(camRot.y(), 0, 1, 0);
    glTranslatef(-m_roomDims.x()/2.0f, -m_roomDims.z()/2.0f, m_roomDims.y()/2.0f);

    paintScene();
    paintFrame();

}

void OpenGlScene::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) w/ (float) h, 0.1, 10000);
    glMatrixMode(GL_MODELVIEW);
}

void OpenGlScene::setFrame(Frame &frame)
{
    m_actualFrame = frame;

    update();
}

void OpenGlScene::paintScene()
{
    glColor3f(0.2, 0.1, 0.7);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0); glVertex3d(0, -1, 0);
    glTexCoord2f(1,0); glVertex3d(m_roomDims.x(), -1, 0);
    glTexCoord2f(1,1); glVertex3d(m_roomDims.x(), -1, -m_roomDims.y());
    glTexCoord2f(0,1); glVertex3d(0, -1, -m_roomDims.y());
    glEnd();
}

void OpenGlScene::paintFrame()
{
    glPushMatrix();
    for(int i = 0; i < m_actualFrame.markers().size(); i++)
    {
        glColor3ub(randomColors[m_actualFrame.markers()[i].id()].red(),randomColors[m_actualFrame.markers()[i].id()].green(),randomColors[m_actualFrame.markers()[i].id()].blue());

        //glColor(randomColors[m_actualFrame.markers()[i].id()]);

        glTranslatef(m_actualFrame.markers()[i].position().x()*m_roomDims.x(), m_actualFrame.markers()[i].position().z()*m_roomDims.z(), -m_actualFrame.markers()[i].position().y()*m_roomDims.y());
        gluSphere(m_quadric, 3,8,8);
        glTranslatef(-m_actualFrame.markers()[i].position().x()*m_roomDims.x(), -m_actualFrame.markers()[i].position().z()*m_roomDims.z(), m_actualFrame.markers()[i].position().y()*m_roomDims.y());

    }
    glPopMatrix();
}
