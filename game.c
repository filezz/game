#include<stdlib.h>
#include<stdio.h>
#include<SDL2/SDL.h> //using sdl3 and its header 
#include<time.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define FRUITSIZE 10
#define SNAKESIZE 12

typedef struct {
    SDL_Renderer *renderer; 
    SDL_Window *window ;
    int up; 
    int down;
    int left;
    int right; // 4 directions for tracking movement
} App; //app struct holds reference to our renderer and window that we will set up

App app;

typedef struct coordonates   // coordonates 
{
    int x,y;

} coordonates;

typedef struct color  //render color
{
    unsigned int R;
    unsigned int G;
    unsigned int B;
    unsigned int A ;
} color_code;


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

void render_object(int x,int y,int sizex,int sizey,char *color)
{ 
    color_code rgb;
    if(strcmp(color,"red") == 0)
        rgb.R =227 , rgb.G = 11 , rgb.B = 93, rgb.A = 255; 
    else if(strcmp(color, "white") == 0)
        rgb .R = 255 , rgb.G = 255 , rgb.B = 255 , rgb.A = 255;
    else if(strcmp(color,"yellow")== 0)
        rgb.R = 255 ,rgb.G = 229 , rgb.B = 180 , rgb.A = 255;

    SDL_SetRenderDrawColor(app.renderer,rgb.R,rgb.G,rgb.B,rgb.A); 
    SDL_Rect square = {x,y,sizex,sizey};
    SDL_RenderFillRect(app.renderer,&square);
    SDL_RenderPresent(app.renderer); 
    
}
void doKeyDown(SDL_KeyboardEvent *event)
{
    if(event->repeat == 0){
        switch(event->keysym.scancode)
        {
            case SDL_SCANCODE_UP:
            app.up = 1;
            break;
            
            case SDL_SCANCODE_DOWN:
            app.down = 1;
            break;

            case SDL_SCANCODE_LEFT: 
            app.left = 1 ;
            break;

            case SDL_SCANCODE_RIGHT:
            app.right = 1;
            break;
        }

    }
}

void doKeyUp(SDL_KeyboardEvent *event)
{
    if(event->repeat == 0){
        switch(event->keysym.scancode)
        {
            case SDL_SCANCODE_UP:
            app.up = 0;
            break;
            
            case SDL_SCANCODE_DOWN:
            app.down = 0;
            break;

            case SDL_SCANCODE_LEFT: 
            app.left = 0 ;
            break;

            case SDL_SCANCODE_RIGHT:
            app.right = 0;
            break;
        }

    }   
}
void doInput()
{
    SDL_Event input;
    
    while(SDL_PollEvent(&input))
    {
        switch(input.type)
        {
            case SDL_QUIT :
            exit(0);
            break;
            
            case SDL_KEYDOWN:
            doKeyDown(&input.key);
            break;

            case SDL_KEYUP : 
            doKeyUp(&input.key);
            break;

            default : 
            break;
        }
    }
}


int main()
{
    initSDL(); 

    coordonates *fruit = malloc(sizeof(coordonates));
    coordonates *snake = malloc(sizeof(coordonates)); 
    
    if (!fruit || !snake)
        perror("Memory allocation friend") ; 

    fruit->x = rand() % (SCREEN_WIDTH - FRUITSIZE);
    fruit->y = rand() % (SCREEN_HEIGHT - FRUITSIZE); 
    
    snake->x = SCREEN_HEIGHT/2;
    snake->y = SCREEN_WIDTH/2;

    SDL_Event current_event; 
    int running = 1;
    while(running) // loop game
    {
        SDL_SetRenderDrawColor(app.renderer,0,0,0,255) ; //clearing the screan
        SDL_RenderClear(app.renderer);

        doInput(); 

        if (app.left) snake->x -= 5;
        if (app.right) snake->x += 5;
        if (app.up) snake->y -= 5;
        if (app.down) snake->y += 5;

        if(snake->x > SCREEN_WIDTH) snake->x = 0;
        if(snake->x < 0) snake->x = SCREEN_WIDTH;
        if(snake->y > SCREEN_HEIGHT) snake->y = 0; 
        if(snake->y < 0 ) snake->y = SCREEN_HEIGHT;
        
        render_object(snake->x,snake->y,SNAKESIZE,SNAKESIZE,"yellow");
        render_object(fruit->x,fruit->y,FRUITSIZE,FRUITSIZE,"red");
        
        SDL_Delay(18);
        
        SDL_RenderPresent(app.renderer);
    }
    
    free(fruit);
    free(snake);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;

}