/*
 * KalmanFilter.c
 *
 *  Created on: Feb 18, 2024
 *      Author: Luka
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_LOWLEVEL_KALMAN_FILTER_H_
#define INC_GAUL_DRIVERS_LOWLEVEL_KALMAN_FILTER_H_

#include "stm32f4xx_hal.h"

#define Q_ANGLE 	0.001f
#define Q_BIAS  	0.003f
#define R_MEASURE  	0.03f

typedef struct {
	double Q_angle;
	double Q_bias;
	double R_measure;

	double angle;
	double bias;
	double rate;

    double S, y;
    double dt,kt;

    double P[2][2];
    double K[2];
} kalman_filter_t;

void KalmanFilter_Init(kalman_filter_t *dev);
double KalmanFilter_Update(kalman_filter_t *dev, double newAngle, double newRate);

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_KALMAN_FILTER_H_ */

