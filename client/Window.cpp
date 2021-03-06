#include "Window.h"
const auto TITLE = "Snake";
const auto CHARSET_FILE = "charset_black.bmp";
const auto FRUIT_FILE_START = "fruit";
const auto FRUIT_FILE_END = ".bmp";

const auto SEGMENT_WIDTH = 32; // px
const auto SEGMENT_HEIGHT = SEGMENT_WIDTH; // px
const auto POINT_SEGMENT = 80;
const auto WINDOW_WIDTH = Server::ARENA_WIDTH * SEGMENT_WIDTH + POINT_SEGMENT;
const auto WINDOW_HEIGHT = Server::ARENA_HEIGHT * SEGMENT_HEIGHT;
const int CLIENT_TICKRATE = 128;
const int CLIENT_TICK_DELAY = 1000 / CLIENT_TICKRATE;
const int SEGMENT_BORDER = 2;
int const DEATH_BLINK_RATE = CLIENT_TICK_DELAY*100;

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
        if (s.segments.size() == 0) continue;

        for (auto it = s.segments.begin(); it < s.segments.end()-1;it++) {
            if (s.alive || (s.isDying && ((SDL_GetTicks() - s.deathTick) % DEATH_BLINK_RATE < DEATH_BLINK_RATE/2))){
                auto next = it + 1;
                int sx = (*it).pos.x;
                int sy = (*it).pos.y;
                int nx = (*next).pos.x;
                int ny = (*next).pos.y;
                
                SDL_Rect r;
                
                if ((ny == 0 && sy == (Server::ARENA_HEIGHT - 1)) || (ny == (Server::ARENA_HEIGHT - 1) && sy == 0)) {
                    r.x = sx * SEGMENT_WIDTH + SEGMENT_BORDER;
                    r.y = 0;
                    r.w = SEGMENT_WIDTH - 2*SEGMENT_BORDER;
                    r.h = SEGMENT_HEIGHT - 2*SEGMENT_BORDER;
                    SDL_RenderFillRect(renderer, &r);
                    r.y = ((sy) ? sy : ny)*SEGMENT_HEIGHT + 2*SEGMENT_BORDER; //use the one that is not 0 at the time
                    SDL_RenderFillRect(renderer, &r);
                }
                else if ((nx == 0 && sx == (Server::ARENA_WIDTH - 1)) || (nx == (Server::ARENA_WIDTH - 1) && sx == 0)) {
                    r.y = sy * SEGMENT_HEIGHT + SEGMENT_BORDER;
                    r.x = 0;
                    r.w = SEGMENT_WIDTH - 2*SEGMENT_BORDER;
                    r.h = SEGMENT_HEIGHT - 2*SEGMENT_BORDER;
                    SDL_RenderFillRect(renderer, &r);
                    r.x = ((sx) ? sx : nx)*SEGMENT_WIDTH + 2*SEGMENT_BORDER; //use the one that is not 0 at the time
                    SDL_RenderFillRect(renderer, &r);
                }
                else {
                    r.x = ((sx<nx) ? sx : nx) * SEGMENT_WIDTH + SEGMENT_BORDER;
                    r.y = ((sy<ny) ? sy : ny) * SEGMENT_HEIGHT + SEGMENT_BORDER;
                    r.w = SEGMENT_WIDTH - 2*SEGMENT_BORDER + ((sx!=nx) ? SEGMENT_WIDTH : 0);
                    r.h = SEGMENT_HEIGHT - 2*SEGMENT_BORDER + ((sy!=ny) ? SEGMENT_HEIGHT : 0);
                    
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }
    }
}

void Window::drawFruits() {
    for (auto &f : arena.fruits) {

        SDL_Rect r;
        r.x = f.pos.x * SEGMENT_WIDTH;
        r.y = f.pos.y * SEGMENT_HEIGHT;
        r.w = SEGMENT_WIDTH;
        r.h = SEGMENT_HEIGHT;

        int t = (f.type > fruit_texture_count - 1) ? 0 : f.type;
        SDL_RenderCopy(renderer, fruit_textures.at(t), NULL, &r);
    }
}

void Window::drawUI() {
    std::vector<Client::Announcement> announcementsToDisplay = client.checkAnnouncements();
    int annoucementYOffset = 10;
    for (auto &a : announcementsToDisplay) {
        drawString(a.getMessage(), 10, WINDOW_HEIGHT - annoucementYOffset);
        annoucementYOffset += 12;
    }

    SDL_Rect r;
    r.x = Server::ARENA_WIDTH * SEGMENT_WIDTH;
    r.y = 0;
    r.w = 1;
    r.h = Server::ARENA_HEIGHT * SEGMENT_HEIGHT;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderFillRect(renderer, &r);

    int pointsYOffset = 10;
    for (auto &p : arena.players) {
        drawString(p.nick + ": " + std::to_string(p.points), Server::ARENA_WIDTH * SEGMENT_WIDTH + 5, pointsYOffset);
        pointsYOffset += 12;
        if (pointsYOffset >= Server::ARENA_HEIGHT*SEGMENT_HEIGHT) break;
    }
}

SDL_Texture * Window::loadTexture(const char * file) {
    SDL_Surface *tempSurface = SDL_LoadBMP(file);
    if (tempSurface == 0) return 0;
    SDL_SetColorKey(tempSurface, 1, SDL_MapRGB(tempSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, tempSurface);
    if (t == 0) return 0;
    SDL_FreeSurface(tempSurface);
    return t;
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

Window::Window(std::string serverHost, Config & cfg) : client(arena, serverHost, cfg.nick, cfg.color) {
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

    charset = loadTexture(CHARSET_FILE);

    SDL_Texture *temp;
    std::string file = FRUIT_FILE_START;
    file = file + '0' + FRUIT_FILE_END;
    int i = 0;
    while ((temp = loadTexture(file.c_str()))) { //intentional
        fruit_textures.push_back(temp);
        i++;
        file = FRUIT_FILE_START + std::to_string(i) + FRUIT_FILE_END;
    }
    fruit_texture_count = i;
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
        client.removeSnakes();
        render();
        SDL_Delay(CLIENT_TICK_DELAY);
    }
}

