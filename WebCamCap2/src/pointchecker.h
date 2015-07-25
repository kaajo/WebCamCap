#ifndef POINTCHECKER_H
#define POINTCHECKER_H

#include <vector>
#include <QQueue>

#include "marker.h"
#include "line.h"
#include "munkres.h"
#include <cstdlib>

#include <opencv2/opencv.hpp>

#include <QVector>
#include <QVector3D>

enum class PointCount
{
    TOOMANY,
    GOOD,
    NOTENOUGH,
    NO
};

class PointChecker
{
    PointCount state = PointCount::NO;

    size_t noFrameDuration = 0;
    size_t maxNoFrameDuration = 30;
    size_t wrongFrameDuration = 0;
    size_t maxWrongFrameDuration; ///not using
    size_t maxIndex = 0;
    int numOfPoints = 1;

    QQueue<size_t> lastRemovedIDs;

    QVector<Marker> lastPoints;
    QVector<Marker> lastGoodFrame;

public:
    PointChecker();

    size_t getNumOfPoints() const;
    void setNumOfPoints(const size_t &value);

    QVector<Marker> getLastPoints() const {return lastPoints;}

    QVector<Marker> solvePointIDs(QVector<QVector3D> points2);
    QVector<Marker> solvePointIDs(QVector<QVector2D> points);
private:

    QVector<Marker> handleNo(QVector<QVector3D> &points);
    QVector<Marker> handleNotEnough(QVector<QVector3D> &points);
    QVector<Marker> handleGood(QVector<QVector3D> &points);

    std::vector<std::vector<double> > createDistanceMap(QVector<Marker> lastPoints, QVector<QVector3D> points);
    void checkRemovedIndexes();
    size_t nextUniqueIndex(int size);
    void addUncoveredPoints(QVector<QVector3D> points, std::vector<std::vector<double> > map, QVector<Marker> &pts);
    QVector<Marker> addCoveredPoints(QVector<QVector3D> points, std::vector<std::vector<double> > map);
    void handleRemovedPoints(QVector<Marker> points);
};

#endif // POINTCHECKER_H
