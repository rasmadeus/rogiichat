#include "client.h"
#include "main_view.h"
#include "participant.h"
#include "server.h"

MainView::MainView(Server& server, Client& client, QWidget* parent)
    : QMainWindow{ parent }
    , _server{ server }
    , _client{ client }
    , _participantsModel{ this }
    , _peersModel{ this }
{
    _ui.setupUi(this);
    _ui.participantsView->setModel(&_participantsModel);
    _ui.participantNameLineEdit->setText(Participant::defaultName());

    connect(&_server, &Server::participantCame, &_participantsModel, static_cast<void(ParticipantsModel::*)(qintptr)>(&ParticipantsModel::appendParticipant));
    connect(&_client, &Client::serverFound, &_peersModel, static_cast<void(ParticipantsModel::*)(const QHostAddress&, quint16)>(&ParticipantsModel::appendParticipant));
}
