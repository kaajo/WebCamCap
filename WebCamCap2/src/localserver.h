#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include "iserver.h"

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QLinkedList>

class LocalServer : public IServer
{
    Q_OBJECT

    QLocalServer *m_server = nullptr;
    QLinkedList<QLocalSocket*> m_sockets;

public:
    explicit LocalServer(QString name = "webcamcap", QObject *parent = 0);
    virtual ~LocalServer();

    virtual bool setEnabled(bool enabled) override;
    virtual void sendMesage(QVariantMap &message) override;

    virtual QVariantMap toVariantMap() const override;
    virtual void fromVariantMap(QVariantMap varMap) override;
private:
    virtual void newConnection() override;
    virtual void socketDisconnected() override;
};

#endif // LOCALSERVER_H
