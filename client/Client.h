#ifndef STEROIDS_CLIENT_H
#define STEROIDS_CLIENT_H

#include "../common/Socket.h"
#include "../common/Arena.h"
#include "../common/net.h"
#include "../server/Server.h"

class Client {
public:
    class Announcement {
    private:
        int ticksRemaining;
        std::string message;
    public:
        Announcement(std::string m, int l) {
            this->message = m;
            this->ticksRemaining = l;
        }
        std::string getMessage() { return message; }
        int tick() { return ticksRemaining--; }
    };
private:

    Socket socket;
    std::string serverHost;
    Arena &arena;
    int myPlayerId = 0;
    std::vector<Announcement> pendingAnnouncements;

    void addPlayer(int playerId, std::string nick, Color color);

    void addSnake(int playerId, Color color);

    void onPlayerConnected(json j);

    void sendMessage(json j);

    void onHello(json json);

    void addPlayer(json j);
    
    void addFruit(json j);
    
    void makeSnakeDying(int playerID);

public:

    Client(Arena &world, std::string serverHost, std::string nick, Color color);

    void receiveMessages();
    void changeDir(vec2 dir);

    void onSnapshot(json j);
    
    void removeSnakes();
	
    std::vector<Announcement> checkAnnouncements();
	
};


#endif //STEROIDS_CLIENT_H
