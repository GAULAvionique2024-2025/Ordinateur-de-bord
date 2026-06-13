/*
 * app_comm.h
 *
 *  Created on: 6 juin 2026
 *      Author: gagno
 */

#ifndef INC_APP_APP_COMM_H_
#define INC_APP_APP_COMM_H_


#include <odb.h>
#include "Drivers/hm11.h"
#include "Systems/config.h"


#define APP_SYNC_1 0xAB
#define APP_SYNC_2 0xCD

typedef enum {
    MSG_TELEMETRY  = 0x01,
    MSG_CONFIG_SET = 0x02,
    MSG_CMD        = 0x03,
    MSG_ACK        = 0x04
} app_msg_type_t;

typedef enum {
    CMD_PING       = 0x01,
    CMD_ARM_DISARM = 0x02,
    CMD_FIRE_PYRO  = 0x03,
    CMD_APPLY_CFG  = 0x04,
    CMD_RESET_CFG  = 0x05,
    CMD_REQ_CFG    = 0x06,
	CMD_RESET_MEM  = 0x07
} app_cmd_id_t;

void AppComm_SendTelemetry(hm11_t *hm11_dev, const odb_data_t *data);
void AppComm_ProcessRx(hm11_t *hm11_dev);

#endif /* INC_APP_APP_COMM_H_ */
