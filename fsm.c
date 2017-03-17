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
static int lastDirectionFromFloor;


//initialiserer og sørger for at current floor blir oppdatert og at heisen settes i stopp og idle
void fsm_initialize(void){
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        
    }
    queue_initialize(); //initialize queue matrix
    elev_set_motor_direction(DIRN_UP);
    direction = 1;
    
    
    while(elev_get_floor_sensor_signal() == -1){
    	
    }
    
    //reached a floor, setting variables to starting position
    currentFloor = elev_get_floor_sensor_signal();
    targetFloor = -1;
    elev_set_motor_direction(DIRN_STOP);
    state = idle;
    timer_reset();
}


void fsm_timeOut(){
    
    timer_reset();
    targetFloor = queue_getNextOrder(currentFloor,direction);
    
    switch(state){
        case unloading:
        	
            //no next order in queue new state idle - stop
            if(targetFloor== -1){
                elev_set_door_open_lamp(0);
                state = idle;
            }
            //next order in queue new state running
            else if(targetFloor != -1){
                elev_set_door_open_lamp(0);
                fsm_chooseMotorDirection();
                lastDirectionFromFloor = direction;
                elev_set_motor_direction(direction);
                state = running;
                
            }
        default: break;
    }
   
}



int fsm_arrivedAtFloor(int Floor){
	
    if(Floor != -1){
	currentFloor = Floor;
    elev_set_floor_indicator(currentFloor);
    }
    

    switch (state){
    	case running:
    		
    		//if arrived at final destination (target floor) state - unloading, removing order, turning of lights
    		
    		if (currentFloor == targetFloor){
    			
    			
    			if(currentFloor == 0){
    				
    				elev_set_button_lamp(0,currentFloor,0);
                    elev_set_button_lamp(2,currentFloor,0);
                    queue_removeOrder(currentFloor,1);
    			}
    			else if(currentFloor == 3){
    				
    				elev_set_button_lamp(1,currentFloor,0);
                    elev_set_button_lamp(2,currentFloor,0);
                    queue_removeOrder(currentFloor,-1);
    			}
    			
    			else if(currentFloor >0 || currentFloor < 3){
    				
    				elev_set_button_lamp(0,currentFloor,0);
    				elev_set_button_lamp(1,currentFloor,0);
    				elev_set_button_lamp(2,currentFloor,0);
    				queue_removeOrder(currentFloor,-1);
    				queue_removeOrder(currentFloor,1);
    				
    			}
    			
    			fsm_unloading();
    			return 1;
    			
    		}
    				
    			
    		// if currentFloor is top or bottom floor and order in queue, state - unloading
    		if(currentFloor == 0){
    		
    			
    		if (queue_floorInQueue(currentFloor, 1) || queue_floorInQueue(currentFloor, -1)){
    				
    				
    				elev_set_button_lamp(0,currentFloor,0);
                    elev_set_button_lamp(2,currentFloor,0);
                    queue_removeOrder(currentFloor,1);
                    
                    fsm_unloading();
    				return 1;
    			}
    			}
    		if(currentFloor == 3){
    				if (queue_floorInQueue(currentFloor, 1) || queue_floorInQueue(currentFloor, -1)){
    				
    				elev_set_button_lamp(1,currentFloor,0);
                    elev_set_button_lamp(2,currentFloor,0);
                    queue_removeOrder(currentFloor,-1);
                    fsm_unloading();
    				return 1;
    			}
    			
    		}
    		
    		
            //if floor in same direction as elevator is moving state - unloading
    		if(queue_floorInQueue(currentFloor,direction) == 1){
    			
    			
    
                elev_set_button_lamp(0,currentFloor,0);
                elev_set_button_lamp(1,currentFloor,0);
                elev_set_button_lamp(2,currentFloor,0);
    				
                queue_removeOrder(currentFloor,direction);
                fsm_unloading();
    			}
                
    		else if(direction == 1){
                
                elev_set_button_lamp(0,currentFloor,0);
    			elev_set_button_lamp(1,currentFloor,0);
    			elev_set_button_lamp(2,currentFloor,0);
    			
    			queue_removeOrder(currentFloor,direction);
                fsm_unloading();

    		}
    		
    		
    		return 1;
    		}
    }
    //setting last direction when elevator leaves currentfloor without stopping in currentfloor 
    lastDirectionFromFloor = direction;
    return 0;
}

//stopping elevator, starting timer and set state unloading
void fsm_unloading(){
    
    elev_set_motor_direction(0);
    elev_set_door_open_lamp(1);
    timer_start();
    state = unloading;
    
}

//If stop button pressed, decides if its in a floor - state emergency_stop_in_floor
//or between floor - emergency_stop_between_floor

void fsm_stopButtonPressed(int position){
    
    switch (state) {
    
        case running:
        if(position == -1){
        	
        	state = emergency_stop_between_floor;
            
            }
        else{
        state = emergency_stop_in_floor;
        }
        	elev_set_motor_direction(0);
            fsm_deleteAllOrders();
            elev_set_stop_lamp(1);
            break;
        

        case idle:
        
        	if(position == -1){
        	
        		state = emergency_stop_between_floor;
            
            	}
        	else{
        	state = emergency_stop_in_floor;
        	}
        	
            
            fsm_deleteAllOrders();
            elev_set_stop_lamp(1);
            break;
            
        case unloading:
            elev_set_motor_direction(0);
            fsm_deleteAllOrders();
            elev_set_stop_lamp(1);
            
            state = emergency_stop_in_floor;
            break;

        default:
            break;
    }
}

//Local function for fsm_stopButtonPressed() deleting queue and set lights to zero
void fsm_deleteAllOrders(){
	
    queue_initialize();
    
    for (int i = 0; i < 4; i++){
    	elev_set_button_lamp(2,i,0);
    }
    for (int i = 0; i < 3; i++){
    	elev_set_button_lamp(0,i,0);
    }
    
    for (int i = 1; i < 4; i++){
    	elev_set_button_lamp(1,i,0);
    }
}

//When stop button is released: sets the elevator in correct state
void fsm_stopButtonUnpressed(){
    switch (state) {
        case emergency_stop_in_floor:
            fsm_unloading();
            elev_set_stop_lamp(0);
            break;
            
        case emergency_stop_between_floor:


            elev_set_stop_lamp(0);
            state = running;
            //state = emergency_stop_running;
            break;
            
        default:
            break;
    }
}


//setting the elevator driving direction

void fsm_chooseMotorDirection(){
    
    if(targetFloor > currentFloor){
        direction = 1;
    }
    else if(targetFloor < currentFloor){
        direction = -1;
    }

    if(targetFloor == currentFloor){
    
        if(lastDirectionFromFloor == 1){
            direction = -1;
    }
        else {
        direction = 1;
        }   
}
    
}

//If a button request is pushed: adding to queue and setting in correct state
void fsm_buttonIsPushed(elev_button_type_t button,int floor){

    switch (state) {
        case idle:
        	
            queue_addToQueue(button,floor);

            targetFloor = queue_getNextOrder(currentFloor,direction);

            fsm_chooseMotorDirection();
            elev_set_motor_direction(direction);
            state = running;
            break;
            
        case running:
            queue_addToQueue(button,floor);
            targetFloor = queue_getNextOrder(currentFloor,direction);
            fsm_chooseMotorDirection();
            elev_set_motor_direction(direction);        
            break;
            
        case unloading:
            queue_addToQueue(button,floor);
            targetFloor = queue_getNextOrder(currentFloor,direction);
            fsm_chooseMotorDirection();
            break;
    
        default:
            break;
    }
    
}

void fsm_checkButton(){
    elev_button_type_t button;
    for(int i = 0; i< 3; i++){

            //checks if button pressed up
            if (i == 0){
                button = 0;
                for (int floor = 0; floor < 3; floor ++){
                
                if(queue_checkOrder(button,floor)== 0){
                if(elev_get_button_signal(button, floor) == 1){
                
                    fsm_buttonIsPushed(button,floor);
                    elev_set_button_lamp(button,floor,1);
            }
            }
            }
            }
            //checks if button pressed down

            if (i == 1){
                button = 1;
                for (int floor = 1; floor < 4; floor ++){
                    
                    if(queue_checkOrder(button,floor)==0){

                    if(elev_get_button_signal(button, floor) == 1){
                        
                        fsm_buttonIsPushed(button,floor);
                        elev_set_button_lamp(button,floor,1);
            }
            }
            }
            }
            //checks if button pressed inside
            if (i == 2){

                button = 2;
                for (int floor = 0; floor < 4; floor ++){
                    if(queue_checkOrder(button,floor) == 0){
                    if(elev_get_button_signal(button, floor) == 1){
                        fsm_buttonIsPushed(button,floor);
                        elev_set_button_lamp(button,floor,1);
            }
            }
            }
            }

    }
}


void printhelper(){
    printf("currentfloor %d\n",currentFloor );
    printf("targetfloor %d\n",targetFloor );
    printf("direction %d\n",direction);
    printf(" state %d\n",state);
    printf("last direction from floor %d\n", lastDirectionFromFloor);
}





















