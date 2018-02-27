#include "participant.h"
#include "participants_model.h"

ParticipantsModel::ParticipantsModel(QObject* parent)
    : QAbstractListModel{ parent }
{
}

int	ParticipantsModel::rowCount(const QModelIndex& parent) const
{
    std::ignore = parent;
    return _participants.size();
}

Qt::ItemFlags ParticipantsModel::flags(const QModelIndex& index) const
{
    std::ignore = index;
    return Qt::ItemIsEnabled;
}

QVariant ParticipantsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    switch(role)
    {
    case Qt::DisplayRole: return _participants[index.row()]->caption();
    default: return {};
    }
}

void ParticipantsModel::appendParticipant(qintptr socketDescriptor)
{
    appendParticipant(new Participant{ socketDescriptor, this });
}

void ParticipantsModel::appendParticipant(const QHostAddress& ip, quint16 port)
{
    const bool isNew = std::none_of(_participants.begin(), _participants.end(), [&ip, &port](const auto& item){ return item->isPeer(ip, port); });
    if (isNew)
    {
        appendParticipant(new Participant{ ip, port, this });
    }
}

void ParticipantsModel::appendParticipant(Participant* participant)
{
    connect(participant, &Participant::lost, this, &ParticipantsModel::removeParticipant);

    beginInsertRows({}, _participants.size(), _participants.size());
    _participants.push_back(participant);
    endInsertRows();
}

void ParticipantsModel::removeParticipant()
{
    const int i = _participants.indexOf(static_cast<Participant*>(sender()));
    auto participant = _participants[i];

    beginRemoveRows({}, i, i);
    _participants.remove(i);
    endRemoveRows();

    participant->deleteLater();
}
