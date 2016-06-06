#include <fstream>
#include <string>
#include "ServerConfig.h"

const std::string CONFIG_PATH = "server_config.json";

ServerConfig loadServerConfig() {
    std::ifstream configFile(CONFIG_PATH.c_str());
    assert(configFile.good());
    json j;
    j << configFile;

    ServerConfig cfg;
    cfg.arenaTickrate = j["arenaTickrate"];

    return cfg;
}