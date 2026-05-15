/*
 * tasks.h
 *
 *  Created on: 10 mai 2026
 *      Author: gagno
 */

#ifndef INC_APP_TASKS_H_
#define INC_APP_TASKS_H_

#include <stdint.h>

// TODO: Check viability of frequencies
#define TASK_FSM_FREQ_HZ              	(200U)
#define TASK_DATA_UPDATE_FREQ_HZ      	(100U)
#define TASK_LOGGER_FREQ_HZ           	(50U)
#define TASK_TELEMETRY_FREQ_HZ        	(10U)
#define TASK_BLUETOOTH_RX_FREQ_HZ     	(20U)
#define TASK_BLUETOOTH_FRAME_FREQ_HZ	(10U)

void Tasks_Init(void);
void Task_ExecuteFsm(void);
void Task_UpdateData(void);
void Task_Logger(void);
void Task_Telemetry(void);
void Task_BluetoothRx(void);
void Task_BluetoothTx(void);

#endif /* INC_APP_TASKS_H_ */
