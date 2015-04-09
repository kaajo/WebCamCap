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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "addproject.h"
#include "addcamera.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QCloseEvent>
#include <QSettings>
#include <QKeyEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    bool m_captureAnimation = false;
    bool m_record = false;
    Room *m_project = nullptr;

    QList<QString> m_recentProjects;
    QList<QAction*> m_recentProjectsQActions;

    QIcon m_saveIcon = QIcon(":/Pictures/SaveIcon.png");

    QSettings m_settings;

    QVector<Animation*> m_animations;

    //scroll area
    QWidget *scrollWidget = nullptr;

protected:
    void closeEvent(QCloseEvent *event);

public:
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    std::string operator<<(QString& string) {
        return string.toStdString();
    }

private slots:

    void on_newProject_triggered();

    void OpenRecentProjects();

    void on_openProject_triggered();

    void on_editProject_triggered();

    void on_saveProject_triggered();

    void on_nahravanie_clicked(bool checked);

    void on_playButton_pressed();

    void on_stopButton_pressed();

    void on_AnimationsTable_cellChanged(int row, int column);

    void on_AnimationsTable_cellClicked(int row, int column);

    void on_LinesCheck_stateChanged(int arg1);

    void on_JointsCheck_stateChanged(int arg1);

    void on_LivePipe_stateChanged(int arg1);

    void on_NumberOfPoints_editingFinished();

    void on_actionAbout_triggered();

private:
    void on_Threshold_valueChanged(int value);

    void on_brightness_valueChanged(int value);

    void createRollOutMenu();
    bool searchForRecentProjects(QString filestring);
    void saveLog();
    void loadLog();
    void setProjectStats();
    int NotSavedMessage();
    void handleMainWProject(Room *p);

    void editProject(Room * m_project);

    void noProjectOpenedWarning();

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
