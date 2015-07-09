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

#include <QtWidgets/QMainWindow>

#include "ivirtualroom.h"

#include "src/virtualroom.h"
#include "src/markercamera.h"

#include "iserver.h"

namespace Ui {
class WccMainWindow;
}

class WccMainWindow : public QMainWindow
{
    Q_OBJECT

    IVirtualRoom *currentProject = nullptr;

    QVector<IServer*> m_servers;

    QWidget *m_scrollWidget;
public:
    explicit WccMainWindow(QWidget *parent = 0);
    ~WccMainWindow();

private:
    Ui::WccMainWindow *m_ui = nullptr;

    bool addServer(IServer *server);
    bool removeServer(QString name);

private slots:
    void newProject();
    void openProject();
    void editCurrentProject();
    void saveCurrentProject();

    void addNewServer();
    void showAboutPage();
};

#endif // WCCMAINWINDOW_H
