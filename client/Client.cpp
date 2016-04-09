#include "Client.h"

const auto SERVER_HOST = "localhost";

using nlohmann::json;

void Client::sendMessage(json j) {
    uint32_t serverIp = net::resolveHost(serverHost, Server::PORT);
    Packet p {serverIp, (uint16_t)Server::PORT};
    p.data = j;

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


void Client::changeDir(vec2 dir) {
    std::vector<int> vec {dir.x, dir.y};
    sendMessage({
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
            onHello(j);
        } else if (message == "playerConnected") {
            onPlayerConnected(j);
        } else if (message == "snakeMoved") {
            onSnakeMoved(j);
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
        std::cerr << "No snake found for player #" << playerId << std::endl;
    }
}

Client::Client(
        Arena &world,
        std::string serverHost,
        std::string nick
) : socket(Socket::ANY_PORT), serverHost(serverHost), arena(world) {
    uint8_t r = (uint8_t) rand();
    uint8_t g = (uint8_t) rand();
    uint8_t b = (uint8_t) rand();

    std::vector<int> vec {r, g, b};

    sendMessage({
            {"message", "connect"},
            {"nick", nick},
            {"color", vec}
    });
}

void Client::addSnake(int playerId, Color color) {
    Snake snake;
    snake.playerId = playerId;
    snake.color = color;
    arena.snakes.push_back(snake);
}

void Client::heartbeat() {

}




