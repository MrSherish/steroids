#include "Client.h"

int const TICKS_AFTER_SNAKE_GETS_REMOVED = 5000;

using nlohmann::json;

void Client::sendMessage(const json &j) {
    uint32_t serverIp = net::resolveHost(serverHost, Server::PORT);
    Packet p {serverIp, (uint16_t)Server::PORT};
    p.data = j.dump();

    std::cerr << p.data << std::endl;

    socket.send(p);
}

void Client::addPlayer(int playerId, std::string nick, Color color) {
    Player player;
    player.playerId = playerId;
    player.nick = nick;
    arena.players.push_back(player);

    addSnake(playerId, color);
}

void Client::addPlayer(json j) {
    int playerId = j["playerId"];
    std::string nick = j["nick"];
    Color color = Color::fromJson(j["color"]);
    addPlayer(playerId, nick, color);
}

void Client::addFruit(json j) {
    Fruit newFruit = Fruit(j["type"], vec2(j["posX"], j["posY"]));
    arena.fruits.push_back(newFruit);
}

void Client::changeDir(vec2 dir) {
    std::vector<int> vec {dir.x, dir.y};
    sendMessage({
            {"message", "dir"},
            {"dir", vec}
    });
}

std::vector<Client::Announcement> Client::checkAnnouncements() {
    for (size_t i = 0; i < pendingAnnouncements.size(); i++) {
        if (pendingAnnouncements.at(i).tick() == 0) {
            pendingAnnouncements.erase(pendingAnnouncements.begin() + i);
        }
    }
    return pendingAnnouncements;
}

void Client::receiveMessages() {
    Packet p;
    while (socket.receive(p)) {
        std::cout << std::hex << p.ip << " " << p.port << std::endl;
        std::cout << p.data << std::endl;

        json j = json::parse(p.data);
        std::string message = j["message"];

        if (message == "hello") {
            onHello(j);
        } else if (message == "playerConnected") {
            onPlayerConnected(j);
        } else if (message == "snapshot") {
            onSnapshot(j);
        } else if (message == "snakeDied"){
            int playerID = j["playerId"];
            makeSnakeDying(playerID);
        }
    }
}

void Client::makeSnakeDying(int playerID){
    for (Snake &s : arena.snakes){
        if (s.playerId == playerID){
            s.isDying = true;
            s.deathTick = SDL_GetTicks();
            break;
        }
    }
}

void Client::removeSnakes(){
    for (auto s = arena.snakes.begin(); s < arena.snakes.end(); s++){
        if (s->isDying && SDL_GetTicks() - s->deathTick >= TICKS_AFTER_SNAKE_GETS_REMOVED){
            s->isDying = false;
        }
    }
}

void Client::onHello(json j) {
    myPlayerId = j["playerId"];
    std::vector<json> players = j["players"];
    for(auto &pj : players) {
        addPlayer(pj);
    }
}

void Client::onPlayerConnected(json j) {
    addPlayer(j);

    std::string announceMessage = arena.players.back().nick + " connected to server.";
    Announcement a(announceMessage, 100);
    pendingAnnouncements.push_back(a);
}

static std::deque<Snake::Segment> makeSegments(std::vector<std::vector<int>> vec) {
    std::deque<Snake::Segment> segments;
    for (auto posv : vec) {
        int x = posv[0];
        int y = posv[1];
        vec2 pos {x, y};
        Snake::Segment seg {pos};
        segments.push_back(seg);
    }
    return segments;
}

Client::Client(Arena &world, std::string serverHost, std::string nick, Color color)
        : socket(Socket::ANY_PORT), serverHost(serverHost), arena(world) {
    sendMessage({
            {"message", "connect"},
            {"nick", nick},
            {"color", color.toJson()}
    });
}

void Client::addSnake(int playerId, Color color) {
    Snake snake;
    snake.playerId = playerId;
    snake.color = color;
    arena.snakes.push_back(snake);
}

void Client::onSnapshot(json j) {
    std::vector<Player> players;
    for (const json &pj : j["players"]) {
        players.push_back(Player::fromJson(pj));
    }

    std::vector<Snake> snakes;
    for (const json &sj : j["snakes"]) {
        snakes.push_back(Snake::fromJson(sj));
    }
    
    for (Snake &newSnake : snakes){
        for (Snake &s : arena.snakes){
            if (newSnake.playerId == s.playerId){
                newSnake.isDying = s.isDying;
                newSnake.deathTick = s.deathTick;
            }
        }
    }

    std::vector<Fruit> fruits;
    for (const json &fj : j["fruits"]) {
        fruits.push_back(Fruit::fromJson(fj));
    }

    arena.players = players;
    arena.snakes = snakes;
    arena.fruits = fruits;
}
