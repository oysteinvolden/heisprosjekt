#include "elev.h"
#include "timer.h"
#include "state.h"
#include <stdio.h>


int main(){
   
    
    //startup condition
    fsm_initialize();


    while (1) {

        fsm_checkButton();  //sjekker om bestillingsknapp trykket inn og oppdaterer
        
        if(elev_get_floor_sensor_signal() != -1){   //sjekker om heisen står i en etasje
            fsm_arrivedAtFloor(elev_get_floor_sensor_signal());
        }
 
        if(timer_isTimeOut()== 1){  //sjekker om heisen har vært oppe i 3 sekunder
        	fsm_timeOut();
        }
       
       if(elev_get_stop_signal()){  //sjekker om stoppsignal er trykket inn
       		fsm_stopButtonPressed(elev_get_floor_sensor_signal());
       		
       while(elev_get_stop_signal()){

       }
       fsm_stopButtonUnpressed();
       
       }
    }
    return 0;
}

