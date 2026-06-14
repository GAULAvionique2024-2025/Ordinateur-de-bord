/*
 * tasks.h
 *
 *  Created on: 10 mai 2026
 *      Author: gagno
 */

#ifndef INC_SYSTEMS_TASKS_H_
#define INC_SYSTEMS_TASKS_H_

#include <stdint.h>

// TODO: Check viability of frequencies
#define TASK_REBOOT_MANAGER_FREQ_MS		50		// 20Hz
#define TASK_FSM_FREQ_MS              	5       // 200Hz
#define TASK_DATA_UPDATE_FREQ_MS      	10      // 100Hz
#define TASK_LOGGER_FREQ_MS           	20      // 50Hz (132 bytes odb_data_t * 50Hz = 6.6KB per second)
#define TASK_TELEMETRY_FREQ_MS        	100     // 10Hz
#define TASK_IDEFIX_FREQ_MS        		30000   // 0.033Hz
#define TASK_BLUETOOTH_APP_RX_FREQ_MS   50      // 20Hz
#define TASK_BLUETOOTH_APP_TX_FREQ_MS	1000    // 1Hz

void Tasks_Init(void);
void Task_RebootManager(void);
void Task_ExecuteFsm(void);
void Task_DataUpdate(void);
void Task_Logger(void);
void Task_Telemetry(void);
void Task_Idefix(void);
void Task_BluetoothAppRx(void);
void Task_BluetoothAppTx(void);

#endif /* INC_SYSTEMS_TASKS_H_ */
