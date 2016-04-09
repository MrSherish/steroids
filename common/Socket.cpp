#include "Socket.h"

#include "Packet.h"

#include <SDL_net.h>

#include <algorithm>

constexpr int PACKET_SIZE = 1024;

using nlohmann::json;

Socket::Socket(uint16_t port) {
    sd = SDLNet_UDP_Open(port);
    assert(sd);

    pt = SDLNet_AllocPacket(PACKET_SIZE);
    assert(pt);
}

Socket::~Socket() {
    SDLNet_FreePacket(pt);
}

void Socket::send(const Packet &packet) {
    std::string jd = packet.data.dump();

    SDLNet_Write32(packet.ip, &pt->address.host);
    SDLNet_Write16(packet.port, &pt->address.port);

    assert(jd.length() + 1 <= PACKET_SIZE);

    std::copy(jd.data(), jd.data() + jd.length() + 1, pt->data);

    pt->len = (int)jd.length() + 1;

    int rv = SDLNet_UDP_Send(sd, -1, pt);
    assert(rv);
}


bool Socket::receive(Packet &packet) {
    if (SDLNet_UDP_Recv(sd, pt)) {
        char *data = (char*) pt->data;
        data[PACKET_SIZE - 1] = '\0';

        std::string js = data;
        json j = json::parse(js);

        packet.ip = SDLNet_Read32(&pt->address.host);
        packet.port = SDLNet_Read16(&pt->address.port);

        packet.data = j;

        return true;
    } else {
        return false;
    }
}