#include "Dialog.h"
#include <cctype>
const auto charset_file = "charset_white.bmp";
const auto DIALOG_HEIGHT = 100;
const auto DIALOG_WIDTH = 220;
const auto DIALOG_TICK_DELAY = 100;

Dialog::Dialog(std::string message, Uint32 color) : message(message), color(color) {

    window = SDL_CreateWindow(
        "Dialog",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DIALOG_WIDTH,
        DIALOG_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *charsetSurface = SDL_LoadBMP(charset_file);

    SDL_SetColorKey(charsetSurface, 1, SDL_MapRGB(charsetSurface->format, 0, 0, 0));
    charset = SDL_CreateTextureFromSurface(renderer, charsetSurface);

    SDL_FreeSurface(charsetSurface);
}

std::string Dialog::show() {
    bool exitFlag = false;
    SDL_Event e;
    while (!exitFlag) {
        input = input + '_';
        render();
        input.pop_back();
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_RETURN) {
                        exitFlag = true; break;
                    } else if (e.key.keysym.sym == SDLK_BACKSPACE) {
                        input.pop_back();
                    } else if ((int)e.key.keysym.sym > 1 && (int)e.key.keysym.sym < 255 &&
                        std::isalnum((char)e.key.keysym.sym)) input = input + (char)e.key.keysym.sym;
                    break;
                case SDL_QUIT:
                    exitFlag = true; break;
            }
        }
        SDL_Delay(100);
    }
    return input;
}

void Dialog::drawString(std::string str, int x, int y) {
    SDL_Rect s, d;
    s.w = s.h = d.w = d.h = 8;
    d.x = x;
    d.y = y;
    const char *text = str.c_str();
    while (*text) {
        if (*text == '\n') {
            d.y += 8;
            d.x = x;
        }
        else {
            s.x = (*text % 16) * 8;
            s.y = (*text / 16) * 8;
            SDL_RenderCopy(renderer, charset, &s, &d);
            d.x += 8;
        }
        ++text;
    }
}

void Dialog::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    drawString(message, 10, 10);
    drawString(input, 10, 20);

    SDL_RenderPresent(renderer);
}

Dialog::~Dialog() {
    SDL_DestroyTexture(charset);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    SDL_Init(SDL_INIT_EVERYTHING);
}