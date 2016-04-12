#include "Player.h"

nlohmann::json Player::toJson() const {
    return {
            {"playerId", playerId},
            {"nick", nick},
            {"points", points}
    };
}

Player Player::fromJson(nlohmann::json j) {
    Player player;
    player.playerId = j["playerId"];
    player.nick = j["nick"];
    player.points = j["points"];
    return player;
}