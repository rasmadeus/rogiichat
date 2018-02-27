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
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ParticipantsModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    const auto* participant = _participants[index.row()];
    switch(role)
    {
    case Qt::DisplayRole: return participant->name();
    case Qt::ToolTipRole: return participant->peerInfo();
    default: return {};
    }
}

void ParticipantsModel::ban(const QModelIndex& index)
{
    if (index.isValid())
    {
        _participants[index.row()]->close();
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

void ParticipantsModel::setName(const QString& name)
{
    for(auto& participant : _participants)
    {
        participant->setName(name);
    }
}

void ParticipantsModel::send(const QString& msg)
{
    for(auto& participant : _participants)
    {
        participant->send(msg);
    }
}

void ParticipantsModel::appendParticipant(Participant* participant)
{
    connect(participant, &Participant::connected, this, &ParticipantsModel::connected);
    connect(participant, &Participant::disconnected, this, &ParticipantsModel::disconnected);
    connect(participant, &Participant::error, this, &ParticipantsModel::error);
    connect(participant, &Participant::lost, this, &ParticipantsModel::removeParticipant);
    connect(participant, &Participant::nameChanged, this, &ParticipantsModel::onNameChanged);
    connect(participant, &Participant::msgCame, this, &ParticipantsModel::msgCame);

    beginInsertRows({}, _participants.size(), _participants.size());
    _participants.push_back(participant);
    endInsertRows();
}

void ParticipantsModel::removeParticipant()
{
    const int i = _participants.indexOf(static_cast<Participant*>(sender()));
    if (i != -1)
    {
        auto participant = _participants[i];

        beginRemoveRows({}, i, i);
        _participants.remove(i);
        endRemoveRows();

        participant->deleteLater();
    }
}

void ParticipantsModel::onNameChanged()
{
    const int i = _participants.indexOf(static_cast<Participant*>(sender()));
    auto index = createIndex({}, i, i);
    emit dataChanged(index, index);
}
