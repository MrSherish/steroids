#ifndef STEROIDS_WINDOW_H
#define STEROIDS_WINDOW_H

#include <SDL_net.h>
#include <SDL.h>
#include <cassert>
#include <zconf.h>

#include "Client.h"
#include "../common/Arena.h"
#include "../server/Server.h"

class Window {
private:

    bool quit = false;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    Arena arena;
    Client client;

    void handleEvents();

    void render();

public:

    Window();

    ~Window();

    void enterEventLoop();
};


#endif //STEROIDS_WINDOW_H
