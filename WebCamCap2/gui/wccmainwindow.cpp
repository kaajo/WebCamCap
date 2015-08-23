#include "wccmainwindow.h"
#include "ui_wccmainwindow.h"

#include "aboutwidget.h"
#include "camerawidget.h"
#include "projectwizard.h"
#include "addcamera.h"
#include "openglscene.h"
#include "src/localserver.h"

#include <QDebug>
#include <QSettings>
#include <QCloseEvent>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QDataStream>

const QString lastOpenedProjectsKey("lastProjects");
const QString camerasControlVisibleKey("camerasControlVisible");

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
/*
    ///add player dock widget
    this->addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, new AnimationPlayer, Qt::Horizontal);
    this->resize(sizeHint());
*/
    ///connect actions
    connect(m_ui->actionNewProject , &QAction::triggered, this, &WccMainWindow::newProject);
    connect(m_ui->actionOpenProject, &QAction::triggered, this, &WccMainWindow::openProject);
    connect(m_ui->actionEditCurrentProject, &QAction::triggered, this, &WccMainWindow::editCurrentProject);
    connect(m_ui->actionSaveCurrentProject, &QAction::triggered, this, &WccMainWindow::saveCurrentProject);
    connect(m_ui->actionAddServer, &QAction::triggered, this, &WccMainWindow::addNewServer);
    connect(m_ui->actionAbout, &QAction::triggered, this, &WccMainWindow::showAboutPage);
    ///view actions
    connect(m_ui->actionCameras, &QAction::triggered, this, &WccMainWindow::handleViewActionChecked);

    ///table for animations
    m_ui->AnimationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::ResizeToContents);
    connect(m_ui->AnimationsTable, &QTableWidget::cellChanged, this, &WccMainWindow::animationCellEdited);

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

    ///enable and view widgets
    enableProjectRelatedWidgets(false);
    m_ui->cameraControlDock->setVisible(settings.value(camerasControlVisibleKey, true).toBool());
    m_ui->actionCameras->setChecked(settings.value(camerasControlVisibleKey, true).toBool());

    ///create default local server
    auto server = new LocalServer;
    server->setEnabled(true);
    addServer(server);
}

WccMainWindow::~WccMainWindow()
{
    delete m_ui;
}

void WccMainWindow::enableProjectRelatedWidgets(bool enabled)
{
    m_ui->animationControlDock->setEnabled(enabled);
    m_ui->actionSaveCurrentProject->setEnabled(enabled);
    m_ui->actionEditCurrentProject->setEnabled(enabled);
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
    m_currentProject = project;

    QVector<ICamera*> cams = m_currentProject->cameraTopology()->getCameras();

    clearCameraWidgets();
    addCameraWidgets(cams);

    QVector<std::shared_ptr<CameraSettings> > settings;

    for(ICamera *cam: cams)
    {
        settings.push_back(cam->settings());
    }

    OpenGlScene::getInstance()->setRoomDimensions(m_currentProject->settings()->roomDimensions());
    OpenGlScene::getInstance()->setCamerasSettings(settings);

    connect(m_ui->recordScene, &QPushButton::clicked, m_currentProject->settings(), &RoomSettings::setRecordScene);
    connect(m_ui->recordAnimation, &QPushButton::clicked, m_currentProject->settings(), &RoomSettings::setRecordAnimation);
    connect(m_ui->markersSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_currentProject->cameraTopology(), &ICameraTopology::setNumberOfPoints);
    connect(m_currentProject->cameraTopology(), &ICameraTopology::frameReady, this, &WccMainWindow::sendFrameToAllServers);
    connect(m_currentProject->cameraTopology(), &ICameraTopology::frameReady, OpenGlScene::getInstance(), &OpenGlScene::setFrame);
    connect(m_currentProject, &IVirtualRoom::animationRecorded, this, &WccMainWindow::addAnimationToTable);

    enableProjectRelatedWidgets(true);
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

    list.insert(0, path);

    list.removeDuplicates();

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

        if(m_currentProject)
        {
            int ret = QMessageBox::warning(this, tr("Deactivation"), tr("This action will close your current project, continue?"), QMessageBox::Ok, QMessageBox::Cancel);

            if(ret == QMessageBox::Cancel)
            {
                return;
            }
            else
            {
                m_currentProject->disconnect();
                clearCameraWidgets();
                delete m_currentProject;
            }
        }

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

void WccMainWindow::handleViewActionChecked(bool view)
{
    QAction * snd = qobject_cast<QAction*>(sender());

    if(! snd)
    {
        return;
    }

    QSettings settings;

    if(snd == m_ui->actionCameras)
    {
        m_ui->cameraControlDock->setVisible(view);
        settings.setValue(camerasControlVisibleKey, view);
    }
}

void WccMainWindow::addAnimationToTable(Animation *animation)
{
    m_animations.push_back(animation);

    int row = m_ui->AnimationsTable->rowCount();
    m_ui->AnimationsTable->insertRow(row);

    QTableWidgetItem *x = new QTableWidgetItem(animation->name());
    m_ui->AnimationsTable->setItem(row, 0, x);
    x = new QTableWidgetItem(QString::number(animation->fps()));
    x->setFlags(x->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
    m_ui->AnimationsTable->setItem(row, 1, x);
    x = new QTableWidgetItem(QString::number(animation->length()));
    x->setFlags(x->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
    m_ui->AnimationsTable->setItem(row, 2, x);

    QPushButton *button = new QPushButton;
    button->setIcon(QIcon(QPixmap(":/img/edit_animation.png").scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    connect(button, &QPushButton::clicked, this, &WccMainWindow::animationEdit);
    m_ui->AnimationsTable->setCellWidget(row, 3, button);
    m_ui->AnimationsTable->itemAt(row, 3)->setFlags(x->flags() ^ (Qt::ItemIsEditable | Qt::ItemIsSelectable));
}

void WccMainWindow::animationEdit()
{
    QWidget *snd = qobject_cast<QWidget*>(sender());

    for(int row = 0; row < m_ui->AnimationsTable->rowCount(); row++)
    {
        if(snd == m_ui->AnimationsTable->cellWidget(row, 3))
        {
            //open editor window with animation
            qDebug() << "row clicked: " << row << m_animations[row]->name();
            m_animations[row]->save(QDir::currentPath());
        }
    }
}

void WccMainWindow::animationCellEdited(int row, int column)
{
    switch (column) {
    case 0:
        m_animations[row]->setName(m_ui->AnimationsTable->itemAt(row, column)->text());
        break;
    default:
        break;
    }
}

void WccMainWindow::sendFrameToAllServers(Frame frame)
{
    foreach(IServer *server, m_servers)
    {
        server->sendMesage(frame.toVariantMap());
    }
}
