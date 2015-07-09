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

#ifndef LINE_H
#define LINE_H

#include "webcamcapprimitives_global.h"

#include <QVector2D>
#include <QVector3D>

class WEBCAMCAPPRIMITIVESSHARED_EXPORT Line
{
    QVector3D m_position;
    QVector3D m_direction;

    size_t m_numberOfIntersections = 0;

public:
    Line();
    Line(QVector3D position, QVector3D direction);

    QVariantMap toVariantMap() const;
    void fromVariantMap(QVariantMap &varMap);

    static bool closestPointsTwoLines(Line line1, Line line, QVector3D &closestPointLine1, QVector3D &closestPointLine2);
    static float distanceTwoPoints(QVector3D point1, QVector3D point2);
    static QVector3D averagePoint(QVector3D point1, QVector3D point2);
    static float lineAngle(Line l1, Line l2);
    static float lineAngle(QVector2D v1, QVector2D v2);
    static bool intersection(Line &l1, Line &l2, float Epsilon,  QVector3D &point);
};



#endif // LINE_H
