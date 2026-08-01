/* Core/Src/Tools/Simulation/Mocks/fatfs.h */
#ifndef FATFS_MOCK_H
#define FATFS_MOCK_H

#include <stdint.h>

typedef struct {
    uint8_t dummy;
} FATFS;

typedef struct {
    uint8_t dummy;
} FIL;

typedef enum {
    FR_OK = 0,
    FR_DISK_ERR,
    FR_INT_ERR,
    FR_NOT_READY,
    FR_NO_FILE,
    FR_NO_PATH
} FRESULT;

#endif /* FATFS_MOCK_H */
