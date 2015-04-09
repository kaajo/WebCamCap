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
    QIcon icon(":/Pictures/main_icon.jpg");
    this->setWindowIcon(icon);

    loadLog();

    createRollOutMenu();

    QDesktopWidget window;
    QRect screen = window.screenGeometry( window.screenNumber(this));
    move(screen.width()/2 - this->width()/2, screen.height()/2 - this->height()/2);

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

    if(m_project != nullptr)
    {
        delete(m_project);
    }

    for(int i = 0; i < m_recentProjectsQActions.size(); i++)
    {
        delete(m_recentProjectsQActions.at(i));
    }

    saveLog();
}

void MainWindow::on_newProject_triggered()
{
    int msg;

    if(m_project != nullptr && !m_project->getSaved())
    {
        msg = NotSavedMessage();

        if(msg == QMessageBox::Cancel)
        {
            return;
        }
        else if(msg == QMessageBox::Save)
        {
            on_saveProject_triggered();

            if(!m_project->getSaved())
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
        delete(m_project);



        Room * temp = NewProjectDialog.resolveProject();

        handleMainWProject(temp);

        this->m_project = temp;
    }
}

void MainWindow::OpenRecentProjects()
{
    QObject * sender = QObject::sender();

    for(int i = 0; i < m_recentProjectsQActions.size(); i++)
    {
        if(m_recentProjectsQActions.at(i) == sender)
        {
            int msg;

            if(m_project != nullptr && !m_project->getSaved())
            {
                msg = NotSavedMessage();

                if(msg == QMessageBox::Cancel)
                {
                    return;
                }
                else if(msg == QMessageBox::Save)
                {
                    on_saveProject_triggered();

                    if(!m_project->getSaved())
                    {
                        return;
                    }
                }
            }

            QFile file(m_recentProjects[i]);
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

    if(m_project != nullptr && !m_project->getSaved())
    {
        msg = NotSavedMessage();

        if(msg == QMessageBox::Cancel)
        {
            return;
        }
        else if(msg == QMessageBox::Save)
        {
            on_saveProject_triggered();

            if(!m_project->getSaved())
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
            m_recentProjects.push_back(filename);
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
    if(m_project != nullptr)
    {
        editProject(m_project);
    }
}

void MainWindow::on_saveProject_triggered()
{
    if(m_project != nullptr)
    {
        QString filename = QFileDialog::getSaveFileName(this,tr("Save Project"),m_project->getName()+".json" , tr(".json Files (*.json)"));

        if(filename != "")
        {
            if(!searchForRecentProjects(filename))
            {
                m_recentProjects.push_back(filename);

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

            QVariantMap map = m_project->toVariantMap();

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
    if(m_project == nullptr)
    {
        ui->nahravanie->setChecked(false);
        return;
    }

    if(checked)
    {
        ui->nahravanie->setText("Stop");
        m_record = true;

        if(m_project != nullptr)
        {
            m_project->RecordingStart();
        }
    }
    else
    {
        ui->nahravanie->setText("Record");
        m_project->RecordingStop();
        m_record = false;
    }

}

void MainWindow::createRollOutMenu()
{
    for(int i = 0; i < m_recentProjects.size(); i++)
    {
        QAction* temp = new QAction(m_recentProjects[i],this);

        m_recentProjectsQActions.push_back(temp);
        connect(m_recentProjectsQActions.at(i),  SIGNAL(triggered()), this, SLOT(OpenRecentProjects() ));
    }

    ui->menuRecent_Projects->addActions(m_recentProjectsQActions);
}

bool MainWindow::searchForRecentProjects(QString filestring)
{
    for(int i = 0; i < m_recentProjects.size();i++)
    {
        if(m_recentProjects[i] == filestring)
        {
            return true;
        }
    }

    return false;
}

void MainWindow::saveLog()
{
    QList<QVariant> var;

    for(int i = 0; i < m_recentProjects.size(); i++)
    {
        var.push_back(QVariant(m_recentProjects[i]));
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
            m_recentProjects.push_back(temp);
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

    if(this->m_project !=  nullptr)
    {
        for(size_t i = 0; i < this->m_project->getcameras().size(); i++)
        {
            this->m_project->HideCameraVideo(i);
            this->m_project->TurnOffCamera(i);
        }
    }

    bool ok = NewProjectDialog.exec();

    if(ok)
    {
        if(this->m_project != nullptr)
        {
            auto cams = this->m_project->getcameras();

            for(size_t i = 0; i < cams.size(); i++)
            {
                scrollWidget->layout()->removeWidget(cams[i]->getWidget());
            }
            delete this->m_project;
        }


        Room * temp = NewProjectDialog.resolveProject();

        handleMainWProject(temp);

        this->m_project = temp;
    }
}

void MainWindow::noProjectOpenedWarning()
{
    QMessageBox::warning(this, "Warning", "No project opened");
}

void MainWindow::on_playButton_pressed()
{
    if(m_project == nullptr)
    {
        return;
    }

    if(!m_captureAnimation)
    {
        m_captureAnimation = true;
        m_project->CaptureAnimationStart();
    }
}

void MainWindow::on_stopButton_pressed()
{
    if(m_project == nullptr)
    {
        return;
    }

    if(m_captureAnimation)
    {
        Animation * ActualAnimation = m_project->CaptureAnimationStop();
        m_captureAnimation = false;

        m_animations.push_back(ActualAnimation);

        int row = ui->AnimationsTable->rowCount();
        ui->AnimationsTable->insertRow(row);

        QTableWidgetItem *x = new QTableWidgetItem(QString::fromStdString(ActualAnimation->getName()));
        ui->AnimationsTable->setItem(row, 0, x);
        x= new QTableWidgetItem(QString::number(ActualAnimation->getFrameRate()));
        ui->AnimationsTable->setItem(row, 1, x);
        x= new QTableWidgetItem(QString::number(ActualAnimation->getLength()));
        ui->AnimationsTable->setItem(row,2,x);
        x= new QTableWidgetItem(m_saveIcon, "");
        ui->AnimationsTable->setItem(row, 3, x);
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
    if(m_project == nullptr)
    {
        ui->LivePipe->setChecked(false);
        return;
    }

    if(arg1 == 0)
    {
        m_project->server()->setServerUsed(false);
    }
    else
    {
        m_project->server()->setServerUsed(true);
    }
}

void MainWindow::on_NumberOfPoints_editingFinished()
{
    if(m_project == nullptr)
    {
        return;
    }

    m_project->setNumberOfPoints(ui->NumberOfPoints->text().toInt());
}

void MainWindow::on_actionAbout_triggered()
{
    AboutWidget about;

    about.exec();
}
