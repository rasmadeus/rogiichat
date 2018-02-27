#include "participant.h"

Participant::Participant(QObject* parent)
    : QObject{ parent }
{
    connect(&_socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &Participant::lost);
    connect(&_socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &Participant::show);
    connect(&_socket, &QTcpSocket::disconnected, this, &Participant::lost);
}

Participant::Participant(const QHostAddress &ip, quint16 port, QObject* parent)
    : Participant{ parent }
{
    _socket.connectToHost(ip, port);
}

Participant::Participant(qintptr socketDescriptor, QObject* parent)
    : Participant{ parent }
{
    _socket.setSocketDescriptor(socketDescriptor);
}

QString Participant::caption() const
{
    return tr("%1 (%2:%3)").arg(_name).arg(_socket.peerAddress().toString()).arg(_socket.peerPort());
}

bool Participant::isPeer(const QHostAddress& ip, quint16 port) const
{
    return _socket.peerAddress().isEqual(ip) && _socket.peerPort() == port;
}

void Participant::show(QAbstractSocket::SocketError er)
{
    qDebug() << er;
}
