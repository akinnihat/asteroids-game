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


int main(int argc, char ** argv)
{
    bool quit = false;
    SDL_Event e;

    float player_start_x1 = SCREEN_WIDTH / 2;
    float player_start_x2 = SCREEN_WIDTH / 2 + 32;
    float player_start_y1 = SCREEN_HEIGHT / 2;
    float player_start_y2 = SCREEN_HEIGHT / 2 - 64;
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

    float theta = M_PI / 6.0f;

    // Init SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("AsteroidsGame",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
        SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
 
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
                for (int i = 0; i < PLAYER_DOT_CNT; i++) 
                {
                    player.rotation -= theta;
                    float cos_theta = cosf(-theta);
                    float sin_theta = sinf(-theta);
                    
                    float x = player.points[i].x;
                    float y = player.points[i].y;

                    float new_x = (x-rotation_x) * cos_theta - (y-rotation_y) * sin_theta + rotation_x;
                    float new_y = (x-rotation_x) * sin_theta + (y-rotation_y) * cos_theta + rotation_y;

                    player.points[i].x = new_x;
                    player.points[i].y = new_y;
                }
            }
            else if (e.key.keysym.sym == SDLK_RIGHT) 
            { 
                for (int i = 0; i < PLAYER_DOT_CNT; i++) 
                {
                    player.rotation += theta;
                    float cos_theta = cosf(theta);
                    float sin_theta = sinf(theta);
                    
                    float x = player.points[i].x;
                    float y = player.points[i].y;

                    float new_x = (x-rotation_x) * cos_theta - (y-rotation_y) * sin_theta + rotation_x;
                    float new_y = (x-rotation_x) * sin_theta + (y-rotation_y) * cos_theta + rotation_y;

                    player.points[i].x = new_x;
                    player.points[i].y = new_y;
                }
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
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red line LINE 1
	    SDL_RenderDrawLineF(renderer, player.points[POINT_1].x, player.points[POINT_1].y, player.points[POINT_2].x, player.points[POINT_2].y);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green line LINE 2
	    SDL_RenderDrawLineF(renderer, player.points[POINT_2].x, player.points[POINT_2].y, player.points[POINT_3].x, player.points[POINT_3].y);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue line LINE 3
	    SDL_RenderDrawLineF(renderer, player.points[POINT_3].x, player.points[POINT_3].y, player.points[POINT_1].x, player.points[POINT_1].y);
        
        // Render window
        SDL_RenderPresent(renderer);
    }
 
    // Cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}