/*
 * logger.c
 *
 *  Created on: 8 mai 2026
 *      Author: gagno
 */

#include "App/logger.h"
#include <string.h> // Pour memcpy

#define FLIGHT_HEADER_MAGIC_NUMBER 0xABCD1234
#define WRITE_TIMEOUT_MS            10


typedef enum {
    LOGGER_IDLE,
    LOGGER_START_WRITE,
    LOGGER_WAIT_FLASH_BUSY
} logger_state_t;

static logger_state_t logger_state = LOGGER_IDLE;
static odb_data_t buffer_A[LOG_BUFFER_SIZE];
static odb_data_t buffer_B[LOG_BUFFER_SIZE];
static odb_data_t *current_write_buf = buffer_A;
static odb_data_t *current_flush_buf = NULL;
static uint8_t write_index = 0;
static uint32_t flash_current_address = 0;
static bool flush_pending = false;

static uint32_t last_flight_header_addr = 0; 
static uint32_t last_flight_id = 0;

extern w25q_t w25q;


static bool Logger_ReadHeader(uint32_t addr, flight_header_t *header) {
    if(W25Q_Read(&w25q, (uint8_t*)header, addr, sizeof(flight_header_t)) != 0) {
        return false;
    }

    return (header->magic_number == FLIGHT_HEADER_MAGIC_NUMBER);
}

static void Logger_ScanFlash(uint32_t *next_free_addr, uint32_t *next_id) {
    flight_header_t header;
    uint32_t addr = 0;
    uint32_t found_id = 0;
    uint32_t found_addr = 0;

    while(addr < W25Q512_FLASH_SIZE) {
        if(Logger_ReadHeader(addr, &header)) {
            found_id = header.flight_id;
            found_addr = addr;
            addr += 4096;
        } else {
            uint32_t first_word;
            W25Q_Read(&w25q, (uint8_t*)&first_word, addr, 4);
            if(first_word == 0xFFFFFFFF) {
                break;
            }
            addr += 4096;
        }
    }
    
    last_flight_header_addr = found_addr;
    last_flight_id = found_id;
    *next_id = found_id + 1;
    *next_free_addr = addr;
}

odb_data_t Logger_GetLastFlightData(void) {
    odb_data_t last_valid_packet = {0};
    if(last_flight_id == 0) return last_valid_packet;

    uint32_t read_addr = last_flight_header_addr + 4096; 
    odb_data_t temp_packet;
    while (read_addr < W25Q512_FLASH_SIZE) {
        W25Q_Read(&w25q, (uint8_t*)&temp_packet, read_addr, sizeof(odb_data_t));

        uint32_t check;
        memcpy(&check, &temp_packet, 4);
        
        if(check == FLIGHT_HEADER_MAGIC_NUMBER || check == 0xFFFFFFFF) {
            break; 
        }

        last_valid_packet = temp_packet;
        read_addr += sizeof(odb_data_t);
    }

    return last_valid_packet;
}

int8_t Logger_Init(void) {
    uint32_t next_id = 0;
    Logger_ScanFlash(&flash_current_address, &next_id);

    if(flash_current_address >= (W25Q512_FLASH_SIZE - 4096)) {
        flash_current_address = 0;
        next_id = 1;
    }

    flight_header_t header = {
        .magic_number = FLIGHT_HEADER_MAGIC_NUMBER,
        .flight_id = next_id,
        .metadata_rsv = 0
    };

    if(W25Q_WritePage(&w25q, (uint8_t*)&header, flash_current_address, sizeof(flight_header_t)) != 0) {
    	return -1;
    }
    flash_current_address += 4096; 

    write_index = 0;
    flush_pending = false;
    current_write_buf = buffer_A;
    current_flush_buf = NULL;
    logger_state = LOGGER_IDLE;

    return 0;
}

void Logger_PushData(odb_data_t *new_data) {
    current_write_buf[write_index] = *new_data;
    write_index++;

    if(write_index >= LOG_BUFFER_SIZE) {
        if(flush_pending) {
            // TODO: add error counter "missed frames"
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
            uint32_t size = LOG_BUFFER_SIZE * sizeof(odb_data_t);
            if(W25Q_WritePageNoWait(&w25q, (uint8_t*)current_flush_buf, flash_current_address, size) == 0) {
                flash_current_address += size;
                if(flash_current_address % 256 != 0) {
                    flash_current_address = ((flash_current_address / 256) + 1) * 256;
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
