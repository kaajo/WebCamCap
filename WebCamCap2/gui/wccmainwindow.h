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

#ifndef WCCMAINWINDOW_H
#define WCCMAINWINDOW_H

#include "ivirtualroom.h"

#include "src/virtualroom.h"
#include "src/markercamera.h"

#include "iserver.h"

#include <QVBoxLayout>
#include <QtWidgets/QMainWindow>

namespace Ui {
class WccMainWindow;
}

class WccMainWindow : public QMainWindow
{
    Q_OBJECT

    IVirtualRoom *m_currentProject = nullptr;

    QVector<IServer*> m_servers;

    QVBoxLayout *m_scrollLayout;

    QVector<Animation*> m_animations;
public:
    explicit WccMainWindow(QWidget *parent = 0);
    ~WccMainWindow();

private:
    Ui::WccMainWindow *m_ui = nullptr;

    void enableProjectRelatedWidgets(bool enabled);
    void closeEvent(QCloseEvent *event) override;

    bool addServer(IServer *server);
    bool removeServer(QString name);
    void setProject(IVirtualRoom *project);

    bool saveVariantMapToFile(const QString &path,const QVariantMap &map) const;

    void addCameraWidgets(QVector<ICamera *> allCameras);
    void clearCameraWidgets();

    void addProjectToSettings(const QString &path);
    void removeProjectFromSettings(const QString &path);

private slots:
    void newProject();
    void openProject();
    void loadProject(QString filePath);
    void editCurrentProject();
    void saveCurrentProject();

    void addNewServer();
    void showAboutPage();

    void handleViewActionChecked(bool view);

    void addAnimationToTable(Animation *animation);
};

#endif // WCCMAINWINDOW_H
