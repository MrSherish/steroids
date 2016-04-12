#include <fstream>
#include <string>
#include "Config.h"

const std::string CONFIG_PATH = "config.json";

Config loadConfig() {
    std::ifstream configFile(CONFIG_PATH.c_str());
    assert(configFile.good());
    json j;
    j << configFile;

    Config cfg;
    cfg.nick = j["nick"];
    cfg.color = Color::fromJson(j["color"]);

    return cfg;
}