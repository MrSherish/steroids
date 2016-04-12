#ifndef STEROIDS_DIALOG_H
#define STEROIDS_DIALOG_H


#include <string>
#include <SDL.h>

class Dialog {
private:
    std::string message, input;
    Uint32 color;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *charset;
    bool didInit = false;

    void render();
    void drawString(std::string str, int x, int y);

public:
    Dialog(std::string message, Uint32 color);
    ~Dialog();
    std::string show();
};


#endif //STEROIDS_DIALOG_H
