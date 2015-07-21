#include "frame.h"

Frame::Frame()
{

}

Frame::Frame(int elapsed, QVector<Marker> markers, QVector<QVector<Line>> lines)
{
    this->m_elapsedTime = elapsed;
    this->m_markers = markers;
    this->m_lines = lines;
}

int Frame::elapsedTime() const
{
    return m_elapsedTime;
}

void Frame::setElapsedTime(int elapsedTime)
{
    m_elapsedTime = elapsedTime;
}

QVector<Marker> Frame::markers() const
{
    return m_markers;
}

void Frame::setMarkers(const QVector<Marker> &markers)
{
    m_markers = markers;
}

QVector<QVector<Line>> Frame::lines() const
{
    return m_lines;
}

void Frame::setLines(const QVector<QVector<Line>> &lines)
{
    m_lines = lines;
}

//#########################################################

QString elapsedKey("elapsedTime");
QString linesKey("lines");
QString markersKey("markers");

QVariantMap Frame::toVariantMap() const
{
    QVariantMap varMap;

    varMap[elapsedKey] = m_elapsedTime;

    QVariantList linesList;

    for(auto &vector : m_lines)
    {
        QVariantList nestedList;

        for(const Line &line: vector)
        {
            nestedList.append(line.toVariantMap());
        }

        linesList.append(nestedList);
    }

    varMap[linesKey] = linesList;

    QVariantList markersList;

    foreach (const Marker &marker, m_markers) {
        markersList.append(marker.toVariantMap());
    }

    varMap[markersKey] = markersList;

    return varMap;
}

bool Frame::fromVariantMap(QVariantMap map)
{
    if(! map.contains(elapsedKey) || ! map.contains(linesKey) || ! map.contains(markersKey))
    {
        return false;
    }

    m_elapsedTime = map.value(elapsedKey).toInt();

    return true;
}
