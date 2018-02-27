#include "client.h"
#include "main_view.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QApplication app{ argc, argv };

    Server server{ nullptr };
    Client client{ server, nullptr };

    MainView mainView{ server, client, nullptr };
    mainView.show();

    server.listen();
    client.start();

    return app.exec();
}
