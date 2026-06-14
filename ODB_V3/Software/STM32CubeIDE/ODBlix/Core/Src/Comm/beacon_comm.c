/*
 * beacon_comm.c
 *
 *  Created on: 14 juin 2026
 *      Author: gagno
 */

#include "Comm/beacon_comm.h"
#include "Systems/config.h"


void Beacon_SendCoordinates(idefix_t *idefix_dev, const int32_t lat_e7, const int32_t lon_e7) {
    if(!idefix_dev) return;

    if(Idefix_SendCommand(idefix_dev, IDEFIX_CMD_SET_COORD) != IDEFIX_OK) {
        return;
    }

    uint8_t payload[8];
    payload[0] = (uint8_t)(lat_e7 & 0xFF);
    payload[1] = (uint8_t)((lat_e7 >> 8) & 0xFF);
    payload[2] = (uint8_t)((lat_e7 >> 16) & 0xFF);
    payload[3] = (uint8_t)((lat_e7 >> 24) & 0xFF);

    payload[4] = (uint8_t)(lon_e7 & 0xFF);
    payload[5] = (uint8_t)((lon_e7 >> 8) & 0xFF);
    payload[6] = (uint8_t)((lon_e7 >> 16) & 0xFF);
    payload[7] = (uint8_t)((lon_e7 >> 24) & 0xFF);

    Idefix_SendData(idefix_dev, payload, 8);
}

void Beacon_SetFrequency(idefix_t *idefix_dev) {
	if(Idefix_SendCommand(idefix_dev, IDEFIX_CMD_SET_FREQ) != IDEFIX_OK) {
		return;
    }

    uint8_t payload[4];
    payload[0] = (uint8_t)(current_config.idefix_frequency_hz & 0xFF);
    payload[1] = (uint8_t)((current_config.idefix_frequency_hz >> 8) & 0xFF);
    payload[2] = (uint8_t)((current_config.idefix_frequency_hz >> 16) & 0xFF);
    payload[3] = (uint8_t)((current_config.idefix_frequency_hz >> 24) & 0xFF);

    Idefix_SendData(idefix_dev, payload, 4);
}
