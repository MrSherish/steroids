#ifndef STEROIDS_WINDOW_H
#define STEROIDS_WINDOW_H

#include <SDL_net.h>
#include <SDL.h>
#include <cassert>

#include "../common/Config.h"
#include "../common/Arena.h"
#include "../server/Server.h"
#include "Client.h"

const auto NICK = "player0";

class Window {
private:

    bool quit = false;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* charset = nullptr;
    std::vector<SDL_Texture*> fruit_textures;
    int fruit_texture_count = 0;

    Arena arena;
    Client client;

    void handleEvents();

    void render();

    void drawString(std::string str, int x, int y);

    void drawSnakes();

    void drawFruits();

    void drawUI();

    SDL_Texture* loadTexture(const char *file);
public:

    Window(std::string serverHost, Config & cfg);

    ~Window();

    void enterEventLoop();
};


#endif //STEROIDS_WINDOW_H
