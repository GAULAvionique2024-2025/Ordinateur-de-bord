/*
 * telemetry.c
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#include <GAUL_Drivers/LowLevel/measure.h>


#define DIV_ADC_STEP 0.00080586	// 3.3 / 4095

extern uint16_t adc_buffer[9];


void System_Measurements_Update(system_measurements_t *data) {
	data->vin_batt = (adc_buffer[0] * DIV_ADC_STEP) * DIV_RATIO_VIN_BATT;
	data->V5_buck  = (adc_buffer[1] * DIV_ADC_STEP) * DIV_RATIO_V5_BUCK;
	data->V3_buck  = (adc_buffer[2] * DIV_ADC_STEP) * DIV_RATIO_V3_BUCK;

    float v_temp = adc_buffer[3] * DIV_ADC_STEP;
    data->temperature = (((v_temp - 0.40f) / 0.01953f) - 0.40f) / 0.01953f;

    //float arming_continuity = adc_buffer[4] * DIV_ADC_STEP;
    //printf("pyros_arming continuity: %f\n", arming_continuity);
    if(adc_buffer[4] >= PYROS_THRESHOLD) {
		data->pyros_arming = false;
	} else {
		data->pyros_arming = true;
	}

    for(int8_t i = 0; i < 4; i++) {
    	// TODO: remove voltage conversion (not necessary -> use adc output directly)
        //float pyro_continuity = adc_buffer[5 + i] * DIV_ADC_STEP;
        //printf("pyro[%i] continuity: %f\n", i, pyro_continuity);
        if(adc_buffer[5 + i] >= PYROS_THRESHOLD) {
        	data->pyro_status[i] = false;
        } else {
        	data->pyro_status[i] = true;
        }
    }
}
