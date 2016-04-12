#include "../common/json.hpp"
#include "../common/net.h"
#include "Window.h"
#include "../common/Config.h"
#include "Dialog.h"

using nlohmann::json;

int main (int argc, char** argv)
{
    srand((unsigned)time(nullptr));
    net::init();

    std::string serverHost = "localhost";

    Config cfg = loadConfig();
    Window window(serverHost, cfg);
    window.enterEventLoop();

    net::quit();

    return EXIT_SUCCESS;
}