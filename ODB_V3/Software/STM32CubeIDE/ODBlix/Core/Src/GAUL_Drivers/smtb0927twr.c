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


static void Buzzer_SetFreq(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t freq) {
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

static void Buzzer_PlayTone(buzzer_t *dev, uint16_t freq, uint32_t duration) {
    Buzzer_SetFreq(dev->htim, dev->channel, freq);
    HAL_TIM_PWM_Start(dev->htim, dev->channel);
    HAL_Delay(duration);
}

static void Buzzer_Bip(buzzer_t *dev, uint8_t count, uint32_t on_time, uint32_t off_time, uint16_t freq) {
    for(uint8_t i = 0; i < count; i++) {
        Buzzer_PlayTone(dev, freq, on_time);
        HAL_TIM_PWM_Stop(dev->htim, dev->channel);

        if(i < count - 1) {
            HAL_Delay(off_time);
        }
    }
}

static void Buzzer_Pause(uint32_t ms) {
    HAL_Delay(ms);
}

void Buzzer_RunRoutine(buzzer_t *dev, buzzer_routines_t routine) {
    if(routine >= (sizeof(buzzParams)/sizeof(buzzParams[0]))) {
        return;
    }
    const buzzer_parametres_t parameters = buzzParams[routine];
    for(uint8_t bip = 0; bip < parameters.nbBips; bip++) {
        if(parameters.frequencyStart != parameters.frequencyEnd) {
            int step = (parameters.frequencyEnd > parameters.frequencyStart) ? 10 : -10;
            int currentFreq = parameters.frequencyStart;
            int targetFreq = parameters.frequencyEnd;

            int finished = 0;
            while(!finished) {
                Buzzer_PlayTone(dev, currentFreq, parameters.delayModulation);

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
            Buzzer_PlayTone(dev, parameters.frequencyStart, parameters.delayModulation);
        }

        HAL_TIM_PWM_Stop(dev->htim, dev->channel);

        if(bip < (parameters.nbBips - 1)) {
            HAL_Delay(parameters.delayPause);
        }
    }
}

// TODO: add latest max altitude reached (in FLASH)
/*
 * Exemple of status report:
 * Battery: 12.0V (1200mV) -> bip / pause / bip bip / pause / bip bip bip bip bip bip bip bip bip bip x2 (10 bips = 0)
 * Pause 1s
 * Pyros [true, true, false, false] -> bip bip / pause / bip bip / pause / bip / pause / bip
 * Pause 1s
 * Global state [OK]: bip
 * Pause 1s
 * Start Bip -> 5s biiiiip...
*/
void Buzzer_ReportStatus(buzzer_t *dev, uint16_t freq, uint16_t battery_dv, bool pyros_continuity[4], uint8_t global_state) {
    // Battery voltage
    uint16_t temp = battery_dv;
    uint8_t digits[5];
    uint8_t len = 0;

    do {
        digits[len++] = temp % 10;
        temp /= 10;
    } while(temp > 0);

    for(int i = len - 1; i >= 0; i--) {
        uint8_t d = digits[i];
        uint8_t bipCount = (d == 0) ? 10 : d;

        Buzzer_Bip(dev, bipCount, 250, 250, freq);
        if(i > 0) Buzzer_Pause(1000);
    }

    Buzzer_Pause(3000);

    // Pyros continuity
    for(int i = 0; i < 4; i++) {
        if(pyros_continuity[i]) {
            Buzzer_Bip(dev, 2, 250, 250, freq);
        } else {
            Buzzer_Bip(dev, 1, 250, 250, freq);
        }
        if(i < 3) Buzzer_Pause(1000);
    }

    Buzzer_Pause(3000);

    // Global state
    if(global_state < 1) global_state = 1;
    if(global_state > 8) global_state = 8;

    Buzzer_Bip(dev, global_state, 250, 250, freq);

    Buzzer_Pause(3000);

    // Start Bip
    if(global_state == 1) {
    	Buzzer_Bip(dev, global_state, 5000, 250, freq);
    }
}
