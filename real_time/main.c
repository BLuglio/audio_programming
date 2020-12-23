//
//  main.c
//  real_time
//
//  Created by Biagio Luglio on 19/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#include <stdio.h>
#include <math.h>
#include "portaudio.h"
#include <SDL2/SDL.h>

#define FRAME_BLOCK_LEN 256 // length of buffer; larger length -> higher latency
#define SAMPLING_RATE 44100
#define TWO_PI (3.14159265f * 2.0f)

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

PaStream* audioStream;

/* sample increment of the phase of the modulator */
double sampleIncrementCarrier = 0, sampleIncrementModulator = 0;

typedef struct{
    float modulatorFreq, carrierFreq;
    float modulatorPhase, carrierPhase;
}
paData;

/*
 PROCESSING HERE
 */
int audio_callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
    paData* data = (paData*) userData;
    sampleIncrementModulator = TWO_PI * data->modulatorFreq / SAMPLING_RATE;
    sampleIncrementCarrier = TWO_PI * data->carrierFreq / SAMPLING_RATE;
    float* out = (float*) outputBuffer;
    //static double phase = 0;
    unsigned long i;
    for(i=0; i<framesPerBuffer; i++){
        float sine1 = sin(data->carrierPhase);
        float sine2 = sin(data->modulatorPhase);
        *out++ = sine1 * sine2;
        *out++ = sine1 * sine2;
        data->modulatorPhase += sampleIncrementModulator;
        data->carrierPhase += sampleIncrementCarrier;
    }
    return paContinue;
}

void terminate(){
    Pa_StopStream(audioStream); // stop the callback
    Pa_CloseStream(audioStream); // destroy the audio stream object
    Pa_Terminate(); // terminate portaudio
    SDL_Quit();
}

int main(int argc, const char * argv[]){
    
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
        printf("SDL could not be initialized!\n");
        return 1;
    }
    else{
        printf("SDL initialized \n");
        window = SDL_CreateWindow("My Audio Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
    }
    if( window == NULL ){
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);//render window in standard monitor, no flags

//    float frequency;
//    int i, id;
//    const PaDeviceInfo* info;
//    const PaHostApiInfo* hostApi;
//    PaStreamParameters outputParameters, inputParameters;
//    paData *data = (paData*) malloc(sizeof(paData));
//
//    printf("Provide the modulator frequency in Hertz: ");
//    //scanf("%f", &frequency);
//    scanf("%f", &(data->modulatorFreq));
//    printf("Provide the carrier frequency in Hertz: ");
//    //scanf("%f", &frequency);
//    scanf("%f", &(data->carrierFreq));
//    data->carrierPhase = 0.0;
//    data->modulatorPhase = 0.0;
//
//    //si = TWO_PI * frequency / SAMPLING_RATE;
//    printf("Initializing audio...\n");
//    Pa_Initialize();
//
//    /*
//     OUTPUT DEVICE
//     */
//    for(i=0; i< Pa_GetDeviceCount(); i++){
//        info = Pa_GetDeviceInfo(i);
//        hostApi = Pa_GetHostApiInfo(info->hostApi);
//
//        if(info->maxOutputChannels > 0) // if the device supports output
//            printf("%d: [%s] %s (output)\n", i, hostApi->name, info->name);
//    }
//
//    printf("Choose audio output device number: ");
//    scanf("%d", &id);
//
//    info = Pa_GetDeviceInfo(id);
//    hostApi = Pa_GetHostApiInfo(info->hostApi);
//    printf("Opening selected audio output device [%s] %s\n", hostApi->name, info->name);
//
//    outputParameters.device = id;
//    outputParameters.channelCount = 2; // stereo output
//    outputParameters.sampleFormat = paFloat32;
//    outputParameters.suggestedLatency = info->defaultLowOutputLatency;
//    outputParameters.hostApiSpecificStreamInfo = NULL;
//
//    Pa_OpenStream(
//                  &audioStream,         //portaudio stream object
//                  NULL,     //input params
//                  &outputParameters,    //output params
//                  SAMPLING_RATE,
//                  FRAME_BLOCK_LEN,      //frames per buffer
//                  paClipOff,            // no clip
//                  audio_callback,       // callback function address
//                  data);                // data for the callback function
//
//    Pa_StartStream(audioStream); //start callback mechanism
    
    
//    printf("running...... (press q and enter to exit)\n");
//    while(getchar() != 'q') {
//        scanf("%f", &(data->carrierFreq));// real time frequency update
//        Pa_Sleep(100);
//    }
    
    
    SDL_Event e;
    while (getchar() != 'q'){
        while (SDL_PollEvent(&e)){
            printf("running\n");
        }
    }
    terminate();
    return 0;
}
