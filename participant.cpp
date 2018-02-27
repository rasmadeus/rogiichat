#include "participant.h"

namespace
{
    QByteArray castToBytes(int value)
    {
        QByteArray buf;
        QDataStream stream(&buf, QIODevice::WriteOnly);
        stream << value;
        return buf;
    }

    int castToInt(QByteArray data)
    {
        int res = 0;
        QDataStream stream(&data, QIODevice::ReadOnly);
        stream >> res;
        return res;
    }
}

Participant::Participant(QObject* parent)
    : QObject{ parent }
{
    connect(&_socket, &QTcpSocket::readyRead, this, &Participant::onReadyRead);

    connect(&_socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &Participant::onError);
    connect(&_socket, &QTcpSocket::connected, this, &Participant::onConnected);

    connect(&_socket, &QTcpSocket::disconnected, this, &Participant::lost);
    connect(&_socket, static_cast<void(QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &Participant::lost);
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

void Participant::send(const QString& msg)
{
    sendData(MsgType::Msg, msg);
}

void Participant::setName(const QString& name)
{
    _name = name;
    sendData(MsgType::Name, _name);
}

QString Participant::peerInfo() const
{
    return tr("%1:%2").arg(_socket.peerAddress().toString()).arg(_socket.peerPort());
}

bool Participant::isPeer(const QHostAddress& ip, quint16 port) const
{
    return _socket.peerAddress().isEqual(ip) && _socket.peerPort() == port;
}

void Participant::onConnected()
{
    emit connected(_name, tr("Connected to: %1").arg(peerInfo()));
}

void Participant::onDisconnected()
{
    emit connected(_name, tr("Disconnected from: %1").arg(peerInfo()));
}

void Participant::onError(QAbstractSocket::SocketError socketError)
{
    emit error(_name, tr("Connection error to: %1. Error code: %2").arg(peerInfo()).arg(socketError));
}

void Participant::onReadyRead()
{
    while (_socket.bytesAvailable() > 0)
    {
        _data.append(_socket.readAll());
        processData();
    }
}

void Participant::processData()
{
    static const int typeSize = sizeof(int);
    static const int msgSize = sizeof(int);
    static const int headerSize = typeSize + msgSize;

    if (_data.size() < headerSize)
        return;

    const int msgLength = ::castToInt(_data.mid(typeSize, msgSize));
    const int dataLength = headerSize + msgLength;

    if (_data.size() < dataLength)
        return;

    const auto type = static_cast<MsgType>(::castToInt(_data.left(typeSize)));
    const auto msg = QString::fromUtf8(_data.mid(headerSize, msgLength));

    _data.remove(0, dataLength);

    processMsg(type, msg);
}

void Participant::processMsg(MsgType type, const QString& msg)
{
    switch (type)
    {
    case MsgType::Name:
        _name = msg;
        emit nameChanged();
        break;
    case MsgType::Msg:
        emit msgCame(_name, msg);
        break;
    }
}

void Participant::sendData(MsgType type, const QString& msg)
{
    auto message = msg.toUtf8();
    QByteArray data;
    data.append(::castToBytes(static_cast<int>(type)));
    data.append(::castToBytes(message.size()));
    data.append(std::move(message));

    _socket.write(data);
    _socket.flush();
    _socket.waitForBytesWritten();
}
