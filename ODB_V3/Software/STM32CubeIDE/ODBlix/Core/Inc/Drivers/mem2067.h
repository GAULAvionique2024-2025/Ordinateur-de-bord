/*
 * mem2067.h
 *
 *  Created on: 19 juin 2026
 *      Author: gagno
 */

#ifndef INC_DRIVERS_MEM2067_H_
#define INC_DRIVERS_MEM2067_H_

#define BUFFER_SIZE 1024

#include <fatfs.h>
#include "Drivers/LowLevel/fatfs_sd.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define HEADER_NUM 18

typedef union {
    int 	i;
    float 	f;
    double 	d;
    short 	s;
    char 	c;
    char* 	str;
} DataUnion;

typedef enum {
    DATA_TYPE_INT,
    DATA_TYPE_FLOAT,
    DATA_TYPE_DOUBLE,
    DATA_TYPE_SHORT,
    DATA_TYPE_CHAR,
	DATA_TYPE_STRING
} DataType;

typedef struct {
    DataType 	type;
    DataUnion 	data;
} DataField;

typedef enum {
	MEM2067_OK,
	MEM2067_ERROR
} mem2067_state_t;

typedef struct {
	uint32_t total_space;
	uint32_t free_space;
} mem2067_t;


mem2067_state_t MEM2067_Mount(const char *filename);
void MEM2067_Write(const char *filename, const DataField data[], size_t num_fields);
char *MEM2067_Read(const char *filename);
void MEM2067_Unmount(void);
void MEM2067_Infos(mem2067_t *dev);

const char* FATFS_ErrorToString(FRESULT result);

int bufsize(char *buf);
void bufclear(char *p_Buffer);

#endif /* INC_DRIVERS_MEM2067_H_ */
