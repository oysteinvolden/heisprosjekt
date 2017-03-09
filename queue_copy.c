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

static int queue[4][3];
static int Nfloor = 4;


void queue_initialize(){
    for (int i = 0; i< 4; i++){
        for (int j= 0; j < 3; j++){
            queue[i][j] = 0;
        }
    }
}

void queue_addToQueue(elev_button_type_t button, int floor){
    assert(floor >= 0 && floor <= 3);
    
    
    
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

void queue_removeOrder(int floor, int direction){
    assert(floor >= 0 && floor <= 3);
    int counter = 1;//what is this doing?
    
    //delete order in button inside (first priority)
    queue[floor][2]= 0;
    switch (direction) {
        case 1:
            //if direction up, the order up in floor is deleted
            if(queue[floor][0] == 1){
                queue[floor][1]= 0;
            }
            queue[floor][0]= 0;
            break;
            
        case -1:
            //if direction down, the order down in floor is deleted
            if(queue[floor][1] == 1){
                queue[floor][0]= 0;
            }
            queue[floor][1]= 0;
            break;
            
        default:
            break;
    }
}


//will just verify if the conditions is satisfied, not priority between orders
int queue_floorInQueue(int floor, int direction){
    //assert(floor >= 0 && floor <= 3));
    // check if queue should stop in lowest or highest floor
    
    if(floor == 0){
        if (queue[0][0] == 1 || queue[0][2] == 1){
            //switchDir(direction);
            return 1;
        }
        }
        
       if (floor == 3){
        if (queue[3][1] == 1 || queue[3][2] == 1){
            //switchDir(direction);
            return 1;
    }
    }
    
    //check if inside orders (will be priorited)
    if(queue[floor][2] == 1){
        return 1;
    }
    
    //checking if direction
    if(direction == 1){
            //printf("checking inside queue");
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

//obs: a state function set a floor as -1, if getNextOrder return -1.
int queue_getNextOrder(int currentFloor, int direction){
    
    if(queueEmpty() == 0){
        return -1;
    }
    else{
        //printf("return true order %d\n", queue_selectNextOrder(currentFloor,direction) );

        //dersom det bare ligger en bestilling i køen og den er over eller under retningen vi kjører nå
        if(queue_selectNextOrder(currentFloor,direction) == -1){
            if(direction == -1){
                int byttetall = 1;
                //printf("return change %d", queue_selectNextOrder(currentFloor,byttetall) );
                return queue_selectNextOrder(currentFloor,byttetall);
            }
            else if(direction == 1){
            int byttetall = -1;
            return queue_selectNextOrder(currentFloor,byttetall);
        }
        }
        return queue_selectNextOrder(currentFloor,direction);
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

int queueiteratorup(int currentFloor, int it){
    for(int i =3; i >= (currentFloor); i--){
            //printf("denne skal være%d\n",queue[i][it] );

            if (queue[i][it] == 1){
            	//printf("her skal vi retuirnere 3 %d\n ", i); 
                return (i);
            }
        }
        return -1;
}

int queueiteratordown(int currentFloor, int it){
    for(int i =0; i <=(currentFloor); i++){
            if (queue[i][it] == 1){
                return (i);
            }
        }
        return -1;
}



int queue_selectNextOrder(int currentFloor,int direction){
    //printf("queue_selectNextOrder");
    
    /*
    if (currentFloor == 3) {
        direction = -1;
    }
    if(currentFloor == 0){
        direction = 1;
    }
    */
    
    
    //printf("%d heisdireksjon",direction);
    if (direction == 1){
            //printf("case 2");
            // is there someone inside wanting up?
            if (queueiteratorup(currentFloor,2) != -1){
                
                return queueiteratorup(currentFloor,2);
            }
            
            
            // her må vi sjekke at dersom vil ned > vil opp så skal target floor settes til ned ellers skal opp gjelde. 
            //is there someone over us wanting down?
            
            if (queueiteratorup(currentFloor,1)>queueiteratorup(currentFloor,0)){
            	return queueiteratorup(currentFloor,1);
            }
            
            //is there someone over us wanting up?
            
            if (queueiteratorup(currentFloor,0) != -1){
                return queueiteratorup(currentFloor,0);
            }
            
            if (queueiteratorup(currentFloor,1) != -1){
                return queueiteratorup(currentFloor,1);
            }
           
            
           
            //printf("not working");
    }
            //OBS OBS denne rekkefølgen må dobbeltsjekkes men tror den stemmer
            
            
    if(direction == -1){
            //printf("case -1");
            // is there someone inside wanting down?

       if (queueiteratordown(currentFloor,2) != -1){
                
                return queueiteratordown(currentFloor,2);
            }
            
            if ((queueiteratordown(currentFloor,1) != -1) && (queueiteratordown(currentFloor,0) != -1)){
            	if(queueiteratordown(currentFloor,1) > queueiteratordown(currentFloor,0)){
                    //assert(0);
                    return queueiteratordown(currentFloor,0);

            }
        }
            
            //is there someone under us wanting down?
            
            if (queueiteratordown(currentFloor,1) != -1){
                //assert(0);
                return queueiteratordown(currentFloor,1);
            }
            //is there someone under us wanting up?
            
            if (queueiteratordown(currentFloor,0) != -1){
                return queueiteratordown(currentFloor,0);
            }
            // is there someone inside wanting down?
            /*
            for(int i =0; i < (currentFloor); i++){
                if (queue[i][2] == 1){
                    return (i);
                }
            }
            
            //is there someone under us wanting up?
            for(int i =0; i < (currentFloor); i++){
                if (queue[i][0] == 1){
                    return (i);
                }
            }
            
            */
            //is there someone under us wanting down?
            /*
            for(int i =0; i == (currentFloor); i++){
                printf("yo");
                if (queue[i][1] == 1){
        
                    return (i);
                }
            }
            */
    //}
    
}
return -1;
}



void printQueue(){
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


/*
void queue_setUpdatedFloor(int floor){
    lastUpdatedFloor = floor;
}

int queue_getUpdatedFloor(){
    return lastUpdatedFloor;
}
*/
