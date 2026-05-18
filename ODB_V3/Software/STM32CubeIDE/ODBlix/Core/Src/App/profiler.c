/*
 * profiler.c
 *
 *  Created on: 18 mai 2026
 *      Author: gagno
 */


#include "App/profiler.h"
#include "GAUL_Drivers/LowLevel/dwt.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>


static task_metrics_t system_tasks[PROFILE_MAX_TASKS];
static uint32_t last_log_time = 0;

void Profiler_Init(void) {
    for(int i = 0; i < PROFILE_MAX_TASKS; i++) {
        system_tasks[i].start_cycles = 0;
        system_tasks[i].current_time_ms = 0.0f;
        system_tasks[i].max_time_ms = 0.0f;
    }

    last_log_time = HAL_GetTick();
}

void Profiler_StartTask(profiler_task_id_t task_id) {
    if(task_id >= PROFILE_MAX_TASKS) return;

    system_tasks[task_id].start_cycles = DWT_GetCycles();
}

void Profiler_StopTask(profiler_task_id_t task_id) {
    if(task_id >= PROFILE_MAX_TASKS) return;

    uint32_t stop_cycles = DWT_GetCycles();
    uint32_t elapsed_cycles = stop_cycles - system_tasks[task_id].start_cycles;
    float elapsed_ms = ((float)elapsed_cycles / (float)SystemCoreClock) * 1000.0f;

    system_tasks[task_id].current_time_ms = elapsed_ms;

    if(elapsed_ms > system_tasks[task_id].max_time_ms) {
        system_tasks[task_id].max_time_ms = elapsed_ms;
    }
}

void Profiler_LogResults(uint32_t interval_ms) {
	uint32_t current_time = HAL_GetTick();
	if(current_time - last_log_time < interval_ms) return;
	last_log_time = current_time;

    const char* task_names[PROFILE_MAX_TASKS] = {
        "BARO",
        "IMU",
		"HIGHG",
        "GPS",
        "Kalman",
        "RFD",
		"BLE"
    };

    printf("P_LOG: ");
	for(int i = 0; i < PROFILE_MAX_TASKS; i++) {
		printf("%s:%.1f(%.1f) | ",
			   task_names[i],
			   system_tasks[i].current_time_ms,
			   system_tasks[i].max_time_ms);
	}
	printf("\n");
}
