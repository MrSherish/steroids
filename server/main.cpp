#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <SDL_net.h>
#include "../common/net.h"
#include "Server.h"

int main(int argc, char **argv) {
    net::init();

    Arena arena;
    Server server{arena};
    server.run();

    net::quit();

    return EXIT_SUCCESS;
}