//
//  pan.c
//  audio_programming
//
//  Created by Biagio Luglio on 18/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#include "pan.h"

/*
 input: position in the range [-1, +1]
 output: the panned position representing the amplitude scaling
 factors (range [0, 1]) for the left and right output
 channels; the result is a panned version of the input
 having less intensity than the original audio signal
 */
PANPOS linearpan(double position){
    PANPOS pos;
    position *= 0.5;
    pos.left = position - 0.5;
    pos.right = position + 0.5;
    return pos;
}

/*
input: position in the range [-1, +1]
output: the panned position representing the amplitude scaling
factors (range [0, 1]) for the left and right output
channels applying a constant power function to the signal; the result
is a panned version if the input having the same intensity of the original
audio signal
*/
PANPOS constantpwrpan(double position){
    PANPOS pos;
    const double piover2 = 4.0 * atan(1.0) * 0.5;// pi/2, a 1/4 cycle of a sinusoid
    const double root2ovr2 = sqrt(2.0) * 0.5;//sqrt(2)/2
    double newPosition = position * piover2;
    double angle = newPosition * 0.5;// angle of panned signal
    pos.left = root2ovr2 * (cos(angle) - sin(angle));
    pos.right = root2ovr2 * (cos(angle) + sin(angle));
    return pos;
}
