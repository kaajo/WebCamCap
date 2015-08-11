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

#ifndef WCCTOFBXEXPORTER_H
#define WCCTOFBXEXPORTER_H

#include <fbxsdk/scene/fbxscene.h>
#include <fbxsdk/scene/animation/fbxanimlayer.h>
#include <fbxsdk/scene/geometry/fbxnode.h>

#include <QVector3D>

using namespace FBXSDK_NAMESPACE;

class WccToFbxExporter
{
public:
    WccToFbxExporter();

    static FbxNode *createSphere(FbxScene* pScene, const char* pName, float size);
    static void addPositionKeyToNode(FbxNode* pNode, FbxAnimLayer* pAnimLayer, int time, const QVector3D &position);
};

#endif // WCCTOFBXEXPORTER_H
