#include<stdlib.h>
#include<stdio.h>
#include<SDL2/SDL.h> //using sdl3 and its header 
#include<time.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FRUITSIZE 10

typedef struct {
    SDL_Renderer *renderer; 
    SDL_Window *window ;
} App; //app struct holds reference to our renderer and window that we will set up

App app;

typedef struct coordonates   // coordonates 
{
    int x,y;

} coordonates;

void initSDL() //intitilizing SDL and opening a window;
{
    int renderFlags, windowFlags;
    renderFlags = SDL_RENDERER_ACCELERATED;// renderer uses hardware accelaretion
    windowFlags = 0; 
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {// checking errors and init. video subsystem
        printf("Error initializing SDL_INIT_VIDEO :%s \n" , SDL_GetError());
        exit(1);
    }
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

void render_fruit(int x,int y)
{

    SDL_SetRenderDrawColor(app.renderer,255,255,255,255); 
    SDL_Rect square = {x,y,FRUITSIZE,FRUITSIZE};
    SDL_RenderFillRect(app.renderer,&square);
    SDL_RenderPresent(app.renderer); 
    
}
int main()
{
    initSDL(); 

    coordonates *fruit = malloc(sizeof(coordonates));

    fruit->x = rand() % (SCREEN_WIDTH - FRUITSIZE);
    fruit->y = rand() % (SCREEN_HEIGHT - FRUITSIZE);
    
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    render_fruit(fruit->x ,fruit->y);

    SDL_Event current_event; 
    int running = 1;
    while(running)
    {
        while(SDL_PollEvent(&current_event))
            if(current_event.type==SDL_QUIT)
                running = 0;
    }
    
    free(fruit);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
   

}