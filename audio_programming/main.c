//
//  main.c
//  audio_programming
//
//  Created by Biagio Luglio on 08/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "portsf.h"


int main(int argc, const char * argv[]) {
    
    PSF_PROPS props;
    long framesread, totalread;
    float* frame = NULL;
    int ifd = -1;
    
    float amplitude_factor = 0.5;
    
    if(psf_init()){
        printf("Unable to start portsf\n");
        return 1;
    }
    
    ifd = psf_sndOpen("/Users/biagio/Desktop/dev/audio_programming/audio_programming/file_example_WAV_1MG.wav", &props, 0);
    if(ifd < 0){
        printf("Error: unable to open soundfile\n");
        return 1;
    }
    switch(props.samptype){
        case(PSF_SAMP_8):
            printf("sample type: 8 bit\n");
            break;
        case(PSF_SAMP_16):
            printf("sample type: 16 bit\n");
            break;
        case(PSF_SAMP_24):
            printf("sample type: 24 bit\n");
            break;
        case(PSF_SAMP_32):
            printf("sample type: 32 bit (integer)\n");
            break;
        case(PSF_SAMP_UNKNOWN):
            printf("sample type: unknown\n");
            break;
        case(PSF_SAMP_IEEE_FLOAT):
            printf("sample type: 32 bit (floating point)\n");
            break;
        default:
            printf("uknown\n");
            break;
    }
    
    printf("Sample rate: %d\n", props.srate);
    printf("Number of channels: %d\n", props.chans);
    
    /*
     
     allocate space for 1 sample frame;
     supply a buffer of at least nFrames * props.chans * sizeof(float) bytes
     
     */
    frame = (float *) malloc(props.chans * sizeof(float));
    if(frame == NULL){
        puts("No memory available!\n");
        goto exit;
    }
    
    /*
     - returns -1 in case of error
     - a return value less than nFrames will indicate that end of file has been reached
     - nFrames is a custom DWORD type and it is an unsigned long
     */
    framesread = psf_sndReadFloatFrames(ifd, frame, 1);
    totalread = 0;
    while(framesread == 1){
        totalread++;
        
        /* PROCESSING */
        framesread = psf_sndReadFloatFrames(ifd, frame, 1);
        if(framesread < 0){
            printf("Error reading file\n");
        }
        else {
            //float ch1 = frame[0];
            //float ch2 = frame[1];
            //printf("Channel 1: %f\n", ch1);
            //printf("Channel 2: %f\n", ch2);
            
            /*
             Applies a gain factor to each sample
             */
            for(int i =0; i < props.chans; i++){
                frame[i] *= amplitude_factor;
            }
            
            /*
             TODO:
                1) collect input samples in a buffer
                2) perform stft on each buffer
             */
            
        }
    }

exit:
    if(ifd >= 0){
        psf_sndClose(ifd);
    }
    if(frame){
        free(frame);
    }
    psf_finish();
    return 0;
    
}
