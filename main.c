#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define M_PI 3.14159265358979323846
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

SDL_Color white = {255, 255, 255};


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
    int direction[2];
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
    if (player->coordinates.x > SCREEN_WIDTH) { player->coordinates.x = player->coordinates.x - SCREEN_WIDTH; }
    else if (player->coordinates.x <= 0) { player->coordinates.x = player->coordinates.x + SCREEN_WIDTH; }
    if (player->coordinates.y > SCREEN_HEIGHT) { player->coordinates.y = player->coordinates.y - SCREEN_HEIGHT; }
    else if (player->coordinates.y <= 0) { player->coordinates.y = player->coordinates.y + SCREEN_HEIGHT; }
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

void display_score(SDL_Renderer* renderer, TTF_Font* font, int score) {

    char score_text[1024];
    sprintf(score_text, "%d", score);
    char score_label[7] = "Score: ";
    {
        char result[1024] = {0};
        snprintf(result, sizeof(result), "%s", score_label, score_text);

        SDL_Surface* message_surface = TTF_RenderText_Solid(font, result, white); 
        if (!message_surface) {
            fprintf(stderr, "Could not create surface: %s\n", TTF_GetError());
            TTF_CloseFont(font);
            return;
        }

        SDL_Texture* message_texture = SDL_CreateTextureFromSurface(renderer, message_surface);
        if (!message_texture) {
            fprintf(stderr, "Could not create texture: %s\n", SDL_GetError());
            SDL_FreeSurface(message_texture);
            TTF_CloseFont(font);
            return;
        }

        SDL_Rect message_rect = { .x =  SCREEN_WIDTH - 192, .y = 0,
                                  .w = message_surface->w, .h = message_surface->h }; 

        SDL_RenderCopy(renderer, message_texture, NULL, &message_rect);

        SDL_FreeSurface(message_surface);
        SDL_DestroyTexture(message_texture);
    }
}

bool detect_collision(SDL_Point* player_coordinates, Asteroid* asteroids)
{   
    // To be added
    printf("To be added.\n");
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
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("Ubuntu-Light.ttf", 32);
    if (!font) {
        fprintf(stderr, "Could not open font: %s\n", TTF_GetError());
        return;
    }
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

    Asteroid asteroids[8];
    int asteroid_count = 0;
    
    srand(time(NULL));
    while (asteroid_count < 8)
    {
        int random_diameter = rand() % 20 + 30;
        int random_x = rand() % SCREEN_WIDTH;
        int random_y = rand() % SCREEN_HEIGHT;
        int random_y_direction = 0;
        do 
        { 
            random_y_direction = (rand() % 5) - 2; 
        } while (random_y_direction == 0);
        int random_x_direction = (rand() % 5) - 2;
        Asteroid asteroid = { .coordinates.x = random_x, .coordinates.y = random_y,
                                .diameter = random_diameter, .direction = {random_x_direction, random_y_direction}};
        asteroids[asteroid_count] = asteroid; 
        asteroid_count += 1;
    }

    int score = 0;
    Uint32 last_time = SDL_GetTicks();
    bool collided = false;

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
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
        for (int i = 0; i < asteroid_count; i++) 
        { 
            draw_circle(renderer, asteroids[i].coordinates.x, asteroids[i].coordinates.y, asteroids[i].diameter); 
            asteroids[i].coordinates.x += asteroids[i].direction[0];  
            asteroids[i].coordinates.y += asteroids[i].direction[1];
            if (asteroids[i].coordinates.x > SCREEN_WIDTH) { asteroids[i].coordinates.x = asteroids[i].coordinates.x - SCREEN_WIDTH; }
            else if (asteroids[i].coordinates.x <= 0) { asteroids[i].coordinates.x = asteroids[i].coordinates.x + SCREEN_WIDTH; }
            if (asteroids[i].coordinates.y > SCREEN_HEIGHT) { asteroids[i].coordinates.y = asteroids[i].coordinates.y - SCREEN_HEIGHT; }
            else if (asteroids[i].coordinates.y <= 0) { asteroids[i].coordinates.y = asteroids[i].coordinates.y + SCREEN_HEIGHT; }
        }
        //draw_asteroids(renderer, &asteroids, asteroid_count);
        SDL_SetRenderTarget(renderer, NULL);


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        const Uint8* player_input = SDL_GetKeyboardState(NULL);
        handle_player_input(player_input, &player);

        SDL_Rect player_dst_rect = {.x = player.coordinates.x, .y = player.coordinates.y, .w = 65, .h = 65};
        SDL_Rect aster_dst_rect = {.x = 0, .y = 0, .w = SCREEN_WIDTH, .h = SCREEN_HEIGHT};
        SDL_RenderCopyEx(renderer, asteroid_texture, NULL, &aster_dst_rect, 0, NULL, SDL_FLIP_NONE);
        SDL_RenderCopyEx(renderer, player.texture, NULL, &player_dst_rect, player.angle+90, NULL, SDL_FLIP_NONE);
        
        Uint32 current_time = SDL_GetTicks();
        if (current_time - last_time >= 1000) 
        {
            score++;
            last_time = current_time;
        }
        display_score(renderer, font, score);
        //collided = detect_collision(&player.coordinates, asteroids);

        SDL_RenderPresent(renderer);
    }
 
    // Cleanup SDL
    SDL_DestroyTexture(player.texture);
    SDL_DestroyTexture(asteroid_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
 
    return 0;
}