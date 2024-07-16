#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
 
#define M_PI 3.14159265358979323846

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int PLAYER_DOT_CNT = 3;


enum Points
{
    POINT_1 = 0,
    POINT_2,
    POINT_3
};

struct Player
{
    SDL_Point points[3];
};

int main(int argc, char ** argv)
{
    bool quit = false;
    SDL_Event e;

    int player_start_x1 = SCREEN_WIDTH/2;
    int player_start_x2 = SCREEN_WIDTH/2 + 20;
    int player_start_y1 = SCREEN_HEIGHT/2;
    int player_start_y2 = SCREEN_HEIGHT/2 - 60;
    int player_start_line_end_x1 = player_start_x1+40;
    int player_start_line_end_y1 = player_start_y1;

    struct Player player;
    player.points[POINT_1].x = player_start_x1;
    player.points[POINT_1].y = player_start_y1; 

    player.points[POINT_2].x = player_start_x2;
    player.points[POINT_2].y = player_start_y2; 

    player.points[POINT_3].x = player_start_line_end_x1;
    player.points[POINT_3].y = player_start_line_end_y1; 

    float theta = M_PI / 16.0f;
    float cos_theta = cos(theta);
    float sin_theta = sin(theta);

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

        if (e.type == SDL_QUIT)
        {
            quit = true;
            break;   
        }
        else if (e.type == SDL_KEYDOWN)
        {
            // USE QUATERNIONS INSTEAD
            // vvvvvvvvvvvvvvvvvvvvvvv
            if (e.key.keysym.sym == SDLK_LEFT) 
            { 
                for (int i = 0; i < PLAYER_DOT_CNT; i++) 
                {
                    float cos_theta = cos(-theta);
                    float sin_theta = sin(-theta);
                    
                    int x = player.points[i].x;
                    int y = player.points[i].y;
                    
                    int centerX = (player.points[0].x + player.points[1].x + player.points[2].x) / 3;
                    int centerY = (player.points[0].y + player.points[1].y + player.points[2].y) / 3;
        
                    int translated_x = x - centerX;
                    int translated_y = y - centerY;

                    int new_x = translated_x * cos_theta - translated_y * sin_theta + centerX;
                    int new_y = translated_x * sin_theta + translated_y * cos_theta + centerY;

                    player.points[i].x = new_x;
                    player.points[i].y = new_y;
                }
            }
            else if (e.key.keysym.sym == SDLK_RIGHT) 
            { 
                for (int i = 0; i < PLAYER_DOT_CNT; i++) 
                {
                    float cos_theta = cos(theta);
                    float sin_theta = sin(theta);
                    
                    int x = player.points[i].x;
                    int y = player.points[i].y;
                    
                    int center_x = (player.points[0].x + player.points[1].x + player.points[2].x) / 3;
                    int center_y = (player.points[0].y + player.points[1].y + player.points[2].y) / 3;
        
                    int translated_x = x - center_x;
                    int translated_y = y - center_y;

                    int new_x = translated_x * cos_theta - translated_y * sin_theta + center_x;
                    int new_y = translated_x * sin_theta + translated_y * cos_theta + center_y;

                    player.points[i].x = new_x;
                    player.points[i].y = new_y;
                }
            }
            else if (e.key.keysym.sym == SDLK_UP) 
            { 
                for (int i = 0; i < PLAYER_DOT_CNT; i++) 
                {
                    player.points[i].y -= 10;
                }
            }
            else if (e.key.keysym.sym == SDLK_DOWN) 
            { 
                for (int i = 0; i < PLAYER_DOT_CNT; i++) 
                {
                    player.points[i].y += 10;
                }
            }
        }

        // Clear window
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red line LINE 1
	    SDL_RenderDrawLine(renderer, player.points[POINT_1].x, player.points[POINT_1].y, player.points[POINT_2].x, player.points[POINT_2].y);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green line LINE 2
	    SDL_RenderDrawLine(renderer, player.points[POINT_2].x, player.points[POINT_2].y, player.points[POINT_3].x, player.points[POINT_3].y);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);  // Blue line LINE 3
	    SDL_RenderDrawLine(renderer, player.points[POINT_3].x, player.points[POINT_3].y, player.points[POINT_1].x, player.points[POINT_1].y);
        
        // Render window
        SDL_RenderPresent(renderer);
    }
 
    // Cleanup SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}