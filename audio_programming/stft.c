//
//  stft.c
//  audio_programming
//
//  Created by Biagio Luglio on 08/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#include "stft.h"
#include <stdio.h>
#include <stdlib.h>



void fft(float* input, float* output, int N){
    const double two_pi = 2*acos(-1.);
    for(int i = 0,k = 0; k < N; i += 2, k++){
        output[i] = output[i + 1] = 0.f;
        for(int n = 0; n < N; n++){
            output[i] += input[n]*cos(k * n * two_pi/N);
            output[i + 1] -= input[n]*sin(k * n * two_pi/N);
        }
        output[i] /= N;
        output[i + 1] /= N;
    }
}

int stft(float* input, float* window, float* output, int input_size, int fft_size, int hop_size){
    int posin, posout, i;
    float* sigframe, *specframe;
    sigframe = (float*) malloc(fft_size);
    specframe = (float*) malloc(fft_size);
    
    for(posin = posout = 0; posin < input_size; posin += hop_size){
        //window a signal frame
        for(i = 0; i < fft_size; i++){
            if(posin + i < input_size)
                sigframe[i] = input[posin + i] * window[i];
            else sigframe[i] = 0;
        }
        
        fft(sigframe, specframe, fft_size);
        for(i = 0; i < fft_size; i++){
            output[posout] = specframe[i];
        }
    }
    free(sigframe);
    free(specframe);
    return posout;
}






