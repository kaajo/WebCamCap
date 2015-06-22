#ifndef MARKER_H
#define MARKER_H

#include <QVector3D>

class Marker
{
    int m_id = -1;
    QVector3D m_direction;

public:
    Marker();
    Marker(int id, QVector3D directionVector);

    QVector3D direction() const;
    void setDirection(const QVector3D &direction);

    int id() const;
    void setId(int id);
};

#endif // MARKER_H
