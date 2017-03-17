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
int fsm_arrivedAtFloor(int);
void fsm_stopButtonPressed();
void fsm_unloading();
void fsm_stopButtonUnpressed();
void fsm_buttonIsPushed(elev_button_type_t,int);
void fsm_chooseMotorDirection();
void fsm_turnOfButtonLights();
void printhelper();
void switchDir(int);
void fsm_LastMeasuredFloor();
void fsm_deleteAllOrders();
int getstate();
int fsm_betweenFloorChooseDirection();
void fsm_checkButton();
#endif /* state_h */
