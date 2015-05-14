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

#ifndef ANIMATION_H
#define ANIMATION_H

#include "frame.h"

class Animation
{
    glm::vec3 m_roomDimensions;
    float m_frameRate = 0.0f;

    std::string m_name;
    std::vector<Frame> m_frames;
public:
    Animation(glm::vec3 roomdims, std::string name = "Animation Default");

    void addFrame(Frame k);
    void save(std::string file);
    void postProcess();

    std::string getName() const {return m_name;}
    float getFrameRate() const {return m_frameRate;}
    int getLength() const {return  m_frames.size();}

    void setName(std::string name) {m_name = name;}

};

#endif // ANIMATION_H
