/*
 * config.c
 *
 *  Created on: 19 mai 2026
 *      Author: gagno
 */


#include "App/config.h"


#if (ROCKET_ROLE != ROCKET_ROLE_BOOSTER) && \
    (ROCKET_ROLE != ROCKET_ROLE_SUSTAINER)

#error "Invalid ROCKET_ROLE configuration"

#endif

const rocket_config_t rocket_config = {
    .gps_profile = CONFIG_GPS_PROFILE,
    .modem_id    = CONFIG_MODEM_ID,
};
