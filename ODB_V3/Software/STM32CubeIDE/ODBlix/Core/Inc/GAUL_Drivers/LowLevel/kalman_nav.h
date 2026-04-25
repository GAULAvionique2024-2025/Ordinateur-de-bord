/*
 * kalman_nav.h
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */

#ifndef INC_GAUL_DRIVERS_LOWLEVEL_KALMAN_NAV_H_
#define INC_GAUL_DRIVERS_LOWLEVEL_KALMAN_NAV_H_

#include <stdint.h>
#include <stdbool.h>


#define Q_ACCEL 0.02 // Process noise for acceleration (tune based on IMU/HIGHG noise)
#define Q_BIAS 0.001 // Process noise for bias (tune based on IMU/HIGHG noise)


typedef struct {
    // State variables
    double z;      // Altitude (m)
    double v;      // Speed (m/s)
    double a_bias; // Acceleration bias (m/s^2)

    // Covariance matrix of the error
    double P[3][3];
    // Noise parameters
    double Q_accel;  // Process noise (acceleration)
    double Q_bias;   // Bias drift
    double R_alt;    // Measurement noise (Baro/GPS)
    uint32_t last_tick;
} kalman_nav_t;


void KalmanNav_Init(kalman_nav_t *dev, float mean_alt, float *samples, uint8_t sample_count);

void KalmanNav_Predict(kalman_nav_t *dev, double acc_world_z);
void KalmanNav_Update(kalman_nav_t *dev, double measured_alt, double current_R, bool is_machlock);

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_KALMAN_NAV_H_ */
