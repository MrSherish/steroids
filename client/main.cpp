#include "../common/net.h"
#include "../common/Packet.h"
#include "../common/Socket.h"

#include "../common/json.hpp"
#include "Window.h"
#include "Dialog.h"

int main (int argc, char** argv)
{
    std::string serverHost = "localhost", nick = "player0";
    if(argc >= 2) {
        serverHost = argv[1];
        if (argc >= 3) {
            nick = argv[2];
        } else {
            Dialog *d = new Dialog(nick, 0);
            nick = d->show("Enter your nickname:");
            delete d;
        }
    }
    else {
        Dialog *d = new Dialog(serverHost, 0);
        serverHost = d->show("Enter IP address to connect to:");
        delete d;
        d = new Dialog(nick, 0);
        nick = d->show("Enter your nickname:");
        delete d;

    }

    srand((unsigned)time(nullptr));
    net::init();

    Window window(serverHost);
    window.enterEventLoop();

    net::quit();

    return EXIT_SUCCESS;
}