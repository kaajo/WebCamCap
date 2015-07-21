#include "wccmainwindow.h"
#include "ui_wccmainwindow.h"

#include "aboutwidget.h"
#include "camerawidget.h"
#include "projectwizard.h"
#include "addcamera.h"
#include "openglscene.h"

#include <QDebug>
#include <QCloseEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QDataStream>

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
    auto scrollWidget = new QWidget;

    m_scrollLayout = new QVBoxLayout(scrollWidget);
    scrollWidget->setLayout(m_scrollLayout);
    m_ui->cameraScrollArea->setWidget(scrollWidget);
}

WccMainWindow::~WccMainWindow()
{
    delete m_ui;
}

void WccMainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();

    if(m_currentProject)
    {
        delete m_currentProject;
    }
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
    if(m_currentProject == project)
    {
        return;
    }

    this->setEnabled(true);

    if(m_currentProject)
    {
        ///handle save, deactivate etc
        ///
        m_currentProject->disconnect();

        ///clean cam widgets
        clearCameraWidgets();

        if(project != m_currentProject)
        {
            delete m_currentProject;
        }
    }

    m_currentProject = project;

    addCameraWidgets(m_currentProject->cameraTopology()->getCameras());

    connect(m_ui->recordScene, &QPushButton::clicked, m_currentProject->settings(), &RoomSettings::setRecordScene);
    connect(m_ui->recordAnimation, &QPushButton::clicked, m_currentProject->settings(), &RoomSettings::setRecordAnimation);
    connect(m_currentProject->cameraTopology(), &ICameraTopology::frameReady, OpenGlScene::getInstance(), &OpenGlScene::setFrame);
    connect(m_currentProject, &IVirtualRoom::animationRecorded, this, &WccMainWindow::addAnimationToTable);
}

bool WccMainWindow::saveVariantMapToFile(const QString &path,const QVariantMap &map) const
{
    QFile file(path);

    if( file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QDataStream doc(&file);

        doc << map;

        file.close();

        return true;
    }

    QMessageBox::warning(0, "warning", "cannot save file to this location");

    return false;
}

void WccMainWindow::addCameraWidgets(QVector<ICamera *> allCameras)
{
    for(ICamera *camera : allCameras)
    {
        CameraWidget *camWidget = new CameraWidget(camera->settings(), m_ui->cameraScrollArea);

        m_scrollLayout->layout()->addWidget(camWidget);

        connect(camera, &ICamera::imageShow, camWidget->cameraVideoPreview(), &QtOpenCVViewerGl::showImage);
    }

    m_scrollLayout->addStretch(1);
}

void WccMainWindow::clearCameraWidgets()
{
    if (m_scrollLayout->layout()) {
        while(m_scrollLayout->layout()->count() > 0){
            QLayoutItem *item = m_scrollLayout->takeAt(0);
            delete item->widget();
            delete item;
        }
    }
}

void WccMainWindow::newProject()
{
    ProjectWizard *wizard = new ProjectWizard();

    int accepted = wizard->exec();

    if(accepted == ProjectWizard::Accepted)
    {
        setProject(wizard->project());
    }
}

void WccMainWindow::openProject()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Load Project"), ".wcc", tr(".json Files (*.wcc)"));

    if(filename != "")
    {
        qDebug() << "Open project:" << filename;

        QFile file(filename);
        file.open(QFile::OpenModeFlag::ReadOnly);

        QDataStream doc(&file);

        QVariantMap map;
        doc >> map;

        VirtualRoom* temp = new VirtualRoom(map);

        file.close();

        ProjectWizard *wizard = new ProjectWizard(temp);

        int accepted = wizard->exec();

        if(accepted == ProjectWizard::Accepted)
        {
            setProject(wizard->project());
        }
    }
}

void WccMainWindow::editCurrentProject()
{
    if(! m_currentProject)
    {
        return;
    }

    ProjectWizard *wizard = new ProjectWizard(m_currentProject);

    int accepted = wizard->exec();

    if(accepted == ProjectWizard::Accepted)
    {
        setProject(wizard->project());
    }
}

void WccMainWindow::saveCurrentProject()
{
    if(! m_currentProject)
    {
        return;
    }

    QString file = QFileDialog::getSaveFileName(this,tr("Save Project"),m_currentProject->settings()->name()+".wcc" , tr(".wcc Files (*.wcc)"));

    if(file != "")
    {
        /*
        if(!searchForRecentProjects(filename))
        {
            m_recentProjects.push_back(filename);

        }*/

        if(saveVariantMapToFile(file, m_currentProject->toVariantMap()))
        {
            m_currentProject->settings()->save();
        }
    }

}

void WccMainWindow::addNewServer()
{

}

void WccMainWindow::showAboutPage()
{
    AboutWidget *aboutWidget = new AboutWidget(this);

    aboutWidget->exec();
}

void WccMainWindow::addAnimationToTable(Animation *animation)
{

}
