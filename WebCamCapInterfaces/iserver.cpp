#include "iserver.h"


QString IServer::name() const
{
    return m_name;
}

void IServer::setName(const QString &name)
{
    m_name = name;
}

IServer::IServer(QObject *parent) :
    QObject(parent)
{
}

IServer::~IServer()
{
    
}
