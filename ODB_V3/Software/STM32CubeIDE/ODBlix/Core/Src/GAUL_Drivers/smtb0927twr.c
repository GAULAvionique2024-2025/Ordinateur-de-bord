/*
 * SMTB0927TWR.c
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */


#include <GAUL_Drivers/smtb0927twr.h>

static const buzzer_parametres_t buzzParams[] = {
    // nbBips, freqStart, freqEnd, delayModulation, delayPause
    { 2,  2000, 1900, 10,   300  }, 	// STOP
    { 10, 2700, 2600, 15,   300  }, 	// START
    { 1,  2700, 2300, 100,  1000 }, 	// PENDING
    { 3,  2700, 2300, 100,  1000 }, 	// ARMED
    { 1,  2700, 2690, 3000, 10   }  	// CRASH
};


static void Buzzer_SetFreq(TIM_HandleTypeDef *htim, uint32_t channel, int freq) {
    if(freq <= 0) {
        HAL_TIM_PWM_Stop(htim, channel);
        return;
    }

    // <= 2700 Hz
    if(freq > BUZZER_MAX_FREQ) freq = BUZZER_MAX_FREQ;

    uint32_t arr = (TIMER_CLK_HZ / freq) - 1;
    __HAL_TIM_SET_AUTORELOAD(htim, arr);
    // 50% duty cycle
    __HAL_TIM_SET_COMPARE(htim, channel, (arr + 1) / 2);
}

void Buzzer_SetRoutine(buzzer_t *dev, buzzer_routines_t routine) {
    const buzzer_parametres_t parameters = buzzParams[routine];

    for(uint8_t bip = 0; bip < parameters.nbBips; bip++) {

        if(parameters.frequencyStart != parameters.frequencyEnd) {
            int step = (parameters.frequencyEnd > parameters.frequencyStart) ? 10 : -10;
            int currentFreq = parameters.frequencyStart;
            int targetFreq = parameters.frequencyEnd;
            int finished = 0;

            while(!finished) {
                Buzzer_SetFreq(dev->htim, dev->channel, currentFreq);
                HAL_TIM_PWM_Start(dev->htim, dev->channel);
                HAL_Delay(parameters.delayModulation);

                if(currentFreq == targetFreq) {
                    finished = 1;
                } else {
                    currentFreq += step;

                    if((step > 0 && currentFreq > targetFreq) || (step < 0 && currentFreq < targetFreq)) {
                        currentFreq = targetFreq;
                    }
                }
            }
        } else {
            Buzzer_SetFreq(dev->htim, dev->channel, parameters.frequencyStart);
            HAL_TIM_PWM_Start(dev->htim, dev->channel);
            HAL_Delay(parameters.delayModulation);
        }

        HAL_TIM_PWM_Stop(dev->htim, dev->channel);

        if(bip < (parameters.nbBips - 1)) {
            HAL_Delay(parameters.delayPause);
        }
    }
}
