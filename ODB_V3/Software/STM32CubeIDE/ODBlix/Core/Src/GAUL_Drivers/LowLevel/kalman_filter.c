/*
 * KalmanFilter.c
 *
 *  Created on: Feb 18, 2024
 *      Author: Luka
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#include <GAUL_Drivers/LowLevel/kalman_filter.h>


void KalmanFilter_Init(kalman_filter_t *dev) {
    dev->Q_angle = Q_ANGLE;
    dev->Q_bias = Q_BIAS;
    dev->R_measure = R_MEASURE;

    dev->angle = 0.0f;
    dev->bias = 0.0f;

    dev->dt = 0;
    dev->kt = HAL_GetTick();

    dev->P[0][0] = 0.0f;
    dev->P[0][1] = 0.0f;
    dev->P[1][0] = 0.0f;
    dev->P[1][1] = 0.0f;
}

double KalmanFilter_Update(kalman_filter_t *dev, double newAngle, double newRate) {
    dev->dt = (double)(HAL_GetTick() - dev->kt) / 1000.0;

    dev->rate = newRate - dev->bias;
    dev->angle += dev->dt * dev->rate;

    dev->P[0][0] += dev->dt * (dev->P[1][1] + dev->P[0][1]) + dev->Q_angle * dev->dt;
    dev->P[0][1] -= dev->dt * dev->P[1][1];
    dev->P[1][0] -= dev->dt * dev->P[1][1];
    dev->P[1][1] += dev->Q_bias * dev->dt;

    dev->S = dev->P[0][0] + dev->R_measure;

    dev->K[0] = dev->P[0][0] / dev->S;
    dev->K[1] = dev->P[1][0] / dev->S;

    dev->y = newAngle - dev->angle;

    while (dev->y > 180.0) { dev->y -= 360.0; }
    while (dev->y < -180.0) { dev->y += 360.0; }

    dev->angle += dev->K[0] * dev->y;
    dev->bias += dev->K[1] * dev->y;

    while (dev->angle > 180.0) { dev->angle -= 360.0; }
    while (dev->angle < -180.0) { dev->angle += 360.0; }

    dev->P[0][0] -= dev->K[0] * dev->P[0][0];
    dev->P[0][1] -= dev->K[0] * dev->P[0][1];
    dev->P[1][0] -= dev->K[1] * dev->P[0][0];
    dev->P[1][1] -= dev->K[1] * dev->P[0][1];

    dev->kt = HAL_GetTick();

    return dev->angle;
}

