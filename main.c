#include "elev.h"
#include "timer.h"
#include "state.h"
#include <stdio.h>


int main() {
    /*
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");
    
    
    elev_set_motor_direction(DIRN_UP);

     */
    
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
                    //printf("hallo");
                    fsm_buttonIsPushed(button,floor);
                    elev_set_button_lamp(button,floor,1);
                }
                }
            }
            //checks if button pressed down
            if (i == 1){
                button = 1;
                for (int floor = 1; floor < 3; floor ++){
                    
                    if(elev_get_button_signal(button, floor) == 1){
                        fsm_buttonIsPushed(button,floor);
                        elev_set_button_lamp(button,floor,1);
                    }
                }
            }
            //checks if button pressed inside
            if (i == 2){
                button = 2;
                for (int floor = 0; floor < 3; floor ++){
                    
                    if(elev_get_button_signal(button, floor) == 1){
                        fsm_buttonIsPushed(button,floor);
                        elev_set_button_lamp(button,floor,1);
                    }
                }
            }

        }
        
        
        
        
        //sjekker om heisen står i en etasje - dette funker
        if(elev_get_floor_sensor_signal() != -1){
            //printf("Får signal og går til arrived at floor\n");
            fsm_arrivedAtFloor(elev_get_floor_sensor_signal());
        }

        printhelper();
        printQueue();
        
        
        if (elev_get_floor_sensor_signal() == 3){
            elev_set_motor_direction(DIRN_DOWN);

        }
        if(elev_get_floor_sensor_signal() == 0){
            elev_set_motor_direction(DIRN_UP);

        }
    }
    return 0;
}
