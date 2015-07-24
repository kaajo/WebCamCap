#include "wccmainwindow.h"
#include "ui_wccmainwindow.h"

#include "aboutwidget.h"
#include "camerawidget.h"
#include "projectwizard.h"
#include "addcamera.h"
#include "openglscene.h"

#include <tr1/functional>
#include <QDebug>
#include <QSettings>
#include <QCloseEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QDataStream>

const QString lastOpenedProjectsKey("lastProjects");

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

    ///init last opened projects
    QSettings settings;

    const QStringList list = settings.value(lastOpenedProjectsKey).toStringList();

    QList<QAction*> actions;

    for(const QString &string:list)
    {
        QAction* temp = new QAction(string,this);

        actions.push_back(temp);
        connect(temp, &QAction::triggered, this, &WccMainWindow::openProject);
    }

    m_ui->menuRecent_projects->addActions(actions);
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

    delete it;
    m_servers.erase(it);

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

void WccMainWindow::addProjectToSettings(const QString &path)
{
    QSettings settings;

    QStringList list = settings.value(lastOpenedProjectsKey).toStringList();

    if(list.contains(path))
    {
        list.replace(0, path);
    }
    else
    {
        list.insert(0, path);
    }

    settings.setValue(lastOpenedProjectsKey, list);
}

void WccMainWindow::removeProjectFromSettings(const QString &path)
{
    QSettings settings;

    QStringList list = settings.value(lastOpenedProjectsKey).toStringList();

    if(list.contains(path))
    {
        list.removeOne(path);
        settings.setValue(lastOpenedProjectsKey, list);
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
    QAction * snd = qobject_cast<QAction*>(sender());

    if(snd == m_ui->actionOpenProject)
    {
        loadProject(QFileDialog::getOpenFileName(this,tr("Load Project"), ".wcc", tr(".wcc Files (*.wcc)")));
    }
    else if(snd)
    {
        loadProject(snd->text());
    }
}

void WccMainWindow::loadProject(QString filePath)
{
    if(filePath != "")
    {
        qDebug() << "Open project:" << filePath;

        QFile file(filePath);
        if(! file.open(QFile::OpenModeFlag::ReadOnly))
        {
            qDebug() << "failed to open project:" << filePath;
            return;
        }

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
            addProjectToSettings(filePath);
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

    QString filename = QFileDialog::getSaveFileName(this,tr("Save Project"),m_currentProject->settings()->name()+".wcc" , tr(".wcc Files (*.wcc)"));

    if(filename != "")
    {
        if(saveVariantMapToFile(filename, m_currentProject->toVariantMap()))
        {
            m_currentProject->settings()->save();
            addProjectToSettings(filename);
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
    int row = m_ui->AnimationsTable->rowCount();
    m_ui->AnimationsTable->insertRow(row);

    QTableWidgetItem *x = new QTableWidgetItem(QString("name"));
    m_ui->AnimationsTable->setItem(row, 0, x);
    x = new QTableWidgetItem(QString::number(animation->fps()));
    m_ui->AnimationsTable->setItem(row, 1, x);
}
