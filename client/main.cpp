#include "../common/net.h"
#include "../common/Packet.h"
#include "../common/Socket.h"
#include "../common/config.h"
#include "../common/json.hpp"
#include "Window.h"
#include <fstream>
using nlohmann::json;
const std::string PATH = "../common/Config.json";

int main (int argc, char** argv)
{
	std::ifstream configFile("../common/Config.json");
	assert(configFile.good());
	json configData;
	configData << configFile;
	config playerConfig(configData);
    std::string serverHost = "localhost";
    if(argc >= 2) {
        serverHost = argv[1];
    }
    srand((unsigned)time(nullptr));
    net::init();
    Window window(serverHost,playerConfig);
    window.enterEventLoop();

    net::quit();

    return EXIT_SUCCESS;
}