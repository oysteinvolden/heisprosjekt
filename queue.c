//
//  queue.c
//  Queue
//
//  Created by Eivind Salvesen on 28/02/17.
//  Copyright © 2017 Apple Computers. All rights reserved.
//

#include "queue_copy.h"
//#include "state.h"
#include <assert.h>


static int queue[4][3];//rows: floor, columns: button type
static int NFloor = 4;
static int NButtons = 3;


//initialize queue matrix
void queue_initialize(){
    for (int i = 0; i < NFloor; i++){
        for (int j= 0; j < NButtons; j++){
            queue[i][j] = 0;
        }
    }
}


//adding order to queue matrix dependent of button type
void queue_addToQueue(elev_button_type_t button, int floor){
    assert(floor >= 0 && floor < NFloor);
    
    switch (button) {
        case 0:
            queue[floor][0]= 1;
            break;
        case 1:
            queue[floor][1]= 1;
            break;
            
        case 2:
            queue[floor][2]= 1;
            break;
                      
        default:
            break;
    }
}


//remove all (valid) orders from queue matrix in given floor
void queue_removeOrder(int floor){
   assert(floor >= 0 && floor <= NFloor);
   queue[floor][2] = 0;
   queue[floor][1] = 0;
   queue[floor][0] = 0;
}


//return order type in given floor
int queue_checkOrder(elev_button_type_t button, int floor){
    return queue[floor][button];

}


//will verify if the conditions for floor in queue is true
int queue_floorInQueue(int floor, int direction){

    //check if queue will stop in lowest or highest floor
    if(floor == 0){
        if (queue[0][0] == 1 || queue[0][2] == 1){
            return 1;
            }
        }
        
    if (floor == 3){
        if (queue[3][1] == 1 || queue[3][2] == 1){  
            return 1;
            }
        }
    
    //check if inside orders
    if(queue[floor][2] == 1){
        return 1;
    }
    
    //checking if direction and button type correspond (same direction)
    if(direction == 1){
            if(queue[floor][0] == 1){
                return 1;
            }
        }
        else if(direction == -1){
            if(queue[floor][1] == 1){
                return 1;
        }
                
        }
    return 0;
}


//will return next order (if there is order in queue)
int queue_getNextOrder(int currentFloor, int direction){
    
    if(queue_empty() == 0){
        return -1;
    }
    else{

        //if there is a order in queue and it lays in opposite direction compared to the direction the elevator are moving
        if(queue_selectNextOrder(currentFloor,direction) == -1){
            if(direction == -1){
                int changeDirection = 1;
                return queue_selectNextOrder(currentFloor,changeDirection);
            }
            else if(direction == 1){
            int changeDirection = -1;
            return queue_selectNextOrder(currentFloor,changeDirection);
            }
        }
        return queue_selectNextOrder(currentFloor,direction);
    }
    return -1;
}


//returns next order
int queue_selectNextOrder(int currentFloor, int direction){    
    
    if (direction == 1){
            // is there someone inside wanting up?
            if (queue_iteratorUp(currentFloor,2) != -1){
                return queue_iteratorUp(currentFloor,2);
            }
            
            //is there someone over us wanting down?
            if ((queue_iteratorUp(currentFloor,1) != -1) && (queue_iteratorUp(currentFloor,0) != -1)){
                if (queue_iteratorUp(currentFloor,1) > queue_iteratorUp(currentFloor,0)){
                    return queue_iteratorUp(currentFloor,1);
                }
            }
            
            //is there someone over us wanting up?
            if (queue_iteratorUp(currentFloor,0) != -1){
                return queue_iteratorUp(currentFloor,0);
            }
            
            //is there someone over us 
            if (queue_iteratorUp(currentFloor,1) != -1){
                return queue_iteratorUp(currentFloor,1);
            }
           
    }            
            
    if(direction == -1){    
            //is there someone inside wanting down?
            if (queue_iteratorDown(currentFloor,2) != -1){  
                return queue_iteratorDown(currentFloor,2);
            }

            //is there someone under us wanting up?
            if ((queue_iteratorDown(currentFloor,1) != -1) && (queue_iteratorDown(currentFloor,0) != -1)){
                if(queue_iteratorDown(currentFloor,1) > queue_iteratorDown(currentFloor,0)){
                    return queue_iteratorDown(currentFloor,0);
                }
            }
            
            //is there someone under us wanting down? 
            if (queue_iteratorDown(currentFloor,1) != -1){
                return queue_iteratorDown(currentFloor,1);
            }

            //is there someone under us wanting up?
            if (queue_iteratorDown(currentFloor,0) != -1){
                return queue_iteratorDown(currentFloor,0);
            }
            
        }
    return -1;
}


//iterate from highest floor to current floor and return floor where order is detected
int queue_iteratorUp(int currentFloor, int it){
    for(int i = NFloor - 1; i > (currentFloor); i--){
            if (queue[i][it] == 1){
                return (i);
            }
        }
        return -1;
}


//iterate from lowest floor to current floor and return floor where order is detected
int queue_iteratorDown(int currentFloor, int it){
    for(int i = 0; i < (currentFloor); i++){
            if (queue[i][it] == 1){
                return (i);
            }
        }
        return -1;
}
           
//check if queue is empty
int queue_empty(){
    for (int i = 0; i < NFloor; i++){
        for(int j= 0; j < NButtons; j++){
            if(queue[i][j] == 1){
                return 1;
            }
        }
    }
    return 0;
}


void queue_printQueue(){
    int k = 0;
    printf("\n");
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



