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

    static FbxNode *createCube(FbxScene* pScene, const char* pName, float size);
    static void addPositionKeyToNode(FbxNode* pNode, FbxAnimLayer* pAnimLayer, int time, const QVector3D &position);
};

#endif // WCCTOFBXEXPORTER_H
