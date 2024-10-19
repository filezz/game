#include<stdlib.h>
#include<stdio.h>
#include<SDL2/SDL.h> //using sdl3 and its header 

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct {
    SDL_Renderer *renderer; 
    SDL_Window *window ;
} App; //app struct holds reference to our renderer and window that we will set up

void initSDL() //intitilizing SDL and opening a window;
{
    int renderFlags, windowFlags;
    renderFlags = SDL_RENDERER_ACCELERATED;// renderer uses hardware accelaretion
    windowFlags = 0; 
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {// checking errors and init. video subsystem
        printf("Error initializing SDL_INIT_VIDEO :%s \n" , SDL_GetError());
        exit(1);
    }
    App app;
    //creating a window 
    app.window = SDL_CreateWindow("Snake" , SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT, windowFlags) ; 
    if(!app.window)
    {
        printf("Failed to open window : %s " ,SDL_GetError()) ; 
        exit(1) ;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"linear") ; //setting the rendering quality to linear
    app.renderer = SDL_CreateRenderer(app.window,-1,renderFlags);// initializing the app renderer 
    if(!app.renderer) 
    {
        printf("Error with renderer: %s", SDL_GetError()) ;
        exit(1); 
    }
}
int main()
{
    initSDL();
}