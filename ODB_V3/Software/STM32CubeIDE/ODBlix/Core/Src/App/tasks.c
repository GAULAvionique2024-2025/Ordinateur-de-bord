/*
 * tasks.c
 *
 *  Created on: 10 mai 2026
 *      Author: gagno
 */


#include "App/tasks.h"
#include "stm32f4xx_hal.h"
#include "App/config.h"
#include "App/flight_fsm.h"
#include "App/logger.h"
#include "App/scheduler.h"
#include "GAUL_Drivers/system.h"


extern odb_data_t flight_data;
extern hm11_t hm11;
extern rfd900x_t rfd900x;


void Task_ExecuteFsm(void) {
    FSM_Update();
}

void Task_UpdateData(void) {
    ODB_Update(&flight_data);
    Logger_PushData(&flight_data);
}

void Task_Logger(void) {
    Logger_Task();
}

void Task_Telemetry(void) {
    if((flight_data.system_states & FLAG_RADIO_OK) == 0U) return;
    Telemetry_SendRocketData(&rfd900x, current_config.stage_role + 2, &flight_data, HAL_GetTick());
}

void Task_BluetoothRx(void) {
    if((flight_data.system_states & FLAG_BT_OK) == 0U) return;
    App_HandleCommands(&hm11);
}

void Task_BluetoothTx(void) {
	if((flight_data.system_states & FLAG_BT_OK) == 0U) return;
	App_SendFrame(&hm11, &flight_data);
}
