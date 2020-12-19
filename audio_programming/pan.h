//
//  pan.h
//  audio_programming
//
//  Created by Biagio Luglio on 18/12/2020.
//  Copyright © 2020 Biagio Luglio. All rights reserved.
//

#ifndef pan_h
#define pan_h

#include <stdio.h>

typedef struct panpos {
    double left;
    double right;
} PANPOS;

PANPOS linearpan(double position);
PANPOS constantpwrpan(double position);

#endif /* pan_h */
