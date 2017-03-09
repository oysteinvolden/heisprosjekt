#include "elev.h"
#include "timer.h"
#include "state.h"
#include <stdio.h>


int main() {
   
    
    //startup condition
    fsm_initialize();

    elev_button_type_t button;


    while (1) {

       
        
        

        for(int i = 0; i< 3; i++){

            //checks if button pressed up
            if (i == 0){
                button = 0;
                for (int floor = 0; floor < 3; floor ++){
                
                if(elev_get_button_signal(button, floor) == 1){
                   
                    fsm_buttonIsPushed(button,floor);
                    elev_set_button_lamp(button,floor,1);
                }
                }
            }
            //checks if button pressed down
            if (i == 1){
                button = 1;
                for (int floor = 1; floor < 4; floor ++){
                    
                    if(elev_get_button_signal(button, floor) == 1){
                        fsm_buttonIsPushed(button,floor);
                        elev_set_button_lamp(button,floor,1);
                    }
                }
            }
            //checks if button pressed inside
            if (i == 2){
                button = 2;
                for (int floor = 0; floor < 4; floor ++){
                    
                    if(elev_get_button_signal(button, floor) == 1){
                        fsm_buttonIsPushed(button,floor);
                        elev_set_button_lamp(button,floor,1);
                    }
                }
            }

        }
        //button = 0; 
        //int floor = 0;
        //fsm_buttonIsPushed(button,floor);
        //printf(elev_get_floor_sensor_signal());

        //sjekker om heisen står i en etasje - dette funker
        
        if(elev_get_floor_sensor_signal() != -1){
            //printf("Får signal og går til arrived at floor\n");
            //printf("floor signal: %d\n",elev_get_floor_sensor_signal());
            fsm_arrivedAtFloor(elev_get_floor_sensor_signal());
        }

        


        //printhelper();
        //printQueue();
        
        if(timer_isTimeOut()== 1){
        	printf("nå har det gått 3 sekunder");
        	fsm_timeOut();
        }
       
       if(elev_get_stop_signal()){
       		fsm_stopButtonPressed(elev_get_floor_sensor_signal());
       		
       while(elev_get_stop_signal()){

       }
       fsm_stopButtonUnpressed();
       
       }
       
       //printf("dette er utskriften til elev stop button %d\n",elev_get_stop_signal());
/*
        printf("heyhey2\n");
        printf("sensor signal: %d\n",elev_get_floor_sensor_signal());
       if (elev_get_floor_sensor_signal() == 3){
            printf("heyhey 4 floor\n");
            elev_set_motor_direction(DIRN_DOWN);
            //fsm_chooseMotorDirection(DIRN_DOWN); 
        }
        if(elev_get_floor_sensor_signal() == 0){
            printf("heyhey 1 floor\n");
            elev_set_motor_direction(DIRN_UP);
            //fsm_chooseMotorDirection(DIRN_UP);
        }
        */
    }
    return 0;
}
