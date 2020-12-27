//
//  audio.c
//  real_time
//
//  Created by Biagio Luglio on 24/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#include "audio.h"

PaStream* audioStream;
//float frequency;
int i, id;
const PaDeviceInfo* info;
const PaHostApiInfo* hostApi;
PaStreamParameters outputParameters, inputParameters;

/* sample increment of the phase of the modulator */
double sampleIncrementCarrier = 0, sampleIncrementModulator = 0;

int initAudio(){
    paData *data = (paData*) malloc(sizeof(paData));
    data->modulatorFreq = 1200;
    data->carrierFreq = 800;
    data->carrierPhase = 0.0;
    data->modulatorPhase = 0.0;
    
    /*
     OUTPUT DEVICE
     */
    Pa_Initialize();
//    if(Pa_Initialize() != paNoError){
//        printf(  "PortAudio error: %s\n", Pa_GetErrorText(paErr) );
//        return 1;
//    };
//    outputDeviceList* outputDevices = createOutputDevicesList(NULL);
    //outputDeviceList* outputDevices = (outputDeviceList *)malloc(sizeof(outputDeviceList));
    outputDeviceList* outputDevices = NULL;
    printf("Number of devices found: %d\n", Pa_GetDeviceCount());
    for(i = 0; i < Pa_GetDeviceCount(); i++){
        info = Pa_GetDeviceInfo(i);
        hostApi = Pa_GetHostApiInfo(info->hostApi);

        if(info->maxOutputChannels > 0){
            // salvo i device in una lista
            outputDevices = push(outputDevices, hostApi->name, info->name, i);
            //printf("%d: [%s] %s\n", i, hostApi->name, info->name);
        } // if the device supports output
    }
    
    showOutputDevices(outputDevices);
    
    //    printf("Provide the modulator frequency in Hertz: ");
    //    //scanf("%f", &frequency);
    //    scanf("%f", &(data->modulatorFreq));
    //    printf("Provide the carrier frequency in Hertz: ");
    //    //scanf("%f", &frequency);
    //    scanf("%f", &(data->carrierFreq));

    return 0;
}

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

//outputDeviceList* createOutputDevicesList(char* h){
//    outputDeviceList* newNode = (outputDeviceList *)malloc(sizeof(outputDeviceList));
//     newNode->val = val;
//     newNode->next = NULL;
//     return newNode;
//}

outputDeviceList* push(outputDeviceList* head, char* hostApiName, char* infoName, int id) {
    outputDeviceList* newNode = (outputDeviceList *) malloc(sizeof(outputDeviceList));
    newNode->hostApiName = hostApiName;
    newNode->infoName = infoName;
    newNode->id = id;
    if(head != NULL){
        newNode->next = head;
    }
    else{
        newNode->next = NULL;
    }
    head = newNode;
    return head;
}

void showOutputDevices(outputDeviceList * head) {
    const outputDeviceList * current = head;

    while (current != NULL) {
        printf("id: %d [%s] - %s\n", current->id, current->hostApiName, current->infoName);
        current = current->next;
    }
}

void terminateAudio(){
    Pa_StopStream(audioStream); // stop the callback
    Pa_CloseStream(audioStream); // destroy the audio stream object
    Pa_Terminate(); // terminate portaudio
}
