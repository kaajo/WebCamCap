#ifndef ROOMSETTINGS_H
#define ROOMSETTINGS_H

#include "webcamcapprimitives_global.h"

#include <QVector3D>
#include <QObject>

class WEBCAMCAPPRIMITIVESSHARED_EXPORT RoomSettings : public QObject
{
    Q_OBJECT

    QString m_name = "Default project";
    QVector3D m_roomDimensions;
    double m_maxError;

    bool m_recordScene = false;
    bool m_recordAnimation = false;
    bool m_saved = false;

public:

    enum class RoomSettingsType {
        ALL,
        NAME,
        DIMENSIONS,
        MAXERROR,
        RECORD,
        RECORDANIMATION
    };

    explicit RoomSettings(QString name, QVector3D roomDims, double maxError , QObject *parent = 0);

    QString name() const;
    void setName(const QString &name);

    QVector3D roomDimensions() const;
    void setRoomDimensions(const QVector3D &roomDimensions);

    double maxError() const;
    void setMaxError(double maxError);

    bool recordScene() const;
    void setRecordScene(bool recordScene);

    bool recordAnimation() const;
    void setRecordAnimation(bool recordAnimation);

    bool saved() const;
    void save();

signals:
    void changed(RoomSettingsType type);

private slots:
    void setSave(RoomSettingsType type);

};

#endif // ROOMSETTINGS_H
