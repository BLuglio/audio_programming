/*
 * main.c
 *
 *  Created on: Dec 12, 2020
 *      Author: biagio
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "math.h"
#include "portsf.h"
#include "fftw3.h"


#define NFRAMES (1024)

bool ft=true, ift=true;
static fftwf_plan forward, inverse;

double maxsamp(float* buffer, unsigned long blocksize);
void myFFT(float* sig, float* spec, int N);
void myIFFT(float* spec, float* sig, int N);

int main(int argc, const char * argv[]){
    PSF_PROPS props;
    long framesread;
    int ifd = -1, ofd = -1;
    float* frame = NULL;
    unsigned long blocksize;
    unsigned long nframes = NFRAMES;

    if(psf_init()){
        printf("unable to open portsf\n");
        return 1;
    }

    ifd = psf_sndOpen("/Users/biagio/Desktop/dev/eclipse/audio-programming/src/file_example_WAV_1MG.wav", &props, 0);
    if(ifd < 0)
    {
        puts("unable to read sound file");
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
            printf("unknown\n");
            break;
    }

    printf("Sample rate: %d\n", props.srate);
    printf("Number of channels: %d\n", props.chans);

    frame = (float*) malloc(nframes * props.chans * sizeof(float));
    framesread = psf_sndReadFloatFrames(ifd, frame, nframes);

    while(framesread > 0){
        double thispeak;
        blocksize = framesread * props.chans;
        thispeak = maxsamp(frame, blocksize);
        printf("max value (peak) of actual buffer: %f\n", thispeak);
        /*
         * separate left from right channel (the samples are interleaved)
         */
        float* spectrum = (float*) malloc(nframes * props.chans * sizeof(float));
        myFFT(frame, spectrum, nframes);
        framesread = psf_sndReadFloatFrames(ifd, frame, nframes);
    }

    if(frame){
        free(frame);
    }
    if(ifd >= 0){
        psf_sndClose(ifd);
    }
//    if(forward){
//        fftw_destroy_plan(forward);
//    }
//    if(inverse){
//        fftw_destroy_plan(inverse);
//    }
    return 0;
}

/*
 * Compute the max (peak) value in a buffer
 */
double maxsamp(float* buffer, unsigned long blocksize){
    double absval, peak = 0.0;
    unsigned long i;

    for(i=0; i<blocksize; ++i){
        absval = fabs(buffer[i]);
        if(absval > peak)
            peak = absval;
    }
    return peak;
}

void myFFT(float* sig, float* spec, int N){
    forward = fftwf_plan_r2r_1d(N, sig, spec, FFTW_R2HC, FFTW_ESTIMATE);
    //forward = fftw_plan_r2r_1d(N, sig, spec, FFTW_R2HC, FFTW_ESTIMATE);
}

//void fft(float* sig, float* spec, int N){
//    /*
//     * fftw_complex is by default a double[2] composed of the real (in[i][0])
//     * and imaginary (in[i][1]) parts of a complex number
//     *
//     * plan is an object that contains all the data that FFTW needs to compute the FFT
//     */
//    //forward = fftwf_execute_r2r(p, in, out)
//    forward = fftwf_plan_r2r_1d(N, sig, spec, FFTW_R2HC, FFTW_ESTIMATE);
//    fftwf_execute(forward);
//
//}
//void ifft(float* spec, float* sig, int N){
//
//}


