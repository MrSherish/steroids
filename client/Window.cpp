#include "Window.h"
#include "../server/Server.h"

const auto TITLE = "Snake";

const auto SEGMENT_WIDTH = 32; // px
const auto SEGMENT_HEIGHT = SEGMENT_WIDTH; // px
const auto WINDOW_WIDTH = Server::ARENA_WIDTH * SEGMENT_WIDTH;
const auto WINDOW_HEIGHT = Server::ARENA_HEIGHT * SEGMENT_HEIGHT;
const int CLIENT_TICKRATE = 128;
const int CLIENT_TICK_DELAY = 1000 / CLIENT_TICKRATE;
const int DEATH_BLINK_RATE = 2*CLIENT_TICKRATE;

const auto NICK = "player0";

void Window::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);    
    
    for (const Snake &s : arena.snakes){
        if (s.alive && s.isDying && ((SDL_GetTicks() - s.deathTick) % DEATH_BLINK_RATE < DEATH_BLINK_RATE/2)){
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
    
    for (const Snake &s : arena.snakes) {
        if (s.alive && !s.isDying){
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

    for (Fruit f : arena.fruits) {
        SDL_SetRenderDrawColor(renderer, 75, 75, 75, 0);

        SDL_Rect r;
        r.x = f.pos.x * SEGMENT_WIDTH;
        r.y = f.pos.y * SEGMENT_HEIGHT;
        r.w = SEGMENT_WIDTH;
        r.h = SEGMENT_HEIGHT;

        SDL_RenderFillRect(renderer, &r);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

        r.x = f.pos.x * SEGMENT_WIDTH + SEGMENT_WIDTH/3;
        r.y = f.pos.y * SEGMENT_HEIGHT + SEGMENT_HEIGHT/3;
        r.w = SEGMENT_WIDTH/3;
        r.h = SEGMENT_HEIGHT/3;

        SDL_RenderFillRect(renderer, &r);
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

Window::Window(std::string serverHost) : client(arena, serverHost, NICK) {
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
}

Window::~Window() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::enterEventLoop() {
    while (!quit)
    {
        handleEvents();
        client.receiveMessages();
        client.removeSnakes();
        render();
        SDL_Delay(CLIENT_TICK_DELAY);
    }
}

