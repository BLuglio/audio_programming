//
//  stft.h
//  audio_programming
//
//  Created by Biagio Luglio on 08/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#ifndef stft_h
#define stft_h

/*
 fft on a windowed portion of input signal
 hop_size: resolution
*/
int stft(float* input, float* window, float* output, int input_size, int fft_size, int hop_size);

/*
 Takes an input signal of size N
 Outputs spectrum *output with N pairs of complex values [real, imag]
 */
void fft(float* input, float* output, int N);

#endif /* stft_h */
