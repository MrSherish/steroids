#include "../common/net.h"
#include "../common/Packet.h"
#include "../common/Socket.h"

#include "../common/json.hpp"
#include "Window.h"


int main (int argc, char** argv)
{
    srand((unsigned)time(nullptr));
    net::init();

    Window window;
    window.enterEventLoop();

    net::quit();

    return EXIT_SUCCESS;
}