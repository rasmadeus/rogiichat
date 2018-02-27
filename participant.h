#ifndef PARTICIPANT_H
#define PARTICIPANT_H


class Participant : public QObject
{
    Q_OBJECT

public:
    explicit Participant(QObject* parent);
    explicit Participant(const QHostAddress& ip, quint16 port, QObject* parent);
    explicit Participant(qintptr socketDescriptor, QObject* parent);

    static QString defaultName() { return tr("Anonym"); }

    QString caption() const;
    bool isPeer(const QHostAddress& ip, quint16 port) const;

signals:
    void lost();

private:
    void show(QAbstractSocket::SocketError er);
    QTcpSocket _socket;
    QString _name = defaultName();
};

#endif // PARTICIPANT_H
