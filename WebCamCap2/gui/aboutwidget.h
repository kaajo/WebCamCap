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

#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QDialog>


namespace Ui {
class AboutWidget;
}

class AboutWidget : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWidget(QWidget *parent =0);
    ~AboutWidget();

private slots:

private:
    Ui::AboutWidget *ui;
};

#endif // ABOUTWIDGET_H
