#ifndef ISERVER_H
#define ISERVER_H

#include "webcamcapinterfaces_global.h"

#include <QObject>
#include <QVariantMap>

class WEBCAMCAPINTERFACESSHARED_EXPORT IServer : public QObject
{
    Q_OBJECT

protected:
    bool m_enabled = false;
    QString m_name;

public:
    explicit IServer(QObject *parent = 0);
    virtual ~IServer();

    virtual bool setEnabled(bool enabled) = 0;
    virtual void sendMesage(QVariantMap &message) = 0;

    QString name() const;
    void setName(const QString &name);

    virtual QVariantMap toVariantMap() const = 0;
    virtual void fromVariantMap(QVariantMap varMap) = 0;

signals:

public slots:
    virtual void newConnection() = 0;
    virtual void socketDisconnected() = 0;
};

#endif // ISERVER_H
