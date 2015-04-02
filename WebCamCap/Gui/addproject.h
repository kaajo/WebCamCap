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

#ifndef ADDPROJECT_H
#define ADDPROJECT_H

#include <QtWidgets/QDialog>

#include "../room.h"
#include "addcamera.h"

namespace Ui {
class AddProject;
}

class AddProject : public QDialog
{
    Q_OBJECT

    Room *newProject;
    std::vector<CaptureCamera*> newCameras;

    bool calibNoMarkers, calibMarkers;

public:
    explicit AddProject(QWidget *parent = 0);

    Room * resolveProject();

    Room* getProject() const {return newProject;}
    void EditProject(Room * PROJECT);


    ~AddProject();

private slots:
    void on_buttonBox_accepted();

    void on_deleteCamera_pressed();

    void on_CameraTable_cellChanged(int row, int column);

    void on_AddCamera_clicked();

    void on_name_textEdited(const QString &arg1);

    void on_CalibNoMarkers_clicked();

    void on_CalibWithMarkers_clicked();

    void on_editCamera_clicked();

private:

    void addCamToTable(CaptureCamera *temp);
    Ui::AddProject *ui;
};

#endif // ADDPROJECT_H
