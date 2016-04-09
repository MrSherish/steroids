#ifndef STEROIDS_SOCKET_H
#define STEROIDS_SOCKET_H

#include "Packet.h"

#include <SDL_net.h>

#include <cstdint>

class Socket {

private:

public:

    UDPsocket sd = 0;

    UDPpacket *pt = nullptr;

public:

    static const uint16_t ANY_PORT = 0;

    Socket(uint16_t port);

    virtual ~Socket();

    void send(const Packet &packet);

    bool receive(Packet &packet);

};

#endif //STEROIDS_SOCKET_H
