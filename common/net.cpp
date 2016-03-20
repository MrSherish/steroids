#include "net.h"

#include <SDL_net.h>
#include <cassert>

namespace net {

void init() {
    int rv = SDLNet_Init();
    assert(rv == 0);
}

void quit() {
    SDLNet_Quit();
}

uint32_t resolveHost(std::string host, uint16_t port) {
    IPaddress ip;
    int rv = SDLNet_ResolveHost(&ip, host.c_str(), port);
    assert(rv == 0);
    return SDLNet_Read32(&ip.host);
}

}

