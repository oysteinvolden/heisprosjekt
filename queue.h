//
//  queue.h
//  Queue
//
//  Created by Eivind Salvesen on 28/02/17.
//  Copyright © 2017 Apple Computers. All rights reserved.
//

#ifndef queue_h
#define queue_h

#include <stdio.h>
#include "elev.h"

#endif /* queue_h */

void queue_initialize();
void queue_addToQueue(elev_button_type_t,int);
void queue_removeOrder(int);
int queue_floorInQueue(int,int);
int queue_getNextOrder(int,int);
int queue_selectNextOrder(int,int);
int queue_empty();
int queue_iteratorUp(int,int);
int queue_iteratorDown(int,int);
void queue_printQueue();
void queue_setUpdatedFloor(int);
int queue_getUpdatedFloor();
int queue_checkOrder(elev_button_type_t button,int floor);

