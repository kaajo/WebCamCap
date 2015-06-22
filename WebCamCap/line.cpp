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

#include "line.h"
#include <glm/gtx/norm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <cmath>

using glm::vec2;
using glm::vec3;
using glm::dot;

Line::Line()
{

}

Line::Line(vec3 pos, vec3 vec)
{
    m_position = pos;
    m_directionVector = vec;
}

void Line::closestPointsOnTwoLines(Line l1, Line l2, vec3 &closestPointLine1, vec3 &closestPointLine2)
{
    float a = dot(l1.m_directionVector, l1.m_directionVector);
    float b = dot(l1.m_directionVector, l2.m_directionVector);
    float e = dot(l2.m_directionVector, l2.m_directionVector);

    float d = a*e - b*b;

    // if lines are not parallel
    if(d != 0)
    {
        vec3 r = l1.m_position - l2.m_position;
        float c = dot(l1.m_directionVector, r);
        float f = dot(l2.m_directionVector, r);

        float s = (b*f - c*e) / d;
        float t = (a*f - c*b) / d;

        closestPointLine1 = l1.m_position + l1.m_directionVector * s;
        closestPointLine2 = l2.m_position + l2.m_directionVector * t;

        //std::cout << "distance of 2 lines: " << distanceTwoPoints(closestPointLine1, closestPointLine2) << std::endl;
    }
}

vec3 Line::intersectionLinePlane(Line L, Line Plane)
{
    float d = Plane.m_directionVector.x * -Plane.m_position.x + Plane.m_directionVector.y * -Plane.m_position.y  + Plane.m_directionVector.z * -Plane.m_position.z;

    float t =       -(Plane.m_directionVector.x * L.m_position.x + Plane.m_directionVector.y * L.m_position.y + Plane.m_directionVector.z * L.m_position.z + d)/
                       (Plane.m_directionVector.x * L.m_directionVector.x + Plane.m_directionVector.y * L.m_directionVector.y + Plane.m_directionVector.z * L.m_directionVector.z);

    return L.m_position + t * L.m_directionVector;
}

float Line::distanceTwoPoints(vec3 point1, vec3 point2)
{
    vec3 vector = point2 - point1;

    return glm::sqrt( vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

float Line::distancePointPlane(vec3 Point, Line Plane)
{
    float    sb, sn, sd;

    sn = -dot( Plane.m_directionVector, (Point - Plane.m_position));
    sd = dot(Plane.m_directionVector, Plane.m_directionVector);
    sb = sn / sd;

    vec3 intersection = Point + sb * Plane.m_directionVector;
    return distanceTwoPoints(Point, intersection);
}

vec3 Line::averagePoint(vec3 point1, vec3 point2)
{
    return vec3((point1.x + point2.x)/2,(point1.y + point2.y)/2,(point1.z + point2.z)/2);
}

float Line::lineAngle(Line l1, Line l2)
{
    return acos( dot(l1.m_directionVector, l2.m_directionVector)/sqrt(glm::length2(l1.m_directionVector) * glm::length2(l2.m_directionVector)) );
}

float Line::lineAngle(vec2 v1, vec2 v2)
{
    return atan2(v2.y,v2.x) - atan2(v1.y, v1.x);
    //CCW is positive
}

vec3 Line::intersection(Line &l1, Line &l2, float Epsilon)
{
    vec3 point1 = vec3(0.0f, 0.0f, 0.0f), point2 = vec3(0.0f, 0.0f, 0.0f);

    Line::closestPointsOnTwoLines(l1, l2, point1, point2);
    if(Epsilon > Line::distanceTwoPoints(point1, point2))
    {
        l2.m_numberOfIntersections += 1;
        l2.found = true;

        std::cout << "3D virtual position: " << Line::averagePoint(point1, point2);

        return Line::averagePoint(point1, point2);
    }
    else
    {
        return vec3(0.0f,0.0f,0.0f);
    }
}

//operators

std::ostream& operator << (std::ostream &stream,const vec3 &position)
{
    stream << position.x << " " << position.y << " " << position.z << " ";

    return stream;
}

std::ostream& operator << (std::ostream &stream,const vec2 &position)
{
    stream << position.x << " " << position.y;

    return stream;
}

std::ostream& operator <<(std::ostream &stream, const Line &line)
{
    stream << "Line position: " << line.m_position << "direction: " << line.m_directionVector;

    return stream;
}

std::ostream &operator <<(std::ostream &stream, const Point &pnt)
{
    stream << pnt.m_id << " " << pnt.m_position;

    return stream;
}
