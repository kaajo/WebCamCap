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

#ifndef LINE_H
#define LINE_H

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

typedef struct Pnt
{
    size_t m_id;
    glm::vec3 m_position;

} Point;

class Line
{
public:
    bool found = false;
    glm::vec3 m_position;
    glm::vec3 m_directionVector;

    size_t m_numberOfIntersections = 0;

    Line();
    Line(glm::vec3 pos, glm::vec3 vec);

    static void closestPointsOnTwoLines(Line l1, Line l2, glm::vec3 &closestPointLine1, glm::vec3 &closestPointLine2);
    static glm::vec3 intersectionLinePlane(Line l1, Line Plane);
    static float distanceTwoPoints(glm::vec3 point1, glm::vec3 point2);
    static float distancePointPlane(glm::vec3 point, Line Plane);
    static glm::vec3 averagePoint(glm::vec3 point1, glm::vec3 point2);
    static float lineAngle(Line l1, Line l2);
    static float lineAngle(glm::vec2 v1, glm::vec2 v2);
    static glm::vec3 intersection(Line &l1, Line &l2, float Epsilon);
};

std::ostream& operator << (std::ostream &stream,const glm::vec3 &position);
std::ostream& operator << (std::ostream &stream,const glm::vec2 &position);
std::ostream& operator << (std::ostream &stream, const Line &line);
std::ostream& operator << (std::ostream &stream, const Point &pnt);

#endif // LINE_H
