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

#include "animation.h"
#include <fstream>

Animation::Animation(glm::vec3 roomdims, std::string name)
{
    m_roomDimensions = roomdims;
    m_name = name;
}

void Animation::AddFrame(Frame k)
{
    m_frames.push_back(k);
}

void Animation::Save(std::string file)
{
    std::ofstream outputFile;
    outputFile.open(file, std::ios_base::out | std::ios_base::trunc);

    outputFile << m_roomDimensions << m_frameRate << std::endl;
    outputFile << m_frames.size() << std::endl;

    outputFile << "# elapsed ms/ number of points / point id , position (2D/3D)" << std::endl;

    for(size_t i = 0; i < m_frames.size(); i++)
    {
        outputFile << m_frames[i] << std::endl;
    }

    outputFile.close();
}

void Animation::PostProcess()
{
    //count framerate
    int sum = 0;

    for(size_t i = 1; i < m_frames.size(); i++)
    {
        sum += m_frames[i].getElapsedTime();
    }

    m_frameRate = 1000 / ( sum/ (float) m_frames.size() );
}
