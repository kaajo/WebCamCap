#include "openglscene.h"

#include <GL/glu.h>
#include <GL/gl.h>
#include <QGL>
#include <QDebug>
#include <QMouseEvent>

OpenGlScene* OpenGlScene::m_scene = nullptr;

OpenGlScene::OpenGlScene(QWidget *parent) : QOpenGLWidget(parent)
{
    generateRandomColors(100);
    update();
}

void OpenGlScene::setRoomDimensions(QVector3D roomDims)
{
    m_roomDims = roomDims;

    update();
}

void OpenGlScene::setCamerasSettings(QVector<std::shared_ptr<CameraSettings>> settings)
{
    m_camSettings = settings;

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
                -m_roomDims.x()*m_zoom, 200*m_zoom, m_roomDims.y()*m_zoom,
                m_roomDims.x()/2.0f, m_roomDims.z()/2.0f , -m_roomDims.y()/2.0f,
                0, 1, 0
                );

    glTranslatef(m_roomDims.x()/2.0f, m_roomDims.z()/2.0f, -m_roomDims.y()/2.0f);
    glRotatef(m_camRot.y(), 1, 0, 0);
    glRotatef(m_camRot.x(), 0, 1, 0);
    glTranslatef(-m_roomDims.x()/2.0f, -m_roomDims.z()/2.0f, m_roomDims.y()/2.0f);

    paintScene();
    paintCameras();
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

void OpenGlScene::setFrame(Frame frame)
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

void OpenGlScene::paintCameras()
{
    glColor3ub(255, 0, 0);

    for(std::shared_ptr<CameraSettings> settings: m_camSettings)
    {
        glPushMatrix();

        glTranslatef(settings->globalPosition().x(),
                     settings->globalPosition().z(),
                     -settings->globalPosition().y());

        glRotatef(180, 0, 0, 1);
        glRotatef(90, 0, 1, 0);

        glMultMatrixf(settings->getRotationMatrix().constData());

        glPushMatrix();
        glRotatef(-90, 0, 1, 0);
        gluCylinder(m_cubeQuadric, 5, 0, 7, 10, 1);
        glPopMatrix();

        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(qMax(m_roomDims.y(), m_roomDims.x())/5, 0, 0);
        glEnd();

        glTranslatef(-settings->globalPosition().x(),
                     -settings->globalPosition().z(),
                     settings->globalPosition().y());

        glPopMatrix();
    }
}

void OpenGlScene::paintFrame()
{
    int markersCount = m_actualFrame.markers().size();

    if(m_randomColors.size() < markersCount)
    {
        generateRandomColors(markersCount);
    }

    glPushMatrix();
    for(int i = 0; i < markersCount; i++)
    {
        glColor3ub(m_randomColors[m_actualFrame.markers()[i].id()].red()
        ,m_randomColors[m_actualFrame.markers()[i].id()].green(),
        m_randomColors[m_actualFrame.markers()[i].id()].blue());

        glTranslatef(m_actualFrame.markers()[i].position().x()*m_roomDims.x(),
                     m_actualFrame.markers()[i].position().z()*m_roomDims.z(),
                     -m_actualFrame.markers()[i].position().y()*m_roomDims.y());

        gluSphere(m_sphereQuadric, 3, 5, 5);

        glTranslatef(-m_actualFrame.markers()[i].position().x()*m_roomDims.x(),
                     -m_actualFrame.markers()[i].position().z()*m_roomDims.z(),
                     m_actualFrame.markers()[i].position().y()*m_roomDims.y());

    }
    glPopMatrix();

    glColor3f(1.0f,0.5f,0.0f);

    const auto &lines = m_actualFrame.lines();

    for(int i = 0; i < lines.size(); i++)
    {
        for(int j = 0; j < lines[i].size(); j++)
        {
            drawLine(lines[i][j]);
        }
    }
}

void OpenGlScene::drawLine(const Line &l) const
{
    glBegin(GL_LINES);
    glVertex3f(l.position().x(), l.position().z(), -l.position().y());
    glVertex3f(l.position().x()+2*l.direction().x(), l.position().z()+2*l.direction().z(), -1*(l.position().y()+2*l.direction().y()));
    glEnd();
}

void OpenGlScene::generateRandomColors(int count)
{
    int size = m_randomColors.size();

    for(int i = 0; i < count - size; i++)
    {
        m_randomColors.push_back({qrand() % 255, qrand() % 255, qrand() % 255});
    }
}

void OpenGlScene::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_leftButton = true;

    }
    else if(event->button() == Qt::RightButton)
    {
        m_rightButton = true;
    }

    setCursor({Qt::DragMoveCursor});
    m_currentMousePos = event->pos();
    event->accept();
}

void OpenGlScene::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_leftButton = false;
    }
    else if(event->button() == Qt::RightButton)
    {
        m_rightButton = false;
    }

    if(!m_leftButton && !m_rightButton)
    {
        setCursor({Qt::OpenHandCursor});
    }

    event->accept();
}

void OpenGlScene::mouseMoveEvent(QMouseEvent *event)
{
    m_lastMousePos = m_currentMousePos;
    m_currentMousePos = event->pos();

    if(m_leftButton)
    {
        m_camRot += (m_currentMousePos - m_lastMousePos);
    }
    else if(m_rightButton)
    {
        m_zoom += m_zoom / 50.0f * (m_currentMousePos - m_lastMousePos).y();
    }

    event->accept();
    update();
}
