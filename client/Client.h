#ifndef STEROIDS_CLIENT_H
#define STEROIDS_CLIENT_H

#include "../common/Socket.h"
#include "../common/Arena.h"
#include "../common/net.h"
#include "../common/Server.h"

class Client {
private:

    Socket socket;
    Arena &arena;
    int myPlayerId = 0;

    void addPlayer(int playerId, std::string nick);

public:

    Client(Arena &world, std::string nick);

    void receiveMessages();

    void onPlayerConnected(json j);

    void onSnakeMoved(json j);

    void changeDir(vec2 dir);

};


#endif //STEROIDS_CLIENT_H
