#ifndef POINTCHECKER_H
#define POINTCHECKER_H

#include <vector>
#include <QQueue>
#include <glm/glm.hpp>

#include "line.h"
#include "munkres.h"
#include <cstdlib>

#include <opencv2/opencv.hpp>

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
    size_t numOfPoints = 1;

    QQueue<size_t> lastRemovedIDs;

    std::vector<Point> lastPoints;
    std::vector<Point> lastGoodFrame;
    Munkres m_munkres;

public:
    PointChecker();

    size_t getNumOfPoints() const;
    void setNumOfPoints(const size_t &value);

    std::vector<Point> getLastPoints() const {return lastPoints;}

    std::vector<Point> solvePointIDs(std::vector<glm::vec3> points);
    std::vector<Point> solvePointIDs(std::vector<glm::vec2> points);
private:

    std::vector<Point> handleNo(std::vector<glm::vec3> &points);
    std::vector<Point> handleNotEnough(std::vector<glm::vec3> &points);
    std::vector<Point> handleGood(std::vector<glm::vec3> &points);

    std::vector<std::vector<double> > createDistanceMap(std::vector<Point> lastPoints, std::vector<glm::vec3> points);
    void checkRemovedIndexes();
    size_t nextUniqueIndex(int size);
    void addUncoveredPoints(std::vector<glm::vec3> points, std::vector<std::vector<double> > map, std::vector<Point> &pts);
    std::vector<Point> addCoveredPoints(std::vector<glm::vec3> points, std::vector<std::vector<double> > map);
    void handleRemovedPoints(std::vector<Point> points);
};

#endif // POINTCHECKER_H
