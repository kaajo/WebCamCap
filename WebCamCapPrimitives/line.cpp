#include "line.h"

#include <QString>
#include <QVariant>
#include <QVariantMap>

Line::Line()
{
}

Line::Line(QVector3D position, QVector3D direction) : m_position(position) , m_direction(direction)
{
    
}

bool Line::closestPointsTwoLines(Line line1, Line line2, QVector3D &closestPointLine1, QVector3D &closestPointLine2)
{
    float a = QVector3D::dotProduct(line1.m_direction, line1.m_direction);
    float b = QVector3D::dotProduct(line1.m_direction, line2.m_direction);
    float e = QVector3D::dotProduct(line2.m_direction, line2.m_direction);

    float d = a*e - b*b;

    // if lines are not parallel
    if(d != 0)
    {
        QVector3D r = line1.m_position - line2.m_position;
        float c = QVector3D::dotProduct(line1.m_direction, r);
        float f = QVector3D::dotProduct(line2.m_direction, r);

        closestPointLine1 = line1.m_position + line1.m_direction * (b*f - c*e) / d;
        closestPointLine2 = line2.m_position + line2.m_direction * (a*f - c*b) / d;

        return true;
    }

    return false;
}

float Line::distanceTwoPoints(QVector3D point1, QVector3D point2)
{
    QVector3D vector = point2 - point1;

    return sqrt(vector.x() * vector.x() + vector.y() * vector.y() + vector.z() * vector.z());
}

QVector3D Line::averagePoint(QVector3D point1, QVector3D point2)
{
    return QVector3D((point1.x() + point2.x())/2,(point1.y() + point2.y())/2,(point1.z() + point2.z())/2);
}

float Line::lineAngle(QVector3D v1, QVector3D v2)
{
    return std::atan2(QVector3D::crossProduct(v1, v2).length(), QVector3D::dotProduct(v1, v2));
}

float Line::lineAngle(QVector2D v1, QVector2D v2)
{
    return atan2(v2.y(), v2.x()) - atan2(v1.y(), v1.x());
}

bool Line::intersection(Line &l1, Line &l2, float epsilon,  QVector3D &point)
{
    QVector3D point1, point2;

    if(! Line::closestPointsTwoLines(l1, l2, point1, point2))
    {
        return false;
    }

    if(epsilon > Line::distanceTwoPoints(point1, point2))
    {
        l1.m_numberOfIntersections += 1;
        l2.m_numberOfIntersections += 1;

        point = Line::averagePoint(point1, point2);

        return true;
    }

    return false;
}

QVector3D Line::position() const
{
    return m_position;
}

QVector3D Line::direction() const
{
    return m_direction;
}

void Line::setDirection(const QVector3D &direction)
{
    m_direction = direction;
}

//################################################################################

const QString positionKey("position");
const QString directionKey("direction");
const QString nOfIntersectionsKey("numberOfIntersections");

QVariantMap Line::toVariantMap() const
{
    QVariantMap varMap;

    varMap[positionKey] = m_position;
    varMap[directionKey] = m_direction;
    varMap[nOfIntersectionsKey] = (uint) m_numberOfIntersections;

    return varMap;
}

bool Line::fromVariantMap(QVariantMap &map)
{
    if(! map.contains(positionKey) || ! map.contains(directionKey) ||
       ! map.contains(nOfIntersectionsKey))
    {
        return false;
    }

    m_position = map.value(positionKey).value<QVector3D>();
    m_direction = map.value(directionKey).value<QVector3D>();
    m_numberOfIntersections = map.value(nOfIntersectionsKey).toUInt();

    return true;
}
