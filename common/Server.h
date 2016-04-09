#ifndef STEROIDS_SERVER_H
#define STEROIDS_SERVER_H


#include "Socket.h"
#include "Arena.h"

constexpr auto ARENA_WIDTH = 32; // segments
constexpr auto ARENA_HEIGHT = 32; // segments

class Server {
private:

    Socket socket;
    Arena &arena;

    void receiveMessages();

    void sendMessages();

public:

    Server(Arena &world);

    void restart();

    void tick();

};


#endif //STEROIDS_SERVER_H
