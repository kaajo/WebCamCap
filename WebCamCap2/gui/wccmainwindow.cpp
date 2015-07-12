#include "wccmainwindow.h"
#include "ui_wccmainwindow.h"

#include "projectwizard.h"
#include "addcamera.h"
#include "openglscene.h"

#include <QDebug>
#include <QHeaderView>

WccMainWindow::WccMainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::WccMainWindow)
{
    m_ui->setupUi(this);

    ///setup opengl scene
    OpenGlScene *scene = OpenGlScene::getInstance();
    m_ui->openGlLayout->addWidget(scene, 1);
    scene->setMinimumSize(300, 100);
    scene->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    this->resize(sizeHint());

    ///connect actions
    connect(m_ui->actionNewProject , &QAction::triggered, this, &WccMainWindow::newProject);
    connect(m_ui->actionOpenProject, &QAction::triggered, this, &WccMainWindow::openProject);
    connect(m_ui->actionEditCurrentProject, &QAction::triggered, this, &WccMainWindow::editCurrentProject);
    connect(m_ui->actionSaveCurrentProject, &QAction::triggered, this, &WccMainWindow::saveCurrentProject);
    connect(m_ui->actionAddServer, &QAction::triggered, this, &WccMainWindow::addNewServer);
    connect(m_ui->actionAbout, &QAction::triggered, this, &WccMainWindow::showAboutPage);

    m_ui->AnimationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);

    ///scrollArea needs widget with layout inside
    m_scrollWidget = new QWidget;
    m_scrollWidget->setLayout(new QVBoxLayout);
    m_ui->cameraScrollArea->setWidget(m_scrollWidget);
}

WccMainWindow::~WccMainWindow()
{
    delete m_ui;
}

bool WccMainWindow::addServer(IServer *server)
{
    QString name = server->name();

    auto it = std::find_if(m_servers.begin(), m_servers.end(), [name](IServer *s){return s->name() == name;});

    if(it == m_servers.end())
    {
        m_servers.push_back(server);
        return true;
    }
    else
    {
        return false;
    }
}

bool WccMainWindow::removeServer(QString name)
{
    auto it = std::find_if(m_servers.begin(), m_servers.end(), [name](IServer *s){return s->name() == name;});

    return true;
}

void WccMainWindow::setProject(IVirtualRoom *project)
{
    this->setEnabled(true);

    if(m_currentProject)
    {
        ///handle save, deactivate etc
        ///
        m_currentProject->disconnect();
    }

    m_currentProject = project;
}

void WccMainWindow::newProject()
{
    RoomSettings *settings= new RoomSettings;

    ProjectWizard *wizard = new ProjectWizard();
    wizard->setSettings(settings);

    int accepted = wizard->exec();

    if(accepted == ProjectWizard::Accepted)
    {
        setProject(wizard->project());

        connect(m_ui->recordScene, &QPushButton::clicked, m_currentProject->settings(), &RoomSettings::setRecordScene);
        connect(m_ui->recordAnimation, &QPushButton::clicked, m_currentProject->settings(), &RoomSettings::setRecordAnimation);
        connect(m_currentProject->cameraTopology(), &ICameraTopology::frameReady, OpenGlScene::getInstance(), &OpenGlScene::setFrame);
        connect(m_currentProject, &IVirtualRoom::animationRecorded, this, &WccMainWindow::addAnimationToTable);
    }
}

void WccMainWindow::openProject()
{

}

void WccMainWindow::editCurrentProject()
{

}

void WccMainWindow::saveCurrentProject()
{

}

void WccMainWindow::addNewServer()
{

}

void WccMainWindow::showAboutPage()
{
}

void WccMainWindow::addAnimationToTable(Animation *animation)
{

}
