//
//  graphics.h
//  audio_programming
//
//  Created by Biagio Luglio on 24/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#ifndef graphics_h
#define graphics_h

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include "constants.h"
#include "shared.h"

typedef struct Color {
  int r;
  int g;
  int b;
  int a;
} Color;

typedef struct CurvePoint {
  int x;
  int y;
} CurvePoint;

typedef struct Pixel {
  CurvePoint point;
  Color color;
} Pixel;

int initGraphics();
void terminateGraphics();
void drawPixel(Pixel pixel, SDL_Renderer* renderer);
void drawSineWave(int w, int h, SDL_Renderer* renderer, State state);
void draw(SDL_Window* window, SDL_Renderer* renderer, State state);
int updateGraphics(State state);
void displayDevices(OutputDeviceList* list);

#endif /* graphics_h */
