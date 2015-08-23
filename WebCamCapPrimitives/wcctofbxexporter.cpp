#include "wcctofbxexporter.h"

#include <QDebug>

#include <fbxsdk/core/base/fbxstring.h>
#include <fbxsdk/core/base/fbxtime.h>
#include <fbxsdk/core/base/fbxfile.h>

#include <fbxsdk/scene/animation/fbxanimcurve.h>
#include <fbxsdk/scene/animation/fbxanimcurvenode.h>

#include <fbxsdk/scene/geometry/fbxmesh.h>
#include <fbxsdk/scene/geometry/fbxnurbs.h>

WccToFbxExporter::WccToFbxExporter()
{

}

FbxNode *WccToFbxExporter::createSphere(FbxScene *pScene, const char *pName, float size)
{
    FbxNurbs* lNurbs = FbxNurbs::Create(pScene,pName);
    // Set nurbs properties.
    lNurbs->SetOrder(4, 4);
    lNurbs->SetStep(2, 2);
    lNurbs->InitControlPoints(8, FbxNurbs::ePeriodic, 7, FbxNurbs::eOpen);
    double lUKnotVector[] = { -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0 };
    memcpy(lNurbs->GetUKnotVector(), lUKnotVector, lNurbs->GetUKnotCount()*sizeof(double));
    double lVKnotVector[] = { 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 4.0, 4.0, 4.0 };
    memcpy(lNurbs->GetVKnotVector(), lVKnotVector, lNurbs->GetVKnotCount()*sizeof(double));
    FbxVector4* lVector4 = lNurbs->GetControlPoints();
    int i, j;
    double lPi = 3.14159;
    double lYAngle[] = { 90.0, 90.0, 52.0, 0.0, -52.0, -90.0, -90.0 };
    double lRadius[] = { 0.0, 0.283, 0.872, 1.226, 0.872, 0.283, 0.0};
    for (i = 0; i < 7; i++)
    {
        for (j = 0; j < 8; j++)
        {
            double lX = size * lRadius[i] * cos(lPi/4*j);
            double lY = size * sin(2*lPi/360*lYAngle[i]);
            double lZ = size * lRadius[i] * sin(lPi/4*j);
            double lWeight = 1.0;
            lVector4[8*i + j].Set(lX, lY, lZ, lWeight);
        }
    }
    FbxNode* lNode = FbxNode::Create(pScene,pName);
    lNode->SetNodeAttribute(lNurbs);
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

