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

#ifndef MARKER_H
#define MARKER_H

#include "webcamcapprimitives_global.h"

#include <QVector3D>

class WEBCAMCAPPRIMITIVESSHARED_EXPORT Marker
{
    int m_id = -1;
    QVector3D m_position;

public:
    Marker();
    Marker(int id, QVector3D positionVector);

    QVariantMap toVariantMap();
    void fromVariantMap(QVariantMap map);

    QVector3D position() const;
    void setPosition(const QVector3D &position);

    int id() const;
    void setId(int id);

    ///operators
    Marker operator -(Marker &point);
    Marker operator *(float &val);
    Marker operator +(Marker point);
};

#endif // MARKER_H
