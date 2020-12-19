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
#include "pan.h"

#define NFRAMES (1)
#define POSITION (0.7)

bool ft=true, ift=true;
static fftwf_plan forward, inverse;

//double maxsamp(float* buffer, unsigned long blocksize);
void myFFT(float* sig, float* spec, int N);
void myIFFT(float* spec, float* sig, int N);

int main(int argc, const char * argv[]){
    PSF_PROPS propsIn, propsOut;
    long framesread;
    int ifd = -1, ofd = -1;
    float* frame = NULL;
    float* outFrame = NULL;
    unsigned long blocksize;
    unsigned long nframes = NFRAMES;

    if(psf_init()){
        printf("unable to open portsf\n");
        return 1;
    }

    ifd = psf_sndOpen("/Users/biagio/Desktop/dev/audio_programming/audio_programming/gtr_mono.wav", &propsIn, 0);
    if(ifd < 0)
    {
        puts("unable to open sound file");
        return 1;
    }
    
    /*
     TODO: ESTENDERE ANCHE A FILE STEREO
     */
    if(propsIn.chans > 1){
        printf("Only mono files are allowed\n");
        goto exit;
    }

    switch(propsIn.samptype){
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

    printf("Sample rate: %d\n", propsIn.srate);
    printf("Number of channels: %d\n", propsIn.chans);
    
    /* input buffer */
    frame = (float*) malloc(nframes * propsIn.chans * sizeof(float));
    if(frame == NULL){
        puts("No memory available\n");
        goto exit;
    }
    
    propsOut = propsIn;
    propsOut.chans = 2;
    
    /* output buffer */
    outFrame = (float*) malloc(nframes * propsOut.chans * sizeof(float));
    if(outFrame == NULL){
        puts("No memory available\n");
        goto exit;
    }
    
    char outFile[] = "/Users/biagio/Desktop/dev/audio_programming/audio_programming/gtr_panned_constantpwr.wav";
    ofd = psf_sndCreate(outFile, &propsOut, 0, 0, PSF_CREATE_RDWR);
    if(ofd < 0){
        printf("Error: unable to create file %s\n", outFile);
        goto exit;
    }

    while(framesread = psf_sndReadFloatFrames(ifd, frame, nframes) > 0){
        //PANPOS newPosition = linearpan(POSITION);
        PANPOS newPosition = constantpwrpan(POSITION);
        int i, j;
        for(i=0, j=0; i < framesread; i++){
            outFrame[j] = (float)(frame[i] * newPosition.left);
            outFrame[j+1] = (float)(frame[i] * newPosition.right);
            j += 2;
        }
        if(psf_sndWriteFloatFrames(ofd, outFrame, framesread) != framesread){
            printf("Error writing to file\n");
            break;
        }
        //double thispeak;
        //blocksize = framesread * props.chans;
        //thispeak = maxsamp(frame, blocksize);
        //printf("max value (peak) of actual buffer: %f\n", thispeak);
        /*
         * separate left from right channel (the samples are interleaved)
         */
        //float* spectrum = (float*) malloc(nframes * props.chans * sizeof(float));
        //myFFT(frame, spectrum, nframes);
        //framesread = psf_sndReadFloatFrames(ifd, frame, nframes);
    }

    exit:
    if(frame){
        free(frame);
    }
    if(ifd >= 0){
        psf_sndClose(ifd);
    }
    if(ofd >= 0){
        psf_sndClose(ofd);
    }
    if(outFrame){
        free(outFrame);
    }
    psf_finish();
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
//double maxsamp(float* buffer, unsigned long blocksize){
//    double absval, peak = 0.0;
//    unsigned long i;
//
//    for(i=0; i<blocksize; ++i){
//        absval = fabs(buffer[i]);
//        if(absval > peak)
//            peak = absval;
//    }
//    return peak;
//}

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


