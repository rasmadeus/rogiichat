#ifndef PARTICIPANTS_MODEL_H
#define PARTICIPANTS_MODEL_H

class Participant;

class ParticipantsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ParticipantsModel(QObject* parent);

    int	rowCount(const QModelIndex& parent) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role) const override;

public:
    void appendParticipant(qintptr socketDescriptor);
    void appendParticipant(const QHostAddress& ip, quint16 port);
    void setName(const QString& name);
    void send(const QString& msg);
    void ban(const QModelIndex& index);

signals:
    void connected(const QString& header, const QString& body);
    void disconnected(const QString& header, const QString& body);
    void error(const QString& header, const QString& body);
    void msgCame(const QString& header, const QString& body);

private:
    void appendParticipant(Participant* participant);
    void removeParticipant();
    void onNameChanged();

private:
    QVector<Participant*> _participants;
};

#endif // PARTICIPANTS_MODEL_H
