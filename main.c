#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
 
#define M_PI 3.14159265358979323846
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define PLAYER_DOT_CNT 3


enum Points
{
    POINT_1 = 0,
    POINT_2,
    POINT_3
};

typedef struct 
{
    SDL_Point points[3];
    float rotation;
} Player;

void draw_player(SDL_Renderer *renderer, SDL_Surface *surface, Player *player)
{
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 0)); // Clear surface with black

    SDL_Surface *temp_surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_FillRect(temp_surface, NULL, SDL_MapRGB(temp_surface->format, 0, 0, 0)); // Clear temp surface with black

    SDL_Renderer *temp_renderer = SDL_CreateSoftwareRenderer(temp_surface);

    // TODO: player is being drawn on texture on every movement. Not necesessary, fix this
    // vvvvvvvvvvvvvvvvvv
    SDL_SetRenderDrawColor(temp_renderer, 255, 0, 0, 255); // Red
    SDL_RenderDrawLine(temp_renderer, player->points[POINT_1].x, player->points[POINT_1].y, player->points[POINT_2].x, player->points[POINT_2].y);
    SDL_SetRenderDrawColor(temp_renderer, 0, 255, 0, 255); // Green
    SDL_RenderDrawLine(temp_renderer, player->points[POINT_2].x, player->points[POINT_2].y, player->points[POINT_3].x, player->points[POINT_3].y);
    SDL_SetRenderDrawColor(temp_renderer, 0, 0, 255, 255); // Blue
    SDL_RenderDrawLine(temp_renderer, player->points[POINT_3].x, player->points[POINT_3].y, player->points[POINT_1].x, player->points[POINT_1].y);

    SDL_DestroyRenderer(temp_renderer);

    SDL_BlitSurface(temp_surface, NULL, surface, NULL);
    SDL_FreeSurface(temp_surface);
}

int main(int argc, char ** argv)
{
    bool quit = false;
    SDL_Event e;

    float player_start_x1 = SCREEN_WIDTH / 2;
    float player_start_x2 = player_start_x1 + 32;
    float player_start_y1 = SCREEN_HEIGHT / 2;
    float player_start_y2 = player_start_y1 - 64;
    float player_start_line_end_x1 = player_start_x1 + 64;
    float player_start_line_end_y1 = player_start_y1;

    Player player;
    player.points[POINT_1].x = player_start_x1;
    player.points[POINT_1].y = player_start_y1; 

    player.points[POINT_2].x = player_start_x2;
    player.points[POINT_2].y = player_start_y2; 

    player.points[POINT_3].x = player_start_line_end_x1;
    player.points[POINT_3].y = player_start_line_end_y1; 

    player.rotation = 0.0f;

    float theta = M_PI / 72.0f;
    float angle = 0.0f;

    // Init SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("AsteroidsGame",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    
    SDL_Surface *surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    // Handle events
    while (!quit)
    {   
        SDL_Delay(10);
        SDL_PollEvent(&e);

        float rotation_x = player.points[POINT_2].x;
        float rotation_y = player.points[POINT_2].y;

        if (e.type == SDL_QUIT)
        {
            quit = true;
            break;   
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_LEFT) 
            { 
                angle -= 10.0f;
            }
            else if (e.key.keysym.sym == SDLK_RIGHT) 
            { 
                angle += 10.0f;
            }
            else if (e.key.keysym.sym == SDLK_UP) 
            { 
                for (int i = 0; i < PLAYER_DOT_CNT; i++) 
                {
                    player.points[i].x -= 8*sinf(player.rotation);
                    player.points[i].y -= 8*cosf(player.rotation);
                }
            }
            else if (e.key.keysym.sym == SDLK_DOWN) 
            { 
                for (int i = 0; i < PLAYER_DOT_CNT; i++) 
                {
                    player.points[i].x += 8*sinf(player.rotation);
                    player.points[i].y += 8*cosf(player.rotation);
                }
            }
        }

        // Clear window
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the triangle on the surface
        draw_player(renderer, surface, &player);

        // Create texture from surface
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        // Set the rendering target to the texture
        SDL_Rect dest_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_Point center = {player.points[POINT_2].x, player.points[POINT_2].y};

        printf("%d\n", player.points[POINT_1].x);

        // Render the texture with rotation
        SDL_RenderCopyEx(renderer, texture, NULL, &dest_rect, angle, &center, SDL_FLIP_NONE);

        // Destroy texture
        SDL_DestroyTexture(texture);

        // Render window
        SDL_RenderPresent(renderer);
    }
 
    // Cleanup SDL
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}