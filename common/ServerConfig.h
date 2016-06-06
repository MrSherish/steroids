#ifndef STEROIDS_SERVER_CONFIG_H
#define STEROIDS_SERVER_CONFIG_H

#include "json.hpp"

using nlohmann::json;

struct ServerConfig {
	int arenaTickrate = 0;
};

ServerConfig loadServerConfig();

#endif //STEROIDS_SERVER_CONFIG_H
