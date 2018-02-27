#ifndef MAIN_VIEW_H
#define MAIN_VIEW_H

#include "participants_model.h"
#include "ui_main_view.h"

class Client;
class Server;

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainView(Server& server, Client& client, QWidget* parent);

private:
    void send();
    void setName();

private:
    Ui::MainView _ui;
    Server& _server;
    Client& _client;
    ParticipantsModel _participantsModel;
    ParticipantsModel _peersModel;
};

#endif // MAIN_VIEW_H
