#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <SDL_net.h>
#include "../common/net.h"
#include "Server.h"
#include "../common/ServerConfig.h"

int main(int argc, char **argv) {
    net::init();

    Arena arena;
    ServerConfig cfg = loadServerConfig();
    Server server{arena, cfg};
    server.run();

    net::quit();

    return EXIT_SUCCESS;
}