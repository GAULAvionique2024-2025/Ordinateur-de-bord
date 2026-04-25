/*
 * flight_fsm.h
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */

#ifndef INC_APP_FLIGHT_FSM_H_
#define INC_APP_FLIGHT_FSM_H_

#include "GAUL_Drivers/utils.h"


typedef enum { 
    STATE_INIT, 
    STATE_PREFLIGHT, 
    STATE_INFLIGHT, 
    STATE_POSTFLIGHT
} global_state_t;

typedef enum {
    SUB_PAD,
    SUB_BOOST, 
    SUB_FAST, 
    SUB_COAST, 
    SUB_DROGUE, 
    SUB_MAIN, 
    SUB_LANDED 
} inflight_sub_t;

void FSM_Update(odb_data *data);
global_state_t FSM_GetGlobalState(void);

#endif /* INC_APP_FLIGHT_FSM_H_ */
