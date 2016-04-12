#include <climits>
#include "Server.h"

const int SERVER_TICKRATE = 128;
const int SERVER_TICK_DELAY = 1000 / SERVER_TICKRATE;
const int ARENA_TICKRATE = 2;

using nlohmann::json;

void Server::receiveMessages() {
    Packet p;

    while (socket.receive(p)) {
        json j = p.data;
        std::string message = j["message"];

        std::cout << std::hex << p.ip << " " << p.port << std::endl;
        std::cout << p.data.dump() << std::endl;

        if (message == "connect") {
            onConnect(p);
        } else if (message == "dir") {
            onDir(p);
        }
    }
}

static Snake makeSnake(int playerId) {
    Snake snake;
    snake.playerId = playerId;
    snake.dir = {1, 0};

    vec2 pos;
    for (int i = 0; i < 5; ++i) {
        Snake::Segment seg{pos};
        snake.segments.push_front(seg);
        ++pos.x;
    }

    return snake;
}

void Server::onDir(Packet p) {
    int playerId = getPlayerId(p.ip, p.port);
    json j = p.data;
    std::vector<int> vec = j["dir"];
    int x = vec[0];
    int y = vec[1];
    if (x < -1) x = 1;
    if (x > 1) x = 1;
    if (y < -1) y = -1;
    if (y > 1) y = 1;
    vec2 dir {x, y};

    if (playerId) {
        Snake & snake = getPlayerSnake(playerId);
        if(snake.dir + dir != vec2{0, 0}) {
            snake.dir = dir;
        }
    } else {
        std::cerr << "No player for " << std::hex << p.ip << std::endl;
    }
}

int Server::getPlayerId(uint32_t ip, uint16_t port) {
    auto & players = arena.players;
    auto it = std::find_if(players.begin(), players.end(), [=](Player &player) {
        return player.ip == ip && player.port == port;
    });
    if (it == players.end()) {
        return 0;
    } else {
        return it->playerId;
    }
}
Snake & Server::getPlayerSnake(int playerId) {
    auto & snakes = arena.snakes;
    auto it = std::find_if(snakes.begin(), snakes.end(), [=](Snake & s) {
        return s.playerId == playerId;
    });
    assert(it != snakes.end());
    return *it;
}

void Server::sendMessages() {

}

Server::Server(Arena &arena) : socket(PORT), arena(arena) { }

void Server::restart() {
    arena = Arena{ARENA_WIDTH, ARENA_HEIGHT};
    arena.createFruitsOnArena();
}

void Server::spawnFruit() {
    //TODO: spawnFruit should spawn fruits randomly
    while (arena.fruits.size() < STARTING_NUMBER_OF_FRUITS){
        arena.spawnNewFruit();
    }
}

void Server::tick() {
    handleCollisions();
    moveSnakes();
    handleFruits();
    spawnFruit();
    broadcastSnakeMoved();
    sendFruits();
}

void Server::run() {
    restart();

    int ticks = 0;
    while (ticks != INT_MAX) {
        receiveMessages();
        sendMessages();

        if(ticks % (SERVER_TICKRATE / ARENA_TICKRATE) == 0) {
            tick();
        }

        SDL_Delay((Uint32) SERVER_TICK_DELAY);
        ++ticks;
    }
}

void Server::moveSnakes() {
    for (Snake &s : arena.snakes) {
        if (s.alive) {
            s.proceed(arena.width, arena.height);
        }
    }
}

void Server::handleCollisions() {
    std::vector<Snake*> snakesToKill;

    for (Snake &a : arena.snakes) {
        for (Snake &b : arena.snakes) {
            if (a.alive && b.alive) {
                handleSnakesCollision(a, b, snakesToKill);
            }
        }
    }

    for (Snake *s : snakesToKill) {
        killSnake(*s);
    }
}

void Server::broadcastSnakeMoved() {
    for (Snake &s : arena.snakes) {
        snakeMoved(s);
    }
}

void Server::handleSnakesCollision(Snake &a, Snake &b, std::vector<Snake *> &snakesToKill) {
    Snake a_ = a;
    Snake b_ = b;
    a_.proceed(arena.width, arena.height);
    b_.proceed(arena.width, arena.height);
    vec2 ah = a.segments.front().pos;
    vec2 bh = b.segments.front().pos;
    vec2 ah_ = a_.segments.front().pos;
    vec2 bh_ = b_.segments.front().pos;
    int ai = a.playerId;
    int bi = b.playerId;

    if (ai != bi && ah_ == bh_) {
        int dx = std::min(std::abs(ah.x - bh.x), std::abs(ah.x - bh.x + arena.width));
        int dy = std::min(std::abs(ah.y - bh.y), std::abs(ah.y - bh.y + arena.height));
        if (dx == 1 && dy == 1) {
            std::cerr << "Head-on collision type C " << a.playerId << ":" << b.playerId << std::endl;
            snakesToKill.push_back(&a);
            snakesToKill.push_back(&b);
        } else {
            std::cerr << "Head-on collision type A " << a.playerId << ":" << b.playerId << std::endl;
            a.proceed(arena.width, arena.height);
            snakesToKill.push_back(&a);
            snakesToKill.push_back(&b);
        }
    } else if(ah_ == bh && bh_ == ah && ai != bi) {
        std::cerr << "Head-on collision type B " << a.playerId << ":" << b.playerId << std::endl;
        snakesToKill.push_back(&a);
        snakesToKill.push_back(&b);
    } else {
        int i = 0;
        for (auto as : a_.segments) {
            if (i != 0 && as.pos == bh_) {
                std::cerr << "Standard collision " << a.playerId << ":" << b.playerId << std::endl;
                snakesToKill.push_back(&b);
                return;
            }
            ++i;
        }
    }
}

void Server::handleFruits() {
    for (Snake &s : arena.snakes) {
        for (auto f = arena.fruits.begin(); f < arena.fruits.end();) {
            if (s.segments.front().pos == f->pos) {
                handleEating(s, *f);
                f = arena.fruits.erase(f);
            }
            else f++;
        }
    }
}
/*Add point amount corresponding to fruit type to player, who owns
the snake playerID to be replaced by player's nick*/
void Server::addPoints(const Fruit&f, const Snake&s){
	assert(s.playerId <= arena.players.size());
	Player* scorer = &arena.players[s.playerId - 1];
	scorer->points += f.type;
	std::cerr << scorer->nick << ":" << scorer->points<<std::endl;
}

void Server::handleEating(Snake &s, Fruit &f) {
    s.segments.front().fat = true;
	addPoints(f,s);
}

void Server::broadcast(nlohmann::json j) {
    for (Player &player : arena.players) {
        Packet p { player.ip, player.port, j };
        socket.send(p);
    }
}

void Server::sendFruits() {
    std::vector<json> fruits;
    for (Fruit f : arena.fruits){
        json j = {
            {"type", f.type},
            {"posX", f.pos.x},
            {"posY", f.pos.y}
        };
        fruits.push_back(j);
    }
    json j = {
        {"message", "fruits"},
        {"fruits", fruits}
    };
    broadcast(j);
}

void Server::killSnake(Snake &s) {
    s.alive = false;
    json j = {
        {"message", "snakeDied"},
        {"playerId", s.playerId}
    };
    broadcast(j);
}

void Server::snakeMoved(const Snake &s) {
    std::vector<std::vector<int>> vec;
    for (auto seg : s.segments) {
        std::vector<int> pos {seg.pos.x, seg.pos.y};
        vec.push_back(pos);
    }
    
    json j = {
        {"message", "snakeMoved"},
        {"playerId", s.playerId},
        {"segments", vec}
    };
    broadcast(j);
}

void Server::onConnect(Packet p) {
    json j = p.data;
    uint8_t r = j["color"][0];
    uint8_t g = j["color"][1];
    uint8_t b = j["color"][2];
    Color color {r, g, b};
    
    int playerId = getPlayerId(p.ip, p.port);
    if (playerId == 0) {
        std::vector<json> players;
        for(auto & player : arena.players) {
            Snake & snake = getPlayerSnake(player.playerId);
            json pj = {
                {"playerId", player.playerId},
                {"nick", player.nick},
                {"color", snake.color.toJson()}
            };
            players.push_back(pj);
        }
        
        Player newPlayer;
        newPlayer.playerId = ++nextPlayerId;
        newPlayer.ip = p.ip;
        newPlayer.port = p.port;
        newPlayer.nick = j["nick"];
        arena.players.push_back(newPlayer);
        
        p.data = {
            {"message", "hello"},
            {"playerId", newPlayer.playerId},
            {"players", players}
        };
        socket.send(p);
        
        json cj = {
            {"message", "playerConnected"},
            {"playerId", newPlayer.playerId},
            {"nick", newPlayer.nick},
            {"color", color.toJson()}
        };
        broadcast(cj);
        
        Snake snake = makeSnake(newPlayer.playerId);
        snake.color = color;
        arena.snakes.push_back(snake);
        
        snakeMoved(snake);
    } else {
        std::cerr << "Player #" << playerId << " tries to connect though did not disconnect." << std::endl;
    }
}
