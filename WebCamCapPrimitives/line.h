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

/**
 * @author Miroslav Krajicek
 * @brief The Line class represents parametric line/ray in 3D space.
 */
class WEBCAMCAPPRIMITIVESSHARED_EXPORT Line
{
    QVector3D m_position;
    QVector3D m_direction;

    size_t m_numberOfIntersections = 0;

public:

    /**
     * @brief Default constructor - inits position and direction to [0,0,0].
     */
    Line();

    /**
     * @brief Constructor
     * @param position arbitrary point on line/ray
     * @param direction direction of line/ray
     */
    Line(QVector3D position, QVector3D direction);

    QVariantMap toVariantMap() const;
    bool fromVariantMap(QVariantMap map);

    QVector3D position() const;
    void setPosition(const QVector3D &position);
    QVector3D direction() const;
    void setDirection(const QVector3D &direction);

    /**
     * @brief Finds closest points on two lines.
     * @param[in] line1 First line
     * @param[in] line2 Second line
     * @param[out] closestPointLine1 Point on line1
     * @param[out] closestPointLine2 Point on line2
     * @return If lines are parallel returns false, otherwise true.
     */
    static bool closestPointsTwoLines(const Line &line1,const Line &line2, QVector3D &closestPointLine1, QVector3D &closestPointLine2);

    /**
     * @brief averagePoint
     * @param[in] point1 first point
     * @param[in] point2 second point
     * @return Midpoint of line segment.
     */
    static QVector3D averagePoint(const QVector3D &point1, const QVector3D &point2);
    static float lineAngle(const QVector3D &v1, const QVector3D &v2);
    static float lineAngle(const QVector2D &v1, const QVector2D &v2);

    /**
     * @brief Function using triangulation to find intersection. Ideally, lines have zero distance from each other.
     * @param[inout] l1
     * @param[inout] l2
     * @param[in] maxError
     * @param[out] point 3D point
     * @return If lines are closer than \a maxError returns true, otherwise false.
     */
    static bool intersection(Line &l1, Line &l2,const float maxError, QVector3D &point);
};


#endif // LINE_H
