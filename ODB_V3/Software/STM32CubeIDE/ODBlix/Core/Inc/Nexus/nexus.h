/*
 * app.h
 *
 *  Created on: 16 avr. 2026
 *      Author: gagno
 */

#ifndef SRC_NEXUS_NEXUS_H_
#define SRC_NEXUS_NEXUS_H_

#include "GAUL_Drivers/hm11.h"
#include <stdint.h>
#include <stdbool.h>

#define ODB_BLE_NAME "ODB_1"
#define ODB_BLE_FRAME_VERSION "1.0"
#define NEXUS_DEFAULT_PERIOD_MS   100U

typedef struct {
    bool        is_enabled;
    uint32_t    period_ms;
    uint32_t    last_ms;
    // ...
} nexus_t;

#endif /* SRC_NEXUS_NEXUS_H_ */
