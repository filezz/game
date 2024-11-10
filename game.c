#include<stdlib.h>
#include<stdio.h>
#include<SDL2/SDL.h> //using sdl3 and its header 
#include<time.h>

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720
#define FRUITSIZE 22
#define SNAKESIZE 22
#define MAX_SNAKE_LENGTH 100
typedef enum 
{
    NONE,UP,DOWN,LEFT,RIGHT 
}direction;

typedef struct {
    SDL_Renderer *renderer; 
    SDL_Window *window ;
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
    direction direction;
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
void doKeyDown(SDL_KeyboardEvent *event,player *snake)
{
    if(event->repeat == 0){
        switch(event->keysym.scancode)
        {
            case SDL_SCANCODE_UP:
            if(snake->direction != DOWN) snake->direction = UP;
            break;
            
            case SDL_SCANCODE_DOWN:
            if(snake->direction != UP) snake->direction = DOWN;
            break;

            case SDL_SCANCODE_LEFT: 
            if(snake->direction != RIGHT ) snake->direction = LEFT ;
            break;

            case SDL_SCANCODE_RIGHT:
            if(snake->direction != LEFT) snake->direction = RIGHT;
            break;
        }

    }
}

void doInput(player *snake)
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
            doKeyDown(&input.key,snake);
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
void checkCollision(player *snake, coordonates *fruit) //use SDL_hasintersection for better collision check 
{
    if(abs(snake->body[0].x - fruit->x) < SNAKESIZE && abs(snake->body[0].y - fruit->y )< SNAKESIZE){
        fruit->x = rand() % (SCREEN_WIDTH-FRUITSIZE);
        fruit->y = rand() % (SCREEN_HEIGHT - FRUITSIZE); 

        //intialize the last segment of the tail, if not initialize the segment can contain garbage values , apearing randomly on the screen
        //in my case it was apearing in the far left corner
        if(snake->size < MAX_SNAKE_LENGTH ){
            snake->body[snake->size] = snake->body[snake->size - 2]; 
            snake->size += 2;
        }
        else 
            printf("You won") ;
    }
    if(snake->size > 2)
        for(int i = 2; i < snake->size ; i++)
            if(snake->body[0].x ==  snake->body[i].x && snake->body[0].y == snake->body[i].y)
                exit(1);
}

void render_snake(player *snake)
{
    for(int i =0; i<snake->size;i++) {
        render_object(snake->body[i].x , snake->body[i].y , SNAKESIZE , SNAKESIZE , "yellow"); 
    }
}

int main()
{
    initSDL(); 

    coordonates fruit;
    player snake;

    snake.direction = RIGHT; 
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

        doInput(&snake); 

        switch(snake.direction) {
            case NONE : dx = 0 , dy = 0; break;
            case LEFT : dx = -4, dy = 0; break;
            case RIGHT : dx = 4, dy = 0; break;
            case UP :   dx = 0 , dy = -4; break; 
            case DOWN : dx = 0, dy = 4; break;
        }


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