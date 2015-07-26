#include "pointchecker.h"

#include "line.h"
#include "std_2d_vector.h"

#include <QVector>

size_t PointChecker::getNumOfPoints() const
{
    return numOfPoints;
}

void PointChecker::setNumOfPoints(const size_t &value)
{
    numOfPoints = value;
}

PointChecker::PointChecker()
{
}

QVector<Marker> PointChecker::solvePointIDs(QVector<QVector3D> points)
{
    QVector<Marker> pts;

    if(points.empty())
    {
        state = PointCount::NO;
        ++noFrameDuration;
        return pts;
    }

    switch (state) {
    case PointCount::NO:
    {
        pts = handleNo(points);
        break;
    }
    case PointCount::NOTENOUGH:
    {
        pts = handleNotEnough(points);
        break;
    }
    case PointCount::GOOD:
    {
        pts = handleGood(points);
        break;
    }
    case PointCount::TOOMANY:
    {
        pts = handleNotEnough(points);
        break;
    }
    default:
        break;
    }

    if(points.size() < lastPoints.size())
    {
        handleRemovedPoints(pts);
    }

    noFrameDuration = 0;
    lastPoints = pts;

    return pts;

}

QVector<Marker> PointChecker::solvePointIDs(QVector<QVector2D> points)
{
    QVector<QVector3D> pts;

    for(const QVector2D &point : points)
    {
        pts.push_back(QVector3D(point.x(), point.y(), 0.0f));
    }

    return solvePointIDs(pts);
}

QVector<Marker> PointChecker::handleNo(QVector<QVector3D> &points)
{
    QVector<Marker> pts;

    if(points.size() < numOfPoints)
    {
        state = PointCount::NOTENOUGH;
    }
    else if(points.size() == numOfPoints)
    {
        state = PointCount::GOOD;
    }
    else if(points.size() > numOfPoints)
    {
        state = PointCount::TOOMANY;
    }

    if(noFrameDuration < maxNoFrameDuration && !lastGoodFrame.empty())
    {
        auto m = createDistanceMap(lastGoodFrame, points);

        if(! m.empty())
        {
            my_solve(m);
        }

        pts = addCoveredPoints(points, m);
    }
    else
    {
        lastRemovedIDs.clear();

        for(size_t i = 0; i < points.size(); i++)
        {
            pts.push_back({nextUniqueIndex(i), points[i]});
        }
    }

    return pts;
}

QVector<Marker> PointChecker::handleNotEnough(QVector<QVector3D> &points)
{
    QVector<Marker> pts;

    std::vector< std::vector<double>> m = createDistanceMap(lastPoints, points);

    if(! m.empty())
    {
        my_solve(m);
    }

    pts = addCoveredPoints(points,m);

    if(points.size() < numOfPoints)
    {
        state = PointCount::NOTENOUGH;

        addUncoveredPoints(points,m,pts);

    }
    else if(points.size() == numOfPoints)
    {
        state = PointCount::GOOD;

        if(!lastGoodFrame.empty())
        {
            m = createDistanceMap(lastGoodFrame, points);

            if(! m.empty())
            {
                my_solve(m);
            }
        }
        addUncoveredPoints(points, m, pts);

    }
    else if(points.size() > numOfPoints)
    {
        state = PointCount::TOOMANY;

        addUncoveredPoints(points, m, pts);
    }

    return pts;
}

QVector<Marker> PointChecker::handleGood(QVector<QVector3D> &points)
{
    QVector<Marker> pts;

    std::vector< std::vector<double>> m = createDistanceMap(lastPoints, points);

    if(! m.empty())
    {
        my_solve(m);
    }

    pts = addCoveredPoints(points,m);

    if(pts.size() == numOfPoints)
    {
        state = PointCount::GOOD;
        lastGoodFrame = pts;
    }
    else
    {
        state = PointCount::NOTENOUGH;
    }

    if(points.size() > numOfPoints)
    {
        for(int i = 0; i < points.size(); i++)
        {
            bool ok = false;
            for(int j = 0; j < pts.size(); j++)
            {
                QVector3D pnt = pts[j].position();

                if(points[i] == pnt)
                {
                    ok = true;
                    break;
                }
            }
            if(!ok)
            {
                points.erase(points.begin()+i);
            }
        }
    }

    return pts;
}


std::vector<std::vector<double> > PointChecker::createDistanceMap(QVector<Marker> lastPoints, QVector<QVector3D> points)
{
    std::vector< std::vector<double>> matrix;

    for(int i = 0; i < lastPoints.size(); i++)
    {
        std::vector<double> vec;
        for(int j = 0; j < points.size(); j++)
        {
            QVector3D f = lastPoints[i].position();

            vec.push_back( f.distanceToPoint(points[j]));
        }
        matrix.push_back(vec);
    }

return matrix;
}

size_t PointChecker::nextUniqueIndex(int size)
{
    if(lastRemovedIDs.empty())
    {
        return size;
    }
    else
    {
        size_t val = lastRemovedIDs.front();
        lastRemovedIDs.pop_front();
        return val;
    }
}

void PointChecker::addUncoveredPoints(QVector<QVector3D> points, std::vector<std::vector<double> > map, QVector<Marker> &pts)
{
    size_t rows = map.size();

    if(rows == 0)
    {
        return;
    }

    size_t cols = map[0].size();

    for(size_t i = 0; i < cols; i++)
    {
        bool ok  = false;
        for(size_t j = 0; j < rows; j++)
        {
            if(map[j][i] == 0)
            {
                ok = true;
                break;
            }
        }
        if(!ok)
        {
            pts.push_back({nextUniqueIndex(pts.size()), points[i]});
        }
    }
}

QVector<Marker> PointChecker::addCoveredPoints(QVector<QVector3D> points, std::vector<std::vector<double> > map)
{
    QVector<Marker> pts;

    for(size_t i = 0; i < map.size(); i++)
    {
        for(size_t j = 0; j < map[i].size(); j++)
        {
            if(map[i][j] == 0)
            {
                if(i < lastPoints.size() && j < points.size())
                {
                    pts.push_back({lastPoints[i].id(), points[j]});
                }
            }
        }
    }

    return pts;
}

void PointChecker::handleRemovedPoints(QVector<Marker> points)
{
    for(int i = 0; i < lastPoints.size(); i++)
    {
        bool found = false;
        for(int j = 0; j < points.size(); j++)
        {
            if(lastPoints[i].id() == points[j].id())
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            if(lastPoints[i].id() <= lastPoints.size())
                lastRemovedIDs.push_back(lastPoints[i].id());
        }
    }
}
