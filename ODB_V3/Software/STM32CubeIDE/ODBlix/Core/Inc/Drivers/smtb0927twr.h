/*
 * SMTB0927TWR.h
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_DRIVERS_SMTB0927TWR_H_
#define INC_DRIVERS_SMTB0927TWR_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>


#define BUZZER_MAX_FREQ 2700


typedef enum {
    STOP,
    START,
    PENDING,
    ARMED,
    CRASH,
} buzzer_routines_t;

typedef struct {
    uint8_t   nbBips;
    uint16_t  frequencyStart;
    uint16_t  frequencyEnd;
    uint32_t  delayModulation;
    uint32_t  delayPause;
} buzzer_parametres_t;

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t channel;
} buzzer_t;

void Buzzer_RunRoutine(buzzer_t *dev, buzzer_routines_t routine);
void Buzzer_ReportStatus(buzzer_t *dev, uint16_t freq, uint16_t battery_dv, bool pyros_continuity[4], uint8_t global_state);

#endif /* INC_DRIVERS_SMTB0927TWR_H_ */
