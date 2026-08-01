/* Core/Src/Tools/Simulation/Mocks/diskio.h */
#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

#include "integer.h"

/* Status of Disk Functions */
typedef BYTE    DSTATUS;

/* Results of Disk Functions */
typedef enum {
    RES_OK = 0,     /* 0: Successful */
    RES_ERROR,      /* 1: R/W Error */
    RES_WRPRT,      /* 2: Write Protected */
    RES_NOTRDY,     /* 3: Not Ready */
    RES_PARERR      /* 4: Invalid Parameter */
} DRESULT;

#endif /* _DISKIO_DEFINED */
