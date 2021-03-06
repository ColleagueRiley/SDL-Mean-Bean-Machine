#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

std::vector<SDL_Color> COLORS = {{255,0,0,0},{255,255,0,0},{0,255,0,0},{0,0,255,0},{23,19,25,0}};

struct Bean{
    int x;
    int y;
    int WIDTH = 25;
    int LENGTH = 25;
    int colors[2];
    int quad;
    
    int place = 4;
    
    SDL_Rect rect = {x,y,WIDTH,LENGTH};
    SDL_Rect rect2 = rect;
    
    SDL_Rect crect = rect;
    SDL_Rect crect2 = rect2;
    
    void draw(SDL_Renderer* renderer);
    bool N = true;
    int WAIT = 24;int t=0;
    bool collide = false;
    bool collide2 = false;
};

const int WINDOW_WIDTH = 500;
const int WINDOW_LENGTH = 500;

bool New = true;
bool db = false;


void Bean::draw(SDL_Renderer* renderer){
    crect.w = WIDTH/2;
    crect.h = LENGTH/4;
    crect.x = WIDTH/2+rect.x-crect.h;
    crect.y = LENGTH+rect.y;

    crect2.w = WIDTH/2; 
    crect2.h = LENGTH/4;
    crect2.x = WIDTH/2+rect2.x-crect2.h;
    crect2.y = LENGTH+rect2.y;
    
    t++;
    bool no = false;
    if (place == 3 && rect.y > WINDOW_LENGTH-(LENGTH*2) || place == 3 && collide2){no = true;}
    
    if (rect.y < WINDOW_LENGTH-LENGTH && t==WAIT && !collide && !no){
        rect.y++;
        t=0;
    }
    else if (N && rect.y >= WINDOW_LENGTH-LENGTH || N && collide || N && no){New = true; N=false;}
    
    if (rect2.y < WINDOW_LENGTH-LENGTH && t==WAIT && !collide2){
        rect2.y++;
        t=0;
    }
  
    if (place == 1){rect2.x = rect.x; rect2.y = rect.y - LENGTH;}
    else if (place == 2){rect2.y = rect.y; rect2.x = rect.x + WIDTH;}
    else if (place == 3){rect2.x = rect.x; rect2.y = rect.y + LENGTH;}
    else if (place == 4){rect2.y = rect.y; rect2.x = rect.x - WIDTH;}

    SDL_SetRenderDrawColor(renderer,COLORS[colors[0]].r,COLORS[colors[0]].g,COLORS[colors[0]].b,COLORS[colors[0]].a);
    SDL_RenderDrawRect(renderer,&rect);
    SDL_RenderFillRect(renderer,&rect);

    SDL_SetRenderDrawColor(renderer,COLORS[colors[1]].r,COLORS[colors[1]].g,COLORS[colors[1]].b,COLORS[colors[1]].a);
    SDL_RenderDrawRect(renderer,&rect2);
    SDL_RenderFillRect(renderer,&rect2);

    SDL_SetRenderDrawColor(renderer,0,255,0,0);
    SDL_RenderDrawRect(renderer,&crect);
    SDL_RenderFillRect(renderer,&crect);    
    
    SDL_SetRenderDrawColor(renderer,0,255,0,0);
    SDL_RenderDrawRect(renderer,&crect2);
    SDL_RenderFillRect(renderer,&crect2);
}


bool running=true;
SDL_Event e;
const Uint8* keys = SDL_GetKeyboardState(NULL);
std::vector<Bean> beans;

int main(){
    std::vector<int> quads;
    Bean* beanData = new Bean;
    int qs = WINDOW_WIDTH/beanData->WIDTH;
    for (int i=0; i < qs; i++){
        quads.insert(quads.end(),i*beanData->WIDTH);
    }

    int SDL_Init();
    SDL_Window* window = SDL_CreateWindow("Mean Bean Machine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,500,500,SDL_WINDOW_OPENGL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    while (running){
        if (New){beans.insert(beans.begin(),{0,0,.colors={0,1},.quad = 10});New=false;}

        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                running = false;
            }
            if (keys[SDL_SCANCODE_ESCAPE]){
                running = false;
            }
            if (e.type == SDL_KEYDOWN && keys[SDL_SCANCODE_Z]){
                if(beans[0].place > 4){beans[0].place=1;}
                else{beans[0].place++;}
            }
            if (keys[SDL_SCANCODE_LEFT]){
                if(beans[0].quad-1 >= 0){
                    beans[0].quad -= 1;
                }
            }
            if (keys[SDL_SCANCODE_RIGHT]){
                if(beans[0].quad+1 < quads.size()){
                    beans[0].quad += 1;
                }
            }
            if (keys[SDL_SCANCODE_D]){
                db = true;
            }
        }
        SDL_RenderClear(renderer);
        for (int i=0;i < beans.size(); i++){
            
            for (int j=0;j < beans.size(); j++){
                if (j == i){}
                else{
                    beans[i].collide = SDL_HasIntersection(&beans[i].crect,&beans[j].crect);
                    if (!beans[i].collide) {beans[i].collide = SDL_HasIntersection(&beans[i].crect,&beans[j].crect2);}
                    if (beans[i].collide){j = beans.size();}
                }
            }
            for (int j=0;j < beans.size(); j++){ 
                if (j == i){}
                else{
                    beans[i].collide2 = SDL_HasIntersection(&beans[i].crect2,&beans[j].crect);
                    if (!beans[i].collide2) {beans[i].collide2 = SDL_HasIntersection(&beans[i].crect2,&beans[j].crect2);}
                    if (beans[i].collide2){j = beans.size();}
                }
            }            
            
            beans[i].rect.x = quads[beans[i].quad];
            beans[i].draw(renderer);
        }
        SDL_SetRenderDrawColor(renderer,0,0,0,0);
        SDL_RenderPresent(renderer);
    }
    SDL_Quit();
}
