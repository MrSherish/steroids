#include "Window.h"
#include "../server/Server.h"

const auto TITLE = "Snake";
const auto charset_file = "charset_black.bmp";

const auto SEGMENT_WIDTH = 32; // px
const auto SEGMENT_HEIGHT = SEGMENT_WIDTH; // px
const auto WINDOW_WIDTH = Server::ARENA_WIDTH * SEGMENT_WIDTH;
const auto WINDOW_HEIGHT = Server::ARENA_HEIGHT * SEGMENT_HEIGHT;
const int CLIENT_TICKRATE = 128;
const int CLIENT_TICK_DELAY = 1000 / CLIENT_TICKRATE;




void Window::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    drawFruits();
    drawSnakes();
    drawUI();

    SDL_RenderPresent(renderer);
}

void Window::drawSnakes() {
    for (const Snake &s : arena.snakes) {
        Color c = s.color;

        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);

        for (Snake::Segment seg : s.segments) {
            int sx = seg.pos.x;
            int sy = seg.pos.y;

            SDL_Rect r;
            r.x = sx * SEGMENT_WIDTH;
            r.y = sy * SEGMENT_HEIGHT;
            r.w = SEGMENT_WIDTH;
            r.h = SEGMENT_HEIGHT;

            SDL_RenderFillRect(renderer, &r);
        }
    }
}

void Window::drawFruits() {
    for (Fruit f : arena.fruits) {
        SDL_SetRenderDrawColor(renderer, 75, 75, 75, 0);

        SDL_Rect r;
        r.x = f.pos.x * SEGMENT_WIDTH;
        r.y = f.pos.y * SEGMENT_HEIGHT;
        r.w = SEGMENT_WIDTH;
        r.h = SEGMENT_HEIGHT;

        SDL_RenderFillRect(renderer, &r);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        r.x = f.pos.x * SEGMENT_WIDTH + SEGMENT_WIDTH / 3;
        r.y = f.pos.y * SEGMENT_HEIGHT + SEGMENT_HEIGHT / 3;
        r.w = SEGMENT_WIDTH / 3;
        r.h = SEGMENT_HEIGHT / 3;

        SDL_RenderFillRect(renderer, &r);
    }
}

void Window::drawUI() {
    std::vector<Client::Announcement> announcementsToDisplay = client.checkAnnouncements();
    int annoucementYOffset = 10;
    for (Client::Announcement a : announcementsToDisplay) {
        drawString(a.getMessage(), 10, WINDOW_HEIGHT - annoucementYOffset);
        annoucementYOffset += 12;
    }
}

void Window::drawString(std::string str, int x, int y) {
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

void Window::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
//                printf("MOUSEMOTION event\n");
                break;
            case SDL_MOUSEBUTTONDOWN:
//                printf("MOUSEBUTTONDOWN event\n");
                break;
            case SDL_MOUSEBUTTONUP:
//                printf("MOUSEBUTTONUP event\n");
                break;
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        client.changeDir({-1, 0});
                        break;
                    case SDLK_RIGHT:
                        client.changeDir({1, 0});
                        break;
                    case SDLK_UP:
                        client.changeDir({0, -1});
                        break;
                    case SDLK_DOWN:
                        client.changeDir({0, 1});
                        break;
                    case SDLK_SPACE:
                        client.changeDir({0, 0});
                        break;
                    default:
                        break;
                }
                break;
            }
            case SDL_KEYUP:
//                printf("KEYUP event\tkey:%s\n", SDL_GetKeyName(event.key.keysym.sym));
                break;
            default:
//                printf("UNKNOWN event\n");
                break;
        }
    }
}

Window::Window(std::string serverHost, std::string nickname) : client(arena, serverHost, nickname) {
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0);

    window = SDL_CreateWindow(
            TITLE,
            SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN
    );

    assert(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    assert(renderer);

    SDL_Surface *charsetSurface = SDL_LoadBMP(charset_file);
    assert(charsetSurface);
    SDL_SetColorKey(charsetSurface, 1, SDL_MapRGB(charsetSurface->format, 0xFF, 0xFF, 0xFF));
    charset = SDL_CreateTextureFromSurface(renderer, charsetSurface);
    assert(charset);
    SDL_FreeSurface(charsetSurface);
}

Window::~Window() {
    SDL_DestroyTexture(charset);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::enterEventLoop() {
    while (!quit)
    {
        handleEvents();
        client.receiveMessages();
        render();
        SDL_Delay(CLIENT_TICK_DELAY);
    }
}

