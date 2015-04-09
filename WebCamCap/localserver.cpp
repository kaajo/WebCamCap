#include "localserver.h"
#include "sstream"

QLocalServer *LocalServer::getServer() const
{
    return m_server;
}

void LocalServer::setServer(QLocalServer *value)
{
    m_server = value;
}
LocalServer::LocalServer(QObject *parent) :
    QObject(parent)
{

    m_server = new QLocalServer();
    m_server->setMaxPendingConnections(1);

    connect(m_server, SIGNAL(newConnection()), this, SLOT(handleConnection()));
}

LocalServer::~LocalServer()
{


    if(m_usePipe)
    {
        setServerUsed(false);
    }
}

void LocalServer::handleConnection()
{
    qDebug() << "connect";

    m_socket = m_server->nextPendingConnection();

    if(m_socket != nullptr)
    {
        connect(m_socket, SIGNAL(disconnected()), this, SLOT(handleDisconnected()));
    }
}

void LocalServer::handleDisconnected()
{
    delete m_socket;
    m_socket = nullptr;
}

void LocalServer::setServerUsed(bool pipe)
{
    if(pipe)
    {
        if(!m_server->listen(m_serverName))
        {
            if(m_server->removeServer(m_serverName))
            {
                if(!m_server->listen(m_serverName))
                {
                    qDebug() << "Server down somehow!";
                    m_usePipe = false;
                    return;
                }
            }
            else
            {
                qDebug() << "Server down somehow!";
                m_usePipe = false;
                return;
            }


        }
        qDebug() << "server connected";
        m_usePipe = true;

    }
    else
    {

        m_server->close();

        if(m_socket != nullptr)
        {
            m_socket->disconnectFromServer();
            delete m_socket;
            m_socket = nullptr;
        }

        qDebug() << "Server closed";
        m_usePipe = false;
    }
}

void LocalServer::sendMessage(std::vector<Pnt> &Points)
{
    if(!m_usePipe)
    {
        return;
    }

    std::stringstream ss;

    ss << " " << Points.size();

    for(size_t i = 0; i < Points.size(); i++)
    {
        ss << " " << Points[i];
    }

    ss << std::endl;


    std::string msg = ss.str();

    //std::cout << msg << std::endl;

    sendMessageString(msg);
}

void LocalServer::sendMessageString(std::string str)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;

    out << QString::fromStdString(str);

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    if(m_socket != nullptr && m_socket->isOpen() && m_socket->isWritable())
    {
        m_socket->write(block);
        m_socket->flush();
    }
}
