#include "../common/net.h"
#include "../common/Packet.h"
#include "../common/Socket.h"

#include "../common/json.hpp"
#include "Window.h"
#include "Dialog.h"

int main (int argc, char** argv)
{
    std::string serverHost = "localhost";
    if(argc >= 2) {
        serverHost = argv[1];
    }
    else {
        Dialog d("Enter IP address to connect to:", 0);
        serverHost = d.show();
    }

    srand((unsigned)time(nullptr));
    net::init();

    Window window(serverHost);
    window.enterEventLoop();

    net::quit();

    return EXIT_SUCCESS;
}