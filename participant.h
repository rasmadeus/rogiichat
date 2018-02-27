#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include "pch.h"

class Participant : public QObject
{
    Q_OBJECT

public:
    enum class MsgType
    {
        Name,
        Msg,
    };

    static QString defaultName() { return tr("Anonymous"); }

    explicit Participant(QObject* parent);
    explicit Participant(const QHostAddress& ip, quint16 port, QObject* parent);
    explicit Participant(qintptr socketDescriptor, QObject* parent);

    void close();
    void send(const QString& msg);
    const QString& name() const { return _name; }
    void setName(const QString& name);
    QString peerInfo() const;
    bool isPeer(const QHostAddress& ip, quint16 port) const;

signals:
    void connected(const QString& header, const QString& body);
    void disconnected(const QString& header, const QString& body);
    void error(const QString& header, const QString& body);
    void msgCame(const QString& header, const QString& body);
    void lost();
    void nameChanged();

private:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadyRead();

    void processData();
    void processMsg(MsgType type, const QString& msg);
    void sendData(MsgType type, const QString& msg);

private:
    QTcpSocket _socket;
    QString _name = defaultName();
    QByteArray _data;
};

#endif // PARTICIPANT_H
