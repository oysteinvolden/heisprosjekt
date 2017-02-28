//
//  queue.c
//  Queue
//
//  Created by Eivind Salvesen on 28/02/17.
//  Copyright © 2017 Apple Computers. All rights reserved.
//

#include "queue.h"
#include <assert.h>



static int queue[4][3];

void queue_initialize(){
    for (int i = 0; i< 4; i++){
        for (int j= 0; j < 3; j++){
            queue[i][j] = 0;
        }
    }
}

void queue_addToQueue(int floor, int buttondirn){
    assert(floor >= 0 && floor <= 11);
    int counter = 1;

    
    switch (buttondirn) {
        case 1:
            
            for (int i =0; i<4; i++){
                if (counter == floor){
                    queue[i][0] = 1;
                    break;
                }
                counter++;
            }
            break;
        case 2:
            for (int i =0; i<4; i++){
                if (counter == floor){
                    queue[i][1] = 1;
                    break;
                }
                counter++;
            }
            break;
            
        case 3:
            for (int i =0; i<4; i++){
                if (counter == floor){
                    queue[i][2] = 1;
                    break;
                }
                counter++;
            }
            break;
            
            
        default:
            break;
    }
}

void queue_removeOrder(int floor, int buttondirn){
    assert(floor >= 0 && floor <= 11);
    int counter = 1;
    
    
    switch (buttondirn) {
        case 1:
            
            for (int i =0; i<4; i++){
                if (counter == floor){
                    queue[i][0] = 0;
                    break;
                }
                counter++;
            }
            break;
        case 2:
            for (int i =0; i<4; i++){
                if (counter == floor){
                    queue[i][1] = 0;
                    break;
                }
                counter++;
            }
            break;
            
        case 3:
            for (int i =0; i<4; i++){
                if (counter == floor){
                    queue[i][2] = 0;
                    break;
                }
                counter++;
            }
            break;
            
            
        default:
            break;
    }
}



int queue_floorInQueue(int floor){
    int counter = 0;
    for (int i =0; i<4; i++){
        for(int j= 0; j< 3; j++){
            if(counter == floor){
                return 1;
            }
        }
        counter++;
    }
    return 0;
}


int queue_getNextOrder(int position, int direction){
    if(queueEmpty() == 0){
        return -1;
    }
    else{
        return queue_selectNextOrder(position,direction);
    }
    
    return -1;
}

int queueEmpty(){
    
    for (int i =0; i<4; i++){
        for(int j= 0; j< 3; j++){
            if(queue[i][j] == 1){
                return 1;
            }
        }
    }
    return 0;
}

int queue_selectNextOrder(int thefloorwereatnow,int direction){
    
    switch (direction) {
        case 1:
            
            // is there someone inside wanting up?
            for(int i =3; i > (thefloorwereatnow-1); i--){
                if (queue[i][2] == 1){
                    return (i+1);
                }
            }
            
            //is there someone over us wanting down?
            for(int i =3; i > (thefloorwereatnow-1); i--){
                if (queue[i][1] == 1){
                    return (i+1);
                }
            }
            //is there someone over us wanting up?

            for(int i =3; i > (thefloorwereatnow-1); i--){
                if (queue[i][0] == 1){
                    return (i+1);
                }
            }
            

            //OBS OBS denne rekkefølgen må dobbeltsjekkes men tror den stemmer
            
            break;
            
        case -1:
            // is there someone inside wanting down?
            for(int i =0; i < (thefloorwereatnow-1); i++){
                if (queue[i][2] == 1){
                    return (i+1);
                }
            }
            
            //is there someone under us wanting up?
            for(int i =0; i < (thefloorwereatnow-1); i++){
                if (queue[i][0] == 1){
                    return (i+1);
                }
            }

            
            //is there someone over us wanting down?
            for(int i =0; i < (thefloorwereatnow-1); i++){
                if (queue[i][1] == 1){
                    return (i+1);
                }
            }
            
        default:
            break;
    }
    
    return 0;
}



void printQueue(){
    int k = 0;
    for (int i =0; i<4; i++){
        for(int j= 0; j< 3; j++){
            printf("%d",queue[i][j]);
            k++;
            if(k ==3){
                k = 0;
                printf("\n");
            }
        }
    }
}

