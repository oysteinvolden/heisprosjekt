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

#endif /* queue_h */

void queue_initialize();
void queue_addToQueue(int,int);
void queue_removeOrder(int,int);
int queue_floorInQueue(int);
int queue_getNextOrder(int,int);
int queue_selectNextOrder(int,int);


void printQueue();



/* queue_getNextOrder()
queue_selectNextOrder()
queue_addToQueue()
queue_removeOrder ()
queue_floorInQueue ()
*/