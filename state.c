//
//  state.c
//  state machine
//
//  Created by Øystein Volden on 28.02.2017.
//  Copyright © 2017 Øystein Volden. All rights reserved.
//

#include "state.h"
#include "assert.h"

//defining states
typedef enum {noState,idle,running,unloading,emergency_stop_in_floor,emergency_stop_between_floor} fsm_state;



//defining variables
static int currentFloor;
static int targetFloor;
static fsm_state state = noState;
static int direction;


//initialiserer og sørger for at current floor blir oppdatert og at heisen settes i stopp og idle
void fsm_initialize(void){
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        
    }
    queue_initialize();//clean queue matrix
    elev_set_motor_direction(DIRN_UP);
    direction = 1;
    while(elev_get_floor_sensor_signal() == -1){
    	//printf("sensor signal -1 \n");
    }
    //reached a floor
    //currentFloor = elev_get_floor_sensor_signal();
    currentFloor = 1;
    elev_set_motor_direction(DIRN_STOP);
    state = idle;
}

void fsm_LastMeasuredFloor(){
    switch(elev_get_floor_sensor_signal()){
        case 0:
            queue_setUpdatedFloor(0);
        case 1:
            queue_setUpdatedFloor(1);
        case 2:
            queue_setUpdatedFloor(2);
        case 3:
            queue_setUpdatedFloor(3);
        default:
        break;
    }

        
}

void fsm_timeOut(){
    
    timer_reset();//kanskje innenfor switch løkka?
    
    switch(state){
        case unloading:
            //no order in queue
            if(queue_selectNextOrder(currentFloor,direction) == 0){
                elev_set_door_open_lamp(0);
                state = idle;
                //more things happening here?
            }
            //order in queue
            else if(queue_selectNextOrder(currentFloor,direction) >= 1 && queue_selectNextOrder(currentFloor,direction) <= 3){
                elev_set_door_open_lamp(0);
                targetFloor = queue_getNextOrder(currentFloor,direction);
                fsm_chooseMotorDirection();
                elev_set_motor_direction(direction);
                state = running;
                //more things happening here?
            }
        default: break;
    }
    
}




void fsm_arrivedAtFloor(int signal_floor){
    
    //set current floor
    currentFloor = signal_floor;
    elev_button_type_t buttonout;
    elev_button_type_t buttoninside= 2;

    /*
    if(currentFloor == 0 || direction == -1){
        switchDir(direction);
    }
    else if (currentFloor == 3 || direction == 1){
        switchDir(direction);
    }
    */

    printf("her skal vi inn i state running\n");
    //printf(" state %d\n",state);//ok
    switch (state) {
        case running:
        //printf("inne i state running");
            //check if the order is in right direction
        	//printf("This should stop if 1    %d\n",(queue_floorInQueue(currentFloor,direction) == 1) );
            if(queue_floorInQueue(currentFloor,direction) == 1){//direction set in fsm_initialize()
                printf("floor is in queue- should stop\n");
                printf("direction:%d\n",direction);
                if (direction == 1){
                    printf("in direction 1\n");
                    buttonout = 1;//endret fra 0 til 1 her (f.eks 4.etg: buttonout = 1 (=call_down) )
                    queue_removeOrder(currentFloor,direction);
                    printf("hei 1\n");
                    printf("button (should be button_call_down= 0):%d\n",buttonout);
                    printf("floor (should be 3):%d\n",signal_floor);
                    elev_set_button_lamp(buttonout,signal_floor,0);
                    elev_set_button_lamp(buttoninside,signal_floor,0);
                    
                }
                //her er det klart feil dersom vi er på vei ned og skal plukke opp noen på vei oppp.... 

                else if(direction == -1) {
                    printf("in direction -1\n");
                    buttonout = 0;//tilsvarende fra 1 til 0 her ved 1 etasje
                    queue_removeOrder(currentFloor,direction);
                    printf("button (should be equal button_call_up = 1):%d\n",buttonout);
                    printf("floor (should be equal 0):%d\n",signal_floor);
                    elev_set_button_lamp(buttonout,signal_floor,0);
                    elev_set_button_lamp(buttoninside,signal_floor,0);
                    
                }
                //button inside is set independly of direction as long as it is in queue
                
                printf("Nå skal vi egentlig stoppe\n");
                fsm_unloading();
                break;
            }
            else if(queue_floorInQueue(currentFloor,direction) == 0){
                state = running; //stay in running mode until you reach target floor
                break;
            }
            
            //more states that can use this logic? no?
        default:
            break;
    }
    
    
}


//local function
void fsm_turnOfButtonLights(){
    for(int i = 0; i< 3; i++){
        elev_button_type_t button = i;
        for(int floor= 0; floor< 4; floor++){
            elev_set_button_lamp(button,floor,0);//clear light bits
        }
    }
    
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
            elev_set_motor_direction(1);
            elev_set_door_open_lamp(1);
            queue_initialize();//delete all orders
            fsm_turnOfButtonLights();
            
            state = emergency_stop_in_floor;
            break;
            
        default:
            break;
    }
}

void fsm_unloading(){
    printf("er i unloading\n");
    elev_set_motor_direction(0);
    elev_set_door_open_lamp(1);
    timer_start();
    state = unloading;
}

void fsm_stopButtonUnpressed(){
    switch (state) {
        case emergency_stop_in_floor:
            fsm_unloading();
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
/*
void fsm_chooseMotorDirection(){
     printf("direction before %d\n",direction);
    if(targetFloor > currentFloor){
        direction = 1;
    }
    else if(targetFloor < currentFloor){
        direction = -1;
    }
    printf("direction after %d\n",direction);
}*/

void fsm_chooseMotorDirection(){
     if (elev_get_floor_sensor_signal() == 3){
        direction = -1;
     }
     else if(elev_get_floor_sensor_signal() == 0){
        direction = 1;
    }
}

void fsm_buttonIsPushed(elev_button_type_t button,int floor){
    //check if the button is valid
    //assert(button >= 0 && button <= 2);

    //set current floor
    //currentFloor = signal_floor;
    //elev_button_type_t buttonout;
    //elev_button_type_t buttoninside= 2;

    
    switch (state) {
        case idle:
        	printf(" current floor %d\n",currentFloor);
            printf(" state %d\n",state);
            queue_addToQueue(button,floor);
            
            //Case: Heisen starter i 1.etasje og får en bestilling fra 4.etasje. Heisen 
            //kjører til 4.etasje


            targetFloor = queue_getNextOrder(currentFloor,direction);
            fsm_chooseMotorDirection();
            elev_set_motor_direction(direction);
            state = running;
            printQueue();
            printf("direction %d\n",direction);
            printf("target floor %d\n",targetFloor);

            break;
            
        case running:
            queue_addToQueue(button,floor);
            //must happend things here as well...(?)
            /*
            targetFloor = queue_getNextOrder(currentFloor,direction);//this is not right if it returns -1
            fsm_chooseMotorDirection();
            elev_set_motor_direction(direction);
            state = running;
            printQueue();
            printf("direction %d\n",direction);
            printf("target floor %d\n",targetFloor);
            */

            //printQueue();
            break;
            
        case unloading:
            queue_addToQueue(button,floor);
            
            
        default:
            break;
    }
    
}

void switchDir(int direction){
    if (direction == 1){
        direction = -1;
    }
    else if (direction== -1){
        direction = 1;
    }
}

void printhelper(){
    printf("currentfloor %d\n",currentFloor );
    printf("targetfloor %d\n",targetFloor );
    printf(" state %d\n",state);
}





























