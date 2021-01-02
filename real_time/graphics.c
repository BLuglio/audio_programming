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

/* canvas */
SDL_Surface* windowSurface = NULL;
SDL_Surface* imageSurface = NULL;
SDL_Event event;
Uint32 ticksLastFrame = 0;// ticks since last frame

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
        SDL_UpdateWindowSurface(window);
        SDL_Delay(2000);//pause the execution for 2 seconds
    }
    
    if(window == NULL){
         printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
         //SDL_Quit();
         return -1;
     }
    
    TTF_Init();
    font = TTF_OpenFont("/System/Library/Fonts/Keyboard.ttf", 18);
    if (font == NULL) {
        fprintf(stderr, "error: font not found\n");
        return -1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if(renderer == NULL) {
      printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -1;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    
    return 1;
}

void terminateGraphics(){
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/*
 Draws a single pixel on the screen
 */
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

void drawSineWave(int w, int h, SDL_Renderer *renderer, State state){
    static float startAngle = PI;
    //static int amp = 10;
    int amp = state.sineWaveAmp;
    static float frequency = (PI / 60);
    //float frequency = TWO_PI * 500 / SAMPLING_RATE;

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
    
    // get window's dimensions and the desktop's format and refresh rate
    int w;
    int h;
    SDL_DisplayMode mode;
    SDL_GetWindowDisplayMode(window, &mode);
    w = mode.w;
    h = mode.h;

    /* Clear the screen */
    Color bgColor = { .r = 0, .g = 0, .b = 0, .a = 255 };
    
    SDL_SetRenderDrawColor(
      renderer,
      bgColor.r,
      bgColor.g,
      bgColor.b,
      bgColor.a
    );
    
    SDL_RenderClear(renderer);

    /* DRAW SINE WAVE*/
    drawSineWave(w, h, renderer, state);
    
    /* FONT */
//    SDL_Texture *texture;
//    SDL_Rect rect;
    //displayDevices(state.outputDevices);
//    get_text_and_rect(renderer, 0, 0, "Output Device", font, &texture, &rect);
//    get_text_and_rect(renderer, 40, 0, state.outputDevices->infoName, font, &texture, &rect);
    /* Use TTF textures. */
//    SDL_RenderCopy(renderer, texture, NULL, &rect);
}

//void displayDevices(OutputDeviceList* list){
//    const OutputDeviceList* device = list;
//    SDL_Texture *texture;
//    SDL_Rect rect;
//    int y = 22;
//    while(device != NULL){
////        char text[512];
////        int n = sprintf(text, "id: %d [%s] - %s\n", device->id, device->hostApiName, device->infoName);
//        device = device->next;
//        get_text_and_rect(renderer, 0, 2*y + 10, device->infoName, font, &texture, &rect);
//        y += 2;
//    }
//}

int updateGraphics(State state){
    Uint32 ticksCurrent = SDL_GetTicks();
    float deltaTimeSeconds;
    /* If current ticks are lower or equal with last frame ticks
    * then we wait 1ms to make sure we're not rendering for no reason. */
    if (!(ticksLastFrame < ticksCurrent)) {
        SDL_Delay(1);  // wait 1ms
    }
    
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
    /* compute our delta time in seconds */
    deltaTimeSeconds = (ticksCurrent - ticksLastFrame) / 1000.0f;
    
    /* set the new ticks to be used in the next pass */
    ticksLastFrame = ticksCurrent;

    draw(window, renderer, state);
    SDL_RenderPresent(renderer);
    SDL_Delay(1);//per limitare la CPU sempre al massimo, ma bisogna capire come risolvere

    return 1;
}
