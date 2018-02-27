#include "client.h"
#include "server.h"

namespace
{
    const quint16 broadcastPort = 10000;
}

Client::Client(Server& server, QObject* parent)
    : QObject{ parent }
    , _server{ server }
{

    connect(&_notifySocket, &QUdpSocket::readyRead, this, &Client::onServerFound);
}

void Client::start()
{
    _notifySocket.bind(QHostAddress::Any, ::broadcastPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    static const std::chrono::seconds serverLookInterval{ 2 };
    _serverLookTimerId = startTimer(serverLookInterval);
}

void Client::timerEvent(QTimerEvent* ev)
{
    if (ev->timerId() == _serverLookTimerId)
    {
        const auto data = QByteArray::number(_server.serverPort());
        for(const auto& interface : QNetworkInterface::allInterfaces())
        {
            notifyServerWorking(interface, data);
        }
    }
}

void Client::notifyServerWorking(const QNetworkInterface& interface, const QByteArray& data)
{
    for(const auto& entry : interface.addressEntries())
    {
        const auto broadcastAddress = entry.broadcast();
        if (broadcastAddress != QHostAddress::Null && entry.ip() != QHostAddress::LocalHost)
        {
            _notifySocket.writeDatagram(data, broadcastAddress, ::broadcastPort);
        }
    }
}

void Client::onServerFound()
{
    while (_notifySocket.hasPendingDatagrams())
    {
        QHostAddress serverIp;
        quint16 senderPort = 0;
        auto data = emptyData();

        if (_notifySocket.readDatagram(data.data(), data.size(), &serverIp, &senderPort) == -1)
        {
            continue;
        }

        const quint16 serverPort = data.toInt();

        if (_server.isLocal(serverIp, serverPort))
        {
            continue;
        }

        emit serverFound(serverIp, serverPort);
    }
}

QByteArray Client::emptyData() const
{
    QByteArray data;
    data.resize(_notifySocket.pendingDatagramSize());
    return data;
}
