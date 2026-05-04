/*
 * kalman_nav.c
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */


#include "GAUL_Drivers/LowLevel/kalman_nav.h"
#include "stm32f4xx_hal.h"


void KalmanNav_Init(kalman_nav_t *dev, float mean_alt, float *samples, uint8_t sample_count) {
    dev->z = mean_alt;
    dev->v = 0.0;
    dev->a_bias = 0.0;
    // Calculate variance of the altitude measurements for initial noise estimation
    float variance = 0;
    for(int i = 0; i < sample_count; i++) {
        variance += (samples[i] - mean_alt) * (samples[i] - mean_alt);
    }
    variance /= sample_count;

    // Init covariance matrix P
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            dev->P[i][j] = 0.0;
        }
    }
    dev->P[0][0] = variance;    // Initial uncertainty in altitude
    dev->P[1][1] = 10.0;        // Initial uncertainty in speed
    dev->P[2][2] = 1.0;         // Initial uncertainty in acceleration bias

    dev->Q_accel    = Q_ACCEL;
    dev->Q_bias     = Q_BIAS;
    dev->R_static   = variance;
    dev->R_alt      = variance;
    dev->last_tick  = HAL_GetTick();
}

/*
 * acc_world_z should be the vertical acceleration in the world frame (after removing gravity and transforming from body frame using IMU gyro/mag data)
*/
void KalmanNav_Predict(kalman_nav_t *dev, double acc_world_z) {
    uint32_t now = HAL_GetTick();
    double dt = (double)(now - dev->last_tick) / 1000.0;
    if(dt <= 0) return;
    dev->last_tick = now;

    // State prediction
    double a = acc_world_z - dev->a_bias;
    dev->z += dev->v * dt + 0.5 * a * dt * dt;
    dev->v += a * dt;

    // Covariance update P = FPF' + Q
    double p00 = dev->P[0][0], p01 = dev->P[0][1], p02 = dev->P[0][2];
    double p10 = dev->P[1][0], p11 = dev->P[1][1], p12 = dev->P[1][2];
    double p20 = dev->P[2][0], p21 = dev->P[2][1], p22 = dev->P[2][2];

    dev->P[0][0] = p00 + dt*(p10 + p01 + dt*p11) + dev->Q_accel * 0.25 * dt*dt*dt*dt;
    dev->P[0][1] = p01 + dt*p11;
    dev->P[0][2] = p02 + dt*p12;
    dev->P[1][0] = p10 + dt*p11;
    dev->P[1][1] = p11 + dev->Q_accel * dt * dt;
    dev->P[1][2] = p12;
    dev->P[2][0] = p20 + dt*p21;
    dev->P[2][1] = p21;
    dev->P[2][2] = p22 + dev->Q_bias * dt;
}

void KalmanNav_Update(kalman_nav_t *dev, double measured_alt) {
    // Dynamic Measurement Noise Covariance (R_alt) & Mach Lock Override
    if(dev->v > MACH_LOCK_VELOCITY || dev->z >= ALT_90K_FT) {
        // If we're above Mach lock velocity or above 90k ft, we consider the altitude measurement by barometer to be unreliable and increase R_alt to reduce its influence on the state update
        dev->R_alt = R_PENALTY;
    } else if(dev->z >= ALT_60K_FT) {
        // If we're above 60k ft, we consider the altitude measurement by barometer to be less reliable and increase R_alt moderately
        dev->R_alt = dev->R_static * 10.0;
    } else {
        // Below 60k ft, we consider the altitude measurement by barometer to be reliable and use the static R value based on initialization
        dev->R_alt = dev->R_static;
    }

    // Innovation
    double y = measured_alt - dev->z;

    // Kalman scale K = PH' (HPH' + R)^-1
    double S = dev->P[0][0] + dev->R_alt;
    double K[3];
    K[0] = dev->P[0][0] / S;
    K[1] = dev->P[1][0] / S;
    K[2] = dev->P[2][0] / S;

    // State update
    dev->z += K[0] * y;
    dev->v += K[1] * y;
    dev->a_bias += K[2] * y;

    // Covariance update P = (I - KH)P
    double p00 = dev->P[0][0], p01 = dev->P[0][1], p02 = dev->P[0][2];
    dev->P[0][0] -= K[0] * p00; dev->P[0][1] -= K[0] * p01; dev->P[0][2] -= K[0] * p02;
    dev->P[1][0] -= K[1] * p00; dev->P[1][1] -= K[1] * p01; dev->P[1][2] -= K[1] * p02;
    dev->P[2][0] -= K[2] * p00; dev->P[2][1] -= K[2] * p01; dev->P[2][2] -= K[2] * p02;
}
