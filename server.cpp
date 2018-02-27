#include "server.h"

Server::Server(QObject* parent)
    : QTcpServer{ parent }
{
}

bool Server::isLocal(const QHostAddress& address, quint16 port) const
{
    return address.isEqual(serverAddress()) && serverPort() == port;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    emit participantCame(socketDescriptor);
}
