#include "Window.h"
#include "../common/Server.h"

constexpr auto TITLE = "Snake";

constexpr auto SEGMENT_WIDTH = 32; // px
constexpr auto SEGMENT_HEIGHT = SEGMENT_WIDTH; // px
constexpr auto WINDOW_WIDTH = ARENA_WIDTH * SEGMENT_WIDTH;
constexpr auto WINDOW_HEIGHT = ARENA_HEIGHT * SEGMENT_HEIGHT;

void Window::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

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

    SDL_RenderPresent(renderer);
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
                printf("MOUSEMOTION event\n");
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("MOUSEBUTTONDOWN event\n");
                break;
            case SDL_MOUSEBUTTONUP:
                printf("MOUSEBUTTONUP event\n");
                break;
            case SDL_KEYDOWN:
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        dir = vec2 {-1, 0};
                        break;
                    case SDLK_RIGHT:
                        dir = vec2 {1, 0};
                        break;
                    case SDLK_UP:
                        dir = vec2 {0, -1};
                        break;
                    case SDLK_DOWN:
                        dir = vec2 {0, 1};
                        break;
                    case SDLK_SPACE:
                        arena.snakes.front().segments.front().fat = true; // HACK
                        break;
                    default:
                        break;
                }
                break;

            case SDL_KEYUP:
                printf("KEYUP event\tkey:%s\n",
                       SDL_GetKeyName(event.key.keysym.sym));
                break;
            default:
                printf("UNKNOWN event\n");
                break;
        }
    }
}

Window::Window() : client(arena), server(arena) {
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) == 0);

    window = SDL_CreateWindow(
            TITLE,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN
    );

    assert(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    assert(renderer);

    server.restart(); // HACK
}

Window::~Window() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::enterEventLoop() {
    while (!quit)
    {
        handleEvents();

        arena.snakes.front().dir = dir; // HACK

        server.tick();
        render(); // HACK
        SDL_Delay(256);
    }
}

