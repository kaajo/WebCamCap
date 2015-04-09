/*
 *
 * Copyright (C) 2014  Miroslav Krajicek, Faculty of Informatics Masaryk University (https://github.com/kaajo).
 * All Rights Reserved.
 *
 * This file is part of WebCamCap.
 *
 * WebCamCap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WebCamCap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU LGPL version 3
 * along with WebCamCap. If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */


#include "aboutwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sstream>

#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFile>
#include <QJsonDocument>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("");

    /* SETUP ICONS */
    QIcon icon(QDir::currentPath() + "/Pictures/main_icon.jpg");
    this->setWindowIcon(icon);

    QIcon play(QDir::currentPath() + "/Pictures/PlayIcon.png");
    QIcon edit(QDir::currentPath() + "/Pictures/EditIcon.png");
    QIcon save(QDir::currentPath() + "/Pictures/SaveIcon.png");

    index = 0;

    playIcon = play;
    editIcon = edit;
    saveIcon = save;

    project = nullptr;

    loadLog();

    createRollOutMenu();

    QDesktopWidget window;
    QRect screen = window.screenGeometry( window.screenNumber(this));
    move(screen.width()/2 - this->width()/2, screen.height()/2 - this->height()/2);

    record = captureAnimation = false;

    //ui->AnimationsTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    ui->CamerasWindows->setWidgetResizable(true);
    scrollWidget = new QWidget;
    scrollWidget->setLayout(new QVBoxLayout);
    ui->CamerasWindows->setWidget(scrollWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if(e->key() == Qt::Key_W)
    {
        //ui->OpenGLWIndow->ZoomPlus();
    }
    else if(e->key() == Qt::Key_S)
    {
        //ui->OpenGLWIndow->ZoomMinus();
    }

}

void MainWindow::keyReleaseEvent(QKeyEvent* /*e*/)
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();

    if(project != nullptr)
    {
        delete(project);
    }

    for(int i = 0; i < recentProjectsQActions.size(); i++)
    {
        delete(recentProjectsQActions.at(i));
    }

    saveLog();
}

void MainWindow::on_newProject_triggered()
{
    int msg;

    if(project != nullptr && !project->getSaved())
    {
        msg = NotSavedMessage();

        if(msg == QMessageBox::Cancel)
        {
            return;
        }
        else if(msg == QMessageBox::Save)
        {
            on_saveProject_triggered();

            if(!project->getSaved())
            {
                return;
            }
        }
    }

    AddProject NewProjectDialog(this);
    NewProjectDialog.setModal(true);
    bool ok = NewProjectDialog.exec();

    if(ok)
    {
        delete(project);



        Room * temp = NewProjectDialog.resolveProject();

        handleMainWProject(temp);

        this->project = temp;
    }
}

void MainWindow::OpenRecentProjects()
{
    QObject * sender = QObject::sender();

    for(int i = 0; i < recentProjectsQActions.size(); i++)
    {
        if(recentProjectsQActions.at(i) == sender)
        {
            int msg;

            if(project != nullptr && !project->getSaved())
            {
                msg = NotSavedMessage();

                if(msg == QMessageBox::Cancel)
                {
                    return;
                }
                else if(msg == QMessageBox::Save)
                {
                    on_saveProject_triggered();

                    if(!project->getSaved())
                    {
                        return;
                    }
                }
            }

            QFile file(recentProjects[i]);
            file.open(QFile::OpenModeFlag::ReadOnly);

            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

            QVariantMap map = doc.toVariant().toMap();

            Room * temp = new Room();
            temp->fromVariantMap(ui->OpenGLWIndow , map);

            file.close();

            editProject(temp);
        }
    }
}

void MainWindow::on_openProject_triggered()
{
    int msg;

    if(project != nullptr && !project->getSaved())
    {
        msg = NotSavedMessage();

        if(msg == QMessageBox::Cancel)
        {
            return;
        }
        else if(msg == QMessageBox::Save)
        {
            on_saveProject_triggered();

            if(!project->getSaved())
            {
                return;
            }
        }
    }

    QString filename = QFileDialog::getOpenFileName(this,tr("Load Project"), ".", tr(".json Files (*.json)"));

    if(filename != "")
    {
        std::cout << "Open project:" << filename.toStdString() << std::endl;

        if(!searchForRecentProjects(filename))
        {
            recentProjects.push_back(filename);
        }

        QFile file(filename);
        file.open(QFile::OpenModeFlag::ReadOnly);

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

        QVariantMap map = doc.toVariant().toMap();

        Room* temp = new Room();
        temp->fromVariantMap(ui->OpenGLWIndow , map);

        file.close();

        editProject(temp);
    }

}

void MainWindow::on_editProject_triggered()
{
    if(project != nullptr)
    {
        editProject(project);
    }
}

void MainWindow::on_saveProject_triggered()
{
    if(project != nullptr)
    {
        QString filename = QFileDialog::getSaveFileName(this,tr("Save Project"),project->getName()+".json" , tr(".json Files (*.json)"));

        if(filename != "")
        {
            if(!searchForRecentProjects(filename))
            {
                recentProjects.push_back(filename);

                /*if(!project->getSaved())
                {
                    return;
                }*/
            }

            QFile file(filename);

            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
            {
                QMessageBox::warning(this, "warning", "cannot save file to this location");
                return;
            }

            QVariantMap map = project->toVariantMap();

            QJsonDocument doc = QJsonDocument::fromVariant(map);

            file.write(doc.toJson());

            file.close();
            std::cout << "Project saved to:" << filename.toStdString() << std::endl;
        }
    }
    else
    {
        noProjectOpenedWarning();
    }
}

void MainWindow::on_nahravanie_clicked(bool checked)
{
    if(project == nullptr)
    {
        ui->nahravanie->setChecked(false);
        return;
    }

    if(checked)
    {
        ui->nahravanie->setText("Stop");
        record = true;

        if(project != nullptr)
        {
            project->RecordingStart();
        }
    }
    else
    {
        ui->nahravanie->setText("Record");
        project->RecordingStop();
        record = false;
    }

}

void MainWindow::createRollOutMenu()
{
    for(int i = 0; i < recentProjects.size(); i++)
    {
        QAction* temp = new QAction(recentProjects[i],this);

        recentProjectsQActions.push_back(temp);
        connect(recentProjectsQActions.at(i),  SIGNAL(triggered()), this, SLOT(OpenRecentProjects() ));
    }

    ui->menuRecent_Projects->addActions(recentProjectsQActions);
}

bool MainWindow::searchForRecentProjects(QString filestring)
{
    for(int i = 0; i < recentProjects.size();i++)
    {
        if(recentProjects[i] == filestring)
        {
            return true;
        }
    }

    return false;
}

void MainWindow::saveLog()
{
    QList<QVariant> var;

    for(int i = 0; i < recentProjects.size(); i++)
    {
        var.push_back(QVariant(recentProjects[i]));
    }

    m_settings.setValue("lastFiles", QVariant(var));
}

void MainWindow::loadLog()
{
    auto v = m_settings.value("lastFiles", QList<QVariant>());
    auto var = v.toList();

    QString temp;

    for(int i = 0; i < var.size(); i++)
    {
        temp = var[i].toString();

        if(QFile::exists(temp))
        {
            recentProjects.push_back(temp);
        }
    }
}

int MainWindow::NotSavedMessage()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(" ");
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Discard | QMessageBox::Cancel | QMessageBox::Save);
    msgBox.setDefaultButton(QMessageBox::Save);

    return msgBox.exec();
}

void MainWindow::handleMainWProject(Room *p)
{
    p->setOpenglWindow(ui->OpenGLWIndow);
    ui->OpenGLWIndow->setRoomDims(p->getDimensions());

    //add cams to scroll
    std::vector<CaptureCamera*> c = p->getcameras();

    for(size_t i = 0; i < c.size(); i++)
    {
        scrollWidget->layout()->addWidget(c[i]->getWidget());
    }
}

void MainWindow::editProject(Room *project)
{
    AddProject NewProjectDialog(this);

    NewProjectDialog.EditProject(project);
    NewProjectDialog.setModal(true);

    if(this->project !=  nullptr)
    {
        for(size_t i = 0; i < this->project->getcameras().size(); i++)
        {
            this->project->HideCameraVideo(i);
            this->project->TurnOffCamera(i);
        }
    }

    bool ok = NewProjectDialog.exec();

    if(ok)
    {
        if(this->project != nullptr)
        {
            auto cams = this->project->getcameras();

            for(size_t i = 0; i < cams.size(); i++)
            {
                scrollWidget->layout()->removeWidget(cams[i]->getWidget());
            }
            delete this->project;
        }


        Room * temp = NewProjectDialog.resolveProject();

        handleMainWProject(temp);

        this->project = temp;
    }
}

void MainWindow::noProjectOpenedWarning()
{
    QMessageBox::warning(this, "Warning", "No project opened");
}

void MainWindow::on_playButton_pressed()
{
    if(project == nullptr)
    {
        return;
    }

    if(!captureAnimation)
    {
        captureAnimation = true;
        project->CaptureAnimationStart();
    }
}

void MainWindow::on_stopButton_pressed()
{
    if(project == nullptr)
    {
        return;
    }

    if(captureAnimation)
    {
        Animation * ActualAnimation = project->CaptureAnimationStop();
        captureAnimation = false;

        m_animations.push_back(ActualAnimation);

        int row = ui->AnimationsTable->rowCount();
        ui->AnimationsTable->insertRow(row);

        QTableWidgetItem *x = new QTableWidgetItem(QString::fromStdString(ActualAnimation->getName()));
        ui->AnimationsTable->setItem(row, 0, x);
        x= new QTableWidgetItem(QString::number(ActualAnimation->getFrameRate()));
        ui->AnimationsTable->setItem(row, 1, x);
        x= new QTableWidgetItem(QString::number(ActualAnimation->getLength()));
        ui->AnimationsTable->setItem(row,2,x);
        x= new QTableWidgetItem(playIcon, "");
        ui->AnimationsTable->setItem(row, 3, x);
        x= new QTableWidgetItem(editIcon, "");
        ui->AnimationsTable->setItem(row, 4, x);
        x= new QTableWidgetItem(saveIcon, "");
        ui->AnimationsTable->setItem(row, 5, x);
    }
}

void MainWindow::on_AnimationsTable_cellChanged(int row, int column)
{
    if(column == 0)
    {
         m_animations[row]->setName(ui->AnimationsTable->item(row, column)->text().toStdString());
    }
}

void MainWindow::on_AnimationsTable_cellClicked(int row, int column)
{
    switch (column)
    {
    case 3:
        //play
        ui->AnimationsTable->item(row, column)->setSelected(false);
        break;
    case 4:
        //edit animation
        ui->AnimationsTable->item(row, column)->setSelected(false);
        break;
    case 5:
        //save animation
        ui->AnimationsTable->item(row, column)->setSelected(false);
        m_animations[row]->Save((ui->AnimationsTable->item(row, 0)->text() + ".wcc").toStdString());
        break;
    default:
        break;
    }
}

void MainWindow::on_LinesCheck_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->OpenGLWIndow->setDrawLines(false);
    }
    else
    {
        ui->OpenGLWIndow->setDrawLines(true);
    }
}

void MainWindow::on_JointsCheck_stateChanged(int arg1)
{
    if(arg1 == 0)
    {
        ui->OpenGLWIndow->setDrawJoints(false);
    }
    else
    {
        ui->OpenGLWIndow->setDrawJoints(true);
    }
}

void MainWindow::on_LivePipe_stateChanged(int arg1)
{
    if(project == nullptr)
    {
        ui->LivePipe->setChecked(false);
        return;
    }

    if(arg1 == 0)
    {
        project->server()->setServerUsed(false);
    }
    else
    {
        project->server()->setServerUsed(true);
    }
}

void MainWindow::on_NumberOfPoints_editingFinished()
{
    if(project == nullptr)
    {
        return;
    }

    project->setNumberOfPoints(ui->NumberOfPoints->text().toInt());
}

void MainWindow::on_actionAbout_triggered()
{
    AboutWidget about;

    about.exec();
}
