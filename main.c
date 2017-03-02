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
    
    fsm_initialize();
    
    while (1) {
        
        
        for(int i = 0; i< 3; i++){
            if (i == 0){
                for (int floor = 0; floor < 3; floor ++){
                elev_button_type_t button = 0;
                if(elev_get_button_signal(floor, button) == 1){
                    fsm_buttonIsPushed(floor,button);
                    elev_set_button_lamp(button,floor,1);
                }
                }
            }
            if (i == 1){
                for (int floor = 1; floor < 4; floor ++){
                    elev_button_type_t button = 1;
                    if(elev_get_button_signal(floor, button) == 1){
                        //fsm_buttonIsPushed(floor,button);
                        elev_set_button_lamp(button,floor,1);
                    }
                }
            }
            if (i == 2){
                for (int floor = 0; floor < 4; floor ++){
                    elev_button_type_t button = 2;
                    if(elev_get_button_signal(floor, button) == 1){
                        //fsm_buttonIsPushed(floor,button);
                        elev_set_button_lamp(button,floor,1);
                    }
                }
            }

        }
        
        
        
        
        if (elev_get_floor_sensor_signal() == 3){
            elev_set_motor_direction(DIRN_DOWN);

        }
        if(elev_get_floor_sensor_signal() == 0){
            elev_set_motor_direction(DIRN_UP);

        }
    }
    return 0;
}
