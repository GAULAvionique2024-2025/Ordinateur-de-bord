/*
 * tasks.c
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */


#include "App/scheduler.h"
#include "App/flight_fsm.h"

extern odb_data flight_data;

// 100Hz : Navigation and critical sensor processing (IMU, Baro, GPS)
void Task_Nav(void) {
    ODB_Update(&flight_data);
}

// 50Hz : State Machine et Pyros
void Task_Control(void) {
    if(FSM_GetGlobalState() != STATE_POSTFLIGHT) {
        FSM_Update(&flight_data);
    }
}

// 10Hz : Telemetry et Communication
void Task_Comm(void) {
    if(FSM_GetGlobalState() == STATE_PREFLIGHT) {
        // HM11_ProcessDebug();
    }

    if(FSM_GetGlobalState() == STATE_POSTFLIGHT) {
        // IdeFIX transmission
    } else {
        // Mavlink transmission
    }
}
