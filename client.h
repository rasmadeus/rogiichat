#ifndef CLIENT_H
#define CLIENT_H

#include "pch.h"

class Server;

class Client : public QObject
{
    Q_OBJECT

public:
    explicit Client(Server& server, QObject* parent);
    void start();

signals:
    void serverFound(const QHostAddress& iup, quint16 port);

protected:
    void timerEvent(QTimerEvent* ev) override;

private:
    void notifyServerWorking(const QNetworkInterface& interface, const QByteArray& data);
    void onServerFound();
    QByteArray emptyData() const;

private:
    Server& _server;
    int _serverLookTimerId = -1;
    QUdpSocket _notifySocket;
};

#endif // CLIENT_H
