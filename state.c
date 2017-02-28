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

static fsm_state state = noState;

//defining variables
int currentFloor;
int targetFloor;


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

void fsm_chooseMotorDirection(){
    switch(state){
        case idle:
            if(fsm_buttonIsPushed() == true){
                if(targetFloor > currentFloor){
                    elev_set_motor_direction(1);
                }
                else if(targetFloor < currentFloor){
                    elev_set_motor_direction(-1);
                }
                else if(targetFloor == currentFloor){
                    break;
                }
            }
        case unloading:
            //cannot check button is pushed in unloading
                if(targetFloor > currentFloor){
                    elev_set_motor_direction(1);
                    state = running;
                }
                else if(targetFloor < currentFloor){
                    elev_set_motor_direction(-1);
                    state = running;
                }
                else if(targetFloor == currentFloor){
                    break;
                }
            
            //more states that can set motor direction?
        default: break;
            
    }
}

void fsm_arrivedAtTargetFloor(int signal_floor){
    
    switch (state) {
        case running:
            if(targetFloor == signal_floor){
                if(queue_floorInQueue() == true){
                    queue_removeOrder();
                    elev_set_motor_direction(0);
                    elev_set_door_open_lamp(0);
                    elev_set_button_lamp(1);
                    state = unloading;
                    break;
                }
                else if(queue_floorInQueue() == false){
                    state = running; //stay in running mode until you reach target floor
                }
            }
            //more states that can use this logic? no?
        default:
            break;
    }
    
}

//local function - not sure if necessary
int fsm_floorSensorActive(){
    //check if some floor sensor are active
    if(elev_get_floor_sensor_signal() == -1){
        return 0;
        break;
    }
    return 1;
}

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
            
            state = emergency_stop_in_floor;
            break;
            
        default:
            break;
    }
}




























