/*
 * utils.h
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_UTILS_H_
#define INC_GAUL_DRIVERS_UTILS_H_


#include <GAUL_Drivers/bno055.h>
#include <GAUL_Drivers/ms5611.h>
#include "stm32f4xx_hal.h"
#include "adxl382.h"
#include "l76lm33.h"
#include "ltste682krkgwt.h"
#include "pyros.h"
#include "rfd900x.h"
#include "smtb0927twr.h"
#include "LowLevel/measure.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>


typedef  struct  {
	uint32_t  	start_time;
	uint32_t  	elapsed_time_ms;
	uint8_t		elapsed_time_s;
	uint16_t  	elapsed_time_m;
	uint16_t  	elapsed_time_remaining_ms;
}  runTimer_t;



void RunTimer_Init(runTimer_t *dev);
void RunTimer_Update(runTimer_t *dev);
int printt(const char *format, ...);
int _write(int le, char *ptr, int len);

#endif /* INC_GAUL_DRIVERS_UTILS_H_ */
