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

void fsm_initialize();
void fsm_timeOut();
void fsm_enableDirection();//is this useful?
void fsm_arrivedAtFloor();
void fsm_stopButtonPressed();
void fsm_buttonIsPushed();
void fsm_chooseMotorDirection();
void fsm_turnOfButtonLights();


#endif /* state_h */
