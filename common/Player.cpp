#include "Player.h"

nlohmann::json Player::toJson() const {
    return {
            {"playerId", playerId},
            {"nick", nick}
    };
}

Player Player::fromJson(nlohmann::json j) {
    Player player;
    player.playerId = j["playerId"];
    player.nick = j["nick"];
    return player;
}