#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include "line.h"

#include <QtNetwork/QLocalServer>
#include <QtNetwork/QLocalSocket>
#include <QObject>

class LocalServer : public QObject
{
    Q_OBJECT

    QString m_serverName = "webcamcap";

    bool m_usePipe = false;
    QLocalServer *m_server = nullptr;
    QLocalSocket *m_socket = nullptr;

public:
    explicit LocalServer(QObject *parent = 0);
    ~LocalServer();

    QLocalServer *getServer() const;
    void setServer(QLocalServer *value);

    void setServerUsed(bool pipe);

    void sendMessage(std::vector<Pnt> &Points);
    void sendMessageString(std::string str);

private slots:
    void handleConnection();
    void handleDisconnected();

};

#endif // LOCALSERVER_H
