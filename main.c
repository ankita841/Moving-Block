#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <assert.h>
#include <stdlib.h>

#define BOOL u32
#define TRUE 1
#define FALSE 0

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 540

typedef Uint32 u32;
typedef Uint64 u64;
typedef Sint32 i32;
typedef Sint64 i64;

typedef struct
{
    int x;
    int y;
    int w;
    int h;
}rect_t;

void FillRect(rect_t rect, u32 pixel_color, u32* screen_pixels)
{
    for (int row = 0; row < rect.h; ++row)
    {
        for (int col = 0; col < rect.w; ++col)
        {
            screen_pixels[(row + rect.y) * SCREEN_WIDTH + col + rect.x] = pixel_color;
        }
    }
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Moving Block", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    assert(window);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);
    assert(renderer);

    SDL_Texture* screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    assert(screen);

    u32* screen_pixels = (u32*)calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(u32));
    assert(screen_pixels);

    BOOL done = FALSE;

    rect_t square = { 0, 0, 40, 40 };
    square.x = (SCREEN_WIDTH - square.w) / 2;
    square.y = (SCREEN_HEIGHT - square.h) / 2; 
        FillRect(square, 255, screen_pixels);

    BOOL pressed_up = FALSE;
    BOOL pressed_down = FALSE;
    BOOL pressed_right = FALSE;
    BOOL pressed_left = FALSE;

    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                done = TRUE;
                break;
            }
            if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
            {
                break;
            }

            SDL_Keycode code = event.key.keysym.sym;

            switch (code)
            {
            case SDLK_ESCAPE:
                done = event.type == SDL_KEYDOWN;
                break;
            case SDLK_UP:
                pressed_up = event.type == SDL_KEYDOWN;
                break;
            case SDLK_DOWN:
                pressed_down = event.type == SDL_KEYDOWN;
                break;
            case SDLK_LEFT:
                pressed_left = event.type == SDL_KEYDOWN;
                break;
            case SDLK_RIGHT:
                pressed_right = event.type == SDL_KEYDOWN;
                break;
            default:
                break;
            }

            memset(screen_pixels, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(u32));

            if (pressed_up)
            {
                square.y -= 1;
            }
            if (pressed_down)
            {
                square.y += 1;
            }
            if (pressed_left)
            {
                square.x -= 1;
            }
            if (pressed_right)
            {
                square.x += 1;
            }

            FillRect(square, 255, screen_pixels);

            SDL_UpdateTexture(screen, NULL, screen_pixels, SCREEN_WIDTH * sizeof(u32));
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, screen, NULL, NULL);
            SDL_RenderPresent(renderer);

            SDL_Delay(15);
        }
    }

    return 0;
}

