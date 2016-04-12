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

	void addPoints(const Fruit&f, const Snake&s);

    void broadcast(nlohmann::json j);

    void onConnect(Packet p);
    
    void sendFruits();

    void snakeMoved(const Snake &s);

    void sendMessages();

    int getPlayerId(uint32_t ip, uint16_t port);

    Snake & getPlayerSnake(int playerId);

    void onDir(Packet packet);

    void moveSnakes();

    void handleCollisions();

    void handleFruits();

    void handleEating(Snake &snake, Fruit &fruit);

    void broadcastSnakeMoved();

    void killSnake(Snake &snake);

    void handleSnakesCollision(Snake &a, Snake &b, std::vector<Snake *> & vector);
    
    void spawnFruit();

public:

    static const auto ARENA_WIDTH = 17; // segments
    static const auto ARENA_HEIGHT = 17; // segments
    static const int PORT = 28666;

    Server(Arena &world);

    void restart();

    void tick();

    void run();
};


#endif //STEROIDS_SERVER_H
