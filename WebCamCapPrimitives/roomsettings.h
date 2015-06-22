#ifndef ROOMSETTINGS_H
#define ROOMSETTINGS_H

#include <QVector3D>
#include <QObject>

class RoomSettings : public QObject
{
    Q_OBJECT

    QString m_name;
    QVector3D m_roomDimensions;
    double m_maxError;

    bool m_recordScene = false;
    bool m_captureAnimation = false;
    bool m_saved = false;

public:
    explicit RoomSettings(QObject *parent = 0);

signals:
    void changed();

public slots:

};

#endif // ROOMSETTINGS_H
