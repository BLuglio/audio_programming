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
TTF_Font* font = NULL;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

int initGraphics(){
    if(SDL_Init( SDL_INIT_VIDEO ) < 0){
        printf("SDL could not be initialized! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    else{
        printf("SDL initialized \n");
        window = SDL_CreateWindow("My Audio Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    }
    
    TTF_Init();
    font = TTF_OpenFont("/System/Library/Fonts/Keyboard.ttf", 18);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        return -1;
    }
    
    if(window == NULL){
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
    TTF_Quit();
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

/*
- x, y: upper left corner.
- texture, rect: outputs.
*/
void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
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

void draw(SDL_Window* window, SDL_Renderer* renderer, State state){
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
    
    /* DRAW SINE WAVE*/
//    drawSineWave(w, h, renderer);
    
    /* FONT */
    SDL_Texture *texture;
    SDL_Rect rect;
    get_text_and_rect(renderer, 0, 0, "Output Device", font, &texture, &rect);
    displayDevices(state.outputDevices);
    /* Use TTF textures. */
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    //get_text_and_rect(renderer, 0, rect1.y + rect1.h, "world", font, &texture2, &rect2);

    if((currentFrameTick - lastFrameTick) >= ticksPerFrame) {
    lastFrameTick = currentFrameTick;
    } else {
    //printf("Delay needed\n");
    Uint32 elapsedTicks = currentFrameTick - lastFrameTick;
    SDL_Delay(ticksPerFrame - elapsedTicks);
    }
}

/*
 I DEVICE NON ESCONO SU SCHERMO E LA MEMORIA USATA AUMENTA AD OGNI ITERAZIONE :(
 */
void displayDevices(OutputDeviceList* list){
    const OutputDeviceList* device = list;
    SDL_Texture *texture;
    SDL_Rect rect;
    int y = 22;
    while(device != NULL){
        char text[512];
        int n = sprintf(text, "id: %d [%s] - %s\n", device->id, device->hostApiName, device->infoName);
        device = device->next;
        get_text_and_rect(renderer, 0, 2*y + 14, text, font, &texture, &rect);
        y += 2;
    }
}

int updateGraphics(State state){
    
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    //showOutputDevices(state.outputDevices);
    draw(window, renderer, state);
    SDL_RenderPresent(renderer);
    return 1;
}
