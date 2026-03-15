/*
 * utils.h
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_UTILS_H_
#define INC_GAUL_DRIVERS_UTILS_H_


#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;


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
