
// Created by Eivind Salvesen on 22/02/17.
//

#include <stdio.h>
#include <time.h>
#include "assert.h"

#ifndef HEADER_FILE
#define HEADER_FILE


#include "timer.h"

#endif


static time_t start;


void timer_start(){
    time (&start);
}

void timer_reset(){
    start = -1;
    
}

int timer_isTimeOut(){
    //if (start != -1){
        time_t end;
        time (&end);
        float dif =  difftime(end, start);

        if(dif < 3.1 && dif > 2.9){
            return 1;
        }
    
        else{
            return -1;
        }
    
    
}
