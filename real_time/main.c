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

#define FRAME_BLOCK_LEN 256 // length of buffer; larger length -> higher latency
#define SAMPLING_RATE 44100
#define TWO_PI (3.14159265f * 2.0f)

PaStream* audioStream;
double si = 0;

/*
 PROCESSING HERE
 */
int audio_callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData){
    float* in = (float*) inputBuffer, *out = (float*) outputBuffer;
    static double phase = 0;
    unsigned long i;
    for(i=0; i<framesPerBuffer; i++){
        float sine = sin(phase);
        *out++ = *in++ * sine; // left channel
        *out++ = *in++ * sine; // right channel (interleaved samples)
        phase += si;
    }
    return paContinue;
}

void init(){
    float frequency;
    int i, id;
    const PaDeviceInfo* info;
    const PaHostApiInfo* hostApi;
    PaStreamParameters outputParameters, inputParameters;
    printf("Provide the modulator frequency in Hertz: ");
    scanf("%f", &frequency);
    
    si = TWO_PI * frequency / SAMPLING_RATE;//sample increment of the phase of the modulator
    printf("Initializing audio...\n");
    Pa_Initialize();
    
    /*
     OUTPUT DEVICE
     */
    for(i=0; i< Pa_GetDeviceCount(); i++){
        //printf("Showing info for device %d\n", i);
        info = Pa_GetDeviceInfo(i);
        hostApi = Pa_GetHostApiInfo(info->hostApi);
        
        if(info->maxOutputChannels > 0) // if the device supports output
            printf("%d: [%s] %s (output)\n", i, hostApi->name, info->name);
    }
    
    printf("Choose audio output device number: ");
    scanf("%d", &id);
    
    info = Pa_GetDeviceInfo(id);
    hostApi = Pa_GetHostApiInfo(info->hostApi);
    printf("Opening selected audio output device [%s] %s\n", hostApi->name, info->name);
    
    outputParameters.device = id;
    outputParameters.channelCount = 2; // stereo output
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = info->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    /*
     INPUT DEVICE
     */
    for(i=0; i< Pa_GetDeviceCount(); i++){
        //printf("Showing info for device %d\n", i);
        info = Pa_GetDeviceInfo(i);
        hostApi = Pa_GetHostApiInfo(info->hostApi);
        
        if(info->maxInputChannels > 0) // if the device supports input
            printf("%d: [%s] %s (input)\n", i, hostApi->name, info->name);
    }
    
    printf("Choose audio input device number: ");
    scanf("%d", &id);
    
    info = Pa_GetDeviceInfo(id);
    hostApi = Pa_GetHostApiInfo(info->hostApi);
    printf("Opening selected audio input device [%s] %s\n", hostApi->name, info->name);
    
    outputParameters.device = id;
    outputParameters.channelCount = 2; // stereo input
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = info->defaultLowInputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    
    Pa_OpenStream(
                  &audioStream,         //portaudio stream object
                  &inputParameters,     //output params
                  &outputParameters,    //input params
                  SAMPLING_RATE,
                  FRAME_BLOCK_LEN,      //frames per buffer
                  paClipOff,            // no clip
                  audio_callback,       // callback function address
                  NULL);                // data for the callback function
    
    Pa_StartStream(audioStream); //start callback mechanism
    printf("running...... (press q and enter to exit)\n");
}

void terminate(){
    Pa_StopStream(audioStream); // stop the callback
    Pa_CloseStream(audioStream); // destroy the audio stream object
    Pa_Terminate(); // terminate portaudio
}

int main(int argc, const char * argv[]){
    init();
    while(getchar() != 'q') Pa_Sleep(100);
    terminate();
    return 0;
}
