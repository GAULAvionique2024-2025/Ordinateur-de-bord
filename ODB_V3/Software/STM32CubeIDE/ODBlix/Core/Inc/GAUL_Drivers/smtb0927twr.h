/*
 * SMTB0927TWR.h
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_SMTB0927TWR_H_
#define INC_GAUL_DRIVERS_SMTB0927TWR_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>


#define BUZZER_MAX_FREQ 2700
#define TIMER_CLK_HZ    1000000

typedef enum {
    STOP,
    START,
    PENDING,
    ARMED,
    CRASH,
} buzzer_routines_t;

typedef struct {
    uint8_t   nbBips;
    int       frequencyStart;
    int       frequencyEnd;
    uint32_t  delayModulation;
    uint32_t  delayPause;
} buzzer_parametres_t;

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t channel;
    buzzer_routines_t routine;
} buzzer_t;

void Buzzer_SetRoutine(buzzer_t *dev, buzzer_routines_t routine);

#endif /* INC_GAUL_DRIVERS_SMTB0927TWR_H_ */
