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
#include "shared.h"

typedef struct paData {
    float modulatorFreq, carrierFreq;
    float modulatorPhase, carrierPhase;
}
paData;

int audio_callback(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);

OutputDeviceList* initAudio();

void startAudioStream();

void terminateAudio();

#endif /* audio_h */
