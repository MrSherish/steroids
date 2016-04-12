#ifndef STEROIDS_CONFIG_H
#define STEROIDS_CONFIG_H

#include "Color.h"

using nlohmann::json;

struct Config {
	std::string nick;
	Color color;
};

Config loadConfig();

#endif //STEROIDS_CONFIG_H
