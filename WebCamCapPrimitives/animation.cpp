#include "animation.h"

Animation::Animation(QString name, QObject *parent) :
    QObject(parent)
{
    this->m_name = name;
}

float Animation::fps()
{
    return m_elapsedMilliSecondsTime / (float) m_frames.size();
}

QVariantMap Animation::toVariantMap()
{
    QVariantMap retVal;

    return retVal;
}

void Animation::fromVariantMap(QVariantMap map)
{

}

void Animation::addFrame(Frame frame)
{
    m_frames.append(frame);

    m_elapsedMilliSecondsTime += frame.elapsedTime();
}
