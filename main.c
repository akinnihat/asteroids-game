#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

enum PlayerPoints
{
    POINT_1 = 0,
    POINT_2,
    POINT_3,
    POINT_END
};

// typedef enum 
// {
//     TYPE_PLAYER,
//     TYPE_ASTEROID
// } ObjectType;

typedef struct 
{
    SDL_Point points[4];
    SDL_Point coordinates;
    int angle;
    float speed;
    SDL_Texture* texture;
} Player;

typedef struct
{
    SDL_Point coordinates;
    int diameter;
    int speed;
} Asteroid;

// typedef struct {
//     void* data;
//     ObjectType type;
// } GameObject;

void handle_player_input(const Uint8* player_input, Player* player)
{
    if (player_input[SDL_SCANCODE_LEFT]) {
        player->angle -= 5;
    }
    if (player_input[SDL_SCANCODE_RIGHT]) {
        player->angle += 5;
    }
    if (player_input[SDL_SCANCODE_UP]) {
        if (player->speed < 6.0f) { player->speed += 0.2f; } 
        player->coordinates.x += player->speed * cos(player->angle * M_PI / 180.0);
        player->coordinates.y += player->speed * sin(player->angle * M_PI / 180.0);
    }
    else
    {
        if (player->speed > 0.0f)
        { 
            player->coordinates.x += player->speed * cos(player->angle * M_PI / 180.0);
            player->coordinates.y += player->speed * sin(player->angle * M_PI / 180.0);
            player->speed -= 1.0f; 
        }
    }
}

void draw_circle(SDL_Renderer* renderer, int32_t center_x, int32_t center_y, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
        SDL_RenderDrawPoint(renderer, center_x + x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x + x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y - y);
        SDL_RenderDrawPoint(renderer, center_x - x, center_y + y);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x + y, center_y + x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y - x);
        SDL_RenderDrawPoint(renderer, center_x - y, center_y + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

void draw_asteroids(SDL_Renderer* renderer, Asteroid* asteroids[], int32_t asteroid_count)
{
    // TO BE UPDATED
    int32_t x = ((asteroids[0]->diameter)/2 - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - asteroids[0]->diameter);

    while (x >= y)
    {
       SDL_RenderDrawPoint(renderer, &asteroids[0]->coordinates.x + x, &asteroids[0]->coordinates.y - y);
       SDL_RenderDrawPoint(renderer, &asteroids[0]->coordinates.x + x, &asteroids[0]->coordinates.y + y);
       SDL_RenderDrawPoint(renderer, &asteroids[0]->coordinates.x - x, &asteroids[0]->coordinates.y - y);
       SDL_RenderDrawPoint(renderer, &asteroids[0]->coordinates.x - x, &asteroids[0]->coordinates.y + y);
       SDL_RenderDrawPoint(renderer, &asteroids[0]->coordinates.x + y, &asteroids[0]->coordinates.y - x);
       SDL_RenderDrawPoint(renderer, &asteroids[0]->coordinates.x + y, &asteroids[0]->coordinates.y + x);
       SDL_RenderDrawPoint(renderer, &asteroids[0]->coordinates.x - y, &asteroids[0]->coordinates.y - x);
       SDL_RenderDrawPoint(renderer, &asteroids[0]->coordinates.x - y, &asteroids[0]->coordinates.y + x);

       if (error <= 0)
       {
          ++y;
          error += ty;
          ty += 2;
       }

       if (error > 0)
       {
          --x;
          tx += 2;
          error += (tx - asteroids[0]->diameter);
       }
    }
}

int main(int argc, char ** argv)
{
    bool quit = false;
    SDL_Event e;
    
    Player player = { .points[POINT_1].x = 0, .points[POINT_1].y = 64, 
                      .points[POINT_2].x = 32, .points[POINT_2].y = 0, 
                      .points[POINT_3].x = 64, .points[POINT_3].y = 64, 
                      .points[POINT_END].x = 0, .points[POINT_END].y = 64,
                      .coordinates.x = SCREEN_WIDTH/2, .coordinates.y = SCREEN_HEIGHT/2,
                      .angle = 90, .speed = 6.0f };

    // Init SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("AsteroidsGame",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    
    // GameObject objects[] = {
    //     { &player, TYPE_PLAYER },
    // };

    player.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 65, 65);
    if (player.texture == NULL) {
        printf("Unable to create texture! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* asteroid_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (asteroid_texture == NULL) {
        printf("Unable to create texture! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //SDL_SetRenderDrawBlendMode(player.texture, SDL_BLENDMODE_BLEND);
    //SDL_SetRenderDrawBlendMode(asteroid_texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, player.texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLines(renderer, player.points, 5);
    SDL_SetRenderTarget(renderer, NULL);

    Asteroid asteroids[64];
    int asteroid_count = 0;

    while (asteroid_count < 64)
    {
        srand(time(NULL));
        int random_diameter = rand() % 20 + 10;
        srand(time(NULL));
        int random_x = rand() % SCREEN_WIDTH;
        srand(time(NULL));
        int random_y = rand() % SCREEN_HEIGHT;

        Asteroid asteroid = { .coordinates.x = random_x, .coordinates.y = random_y,
                                .diameter = random_diameter, .speed = 5};
        asteroids[asteroid_count] = asteroid; 
        asteroid_count += 1;
        printf("%d\n", asteroid_count);
    }

    printf("%d\n", asteroids[21].coordinates.x);

    while (!quit)
    {   
        SDL_Delay(10);
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT)
        {
            quit = true;
            break;   
        }

        SDL_SetRenderTarget(renderer, asteroid_texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        //SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        for (int i = 0; i < asteroid_count; i++) { draw_circle(renderer, asteroids[i].coordinates.x, asteroids[i].coordinates.y, asteroids[i].diameter); }
        //draw_asteroids(renderer, &asteroids, asteroid_count);
        SDL_SetRenderTarget(renderer, NULL);


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        const Uint8* player_input = SDL_GetKeyboardState(NULL);
        handle_player_input(player_input, &player);

        SDL_Rect player_dst_rect = {.x = player.coordinates.x, .y = player.coordinates.y, .w = 65, .h = 65};
        SDL_Rect aster_dst_rect = {.x = 100, .y = 100, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT};
        SDL_RenderCopyEx(renderer, asteroid_texture, NULL, &aster_dst_rect, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, player.texture, NULL, &player_dst_rect, player.angle+90, NULL, SDL_FLIP_NONE);
        
        SDL_RenderPresent(renderer);
    }
 
    // Cleanup SDL
    SDL_DestroyTexture(player.texture);
    SDL_DestroyTexture(asteroid_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}