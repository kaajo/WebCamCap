#include "animation.h"

#include "wcctofbxexporter.h"

#include <fbxsdk/fileio/fbxexporter.h>
#include <fbxsdk/core/fbxmanager.h>

#include <QDebug>

Animation::Animation(QVector3D roomDims, QString name, QObject *parent) :
    QObject(parent)
{
    m_roomDimensions = roomDims;
    m_name = name;
}

float Animation::fps()
{
    return m_elapsedMilliSecondsTime / (float) m_frames.size();
}

float Animation::length()
{
    return m_elapsedMilliSecondsTime / 1000.0;
}

float Animation::lengthMS()
{
    return m_elapsedMilliSecondsTime;
}

bool Animation::save(QString folder)
{
    // ex:
    // create a SdkManager
    FbxManager* lSdkManager = FbxManager::Create();
    // create an IOSettings object
    FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    // set some IOSettings options
    ios->SetBoolProp(EXP_FBX_MATERIAL, true);
    ios->SetBoolProp(EXP_FBX_TEXTURE,  true);
    ios->SetBoolProp(EXP_FBX_SHAPE, true);
    ios->SetBoolProp(EXP_FBX_GOBO,            true);
    ios->SetBoolProp(EXP_FBX_ANIMATION,       true);
    ios->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);
    // create an empty scene
    FbxScene* pScene = FbxScene::Create(lSdkManager, m_name.toStdString().c_str());

    QMap<int,FbxNode*> nodes;

    FbxNode* lRootNode = pScene->GetRootNode();


    // Create the Animation Stack
    FbxAnimStack* lAnimStack = FbxAnimStack::Create(pScene, "Show all faces");
    // The animation nodes can only exist on AnimLayers therefore it is mandatory to
    // add at least one AnimLayer to the AnimStack. And for the purpose of this example,
    // one layer is all we need.
    FbxAnimLayer* lAnimLayer = FbxAnimLayer::Create(pScene, "Base animation Layer");
    lAnimStack->AddMember(lAnimLayer);

    int time = 0;

    foreach (const Frame &frame, m_frames)
    {
        time += frame.elapsedTime();

        foreach (const Marker &marker, frame.markers())
        {
            int id = marker.id();

            if(! nodes.contains(id))
            {
                FbxNode *newMarker = WccToFbxExporter::createSphere(pScene, (QString("marker_")+QString::number(id)).toStdString().c_str() , 1.0f);
                nodes[id] = newMarker;
                lRootNode->AddChild(newMarker);
            }

            WccToFbxExporter::addPositionKeyToNode(nodes[id], lAnimLayer, time, marker.position()*m_roomDimensions);
        }
    }

    // create an exporter.
    FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");
    // initialize the exporter by providing a filename and the IOSettings to use
    QString target = (folder + "/" + name()+ ".fbx");
    qDebug() << "animation saved to: " << target;
    lExporter->Initialize(target.toStdString().c_str(), -1, ios);
    // export the scene.
    if( ! lExporter->Export(pScene))
    {
        return false;
    }

    // destroy the exporter
    lExporter->Destroy();

    return true;
}

QVariantMap Animation::toVariantMap()
{
    QVariantMap retVal;

    return retVal;
}

void Animation::fromVariantMap(QVariantMap map)
{

}

QString Animation::name() const
{
    return m_name;
}

void Animation::setName(const QString &name)
{
    m_name = name;
}

QVector3D Animation::roomDimensions() const
{
    return m_roomDimensions;
}

void Animation::addFrame(Frame frame)
{
    m_frames.append(frame);

    m_elapsedMilliSecondsTime += frame.elapsedTime();
}
