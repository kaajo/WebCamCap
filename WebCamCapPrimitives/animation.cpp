#include "animation.h"


QString Animation::name() const
{
    return m_name;
}

void Animation::setName(const QString &name)
{
    m_name = name;
}
Animation::Animation(QString name, QObject *parent) :
    QObject(parent)
{
    this->m_name = name;
}

float Animation::fps()
{
    return m_elapsedMilliSecondsTime / (float) m_frames.size();
}

float Animation::length()
{
    return m_elapsedMilliSecondsTime / 1000.0;
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
