#include "client.h"
#include "log.h"
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

    connect(&_peersModel, &ParticipantsModel::connected, _ui.log, &Log::info);
    connect(&_peersModel, &ParticipantsModel::disconnected, _ui.log, &Log::info);
    connect(&_peersModel, &ParticipantsModel::error, _ui.log, &Log::error);

    connect(&_participantsModel, &ParticipantsModel::msgCame, _ui.log, &Log::normal);

    connect(_ui.changeNameButton, &QPushButton::clicked, this, &MainView::setName);
    connect(_ui.participantNameLineEdit, &QLineEdit::returnPressed, this, &MainView::setName);
    connect(_ui.sendMsgButton, &QPushButton::clicked, this, &MainView::send);
    connect(_ui.msgLineEdit, &QLineEdit::returnPressed, this, &MainView::send);

    connect(_ui.removeParticipantButton, &QPushButton::clicked, this, &MainView::ban);
}

void MainView::send()
{
    const auto msg = _ui.msgLineEdit->text();
    if (!msg.isEmpty())
    {
        _ui.log->accent(_ui.participantNameLineEdit->text(), msg);
        _peersModel.send(msg);
        _ui.msgLineEdit->clear();
    }
}

void MainView::setName()
{
    const auto name = _ui.participantNameLineEdit->text();
    _peersModel.setName(name.isEmpty() ? Participant::defaultName() : name);
}

void MainView::ban()
{
    _participantsModel.ban(_ui.participantsView->currentIndex());
}
