//
//  state.h
//  state machine
//
//  Created by Øystein Volden on 28.02.2017.
//  Copyright © 2017 Øystein Volden. All rights reserved.
//

#ifndef state_h
#define state_h

#include <stdio.h>
#include "queue_copy.h"
#include "elev.h"
#include "timer.h"


void fsm_initialize();
void fsm_timeOut();
void fsm_arrivedAtFloor(int);
void fsm_stopButtonPressed();
void fsm_unloading();
void fsm_stopButtonUnpressed();
void fsm_buttonIsPushed(int,elev_button_type_t);
void fsm_chooseMotorDirection();
void fsm_turnOfButtonLights();


#endif /* state_h */
