#ifndef STEROIDS_SERVER_H
#define STEROIDS_SERVER_H


#include "../common/Socket.h"
#include "../common/Arena.h"

using nlohmann::json;

class Server {
private:

    Socket socket;
    Arena &arena;
    int nextPlayerId = 0;

    void receiveMessages();

    void broadcast(nlohmann::json j);

    void onConnect(Packet p);

    void snakeMoved(const Snake &s);

    void sendMessages();

    int getPlayerId(uint32_t ip, uint16_t port);

    Snake & getPlayerSnake(int playerId);

public:

    static const auto ARENA_WIDTH = 16; // segments
    static const auto ARENA_HEIGHT = 16; // segments
    static const int PORT = 28666;

    Server(Arena &world);

    void restart();

    void tick();

    void run();

    void onDir(Packet packet);
};


#endif //STEROIDS_SERVER_H
