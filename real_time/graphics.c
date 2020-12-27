//
//  graphics.c
//  real_time
//
//  Created by Biagio Luglio on 24/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#include "graphics.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

int initGraphics(){
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    else{
        printf("SDL initialized \n");
        window = SDL_CreateWindow("My Audio Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    }
    if( window == NULL ){
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(renderer == NULL) {
      printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -1;
    }
    return 1;
}

void terminateGraphics(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawPixel(Pixel pixel, SDL_Renderer *renderer){
    SDL_SetRenderDrawColor(
        renderer,
        pixel.color.r,
        pixel.color.g,
        pixel.color.b,
        pixel.color.a
    );

    SDL_RenderDrawPoint(renderer, pixel.point.x, pixel.point.y);
}

void drawSineWave(int w, int h, SDL_Renderer *renderer){
    static float startAngle = PI;
    static int amp = 10;
    static float frequency = (PI / 60);

    Color c = {0, 255, 0, 255};
    float angle = startAngle;

    for(int i = 0; i < w; i++) {
        CurvePoint p = {i, sin(angle) * amp + (h / 2)};
        Pixel pix = {p, c};
        drawPixel(pix, renderer);
        angle += frequency;
    }
}

void draw(SDL_Window *window, SDL_Renderer *renderer){
    const int fps = 90;
    const Uint32 ticksPerFrame = 1000 / fps;
    SDL_DisplayMode mode;
    int w;
    int h;
    static Uint32 lastFrameTick = 0;
    Uint32 currentFrameTick = SDL_GetTicks();
    if(lastFrameTick == 0) {
        lastFrameTick = SDL_GetTicks();
    }

    //    printf("%d\n", lastFrameTick);
    Color bgColor = { .r = 0, .g = 0, .b = 0, .a = 255 };

    SDL_GetWindowDisplayMode(window, &mode);
    w = mode.w;
    h = mode.h;

    /* Clear the screen */
    SDL_SetRenderDrawColor(
      renderer,
      bgColor.r,
      bgColor.g,
      bgColor.b,
      bgColor.a
    );
    SDL_RenderClear(renderer);

    drawSineWave(w, h, renderer);

    SDL_RenderPresent(renderer);

    if((currentFrameTick - lastFrameTick) >= ticksPerFrame) {
    lastFrameTick = currentFrameTick;
    } else {
    //printf("Delay needed\n");
    Uint32 elapsedTicks = currentFrameTick - lastFrameTick;
    SDL_Delay(ticksPerFrame - elapsedTicks);
    }
}

int updateGraphics(){
    SDL_Event event;
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                return -1;
            } else if(event.type == SDL_KEYUP) {
              if(event.key.keysym.sym == SDLK_q) {
                /* Press q to quit the program */
                return -1;
              }
            }
          }
    draw(window, renderer);
    return 1;
}
