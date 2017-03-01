#include "elev.h"
#include "timer.h"
#include "state.h"
#include <stdio.h>


int main() {
    // Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }

    printf("Press STOP button to stop elevator and exit program.\n");
    
    
    elev_set_motor_direction(DIRN_UP);

    while (1) {
        /*
        if (elev_get_stop_signal() == 100){                 //usikker på hva som kommer ut av io read bits
            fsm_stopButtonPressed();
            while(elev_get_stop_signal() == 100){
                continue;
            }
            fsm_stopButtonUnpressed();
        }
        
        */
        
        for(int i = 0; i< 3; i++){
            elev_button_type_t button = i;
            for(int floor= 0; floor< 4; floor++){
                
                if(elev_get_button_signal(button, floor) == 1){
                    fsm_buttonIsPushed(button,floor);
                    elev_set_button_lamp(button,floor);
                }
            }
        }
        
        if (elev_get_floor_sensor_signal() != -1){
            fsm_arrivedatfloor(elev_get_floor_sensor_signal());
        }
        
        if(timer_isTimeOut() == 1){
            fsm_timeOut();
        }
        

        // Stop elevator and exit program if the stop button is pressed
        if (elev_get_stop_signal()) {
            elev_set_motor_direction(DIRN_STOP);
            break;
        }
    }

    return 0;
}
