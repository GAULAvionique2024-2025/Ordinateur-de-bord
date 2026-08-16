/*
 * logger.c
 *
 * Created on: 8 mai 2026
 * 		Author: gagno
 */

#include "Systems/logger.h"
#include "Drivers/mem2067.h"
#include "Systems/config.h"
#include <string.h>


typedef enum {
    LOGGER_IDLE,
    LOGGER_START_WRITE,
    LOGGER_WAIT_FLASH_BUSY,
	LOGGER_SD_DUMP_HEADER,
	LOGGER_SD_DUMP_DATA
} logger_state_t;
static logger_state_t logger_state = LOGGER_IDLE;

// FLASH
static logger_data_t buffer_A[LOG_BUFFER_SIZE];
static logger_data_t buffer_B[LOG_BUFFER_SIZE];
static logger_data_t *current_write_buf = buffer_A;
static logger_data_t *current_flush_buf = NULL;

static uint8_t write_index = 0;
static uint32_t flash_current_address = 0;
static uint32_t flush_bytes_written = 0;
static bool flush_pending = false;

static uint32_t last_flight_header_addr = 0; 
static uint32_t last_flight_id = 0;
static uint32_t stats_reserved_address = 0;

static uint32_t previous_flight_header_addr = 0xFFFFFFFF;

static bool is_logging = false;

extern w25q_t w25q;

// SD
static uint32_t sd_dump_cursor = 0;
static bool start_sd_dump = false;
static const odb_stats_t *sd_dump_stats = NULL;


static bool Logger_ReadHeader(uint32_t addr, logger_header_t *header) {
    if(W25Q_Read(&w25q, (uint8_t*)header, addr, sizeof(logger_header_t)) != 0) {
        return false;
    }

    return (header->magic_number == LOGGER_MAGIC_HEADER);
}

static void Logger_ScanFlash(uint32_t *next_free_addr, uint32_t *next_id) {
    logger_header_t header;
    uint32_t addr = 0;
    uint32_t found_id = 0;
    uint32_t found_addr = 0;

    while(addr < LOGGER_MAX_ALLOWED_ADDRESS) {
        if(Logger_ReadHeader(addr, &header)) {
            found_id = header.flight_id;
            found_addr = addr;
            addr += FLASH_SECTOR_SIZE_BYTE;
        } else {
            uint32_t first_word;
            W25Q_Read(&w25q, (uint8_t*)&first_word, addr, 4);
            if(first_word != 0xFFFFFFFF) { // Corrupted header, skip sector
                addr += FLASH_SECTOR_SIZE_BYTE;
            } else {
                break; // Found empty sector, stop scanning
            }
        }
    }
    
    if(found_id > 0) {
    	previous_flight_header_addr = found_addr;
    } else {
    	previous_flight_header_addr = 0xFFFFFFFF;
    }

    last_flight_header_addr = found_addr;
    last_flight_id = found_id;
    *next_id = found_id + 1;

    if(addr % FLASH_SECTOR_SIZE_BYTE != 0) {
        addr = ((addr / FLASH_SECTOR_SIZE_BYTE) + 1) * FLASH_SECTOR_SIZE_BYTE;
    }
    *next_free_addr = addr;
}

int8_t Logger_Init(void) {
    uint32_t next_id = 0;
    Logger_ScanFlash(&flash_current_address, &next_id);

    // Check free space
    if((flash_current_address + LOGGER_MIN_FLIGHT_SPACE) >= LOGGER_MAX_ALLOWED_ADDRESS) {
        flash_current_address = 0;
        next_id = 1;

        Logger_Erase();
    }

    if(W25Q_EraseSector(&w25q, flash_current_address) != 0) {
        return LOGGER_ERROR_FLASH_ERASE;
    }

    logger_header_t header = {
        .magic_number = LOGGER_MAGIC_HEADER,
        .flight_id = next_id,
        .time_date = 0,
        .metadata_rsv = 0
    };

    if(W25Q_WritePage(&w25q, (uint8_t*)&header, flash_current_address, sizeof(logger_header_t)) != 0) {
    	return LOGGER_ERROR_FLASH_WRITE;
    }

    last_flight_header_addr = flash_current_address;
    last_flight_id = next_id;

    flash_current_address += FLASH_SECTOR_SIZE_BYTE;

    W25Q_EraseSector(&w25q, flash_current_address);
    stats_reserved_address = flash_current_address;
    flash_current_address += W25Q512_PAGE_SIZE;

    write_index = 0;
    flush_pending = false;
    current_write_buf = buffer_A;
    current_flush_buf = NULL;
    logger_state = LOGGER_IDLE;

    return LOGGER_SUCCESS;
}

void Logger_PushData(odb_data_t *new_data) {
	if(!is_logging || new_data == NULL) return;

    if(write_index >= LOG_BUFFER_SIZE) {
        if(flush_pending) {
        	// TODO: add error counter "missed frames"
            return;
        } else {
            current_flush_buf = current_write_buf;
            flush_pending = true;

            if(current_write_buf == buffer_A) {
                current_write_buf = buffer_B;
            } else {
                current_write_buf = buffer_A;
            }
            write_index = 0;
        }
    }

    current_write_buf[write_index].magic_number = LOGGER_DATA_MAGIC_NUMBER;
    current_write_buf[write_index].data = *new_data;
    write_index++;

    if(write_index >= LOG_BUFFER_SIZE) {
        if(!flush_pending) {
            current_flush_buf = current_write_buf;
            flush_pending = true;

            if(current_write_buf == buffer_A) {
                current_write_buf = buffer_B;
            } else {
                current_write_buf = buffer_A;
            }
            write_index = 0;
        }
    }
}

void Logger_Task(void) {
    switch(logger_state) {
        case LOGGER_IDLE:
            if(flush_pending) {
                flush_bytes_written = 0;
                logger_state = LOGGER_START_WRITE;
            } else if(start_sd_dump) {
                start_sd_dump = false;
                if(file_is_open && sd_dump_stats != NULL) {
                    logger_state = LOGGER_SD_DUMP_HEADER;
                }
            }
            break;

        case LOGGER_START_WRITE:
        {
            uint32_t total_size = LOG_BUFFER_SIZE * sizeof(logger_data_t);
            uint32_t bytes_to_write = total_size - flush_bytes_written;
            uint32_t space_in_page = W25Q512_PAGE_SIZE - (flash_current_address % W25Q512_PAGE_SIZE);

            if(bytes_to_write > space_in_page) {
                bytes_to_write = space_in_page;
            }

            if((flash_current_address + bytes_to_write) > LOGGER_MAX_ALLOWED_ADDRESS) {
                flush_pending = false;
                current_flush_buf = NULL;
                logger_state = LOGGER_IDLE;
                break;
            }

            uint8_t *write_ptr = ((uint8_t*)current_flush_buf) + flush_bytes_written;
            if(W25Q_WritePageNoWait(&w25q, write_ptr, flash_current_address, bytes_to_write) == 0) {
                flash_current_address += bytes_to_write;
                flush_bytes_written += bytes_to_write;
                logger_state = LOGGER_WAIT_FLASH_BUSY;
            }
            break;
        }

        case LOGGER_WAIT_FLASH_BUSY:
            if(!W25Q_IsBusy(w25q.hqspi)) {
                uint32_t total_size = LOG_BUFFER_SIZE * sizeof(logger_data_t);

                if(flush_bytes_written < total_size) {
                    logger_state = LOGGER_START_WRITE;
                } else {
                    flush_pending = false;
                    current_flush_buf = NULL;
                    logger_state = LOGGER_IDLE;
                }
            }
            break;

        case LOGGER_SD_DUMP_HEADER:
			if(file_is_open) {
				char header_buf[256];

				f_puts("# === FLIGHT STATISTICS ===\n", &active_file);

				sprintf(header_buf, "# Date : %lu | Temps de vol : %lu ms\n", sd_dump_stats->date, sd_dump_stats->flight_time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Last GPS Coordinates : Lat %ld, Lon %ld\n", sd_dump_stats->last_lat, sd_dump_stats->last_lon);
				f_puts(header_buf, &active_file);

				f_puts("# -- Pyros Evenements --\n", &active_file);

				sprintf(header_buf, "# Pyro 1 : Fired=%d, Time=%lu ms\n", sd_dump_stats->pyro1.fired, sd_dump_stats->pyro1.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Pyro 2 : Fired=%d, Time=%lu ms\n", sd_dump_stats->pyro2.fired, sd_dump_stats->pyro2.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Pyro 3 : Fired=%d, Time=%lu ms\n", sd_dump_stats->pyro3.fired, sd_dump_stats->pyro3.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Pyro 4 : Fired=%d, Time=%lu ms\n", sd_dump_stats->pyro4.fired, sd_dump_stats->pyro4.time_ms);
				f_puts(header_buf, &active_file);

				f_puts("# -- Windowed Events --\n", &active_file);

				sprintf(header_buf, "# Pyros Arming : Act=%d, Start=%lu ms, End=%lu ms\n", sd_dump_stats->pyros_arm.activated, sd_dump_stats->pyros_arm.start_time_ms, sd_dump_stats->pyros_arm.end_time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Mach Lock : Act=%d, Start=%lu ms, End=%lu ms\n", sd_dump_stats->mach_lock.activated, sd_dump_stats->mach_lock.start_time_ms, sd_dump_stats->mach_lock.end_time_ms);
				f_puts(header_buf, &active_file);

				f_puts("# -- Altitude Metrics (Valid, Value, Time_ms) --\n", &active_file);

				sprintf(header_buf, "# Max Altitude GPS : V=%d, %.4f mm, T=%lu ms\n", sd_dump_stats->max_altitude_gps.valid, sd_dump_stats->max_altitude_gps.value, sd_dump_stats->max_altitude_gps.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Max Altitude Baro : V=%d, %.4f m, T=%lu ms\n", sd_dump_stats->max_altitude_baro.valid, sd_dump_stats->max_altitude_baro.value, sd_dump_stats->max_altitude_baro.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Max Altitude Kalman : V=%d, %.4f m, T=%lu ms\n", sd_dump_stats->max_altitude_kalman.valid, sd_dump_stats->max_altitude_kalman.value, sd_dump_stats->max_altitude_kalman.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Apogee Detected : V=%d, %.4f m, T=%lu ms\n", sd_dump_stats->apogee.valid, sd_dump_stats->apogee.value, sd_dump_stats->apogee.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Main Deployment : V=%d, %.4f m, T=%lu ms\n", sd_dump_stats->main_deploy.valid, sd_dump_stats->main_deploy.value, sd_dump_stats->main_deploy.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Drogue Deployment : V=%d, %.4f m, T=%lu ms\n", sd_dump_stats->drogue_deploy.valid, sd_dump_stats->drogue_deploy.value, sd_dump_stats->drogue_deploy.time_ms);
				f_puts(header_buf, &active_file);

				f_puts("# -- Acceleration and Speed Metrics --\n", &active_file);

				sprintf(header_buf, "# Max Ascend Speed : V=%d, %.4f m/s, T=%lu ms\n", sd_dump_stats->max_ascend_speed.valid, sd_dump_stats->max_ascend_speed.value, sd_dump_stats->max_ascend_speed.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Max Descend Speed : V=%d, %.4f m/s, T=%lu ms\n", sd_dump_stats->max_descend_speed.valid, sd_dump_stats->max_descend_speed.value, sd_dump_stats->max_descend_speed.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Max Ascend Acceleration : V=%d, %.4f m/s2, T=%lu ms\n", sd_dump_stats->max_ascend_accel.valid, sd_dump_stats->max_ascend_accel.value, sd_dump_stats->max_ascend_accel.time_ms);
				f_puts(header_buf, &active_file);

				sprintf(header_buf, "# Max Descend Acceleration : V=%d, %.4f m/s2, T=%lu ms\n", sd_dump_stats->max_descend_accel.valid, sd_dump_stats->max_descend_accel.value, sd_dump_stats->max_descend_accel.time_ms);
				f_puts(header_buf, &active_file);

				f_puts("# =======================================\n\n", &active_file);

				f_puts("V_Maj,V_Min,Payload_Size,TimeBoot_ms,Sys_States,Event_States,Mission_State,Battery_mV,", &active_file);
				f_puts("Roll,Pitch,Yaw,IMU_Acc_X,IMU_Acc_Y,IMU_Acc_Z,IMU_Gyro_X,IMU_Gyro_Y,IMU_Gyro_Z,IMU_Mag_X,IMU_Mag_Y,IMU_Mag_Z,", &active_file);
				f_puts("Alt_MSL_m,Press_Pa,Temp_C,HighG_Acc_X,HighG_Acc_Y,HighG_Acc_Z,", &active_file);
				f_puts("GPS_Fix,Lat,Lon,GPS_Alt_mm,Vel,COG,Sat_NB,SD_Space,IMU_Acc_Vert,HighG_Acc_Vert,Kalman_Z,Kalman_V\n", &active_file);

				Logger_StartReadingFlight(Logger_GetCurrentFlightAddress(), &sd_dump_cursor);
				logger_state = LOGGER_SD_DUMP_DATA;
			} else {
				logger_state = LOGGER_IDLE;
			}
			break;

        case LOGGER_SD_DUMP_DATA:
            if(file_is_open) {
                odb_data_t frame;
                char line_buf[256];

                for(uint8_t i = 0; i < 5; i++) {
                    if(Logger_ReadNextData(&sd_dump_cursor, &frame)) {
                        snprintf(line_buf, sizeof(line_buf),
                            "%u,%u,%u,%lu,%u,%u,%u,%u,"
                            "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,"
                            "%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,"
                            "%u,%ld,%ld,%ld,%u,%u,%u,%u,%.4f,%.4f,%.4f,%.4f\n",
                            frame.version_major, frame.version_minor, frame.payload_size,
                            frame.time_boot_ms, frame.system_states, frame.event_states,
                            frame.mission_state, frame.battery_mv,
                            frame.roll, frame.pitch, frame.yaw,
                            frame.imu_acc_x, frame.imu_acc_y, frame.imu_acc_z,
                            frame.imu_gyro_x, frame.imu_gyro_y, frame.imu_gyro_z,
                            frame.imu_mag_x, frame.imu_mag_y, frame.imu_mag_z,
                            frame.altitude_msl_m, frame.pressure_pa, frame.temp_celsius,
                            frame.highg_acc_x, frame.highg_acc_y, frame.highg_acc_z,
                            frame.gps_fix, frame.lat, frame.lon, frame.gps_alt,
                            frame.vel, frame.cog, frame.satellites_nb, frame.sd_space,
                            frame.imu_acc_vertical, frame.highg_acc_vertical,
                            frame.kalman_z, frame.kalman_v
                        );
                        f_puts(line_buf, &active_file);
                    } else {
                        MEM2067_Sync();
                        MEM2067_CloseFile();
                        MEM2067_Unmount();
                        logger_state = LOGGER_IDLE;
                        break;
                    }
                }
            }
            break;
    }
}

void Logger_SaveStats(const odb_stats_t *stats) {
    if(stats_reserved_address == 0 || !stats) return;

    logger_stats_t stats_packet;
    stats_packet.magic_number = LOGGER_STATS_MAGIC_NUMBER;
    stats_packet.stats = *stats;

    W25Q_WritePage(&w25q, (uint8_t*)&stats_packet, stats_reserved_address, sizeof(logger_stats_t));
}

void Logger_Enable(bool enable) {
    is_logging = enable;
    if(enable) {
        write_index = 0;
        flush_pending = false;
    }
}

bool Logger_IsLogging(void) {
    return is_logging;
}

void Logger_FlushRemaining(void) {
    if(write_index > 0 && !flush_pending) {
        current_flush_buf = current_write_buf;
        flush_pending = true;
        write_index = 0;
    }
}

uint32_t Logger_GetCurrentFlightAddress(void) {
    return last_flight_header_addr;
}

uint32_t Logger_GetCurrentFlightId(void) {
    return last_flight_id;
}

void Logger_StartReadingFlight(uint32_t header_addr, uint32_t *cursor) {
    if(header_addr == 0xFFFFFFFF) {
        *cursor = 0xFFFFFFFF;
        return;
    }
    *cursor = header_addr + FLASH_SECTOR_SIZE_BYTE + W25Q512_PAGE_SIZE;
}

bool Logger_ReadNextData(uint32_t *cursor, odb_data_t *out_data) {
    if(*cursor == 0xFFFFFFFF || *cursor >= LOGGER_MAX_ALLOWED_ADDRESS) {
        return false;
    }

    logger_data_t temp_packet;
    W25Q_Read(&w25q, (uint8_t*)&temp_packet, *cursor, sizeof(logger_data_t));

    if(temp_packet.magic_number == LOGGER_DATA_MAGIC_NUMBER) {
        *out_data = temp_packet.data;
        *cursor += sizeof(logger_data_t);
        return true;
    }

    return false;
}

const odb_stats_t* Logger_GetLastFlightStats(void) {
    if(previous_flight_header_addr == 0xFFFFFFFF) {
        return NULL;
    }

    uint32_t read_addr = previous_flight_header_addr + FLASH_SECTOR_SIZE_BYTE;
    static logger_stats_t temp_stats;

    W25Q_Read(&w25q, (uint8_t*)&temp_stats, read_addr, sizeof(logger_stats_t));

    if(temp_stats.magic_number == LOGGER_STATS_MAGIC_NUMBER) {
        return &temp_stats.stats;
    }

    return NULL;
}

bool Logger_Erase(void) {
	uint32_t current_addr = 0;
	while(current_addr < LOGGER_MAX_ALLOWED_ADDRESS) {
		if((current_addr + W25Q512_BLOCK_SIZE) > LOGGER_MAX_ALLOWED_ADDRESS) {
			if(W25Q_EraseSector(&w25q, current_addr) != 0) {
				return false;
			}
			current_addr += W25Q512_SECTOR_SIZE;
		} else {
			if(W25Q_EraseBlock(&w25q, current_addr) != 0) {
				return false;
			}
			current_addr += W25Q512_BLOCK_SIZE;
		}
	}

	return true;
}

void Logger_ExportToSD(const odb_stats_t *stats) {
    if(stats != NULL) {
        sd_dump_stats = stats;
        start_sd_dump = true;
    }
}

/*
void Test_W25Q_Logging(void) {
	DEBUG_PRINTF("\n=== [TEST] DÉBUT DU TEST D'ENREGISTREMENT W25Q512 ===\n");

    odb_data_t mock_data;
    ODB_Reset(&mock_data, NULL);
    mock_data.version_major = ODB_PROTOCOL_VERSION_MAJOR;

    DEBUG_PRINTF("[TEST] Génération et écriture de 500 frames en Flash...\n");
    for (int i = 0; i < 500; i++) {
        mock_data.time_boot_ms = i * 20;
        mock_data.altitude_msl_m = (float)i * 2.5f;
        mock_data.kalman_z = mock_data.altitude_msl_m;
        mock_data.kalman_v = 150.0f - (i * 0.3f);

        if (i > 250) {
            mock_data.event_states |= FLAG_APOGEE_DETECTED;
            mock_data.kalman_v = -10.0f;
        }

        Logger_PushData(&mock_data);
        Logger_Task();

        while (W25Q_IsBusy(w25q.hqspi)) {
            Logger_Task();
            HAL_Delay(1);
        }
    }
    DEBUG_PRINTF("[TEST] Écriture des frames terminée.\n");

    odb_stats_t mock_stats;
    ODB_Reset(NULL, &mock_stats);
    mock_stats.flight_time_ms = 10000;

    mock_stats.max_altitude_kalman.valid = true;
    mock_stats.max_altitude_kalman.value = 1250.5f;
    mock_stats.max_altitude_kalman.time_ms = 5000;
    mock_stats.apogee.valid = true;
    mock_stats.apogee.value = 1250.5f;

    DEBUG_PRINTF("[TEST] Écriture des Stats...\n");
    Logger_SaveStats(&mock_stats);

    DEBUG_PRINTF("[TEST] Relecture des données depuis la W25Q...\n");
    const odb_stats_t* read_stats = Logger_GetLastFlightStats();

    if (read_stats->max_altitude_kalman.valid && read_stats->max_altitude_kalman.value == 1250.5f) {
    	DEBUG_PRINTF("[TEST] SUCCÈS : Les statistiques ont été lues correctement (%.1f m) !\n", read_stats->max_altitude_kalman.value);
    } else {
    	DEBUG_PRINTF("[TEST] ÉCHEC : Statistiques corrompues ou non trouvées (Valeur lue: %.1f m).\n", read_stats->max_altitude_kalman.value);
    }

    DEBUG_PRINTF("=== [TEST] FIN DU TEST ===\n\n");
}
*/
