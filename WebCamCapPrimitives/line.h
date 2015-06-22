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

    static bool closestPointsTwoLines(Line line1, Line line, QVector3D &closestPointLine1, QVector3D &closestPointLine2);
    static float distanceTwoPoints(QVector3D point1, QVector3D point2);
    static QVector3D averagePoint(QVector3D point1, QVector3D point2);
    static float lineAngle(Line l1, Line l2);
    static float lineAngle(QVector2D v1, QVector2D v2);
    static QVector3D intersection(Line &l1, Line &l2, float Epsilon);
};



#endif // LINE_H
