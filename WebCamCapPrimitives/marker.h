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
