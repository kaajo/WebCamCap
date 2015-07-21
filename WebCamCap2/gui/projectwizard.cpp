#include "projectwizard.h"
#include "ui_projectwizard.h"

#include "addcamera.h"
#include "../src/virtualroom.h"

#include <tr1/functional>

#include <QDebug>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrentMap>


IVirtualRoom *ProjectWizard::project() const
{
    return m_project;
}

void ProjectWizard::setProject(IVirtualRoom *project)
{
    m_project = project;
}
ProjectWizard::ProjectWizard(IVirtualRoom *room, QWidget *parent) :
    QWizard(parent),
    m_ui(new Ui::ProjectWizard)
{
    m_ui->setupUi(this);

    if(! room)
    {
        m_roomSettings = new RoomSettings;
    }
    else
    {
        setRoomSettings(room->settings());

        m_cameras = room->cameraTopology()->getCameras();

        for(const ICamera *camera: room->cameraTopology()->getCameras())
        {
            addCameraSettingsToTable(camera->settings());
        }
        

        m_project = room;
        m_editMode = true;
    }

    connect(m_roomSettings, &RoomSettings::changed, this, &ProjectWizard::handleSettingsChanged);

    connect(this, &QWizard::currentIdChanged, this, &ProjectWizard::handlePageChange);
    connect(m_ui->cameraTable, &QTableWidget::cellChanged, this, &ProjectWizard::handleCellChanged);
    connect(this, &QWizard::accepted, this, &ProjectWizard::handleAccepted);

    connect(m_ui->addCamera, &QPushButton::pressed, this, &ProjectWizard::addCamera);
    connect(m_ui->editCamera, &QPushButton::pressed, this, &ProjectWizard::editCamera);
    connect(m_ui->deleteCamera, &QPushButton::pressed, this, &ProjectWizard::deleteCamera);

    connect(m_ui->calibBackground, &QPushButton::pressed, this, &ProjectWizard::calibBackground);
    connect(m_ui->calibWithMarkers, &QPushButton::pressed, this, &ProjectWizard::calibMarkers);
}

ProjectWizard::~ProjectWizard()
{
    delete m_ui;
}

QVector<CameraSettings *> ProjectWizard::allCameraSettings() const
{
    return m_tableRowToSettings.values().toVector();
}

void ProjectWizard::saveToRoomSettings()
{
    if(! m_roomSettings)
    {
        QMessageBox::warning(this, "No project", "There is no project to edit");
        this->reject();
    }
    
    /*
    if(m_editMode)
    {
        return;
    }
    */
    
    if(m_roomSettings->name() != m_ui->name->text())
    {
        m_roomSettings->setName(m_ui->name->text());
    }
    
    QVector3D roomDims = QVector3D(m_ui->width->text().toInt(),
                                   m_ui->length->text().toInt(),
                                   m_ui->height->text().toInt());
    
    if(m_roomSettings->roomDimensions() != roomDims)
    {
        m_roomSettings->setRoomDimensions(roomDims);
    }
    
    if(m_roomSettings->maxError() != m_ui->error->text().toFloat())
    {
        m_roomSettings->setMaxError(m_ui->error->text().toFloat());
    }
    
}

void ProjectWizard::setRoomSettings(RoomSettings *settings)
{
    m_roomSettings = settings;

    auto roomDims = m_roomSettings->roomDimensions();

    m_ui->name->setText(settings->name());
    m_ui->width->setText(QString::number(roomDims.x()));
    m_ui->length->setText(QString::number(roomDims.y()));
    m_ui->height->setText(QString::number(roomDims.z()));
    m_ui->error->setText(QString::number(m_roomSettings->maxError()));
}

void ProjectWizard::generateCameras()
{
    auto camSettings = m_tableRowToSettings.values();

    QVector<ICamera*> newCams;

    foreach (CameraSettings *settings, camSettings)
    {
        auto it = std::find_if(m_cameras.begin(), m_cameras.end(), [settings](ICamera *camera){return camera->settings() == settings;});

        if(it != m_cameras.end())
        {
            newCams.push_back(*it);
            m_cameras.erase(it);
        }
        else
        {
            newCams.push_back(new MarkerCamera(settings));
        }
    }

    qDeleteAll(m_cameras);
    m_cameras = newCams;
}

void ProjectWizard::handlePageChange(int page)
{
    switch (page) {
    case 1:
        saveToRoomSettings();
        break;
    case 2:
        generateCameras();
        break;
    default:
        break;
    }
}

void ProjectWizard::handleCellChanged(int row, int column)
{
    QTableWidgetItem *item = m_ui->cameraTable->item(row, column);

    if(row >= m_tableRowToSettings.size() || row < 0)
    {
        return;
    }

    switch (column) {
    case 2:
        if(item->checkState() == Qt::Checked)
            m_tableRowToSettings[row]->setTurnedOn(true);
        else
            m_tableRowToSettings[row]->setTurnedOn(false);
        break;
    default:
        break;
    }
}

void ProjectWizard::handleAccepted()
{
    if(! m_project)
    {
        m_project = new VirtualRoom(m_roomSettings);

    }

    m_project->cameraTopology()->addCameras(m_cameras);
}

void ProjectWizard::addCamera()
{
    if(! m_roomSettings)
    {
        return;
    }

    AddCamera *addCamera = new AddCamera(m_roomSettings->roomDimensions());

    int accepted = addCamera->exec();

    if(accepted == AddCamera::Accepted)
    {
        addCameraSettingsToTable(addCamera->cameraSettings());
    }

    delete addCamera;
}

void ProjectWizard::editCamera()
{
    if(m_ui->cameraTable->selectedItems().isEmpty() || ! m_roomSettings)
    {
        return;
    }

    int row = m_ui->cameraTable->currentRow();

    if(row != -1)
    {
        AddCamera *addCamera = new AddCamera(m_roomSettings->roomDimensions());
        addCamera->setCameraSettings(m_tableRowToSettings[row]);

        int accepted = addCamera->exec();

        if(accepted == AddCamera::Accepted)
        {
            addCameraSettingsToTable(addCamera->cameraSettings(), row);
        }
    }
}

void ProjectWizard::deleteCamera()
{
    if(m_ui->cameraTable->selectedItems().isEmpty())
    {
        return;
    }

    int row = m_ui->cameraTable->currentRow();

    m_ui->cameraTable->removeRow(row);
    auto settings = m_tableRowToSettings[row];
    delete settings;
    m_tableRowToSettings.remove(row);
}

void ProjectWizard::addCameraSettingsToTable(CameraSettings *cameraSettings, int row)
{
    if(row == -1)
    {
        row = m_ui->cameraTable->rowCount();
        m_ui->cameraTable->insertRow(row);
    }

    QTableWidgetItem *x = new QTableWidgetItem(QString::number(cameraSettings->videoUsbId()));
    m_ui->cameraTable->setItem(row, 0, x);
    x = new QTableWidgetItem(cameraSettings->name());
    m_ui->cameraTable->setItem(row, 1, x);

    x = new QTableWidgetItem(Qt::CheckStateRole);
    x->setCheckState(Qt::Unchecked);
    m_ui->cameraTable->setItem(row,2,x);

    m_tableRowToSettings[row] = cameraSettings;
}

void ProjectWizard::handleSettingsChanged(RoomSettings::RoomSettingsType type)
{
    if(type == RoomSettings::RoomSettingsType::DIMENSIONS)
    {
        foreach (CameraSettings *camSettings, m_tableRowToSettings)
        {
            camSettings->setRoomDimensions(m_roomSettings->roomDimensions());
        }
    }
}

void ProjectWizard::calibBackground()
{
    for(int i = 0; i < m_cameras.size(); i++)
    {
        m_cameras[i]->calibrate(ICamera::CameraCalibrationType::BACKGROUND);
        m_ui->progressBar->setValue((100*(i+1))/m_cameras.size());
    }
}

void ProjectWizard::calibMarkers()
{
    for(int i = 0; i < m_cameras.size(); i++)
    {
        m_cameras[i]->calibrate(ICamera::CameraCalibrationType::THRESHOLD);
    }
}
