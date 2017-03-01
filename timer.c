
// Created by Eivind Salvesen on 22/02/17.
//

#include <stdio.h>
#include <time.h>

#ifndef HEADER_FILE
#define HEADER_FILE


#include "timer.h"

#endif


static time_t start;


void timerStart(){
    time (&start);
}

void timerReset(){
    start = -1;
    
}

int getTime(){
    //if (start != -1){
        time_t end;
        time (&end);
        float dif =  difftime(end, start);
        printf("Execution time = %f\n", dif);

        if((dif) < 3.1 && dif > 2.9){
            return 1;
        }
    
        else{
            return -1;
        }
    
    
    
    
}
