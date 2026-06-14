/*
 * app_comm.c
 *
 *  Created on: 6 juin 2026
 *      Author: gagno
 */


#include "Comm/app_comm.h"
#include "Comm/beacon_comm.h"
#include "Utils/reboot_manager.h"
#include <string.h>

extern w25q_t w25q;
extern critical_led_t critical_led;
extern system_measurements_t system_measurements;
extern idefix_t idefix;
extern pyro_t pyros[4];
extern bool is_pyros_armed;

static void AppComm_SendFrame(hm11_t *hm11_dev, app_msg_type_t type, const uint8_t *payload, uint8_t len) {
    if(!hm11_dev || !hm11_dev->is_connected) return;

    static uint8_t buffer[256];

    buffer[0] = APP_SYNC_1;
    buffer[1] = APP_SYNC_2;
    buffer[2] = (uint8_t)type;
    buffer[3] = len;

    uint8_t checksum = buffer[2] ^ buffer[3];
    for(uint8_t i = 0; i < len; i++) {
        buffer[4 + i] = payload[i];
        checksum ^= payload[i];
    }
    buffer[4 + len] = checksum;

    HM11_SendData(hm11_dev, buffer, len + 5);
}

void AppComm_SendTelemetry(hm11_t *hm11_dev, const odb_data_t *data) {
    AppComm_SendFrame(hm11_dev, MSG_TELEMETRY, (const uint8_t*)data, sizeof(odb_data_t));
}

static void AppComm_SendAck(hm11_t *hm11_dev, app_cmd_id_t cmd, uint8_t status) {
    uint8_t payload[2] = { (uint8_t)cmd, status };
    AppComm_SendFrame(hm11_dev, MSG_ACK, payload, 2);
}

void AppComm_ProcessRx(hm11_t *hm11_dev) {
    if(!hm11_dev) return;

    static uint8_t rx_buf[256];
    static uint8_t rx_idx = 0;
    static uint8_t expected_len = 0;
    static uint32_t last_rx_time = 0;

    static const char str_conn[] = "OK+CONN";
    static const char str_lost[] = "OK+LOST";
    static uint8_t match_conn = 0;
    static uint8_t match_lost = 0;

    uint8_t byte;
    while(RingBuffer_Dequeue(&hm11_dev->rx_ring, &byte)) {
        uint32_t now = HAL_GetTick();
        if(rx_idx > 0 && (now - last_rx_time) > 500) {
            rx_idx = 0;
        }
        last_rx_time = now;

        if(byte == str_conn[match_conn]) {
            match_conn++;
            if(match_conn == 7) {
            	hm11_dev->is_connected = true; match_conn = 0;
            }
        } else {
        	match_conn = (byte == str_conn[0]) ? 1 : 0;
        }

        if(byte == str_lost[match_lost]) {
            match_lost++;
            if(match_lost == 7) {
            	hm11_dev->is_connected = false; match_lost = 0;
            }
        } else {
        	match_lost = (byte == str_lost[0]) ? 1 : 0;
        }

        rx_buf[rx_idx++] = byte;

        if(rx_idx == 1 && rx_buf[0] != APP_SYNC_1) {
            rx_idx = 0;
        } else if(rx_idx == 2 && rx_buf[1] != APP_SYNC_2) {
            rx_idx = 0;
        } else if(rx_idx == 4) {
            expected_len = rx_buf[3];
        } else if(rx_idx >= 5 && rx_idx == expected_len + 5) {
            uint8_t calc_crc = rx_buf[2] ^ rx_buf[3];
            for(uint8_t i = 0; i < expected_len; i++) {
                calc_crc ^= rx_buf[4 + i];
            }

            uint8_t recv_crc = rx_buf[rx_idx - 1];
            app_msg_type_t type = (app_msg_type_t)rx_buf[2];
            uint8_t *payload = &rx_buf[4];

            if(calc_crc == recv_crc || (type == MSG_GENERIC_DATA && expected_len == CONFIG_DATA_SIZE)) {
				CriticalLED_SetColor(&critical_led, GREEN);

				if(type == MSG_GENERIC_DATA) {
					if(expected_len == CONFIG_DATA_SIZE) {
						memcpy(&current_config, payload, expected_len);
						AppComm_SendAck(hm11_dev, CMD_REQ_CFG, (calc_crc == recv_crc) ? 1 : 2);
					}
				} else if (type == MSG_CMD) {
                    app_cmd_id_t cmd = (app_cmd_id_t)payload[0];
                    if(cmd == CMD_PING) {
                        AppComm_SendAck(hm11_dev, CMD_PING, 1);
                    } else if(cmd == CMD_ARM_DISARM) {
                        bool arm = payload[1] == 1;
                        is_pyros_armed = Pyro_Arming(&system_measurements, arm);
                        AppComm_SendAck(hm11_dev, CMD_ARM_DISARM, is_pyros_armed == arm ? 1 : 0);
                    } else if(cmd == CMD_FIRE_PYRO) {
                        uint8_t pyro_idx = payload[1];
                        if(is_pyros_armed && pyro_idx < PYRO_MAX) {
                            Pyro_Fire(&pyros[pyro_idx], &system_measurements);
                            AppComm_SendAck(hm11_dev, CMD_FIRE_PYRO, 1);
                        } else {
                            AppComm_SendAck(hm11_dev, CMD_FIRE_PYRO, 0);
                        }
                    } else if(cmd == CMD_APPLY_CFG) {
                        if(Config_SaveToFlash() == 0) {
                        	Beacon_SetFrequency(&idefix);

                            AppComm_SendAck(hm11_dev, CMD_APPLY_CFG, 1);
                            RebootManager_RequestReboot();
                        } else {
                            AppComm_SendAck(hm11_dev, CMD_APPLY_CFG, 0);
                        }
                    } else if(cmd == CMD_RESET_CFG) {
                        Config_LoadDefaults();
                        if(Config_SaveToFlash() == 0) {
                        	CriticalLED_SetColor(&critical_led, RED);
                            AppComm_SendAck(hm11_dev, CMD_RESET_CFG, 1);
                            RebootManager_RequestReboot();
                        } else {
                            AppComm_SendAck(hm11_dev, CMD_RESET_CFG, 0);
                        }
                    } else if(cmd == CMD_RESET_MEM) {
                    	while(W25Q_EraseChip(&w25q) != 0) {}
                    	Config_LoadDefaults();
                    	if(Config_SaveToFlash() == 0) {
                    		CriticalLED_SetColor(&critical_led, RED);
							AppComm_SendAck(hm11_dev, CMD_RESET_MEM, 1);
							RebootManager_RequestReboot();
                    	} else {
                    		AppComm_SendAck(hm11_dev, CMD_RESET_MEM, 0);
                    	}
                    } else if(cmd == CMD_REQ_CFG) {
                        const odb_config_t *actual_config = Config_Get();
                        AppComm_SendFrame(hm11_dev, MSG_GENERIC_DATA, (uint8_t*)actual_config, CONFIG_DATA_SIZE);
                    } else if (cmd == CMD_REQ_EVENTS) {
						const odb_stats_t *last_flight_stats = Logger_GetLastFlightStats();
						if (last_flight_stats != NULL) {
							AppComm_SendFrame(hm11_dev, MSG_GENERIC_DATA, (uint8_t*)last_flight_stats, ODB_STATS_SIZE);
						} else {
							AppComm_SendAck(hm11_dev, CMD_REQ_EVENTS, 0);
						}
					}
                }
                CriticalLED_SetColor(&critical_led, NONE);
            } else {
                AppComm_SendAck(hm11_dev, (app_cmd_id_t)0xFF, calc_crc);
            }
            rx_idx = 0;
        }

        if(rx_idx == 255) rx_idx = 0;
    }
}
