//
//  shared.c
//  real_time
//
//  Created by Biagio Luglio on 01/01/2021.
//  Copyright © 2021 Biagio Luglio. All rights reserved.
//

#include "shared.h"

OutputDeviceList* push(OutputDeviceList* head, char* hostApiName, char* infoName, int id) {
    OutputDeviceList* newNode = (OutputDeviceList*) malloc(sizeof(OutputDeviceList));
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

void showOutputDevices(OutputDeviceList * head) {
    const OutputDeviceList * current = head;

    while (current != NULL) {
        printf("id: %d [%s] - %s\n", current->id, current->hostApiName, current->infoName);
        current = current->next;
    }
}
