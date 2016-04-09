#include "../common/net.h"
#include "../common/Packet.h"
#include "../common/Socket.h"

#include "../common/json.hpp"
#include <SDL_net.h>

using nlohmann::json;

int main_(int argc, char **argv) {
    /* Check for parameters */
    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    net::init();

    Socket socket(Socket::ANY_PORT);

    std::string serverHost = argv[1];
    uint16_t serverPort = (uint16_t) atoi(argv[2]);
    uint32_t serverIp = net::resolveHost(serverHost, serverPort);

    int quit = 0;
    while (!quit) {
        Packet p;
        p.ip = serverIp;
        p.port = serverPort;

        std::string s;
        std::cout << "> ";
        std::cin >> s;

        json j = {
                {"message", s},
                {"pi",      3.141},
                {"happy",   true},
                {"name",    "Niels"},
                {"nothing", nullptr},
                {"answer",  {{"everything", 42}}},
                {"list",    {1, 0, 2}},
                {"object",  {{"currency",   "USD"}, {"value", 42.99}}}
        };

        p.data = j;

        socket.send(p);

        if (s == "quit") {
            quit = 1;
        }
    }

    net::quit();

    return EXIT_SUCCESS;
}