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

#ifndef ANIMATION_H
#define ANIMATION_H

#include "webcamcapprimitives_global.h"

#include "frame.h"

#include <QString>
#include <QVector>
#include <QObject>
#include <QVariantMap>

class WEBCAMCAPPRIMITIVESSHARED_EXPORT Animation : public QObject
{
    Q_OBJECT

    QString m_name;
    long int m_elapsedMilliSecondsTime = 0;
    QVector<Frame> m_frames;

public:
    explicit Animation(QString name = "name", QObject *parent = 0);

    float fps();
    float length();

    QVariantMap toVariantMap();
    void fromVariantMap(QVariantMap map);

    QString name() const;
    void setName(const QString &name);

public slots:
    void addFrame(Frame frame);

};

#endif // ANIMATION_H
