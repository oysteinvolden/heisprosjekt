//
//  state.c
//  state machine
//
//  Created by Øystein Volden on 28.02.2017.
//  Copyright © 2017 Øystein Volden. All rights reserved.
//

#include "state.h"

//defining states
typedef enum {noState,idle,running,unloading,emergency_stop_in_floor,emergency_stop_between_floor} fsm_state;

typedef enum {up,down,out}
buttonType;

//defining variables
static int currentFloor;
static int targetFloor;
static fsm_state state = noState;
static buttonType button;


void fsm_initialize(void){
    elev_init();
    state = idle;
}

void fsm_timeOut(){
    
    timer_reset();//kanskje innenfor switch løkka?
    
    switch(state){
        case unloading:
            //no order in queue
            if(queue_selectNextOrder == -1){
                state = idle;
                //more things happening here?
            }
            //order in queue
            else if(queue_selectNextOrder >= 0 && queue_selectNextOrder <= 11){
                state = running;
                //more things happening here?
            }
        default: break;
    }
    
}




void fsm_arrivedAtFloor(int signal_floor){
    
    //set current floor
    currentFloor = signal_floor;
    
    switch (state) {
        case running:
            //check if the order is in right direction
            if(queue_floorInQueue(currentFloor,button) == 1){
                queue_removeOrder();
                elev_set_motor_direction(0);
                elev_set_door_open_lamp(0);
                elev_set_button_lamp(1);
                state = unloading;
                break;
            }
            else if(queue_floorInQueue(currentFloor,button) == 0){
                state = running; //stay in running mode until you reach target floor
            }
            
            //more states that can use this logic? no?
        default:
            break;
    }
    
    
}

/*
 //local function - not sure if necessary
 int fsm_floorSensorActive(){
 //check if some floor sensor are active
 if(elev_get_floor_sensor_signal() == -1){
 return 0;
 break;
 }
 return 1;
 }*/

//local function
void fsm_turnOfButtonLights(){
    
}


void fsm_stopButtonPressed(){
    switch (state) {
        case running:
            state = emergency_stop_between_floor;
            break;
            
        case idle:
            state = emergency_stop_in_floor;
            break;
            
        case unloading:
            elev_set_motor_direction(DIRN_UP);
            elev_set_door_open_lamp(1);
            queue_initialize();//delete all orders
            fsm_turnOfButtonLights();
            
            state = emergency_stop_in_floor;
            break;
            
        default:
            break;
    }
}

void fsm_stopButtonUnpressed(){
    switch (state) {
        case emergency_stop_in_floor:
            
            state = unloading;
            break;
            
        case emergency_stop_between_floor:
            
            state = idle;
            break;
            
        default:
            break;
    }
}


//local function
int fsm_chooseMotorDirection(){
    if(targetFloor > currentFloor){
        return 1;
    }
    else if(targetFloor < currentFloor){
        return -1;
    }
    else if(targetFloor == currentFloor){
        return 0;
    }
    return 0;
    
}

void fsm_buttonIsPushed(int floor,int direction,buttonType button){
    //check if the button is valid
    assert(button >= 0 && button <= 2);
    
    switch (state) {
        case idle:
            queue_addToQueue(floor,button);
            int nextOrderFloor;
            nextOrderFloor = queue_getNextOrder(floor, direction);
            elev_set_motor_direction(fsm_chooseMotorDirection());
            state = running;
            break;
            
        case running:
            queue_addToQueue(floor,button);
            break;
            
        case unloading:
            queue_addToQueue(floor,button);
            
            
        default:
            break;
    }
    
    
    
    
    
    
}





























