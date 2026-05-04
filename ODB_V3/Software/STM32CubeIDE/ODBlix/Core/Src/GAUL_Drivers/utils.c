/*
 * utils.c
 *
 *  Created on: 2 mars 2026
 *      Author: gagno
 */

#include "stm32f4xx_hal.h"
#include "GAUL_Drivers/utils.h"

#include <stdlib.h>
#include <stdarg.h>
#include <math.h>


/* === Offset === */
float Thermal_ComputeOffset(coeff_poly3_t coeffs, float temp) {
    // Horner's method for polynomial evaluation
    return coeffs.c0 + temp * (coeffs.c1 + temp * (coeffs.c2 + temp * coeffs.c3));
}

float Math_ComputeWorldVerticalAcc(const float accel[3], const float quat[4], bool gravity_compensation) {
    float qw = quat[0], qx = quat[1], qy = quat[2], qz = quat[3];
    float ax = accel[0], ay = accel[1], az = accel[2];

    // Check normalization of quaternion to avoid errors in computation (qw^2 + qx^2 + qy^2 + qz^2 = 1)
    float norm_sq = qw * qw + qx * qx + qy * qy + qz * qz;
    if(fabsf(norm_sq - 1.0f) > 0.01f) {
        float inv_norm = 1.0f / sqrtf(norm_sq);
        qw *= inv_norm;
        qx *= inv_norm;
        qy *= inv_norm;
        qz *= inv_norm;
    }

    // Rotate accelerometer vector to world frame and extract vertical component
    float a_world_z = (2.0f * (qx * qz - qw * qy)) * ax + (2.0f * (qy * qz + qw * qx)) * ay + (qw * qw - qx * qx - qy * qy + qz * qz) * az;
    if(gravity_compensation) {
        a_world_z -= GRAVITY_MS2;
    }

    return a_world_z;
}
/* =========== */


/* === HELPERS === */
runTimer_t run_timer;

void RunTimer_Init(runTimer_t* dev) {
	  dev->start_time = HAL_GetTick();
	  dev->elapsed_time_ms = 0;
	  dev->elapsed_time_s = 0;
	  dev->elapsed_time_m = 0;
	  dev->elapsed_time_remaining_ms = 0;
}

void RunTimer_Update(runTimer_t *dev) {
	dev->elapsed_time_ms = HAL_GetTick() - dev->start_time;

	dev->elapsed_time_s = (dev->elapsed_time_ms / 1000) % 60;
	dev->elapsed_time_m = (dev->elapsed_time_ms / 60000);
	dev->elapsed_time_remaining_ms = dev->elapsed_time_ms % 1000;
}

int printt(const char *format, ...) {
    va_list args;
    va_start(args, format);
    RunTimer_Update(&run_timer);
    printf("[%03d:%02d:%03d] ",run_timer.elapsed_time_m, run_timer.elapsed_time_s, run_timer.elapsed_time_remaining_ms);

    int ret = vprintf(format, args);

    va_end(args);

    return ret;
}

int _write(int le, char *ptr, int len) {
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        ITM_SendChar(*ptr++);
    }

    return len;
}
/* =========== */
