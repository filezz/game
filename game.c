#include<stdlib.h>
#include<stdio.h>
#include<SDL2/SDL.h> //using sdl3 and its header 
#include<time.h>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720
#define FRUITSIZE 10
#define SNAKESIZE 12
#define MAX_SNAKE_LENGTH 100

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

typedef struct player
{
    coordonates body[MAX_SNAKE_LENGTH];
    int size;
}player;

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
void snake_position(player *snake ,int x, int y) //moving previous snake head to the last position, then adding a new snake head
{
    for(int i = snake->size-1;i>0;i --)
        snake->body[i] = snake->body[i-1];
    snake->body[0].x += x; //updating head position
    snake->body[0].y += y; 
}
void checkCollision(player *snake, coordonates *fruit)
{
    if(abs(snake->body[0].x - fruit->x) < SNAKESIZE && abs(snake->body[0].y - fruit->y < SNAKESIZE)){
        fruit->x = rand() % (SCREEN_WIDTH-FRUITSIZE);
        fruit->y = rand() % (SCREEN_HEIGHT - FRUITSIZE); 

        snake->size < 100 ? snake->size ++ : printf("You won");
    }
    if(snake->size > 2)
        for(int i = 2; i < snake->size ; i++)
            if(snake->body[0].x ==  snake->body[i].x && snake->body[0].y == snake->body[i].y)
                exit(1);
}

void render_snake(player *snake)
{
    //render_object(snake->body[0].x,snake->body[0].x , SNAKESIZE,SNAKESIZE,"red");
    for(int i =0; i<snake->size;i++) {
        render_object(snake->body[i].x , snake->body[i].y , SNAKESIZE , SNAKESIZE , "yellow"); 
        render_object(snake->body[0].x , snake->body[0].y , SNAKESIZE , SNAKESIZE , "red"); 
    }
}

int main()
{
    initSDL(); 

    coordonates fruit;
    player snake;

    int dx=0,dy=0 ; // directions

    fruit.x = rand() % (SCREEN_WIDTH - FRUITSIZE);
    fruit.y = rand() % (SCREEN_HEIGHT - FRUITSIZE); 
    
    snake.body[0].x = SCREEN_HEIGHT/2;
    snake.body[0].y = SCREEN_WIDTH/2;
    snake.size = 1; // 1 segment for the bodysnake
    int running = 1;
    while(running) // loop game
    {
        SDL_SetRenderDrawColor(app.renderer,0,0,0,255) ; //clearing the screan
        SDL_RenderClear(app.renderer);

        doInput(); 

        if (app.left) dx = -5 , dy = 0; 
        if (app.right) dx = 5 , dy = 0; 
        if (app.up) dx = 0 , dy = 5;
        if (app.down) dx = 0, dy = -5;

        if(snake.body[0].x > SCREEN_WIDTH) snake.body[0].x = 0;
        if(snake.body[0].x < 0) snake.body[0].x = SCREEN_WIDTH;
        if(snake.body[0].y > SCREEN_HEIGHT) snake.body[0].y = 0; 
        if(snake.body[0].y < 0 ) snake.body[0].y = SCREEN_HEIGHT;
 
        render_object(fruit.x,fruit.y,FRUITSIZE,FRUITSIZE,"red");
        
        checkCollision(&snake,&fruit) ; 
        snake_position(&snake,dx,dy);
        render_snake(&snake);
        
        SDL_Delay(16);
        
        SDL_RenderPresent(app.renderer);
    } 

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();

    return 0;

}