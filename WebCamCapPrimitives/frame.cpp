#include "frame.h"

#include "tr1/functional"

Frame::Frame()
{

}

Frame::Frame(int elapsed, QVector<Marker> markers, QVector<QVector<Line>> lines)
{
    m_elapsedTime = elapsed;
    m_markers = markers;
    m_lines = lines;

    for(const Marker &marker : markers)
    {
        if(marker.id() > m_maximalPointID)
        {
            m_maximalPointID = marker.id();
        }
    }
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

QVector<QVector<Line>> Frame::lines() const
{
    return m_lines;
}

bool Frame::changeMarkerId(int oldID, int newID)
{
    auto func = [](const int id, const Marker &marker){return id == marker.id();};

    auto point = std::find_if(m_markers.begin(), m_markers.end(), [func, oldID](const Marker &marker){return func(oldID, marker);});

    if(point == m_markers.end()) return false;

    auto pointWithSameID = std::find_if(m_markers.begin(), m_markers.end(), [func, newID](const Marker &marker){return func(newID, marker);});

    if(pointWithSameID != m_markers.end())
    {
        ++m_maximalPointID;

        changeMarkerId(newID, m_maximalPointID);
    }

    point->setId(newID);
    return true;
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

    foreach (const Marker &marker, m_markers)
    {
        markersList.append(marker.toVariantMap());
    }

    varMap[markersKey] = markersList;

    return varMap;
}

bool Frame::fromVariantMap(QVariantMap varMap)
{
    if(! varMap.contains(elapsedKey) || ! varMap.contains(linesKey) || ! varMap.contains(markersKey))
    {
        return false;
    }

    QVariantList linesList = varMap[linesKey].toList();

    foreach(const QVariant &variant, linesList)
    {
        QVariantList nestedList = variant.toList();
        QVector<Line> lineVector;

        foreach(const QVariant &variant, nestedList)
        {
            Line line;
            line.fromVariantMap(variant.toMap());

            lineVector.push_back(line);
        }

        m_lines.push_back(lineVector);
    }

    QVariantList markersList = varMap[markersKey].toList();

    foreach(const QVariant &variant, markersList)
    {
        Marker marker;

        marker.fromVariantMap(variant.toMap());

        m_markers.push_back(marker);
    }

    m_elapsedTime = varMap[elapsedKey].toInt();

    return true;
}
