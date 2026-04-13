/*
 * w25q512jv.c
 *
 * Created on: 12 avr. 2026
 * Author: gagno
 */

#include "w25q512jv.h"

static QSPI_CommandTypeDef W25Q_MakeCommand(uint8_t instruction, uint8_t address_mode, uint32_t address, uint8_t data_mode, uint32_t dummy_cycles, uint32_t data_length) {
    QSPI_CommandTypeDef sCommand = {0};
    sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = instruction;
    sCommand.AddressMode       = address_mode;
    sCommand.AddressSize       = QSPI_ADDRESS_32_BITS;
    sCommand.Address           = address;
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode          = data_mode;
    sCommand.DummyCycles       = dummy_cycles;
    sCommand.NbData            = data_length;
    sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
    sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
    sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

    return sCommand;
}

static int8_t W25Q_GetID(QSPI_HandleTypeDef *hqspi, uint32_t *id) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_READ_ID, QSPI_ADDRESS_NONE, 0xFFFFFFFF, QSPI_DATA_1_LINE, 0, 3);
    
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return -1;
    }

    uint8_t id_buf[3];
    if(HAL_QSPI_Receive(hqspi, id_buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return -1;
    }

    *id = (id_buf[0] << 16) | (id_buf[1] << 8) | id_buf[2];

    return 0; // success
}

static int8_t W25Q_WriteEnable(QSPI_HandleTypeDef *hqspi) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_WRITE_ENABLE, QSPI_ADDRESS_NONE, 0xFFFFFFFF, QSPI_DATA_NONE, 0, 0);
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return -1;
    }
    return 0; // success
}

static int8_t W25Q_WaitForReady(QSPI_HandleTypeDef *hqspi, uint32_t timeout) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_READ_STATUS_REG1, QSPI_ADDRESS_NONE, 0xFFFFFFFF, QSPI_DATA_1_LINE, 0, 1);
    uint8_t status = 0;
    uint32_t tickstart = HAL_GetTick();

    do {
        if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            return -1; // failure
        }
        if(HAL_QSPI_Receive(hqspi, &status, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            return -1; // failure
        }

        if((status & W25Q_SR1_BUSY) == 0) {
            return 0; // ready
        }

    } while ((HAL_GetTick() - tickstart) < timeout);

    return -1; // timeout
}

static int8_t W25Q_Enter4ByteMode(QSPI_HandleTypeDef *hqspi) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_ENTER_4B_MODE, QSPI_ADDRESS_NONE, 0xFFFFFFFF, QSPI_DATA_NONE, 0, 0);
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return -1;
    }

    return W25Q_WaitForReady(hqspi, W25Q_TIMEOUT);
}

static int8_t W25Q_EnableQuadMode(QSPI_HandleTypeDef *hqspi) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_READ_STATUS_REG2, QSPI_ADDRESS_NONE, 0xFFFFFFFF, QSPI_DATA_1_LINE, 0, 1);
    uint8_t status = 0;
    
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;
    if(HAL_QSPI_Receive(hqspi, &status, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;
    
    if((status & W25Q_SR2_QE) != 0) {
        return 0; // Quad mode already enabled
    }

    // Enable Quad mode
    status |= W25Q_SR2_QE;
    if(W25Q_WriteEnable(hqspi) != 0) return -1;

    sCommand.Instruction = W25Q_CMD_WRITE_STATUS_REG2;
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;
    if(HAL_QSPI_Transmit(hqspi, &status, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;

    return W25Q_WaitForReady(hqspi, W25Q_TIMEOUT);
}

int8_t W25Q_Init(QSPI_HandleTypeDef *hqspi) {
    if(W25Q_Reset(hqspi) != 0) return -1;
    if(W25Q_WaitForReady(hqspi, W25Q_TIMEOUT) != 0) return -1;
    if(W25Q_EnableQuadMode(hqspi) != 0) return -1;
    if(W25Q_Enter4ByteMode(hqspi) != 0) return -1;

    return 0; // success
}

int8_t W25Q_Reset(QSPI_HandleTypeDef *hqspi) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_RESET_ENABLE, QSPI_ADDRESS_NONE, 0xFFFFFFFF, QSPI_DATA_NONE, 0, 0);
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;

    sCommand.Instruction = W25Q_CMD_RESET_MEMORY;
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;

    HAL_Delay(10);

    return 0; // success
}

int8_t W25Q_Read(QSPI_HandleTypeDef *hqspi, uint8_t* pData, uint32_t read_addr, uint32_t size) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_FAST_READ_QUAD_IO_4B, QSPI_ADDRESS_4_LINES, read_addr, QSPI_DATA_4_LINES, 6, size);
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;
    if(HAL_QSPI_Receive(hqspi, pData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;

    return 0; // success
}

int8_t W25Q_Write(QSPI_HandleTypeDef *hqspi, uint8_t* pData, uint32_t WriteAddr, uint32_t size) {
    QSPI_CommandTypeDef sCommand = {0};
    uint32_t end_addr, current_size, current_addr;
    current_addr = WriteAddr;

    while(size > 0) {
        current_size = W25Q512_PAGE_SIZE - (current_addr % W25Q512_PAGE_SIZE);
        if(current_size > size) current_size = size;

        if(W25Q_WriteEnable(hqspi) != 0) return -1;
        
        sCommand = W25Q_MakeCommand(W25Q_CMD_QUAD_PAGE_PROGRAM_4B, QSPI_ADDRESS_1_LINE, current_addr, QSPI_DATA_4_LINES, 0, current_size);
        
        if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;
        if(HAL_QSPI_Transmit(hqspi, pData, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;
        if(W25Q_WaitForReady(hqspi, W25Q_TIMEOUT) != 0) return -1;

        current_addr += current_size;
        pData += current_size;
        size -= current_size;
    }

    return 0; // success
}

int8_t W25Q_EraseSector(QSPI_HandleTypeDef *hqspi, uint32_t sector_addr) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_SECTOR_ERASE_4B, QSPI_ADDRESS_1_LINE, sector_addr, QSPI_DATA_NONE, 0, 0);

    if(W25Q_WriteEnable(hqspi) != 0) return -1;
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;

    return W25Q_WaitForReady(hqspi, W25Q_ERASE_ALL_TIMEOUT);
}

int8_t W25Q_EraseBlock(QSPI_HandleTypeDef *hqspi, uint32_t block_addr) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_BLOCK_ERASE_64K_4B, QSPI_ADDRESS_1_LINE, block_addr, QSPI_DATA_NONE, 0, 0);

    if(W25Q_WriteEnable(hqspi) != 0) return -1;
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;

    return W25Q_WaitForReady(hqspi, W25Q_ERASE_ALL_TIMEOUT);
}

int8_t W25Q_EraseChip(QSPI_HandleTypeDef *hqspi) {
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_CHIP_ERASE, QSPI_ADDRESS_NONE, 0xFFFFFFFF, QSPI_DATA_NONE, 0, 0);

    if(W25Q_WriteEnable(hqspi) != 0) return -1;
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;

    return W25Q_WaitForReady(hqspi, W25Q_ERASE_ALL_TIMEOUT);
}

int8_t W25Q_SetMemoryMappedMode(QSPI_HandleTypeDef *hqspi, bool enable) {
    if(enable) {
        QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(W25Q_CMD_FAST_READ_QUAD_IO_4B, QSPI_ADDRESS_4_LINES, 0xFFFFFFFF, QSPI_DATA_4_LINES, 6, 0);
        QSPI_MemoryMappedTypeDef sMemMappedCfg = {0};
        
        sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
        sMemMappedCfg.TimeOutPeriod     = 0;

        if(HAL_QSPI_MemoryMapped(hqspi, &sCommand, &sMemMappedCfg) != HAL_OK) return -1;
    } 
    else {
        if(HAL_QSPI_Abort(hqspi) != HAL_OK) return -1;
    }

    return 0; // success
}

int8_t W25Q_SetDeepPowerDown(QSPI_HandleTypeDef *hqspi, bool enable) {
    uint8_t instruction = enable ? W25Q_CMD_DEEP_POWER_DOWN : W25Q_CMD_RELEASE_POWER_DOWN;
    
    QSPI_CommandTypeDef sCommand = W25Q_MakeCommand(instruction, QSPI_ADDRESS_NONE, 0xFFFFFFFF, QSPI_DATA_NONE, 0, 0);
    if(HAL_QSPI_Command(hqspi, &sCommand, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) return -1;
    HAL_Delay(1);

    return 0; // success
}