#ifndef SERVER_H
#define SERVER_H

#include "pch.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    explicit Server(QObject* parent);
    bool isLocal(const QHostAddress& address, quint16 port) const;

signals:
    void participantCame(qintptr socketDescriptor);

protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H
