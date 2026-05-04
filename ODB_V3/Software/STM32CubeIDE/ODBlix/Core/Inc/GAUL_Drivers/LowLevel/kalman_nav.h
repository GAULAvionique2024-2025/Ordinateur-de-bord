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

// Altitude thresholds for Mach lock
#define ALT_60K_FT 18288.0
#define ALT_90K_FT 27432.0

#define R_PENALTY           1000.0f
#define MACH_LOCK_VELOCITY  200.0f

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
    double R_static; // Covariance noise of the measurement on the ground (initialization)
    double R_alt;    // Dynamic covariance noise of the measurement (in flight)
    uint32_t last_tick;
} kalman_nav_t;


void KalmanNav_Init(kalman_nav_t *dev, float mean_alt, float *samples, uint8_t sample_count);

void KalmanNav_Predict(kalman_nav_t *dev, double acc_world_z);
void KalmanNav_Update(kalman_nav_t *dev, double measured_alt);

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_KALMAN_NAV_H_ */
