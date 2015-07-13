#ifndef POINTCHECKER_H
#define POINTCHECKER_H

#include <vector>
#include <QQueue>
#include <glm/glm.hpp>

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
    Munkres m_munkres;

public:
    PointChecker();

    size_t getNumOfPoints() const;
    void setNumOfPoints(const size_t &value);

    QVector<Marker> getLastPoints() const {return lastPoints;}

    QVector<Marker> solvePointIDs(QVector<QVector3D> points2);
    QVector<Marker> solvePointIDs(QVector<glm::vec2> points);
private:

    QVector<Marker> handleNo(QVector<glm::vec3> &points);
    QVector<Marker> handleNotEnough(QVector<glm::vec3> &points);
    QVector<Marker> handleGood(QVector<glm::vec3> &points);

    std::vector<std::vector<double> > createDistanceMap(QVector<Marker> lastPoints, QVector<glm::vec3> points);
    void checkRemovedIndexes();
    size_t nextUniqueIndex(int size);
    void addUncoveredPoints(QVector<glm::vec3> points, std::vector<std::vector<double> > map, QVector<Marker> &pts);
    QVector<Marker> addCoveredPoints(QVector<glm::vec3> points, std::vector<std::vector<double> > map);
    void handleRemovedPoints(QVector<Marker> points);
};

#endif // POINTCHECKER_H
