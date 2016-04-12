#include "Fruit.h"

nlohmann::json Fruit::toJson() const {
    return {
            {"pos", pos.toJson()},
            {"type", type}
    };
}

Fruit Fruit::fromJson(nlohmann::json j) {
    Fruit fruit;
    fruit.pos = vec2::fromJson(j["pos"]);
    fruit.type = j["type"];
    return fruit;
}