//
//  shared.h
//  audio_programming
//
//  Created by Biagio Luglio on 01/01/2021.
//  Copyright © 2021 Biagio Luglio. All rights reserved.
//

#ifndef shared_h
#define shared_h

#include <stdio.h>

typedef struct OutputDeviceList {
    int size;
    char* hostApiName;
    char* infoName;
    int id;
    struct node* next;
} OutputDeviceList;

typedef struct State{
    OutputDeviceList* outputDevices
} State;

OutputDeviceList* push(OutputDeviceList* head, char* hostApiName, char* infoName, int id);

void showOutputDevices(OutputDeviceList * head);

#endif /* shared_h */
