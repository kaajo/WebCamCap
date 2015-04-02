/*
 *
 * Copyright (C) 2014  Miroslav Krajicek, Faculty of Informatics Masaryk University (https://github.com/kaajo).
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

#include "frame.h"


Frame::Frame(int elapsed, std::vector<Point> pts, QVector<QVector<Line> > lines)
{
    m_points = pts;
    m_lines = lines;
    m_elapsedTime = elapsed;
}

std::ostream &operator <<(std::ostream &stream, Frame frame)
{
    stream << frame.getElapsedTime() << " ";

    auto points = frame.getPoints();

    stream << points.size() << " ";

    for(size_t i = 0; i < frame.getPoints().size(); i++)
    {
        stream << points[i] << " ";
    }

    return stream;
}
