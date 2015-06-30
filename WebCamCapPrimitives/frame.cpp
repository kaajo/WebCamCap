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

QString elapsedKey("elapsedTime");
QString linesKey("lines");
QString markersKey("markers");

QVariantMap Frame::toVariantMap() const
{
    QVariantMap varMap;

    varMap[elapsedKey] = m_elapsedTime;

    QVariantMap lines;

    lines["i"] = m_lines.size();

    for(size_t i = 0; i < m_lines.size(); i++)
    {
        QVariantMap linesInside;

        linesInside["j"] = m_lines[i].size();

        for(size_t j = 0; j < m_lines[i].size(); j++)
        {
            lines[QString::number(j)] = m_lines[i][j].toVariantMap();
        }

        lines[QString::number(i)] = linesInside;
    }

    varMap[linesKey] = lines;

    return varMap;
}

void Frame::fromVariantMap(QVariantMap varMap)
{
    m_elapsedTime = varMap.value(elapsedKey).toInt();
}
