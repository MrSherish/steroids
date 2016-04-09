#include "Client.h"

const auto SERVER_HOST = "localhost";

using nlohmann::json;

static void sendMessage(Socket &s, json j) {
    uint32_t serverIp = net::resolveHost(SERVER_HOST, Server::PORT);
    Packet p {serverIp, Server::PORT};
    p.data = j;

    s.send(p);
}

void Client::addPlayer(int playerId, std::string nick) {
    Player player;
    player.playerId = playerId;
    player.nick = nick;
    arena.players.push_back(player);
}

void Client::changeDir(vec2 dir) {
    std::vector<int> vec {dir.x, dir.y};
    sendMessage(socket, {
            {"message", "dir"},
            {"dir", vec}
    });
}

static void moveSnake(Snake &snake) {

}

void Client::receiveMessages() {
    Packet p;
    while (socket.receive(p)) {
        std::cout << std::hex << p.ip << " " << p.port << std::endl;
        std::cout << p.data.dump() << std::endl;

        json j = p.data;
        std::string message = j["message"];

        if (message == "hello") {
            myPlayerId = j["playerId"];
        } else if (message == "playerConnected") {
            onPlayerConnected(j);
        } else if (message == "snakeMoved") {
            onSnakeMoved(j);
        }
    }
}

void Client::onPlayerConnected(json j) {
    int playerId = j["playerId"];
    std::string nick = j["nick"];
    addPlayer(playerId, nick);
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

void Client::onSnakeMoved(json j) {
    int playerId = j["playerId"];
    std::vector<std::vector<int>> vec = j["segments"];

    auto &snakes = arena.snakes;
    auto it = std::find_if(snakes.begin(), snakes.end(), [=](Snake &s) {
        return s.playerId == playerId;
    });
    if (it != snakes.end()) {
        Snake &s = *it;
        s.segments = makeSegments(vec);
    } else {
//        std::cerr << "No snake found for player #" << playerId << std::endl;
        Snake snake;
        snake.playerId = playerId;
        snake.segments = makeSegments(vec);
        arena.snakes.push_back(snake);
    }
}

Client::Client(Arena &world, std::string nick) : socket(Socket::ANY_PORT), arena(world) {
    sendMessage(socket, {
            {"message", "connect"},
            {"nick", nick}
    });
}

