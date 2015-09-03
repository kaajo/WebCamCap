/*
 *
 * Copyright (C) 2015  Miroslav Krajicek (https://github.com/kaajo).
 * All Rights Reserved.
 *
 * This file is part of WebCamCap.
 *
 * WebCamCap is free software: you can redistribute it and/or modify
 * it under the terms of the GNU LGPL version 3 as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WebCamCap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU LGPL version 3
 * along with WebCamCap. If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 *
 */

#ifndef FRAME_H
#define FRAME_H

#include "webcamcapprimitives_global.h"

#include "marker.h"
#include "line.h"

#include <QVector>
#include <QVariantMap>

/**
 * @brief Frame contains all information captured in one image.
 */
class WEBCAMCAPPRIMITIVESSHARED_EXPORT Frame
{
    int m_elapsedTime = 0;
    int m_maximalPointID = -1;
    QVector<Marker> m_markers;
    QVector<QVector<Line>> m_lines;

public:
    /**
     * @brief Frame
     */
    Frame();
    /**
     * @brief Constructor
     * @param elapsed elapsed milliseconds since last #Frame
     * @param markers captured Markers
     * @param lines captured Lines
     */
    Frame(int elapsed, QVector<Marker> markers, QVector<QVector<Line>> lines);

    int elapsedTime() const;
    void setElapsedTime(int elapsedTime);

    QVector<Marker> markers() const;
    QVector<QVector<Line>> lines() const;

    bool changeMarkerId(int oldID, int newID);

    QVariantMap toVariantMap() const;
    bool fromVariantMap(QVariantMap varMap);
};

#endif // FRAME_H
