#include "wcctofbxexporter.h"

#include <QDebug>

#include <fbxsdk/core/base/fbxstring.h>
#include <fbxsdk/core/base/fbxtime.h>
#include <fbxsdk/core/base/fbxfile.h>

#include <fbxsdk/scene/animation/fbxanimcurve.h>
#include <fbxsdk/scene/animation/fbxanimcurvenode.h>

#include <fbxsdk/scene/geometry/fbxmesh.h>

WccToFbxExporter::WccToFbxExporter()
{

}

FbxNode *WccToFbxExporter::createCube(FbxScene *pScene, const char *pName, float size)
{
    size /= 2.0;

    int i, j;
    FbxMesh* lMesh = FbxMesh::Create(pScene, pName);
    FbxVector4 lControlPoint0(-size, 0, size);
    FbxVector4 lControlPoint1(size, 0, size);
    FbxVector4 lControlPoint2(size, size*2, size);
    FbxVector4 lControlPoint3(-size, size*2, size);
    FbxVector4 lControlPoint4(-size, 0, -size);
    FbxVector4 lControlPoint5(size, 0, -size);
    FbxVector4 lControlPoint6(size, size*2, -size);
    FbxVector4 lControlPoint7(-size, size*2, -size);
    FbxVector4 lNormalXPos(1, 0, 0);
    FbxVector4 lNormalXNeg(-1, 0, 0);
    FbxVector4 lNormalYPos(0, 1, 0);
    FbxVector4 lNormalYNeg(0, -1, 0);
    FbxVector4 lNormalZPos(0, 0, 1);
    FbxVector4 lNormalZNeg(0, 0, -1);
    // Create control points.
    lMesh->InitControlPoints(24);
    FbxVector4* lControlPoints = lMesh->GetControlPoints();
    lControlPoints[0] = lControlPoint0;
    lControlPoints[1] = lControlPoint1;
    lControlPoints[2] = lControlPoint2;
    lControlPoints[3] = lControlPoint3;
    lControlPoints[4] = lControlPoint1;
    lControlPoints[5] = lControlPoint5;
    lControlPoints[6] = lControlPoint6;
    lControlPoints[7] = lControlPoint2;
    lControlPoints[8] = lControlPoint5;
    lControlPoints[9] = lControlPoint4;
    lControlPoints[10] = lControlPoint7;
    lControlPoints[11] = lControlPoint6;
    lControlPoints[12] = lControlPoint4;
    lControlPoints[13] = lControlPoint0;
    lControlPoints[14] = lControlPoint3;
    lControlPoints[15] = lControlPoint7;
    lControlPoints[16] = lControlPoint3;
    lControlPoints[17] = lControlPoint2;
    lControlPoints[18] = lControlPoint6;
    lControlPoints[19] = lControlPoint7;
    lControlPoints[20] = lControlPoint1;
    lControlPoints[21] = lControlPoint0;
    lControlPoints[22] = lControlPoint4;
    lControlPoints[23] = lControlPoint5;
    // We want to have one normal for each vertex (or control point),
    // so we set the mapping mode to eByControlPoint.
    FbxGeometryElementNormal* lGeometryElementNormal= lMesh->CreateElementNormal();
    lGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
    // Here are two different ways to set the normal values.
    bool firstWayNormalCalculations=true;
    if (firstWayNormalCalculations)
    {
        // The first method is to set the actual normal value
        // for every control point.
        lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
    }
    else
    {
        // The second method is to the possible values of the normals
        // in the direct array, and set the index of that value
        // in the index array for every control point.
        lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
        // Add the 6 different normals to the direct array
        lGeometryElementNormal->GetDirectArray().Add(lNormalZPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalZNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalXNeg);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYPos);
        lGeometryElementNormal->GetDirectArray().Add(lNormalYNeg);
        // Now for each control point, we need to specify which normal to use
        lGeometryElementNormal->GetIndexArray().Add(0); // index of lNormalZPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(0); // index of lNormalZPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(0); // index of lNormalZPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(0); // index of lNormalZPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(1); // index of lNormalXPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(1); // index of lNormalXPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(1); // index of lNormalXPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(1); // index of lNormalXPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(2); // index of lNormalZNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(2); // index of lNormalZNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(2); // index of lNormalZNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(2); // index of lNormalZNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(3); // index of lNormalXNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(3); // index of lNormalXNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(3); // index of lNormalXNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(3); // index of lNormalXNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(4); // index of lNormalYPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(4); // index of lNormalYPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(4); // index of lNormalYPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(4); // index of lNormalYPos in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(5); // index of lNormalYNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(5); // index of lNormalYNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(5); // index of lNormalYNeg in the direct array.
        lGeometryElementNormal->GetIndexArray().Add(5); // index of lNormalYNeg in the direct array.
    }
    // Array of polygon vertices.
    int lPolygonVertices[] = { 0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15,
        16, 17, 18, 19,
        20, 21, 22, 23 };
    // Create UV for Diffuse channel
    FbxGeometryElementUV* lUVDiffuseElement = lMesh->CreateElementUV("DiffuseUV");
    FBX_ASSERT( lUVDiffuseElement != NULL);
    lUVDiffuseElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
    lUVDiffuseElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
    FbxVector2 lVectors0(0, 0);
    FbxVector2 lVectors1(1, 0);
    FbxVector2 lVectors2(1, 1);
    FbxVector2 lVectors3(0, 1);
    lUVDiffuseElement->GetDirectArray().Add(lVectors0);
    lUVDiffuseElement->GetDirectArray().Add(lVectors1);
    lUVDiffuseElement->GetDirectArray().Add(lVectors2);
    lUVDiffuseElement->GetDirectArray().Add(lVectors3);
    // Create UV for Ambient channel
    FbxGeometryElementUV* lUVAmbientElement = lMesh->CreateElementUV("AmbientUV");
    lUVAmbientElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
    lUVAmbientElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
    lVectors0.Set(0, 0);
    lVectors1.Set(1, 0);
    lVectors2.Set(0, 0.418586879968643);
    lVectors3.Set(1, 0.418586879968643);
    lUVAmbientElement->GetDirectArray().Add(lVectors0);
    lUVAmbientElement->GetDirectArray().Add(lVectors1);
    lUVAmbientElement->GetDirectArray().Add(lVectors2);
    lUVAmbientElement->GetDirectArray().Add(lVectors3);
    // Create UV for Emissive channel
    FbxGeometryElementUV* lUVEmissiveElement = lMesh->CreateElementUV("EmissiveUV");
    lUVEmissiveElement->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
    lUVEmissiveElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
    lVectors0.Set(0.2343, 0);
    lVectors1.Set(1, 0.555);
    lVectors2.Set(0.333, 0.999);
    lVectors3.Set(0.555, 0.666);
    lUVEmissiveElement->GetDirectArray().Add(lVectors0);
    lUVEmissiveElement->GetDirectArray().Add(lVectors1);
    lUVEmissiveElement->GetDirectArray().Add(lVectors2);
    lUVEmissiveElement->GetDirectArray().Add(lVectors3);
    //Now we have set the UVs as eIndexToDirect reference and in eByPolygonVertex  mapping mode
    //we must update the size of the index array.
    lUVDiffuseElement->GetIndexArray().SetCount(24);
    lUVAmbientElement->GetIndexArray().SetCount(24);
    lUVEmissiveElement->GetIndexArray().SetCount(24);
    // Create polygons. Assign texture and texture UV indices.
    for(i = 0; i < 6; i++)
    {
        //we won't use the default way of assigning textures, as we have
        //textures on more than just the default (diffuse) channel.
        lMesh->BeginPolygon(-1, -1, false);
        for(j = 0; j < 4; j++)
        {
            //this function points
            lMesh->AddPolygon(lPolygonVertices[i*4 + j] // Control point index.
            );
            //Now we have to update the index array of the UVs for diffuse, ambient and emissive
            lUVDiffuseElement->GetIndexArray().SetAt(i*4+j, j);
            lUVAmbientElement->GetIndexArray().SetAt(i*4+j, j);
            lUVEmissiveElement->GetIndexArray().SetAt(i*4+j, j);
        }
        lMesh->EndPolygon ();
    }
    FbxNode* lNode = FbxNode::Create(pScene, pName);
    lNode->SetNodeAttribute(lMesh);
    lNode->SetShadingMode(FbxNode::eTextureShading);

    lNode->LclTranslation.Set(FbxVector4(0.0, 0.0, 0.0));
    lNode->LclRotation.Set(FbxVector4(0.0, 0.0, 0.0));
    lNode->LclScaling.Set(FbxVector4(1.0, 1.0, 1.0));

    return lNode;
}

void WccToFbxExporter::addPositionKeyToNode(FbxNode *pNode, FbxAnimLayer *pAnimLayer, int timeMs, const QVector3D &position)
{
    FbxAnimCurve* lCurve = nullptr;
    FbxTime lTime;
    int lKeyIndex = 0;
    lCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
    if (lCurve)
    {
        lCurve->KeyModifyBegin();
        lTime.SetMilliSeconds(timeMs);

        lKeyIndex = lCurve->KeyAdd(lTime);
        lCurve->KeySet(lKeyIndex, lTime, position.x(), FbxAnimCurveDef::eInterpolationCubic);

        lCurve->KeyModifyEnd();
    }

    lCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
    if (lCurve)
    {
        lCurve->KeyModifyBegin();
        lTime.SetMilliSeconds(timeMs);

        lKeyIndex = lCurve->KeyAdd(lTime);
        lCurve->KeySet(lKeyIndex, lTime, position.y(), FbxAnimCurveDef::eInterpolationCubic);

        lCurve->KeyModifyEnd();
    }

    lCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
    if (lCurve)
    {
        lCurve->KeyModifyBegin();
        lTime.SetMilliSeconds(timeMs);

        lKeyIndex = lCurve->KeyAdd(lTime);
        lCurve->KeySet(lKeyIndex, lTime, position.z(), FbxAnimCurveDef::eInterpolationCubic);

        lCurve->KeyModifyEnd();
    }
}

