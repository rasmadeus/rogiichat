#ifndef PARTICIPANTS_MODEL_H
#define PARTICIPANTS_MODEL_H

class Participant;

class ParticipantsModel : public QAbstractListModel
{
public:
    explicit ParticipantsModel(QObject* parent);

    int	rowCount(const QModelIndex& parent) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant data(const QModelIndex& index, int role) const override;

public:
    void appendParticipant(qintptr socketDescriptor);
    void appendParticipant(const QHostAddress& ip, quint16 port);

private:
    void appendParticipant(Participant* participant);
    void removeParticipant();

private:
    QVector<Participant*> _participants;
};

#endif // PARTICIPANTS_MODEL_H
