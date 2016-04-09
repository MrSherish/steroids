#include "Server.h"

void Server::receiveMessages() {

}

void Server::sendMessages() {

}

Server::Server(Arena &world) : socket(Socket::ANY_PORT), arena(world) {

}

void Server::restart() {
    arena = Arena{ARENA_WIDTH, ARENA_HEIGHT};

    Snake s{{0, 0}, {1, 0}, {255, 0, 0, 255}};
    arena.snakes.push_back(s);
}

void Server::tick() {
    for (Snake &s : arena.snakes) {
        s.proceed(arena.width, arena.height);
    }
}
