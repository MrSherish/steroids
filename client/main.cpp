#include "../common/net.h"
#include "../common/Packet.h"
#include "../common/Socket.h"

#include "../common/json.hpp"
#include "Window.h"


int main (int argc, char** argv)
{
    Window window;
    window.enterEventLoop();

    return EXIT_SUCCESS;
}