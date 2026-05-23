/*
 * logger.c
 *
 *  Created on: 8 mai 2026
 *      Author: gagno
 */

#include "App/logger.h"
#include "App/config.h"
#include <string.h>


typedef enum {
    LOGGER_IDLE,
    LOGGER_START_WRITE,
    LOGGER_WAIT_FLASH_BUSY
} logger_state_t;
static logger_state_t logger_state = LOGGER_IDLE;

static logger_data_t buffer_A[LOG_BUFFER_SIZE];
static logger_data_t buffer_B[LOG_BUFFER_SIZE];
static logger_data_t *current_write_buf = buffer_A;
static logger_data_t *current_flush_buf = NULL;

static uint8_t write_index = 0;
static uint32_t flash_current_address = 0;
static bool flush_pending = false;

static uint32_t last_flight_header_addr = 0; 
static uint32_t last_flight_id = 0;
static uint32_t stats_reserved_address = 0;

extern w25q_t w25q;


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

    if(flash_current_address >= (LOGGER_MAX_ALLOWED_ADDRESS - FLASH_SECTOR_SIZE_BYTE)) {
        flash_current_address = 0;
        next_id = 1;
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
    if(write_index >= LOG_BUFFER_SIZE) {
        return;
    }

    current_write_buf[write_index].magic_number = LOGGER_DATA_MAGIC_NUMBER;
    current_write_buf[write_index].data = *new_data;
    write_index++;

    if(write_index >= LOG_BUFFER_SIZE) {
        if(flush_pending) {
            // TODO: add error counter "missed frames"
            write_index--;
            return; 
        }

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

void Logger_Task(void) {
    switch(logger_state) {
        case LOGGER_IDLE:
            if(flush_pending) logger_state = LOGGER_START_WRITE;
            break;

        case LOGGER_START_WRITE: {
            uint32_t size = LOG_BUFFER_SIZE * sizeof(logger_data_t);
            if((flash_current_address + size) > LOGGER_MAX_ALLOWED_ADDRESS) {
                flush_pending = false;
                current_flush_buf = NULL;
                logger_state = LOGGER_IDLE;
                break;
            }

            if(flash_current_address % FLASH_SECTOR_SIZE_BYTE == 0) {
                W25Q_EraseSector(&w25q, flash_current_address);
            }

            if(W25Q_WritePageNoWait(&w25q, (uint8_t*)current_flush_buf, flash_current_address, size) == 0) {
                flash_current_address += size;
                if(flash_current_address % W25Q512_PAGE_SIZE != 0) {
                    flash_current_address = ((flash_current_address / W25Q512_PAGE_SIZE) + 1) * W25Q512_PAGE_SIZE;
                }
                logger_state = LOGGER_WAIT_FLASH_BUSY;
            }
            break;
        }

        case LOGGER_WAIT_FLASH_BUSY:
            if(!W25Q_IsBusy(w25q.hqspi)) {
                flush_pending = false;
                current_flush_buf = NULL;
                logger_state = LOGGER_IDLE;
            }
            break;
    }
}

void Logger_SaveStats(const odb_stats_t *stats) {
    if(stats_reserved_address == 0) return;

    logger_stats_t stats_packet;
    stats_packet.magic_number = LOGGER_STATS_MAGIC_NUMBER;
    stats_packet.stats = *stats;

    W25Q_WritePage(&w25q, (uint8_t*)&stats_packet, stats_reserved_address, sizeof(logger_stats_t));
}

odb_data_t Logger_GetLastFlightData(void) {
    odb_data_t last_valid_packet = {0};
    if(last_flight_id == 0) return last_valid_packet;

    uint32_t read_addr = last_flight_header_addr + FLASH_SECTOR_SIZE_BYTE + W25Q512_PAGE_SIZE; 
    logger_data_t temp_packet;
    while(read_addr < LOGGER_MAX_ALLOWED_ADDRESS) {
        W25Q_Read(&w25q, (uint8_t*)&temp_packet, read_addr, sizeof(logger_data_t));
        
        if(temp_packet.magic_number == LOGGER_DATA_MAGIC_NUMBER) {
            last_valid_packet = temp_packet.data;
            read_addr += sizeof(logger_data_t);
        } else {
            break; // No more valid packets
        }
    }

    return last_valid_packet;
}

odb_stats_t Logger_GetLastFlightStats(odb_stats_t *stats) {
    odb_stats_t empty_stats = {0};
    if(last_flight_id == 0) {
        if(stats) {
            *stats = empty_stats;
        }
        
        return empty_stats;
    }

    uint32_t read_addr = last_flight_header_addr + FLASH_SECTOR_SIZE_BYTE;
    logger_stats_t temp_stats;

    W25Q_Read(&w25q, (uint8_t*)&temp_stats, read_addr, sizeof(logger_stats_t));

    if(temp_stats.magic_number == LOGGER_STATS_MAGIC_NUMBER) {
        *stats = temp_stats.stats;
        return temp_stats.stats;
    }
    *stats = empty_stats;

    return empty_stats;
}
