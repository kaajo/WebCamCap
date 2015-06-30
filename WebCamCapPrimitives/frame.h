#ifndef FRAME_H
#define FRAME_H

#include "webcamcapprimitives_global.h"

#include "marker.h"
#include "line.h"

#include <QVector>
#include <QVariantMap>

class WEBCAMCAPPRIMITIVESSHARED_EXPORT Frame
{
    int m_elapsedTime = 0;
    QVector<Marker> m_markers;
    QVector<QVector<Line>> m_lines;

public:
    Frame();
    Frame(int elapsed, QVector<Marker> markers, QVector<QVector<Line>> lines);

    int elapsedTime() const;
    void setElapsedTime(int elapsedTime);

    QVector<Marker> markers() const;
    void setMarkers(const QVector<Marker> &markers);

    QVector<QVector<Line>> lines() const;
    void setLines(const QVector<QVector<Line>> &lines);

    QVariantMap toVariantMap() const;
    void fromVariantMap(QVariantMap varMap);
};

#endif // FRAME_H
