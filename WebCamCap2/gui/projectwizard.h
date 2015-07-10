/*
 *
 * Copyright (C) 2015  Miroslav Krajicek (https://github.com/kaajo).
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

#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include "roomsettings.h"
#include "camerasettings.h"
#include "../src/markercamera.h"
#include "ivirtualroom.h"

#include <QWizard>

namespace Ui {
class ProjectWizard;
}

class ProjectWizard : public QWizard
{
    Q_OBJECT

    RoomSettings *m_roomSettings = nullptr;
    QMap<int, CameraSettings*> m_tableRowToSettings;
    QVector<ICamera*> m_cameras;

    IVirtualRoom *m_project = nullptr;

public:
    explicit ProjectWizard(QWidget *parent = 0);
    ~ProjectWizard();

    RoomSettings *settings() const;
    void setSettings(RoomSettings *settings);

    IVirtualRoom *project() const;
    void setProject(IVirtualRoom *project);

private:
    Ui::ProjectWizard *m_ui;

    ///1. page
    void saveToRoomSettings();

    ///2. page
    void generateCameras();
    void addCameraSettingsToTable(CameraSettings *cameraSettings, int row = -1);

    ///3. page


private slots:

    ///general
    void handlePageChange(int page);
    void handleCellChanged(int row, int column);

    ///2.
    void addCamera();
    void editCamera();
    void deleteCamera();
    void handleSettingsChanged(RoomSettings::RoomSettingsType type);

    ///3.
    void calibBackground();
    void calibMarkers();
    void handleAccepted();

};

#endif // PROJECTWIZARD_H
