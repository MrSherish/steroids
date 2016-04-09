#ifndef STEROIDS_CLIENT_H
#define STEROIDS_CLIENT_H


#include "../common/Socket.h"
#include "../common/Arena.h"

class Client {
private:

    Socket socket;
    Arena &arena;

    void receiveMessages() {

    }

    void sendMessages() {

    }

public:

    Client(Arena &world) : socket(Socket::ANY_PORT), arena(world) {

    }

};


#endif //STEROIDS_CLIENT_H
