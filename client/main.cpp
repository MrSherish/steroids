#include "../common/net.h"
#include "../common/Packet.h"
#include "../common/Socket.h"

#include "../common/json.hpp"
#include "Window.h"


int main (int argc, char** argv)
{
    std::string serverHost = "localhost";
    if(argc >= 2) {
        serverHost = argv[1];
    }

    srand((unsigned)time(nullptr));
    net::init();

    Window window(serverHost);
    window.enterEventLoop();

    net::quit();

    return EXIT_SUCCESS;
}