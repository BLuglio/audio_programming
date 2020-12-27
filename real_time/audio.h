//
//  audio.h
//  real_time
//
//  Created by Biagio Luglio on 24/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#ifndef audio_h
#define audio_h

#include <stdio.h>
#include "portaudio.h"
#include "constants.h"

typedef struct{
    float modulatorFreq, carrierFreq;
    float modulatorPhase, carrierPhase;
}
paData;

typedef struct node {
    char* hostApiName;
    char* infoName;
    int id;
    struct node * next;
} outputDeviceList;

int audio_callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);

int initAudio();

void terminateAudio();

outputDeviceList* createOutputDevicesList(char* hostApiName, char* infoName, int id);
outputDeviceList* push(outputDeviceList* head, char* hostApiName, char* infoName, int id);
void showOutputDevices(outputDeviceList * head);

#endif /* audio_h */
