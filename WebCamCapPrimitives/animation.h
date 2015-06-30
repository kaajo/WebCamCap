#ifndef ANIMATION_H
#define ANIMATION_H

#include "webcamcapprimitives_global.h"

#include "frame.h"

#include <QString>
#include <QVector>
#include <QObject>
#include <QVariantMap>

class WEBCAMCAPPRIMITIVESSHARED_EXPORT Animation : public QObject
{
    Q_OBJECT

    QString m_name;
    long int m_elapsedMilliSecondsTime = 0;
    QVector<Frame> m_frames;

public:
    explicit Animation(QString name, QObject *parent = 0);

    float fps();

    QVariantMap toVariantMap();
    void fromVariantMap(QVariantMap map);

public slots:
    void addFrame(Frame &frame);

};

#endif // ANIMATION_H
