/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @version        : v1.0_Cube
  * @brief          : Memory management layer.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
#include "Systems/config.h"
#include "Drivers/w25q512jv.h"
#include <stdlib.h>
#include <string.h>

extern w25q_t w25q;

/* Runtime write-enable flag. Default 0 = writes disabled (read-only).
 * Call `STORAGE_SetWriteEnabled(1)` from a debug console / button handler
 * to allow host writes. Always backup flash before enabling writes.
 */
static volatile uint8_t usb_write_enabled = 0;

void STORAGE_SetWriteEnabled(uint8_t en)
{
  usb_write_enabled = en ? 1 : 0;
}

uint8_t STORAGE_GetWriteEnabled(void)
{
  return usb_write_enabled;
}

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_STORAGE
  * @brief Usb mass storage device module
  * @{
  */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Defines
  * @brief Private defines.
  * @{
  */

#define STORAGE_LUN_NBR                  1
#define STORAGE_BLK_NBR                  0x10000
#define STORAGE_BLK_SIZ                  0x200

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN INQUIRY_DATA_FS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] = {/* 36 */

  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                      /* Version      : 4 Bytes */
};
/* USER CODE END INQUIRY_DATA_FS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the storage unit (medium) over USB FS IP
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Init_FS(uint8_t lun)
{
  /* USER CODE BEGIN 2 */
 UNUSED(lun);

  return (USBD_OK);
  /* USER CODE END 2 */
}

/**
  * @brief  Returns the medium capacity.
  * @param  lun: Logical unit number.
  * @param  block_num: Number of total block number.
  * @param  block_size: Block size.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */
  UNUSED(lun);

  /* Calculate capacity from W25Q constants */
  *block_size = STORAGE_BLK_SIZ;
  *block_num = (uint32_t)(W25Q512_FLASH_SIZE_BYTE / (uint32_t)(*block_size));
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief   Checks whether the medium is ready.
  * @param  lun:  Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
  /* USER CODE BEGIN 4 */
  UNUSED(lun);

  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Checks whether the medium is write protected.
  * @param  lun: Logical unit number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
  /* USER CODE BEGIN 5 */
  UNUSED(lun);

  /* Return write-protected state based on runtime flag:
   * - When usb_write_enabled == 0 (default), report protected (USBD_OK)
   * - When usb_write_enabled == 1, report not write-protected (USBD_FAIL)
   * Note: ST example uses USBD_OK to mean protected in this project.
   */
  if (usb_write_enabled) {
    return (USBD_FAIL);
  }
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number.
  * @param  buf: data buffer.
  * @param  blk_addr: Logical block address.
  * @param  blk_len: Blocks number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */
  UNUSED(lun);
  uint32_t addr = blk_addr * STORAGE_BLK_SIZ;
  uint32_t size = (uint32_t)blk_len * STORAGE_BLK_SIZ;

  if (W25Q_Read(&w25q, buf, addr, size) == 0) {
    return (USBD_OK);
  }
  return (USBD_FAIL);
  /* USER CODE END 6 */
}

/**
  * @brief  Writes data into the medium.
  * @param  lun: Logical unit number.
  * @param  buf: data buffer.
  * @param  blk_addr: Logical block address.
  * @param  blk_len: Blocks number.
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 7 */
  UNUSED(lun);
  uint32_t addr = blk_addr * STORAGE_BLK_SIZ;
  uint32_t size = (uint32_t)blk_len * STORAGE_BLK_SIZ;

  /* Special control block: allow host to toggle write-enable by writing
   * an ASCII command into a reserved control LBA. This avoids needing CDC.
   * Control LBA is placed immediately before the config sector.
   */
  const uint32_t CONTROL_LBA = (FLASH_CONFIG_START_ADDRESS / STORAGE_BLK_SIZ) - 1;
  if (blk_addr == CONTROL_LBA && blk_len == 1) {
    /* commands: "ODB_CMD:ENABLE" or "ODB_CMD:DISABLE" (prefix match) */
    if (memcmp(buf, "ODB_CMD:ENABLE", 13) == 0) {
      usb_write_enabled = 1;
      return (USBD_OK);
    }
    if (memcmp(buf, "ODB_CMD:DISABLE", 14) == 0) {
      usb_write_enabled = 0;
      return (USBD_OK);
    }
    /* unknown command: ignore and fail */
    return (USBD_FAIL);
  }

  /* Disallow writes unless explicitly enabled */
  if (!usb_write_enabled) {
    return (USBD_FAIL);
  }

  /* Protect config / reserved region at end of flash */
  if (addr >= FLASH_CONFIG_START_ADDRESS) {
    return (USBD_FAIL);
  }
  if ((addr + size) > FLASH_CONFIG_START_ADDRESS) {
    return (USBD_FAIL);
  }

  /* Perform safe sector-level read-modify-write to handle partial-page writes.
   * For each 4 KiB sector touched, read entire sector, apply updates, erase sector,
   * then program the sector page-by-page (256 B pages).
   */
  uint32_t sector_size = FLASH_SECTOR_SIZE_BYTE;
  uint32_t page_size = W25Q512_PAGE_SIZE;

  uint32_t sector_start = (addr / sector_size) * sector_size;
  uint32_t sector_end = ((addr + size + sector_size - 1) / sector_size) * sector_size;

  uint32_t buf_off = 0;
  for (uint32_t sector_addr = sector_start; sector_addr < sector_end; sector_addr += sector_size) {
    uint8_t *sector_buf = malloc(sector_size);
    if (!sector_buf) return (USBD_FAIL);

    if (W25Q_Read(&w25q, sector_buf, sector_addr, sector_size) != 0) {
      free(sector_buf);
      return (USBD_FAIL);
    }

    /* compute overlap of [addr, addr+size) with this sector */
    uint32_t overlap_start = (addr > sector_addr) ? (addr - sector_addr) : 0;
    uint32_t overlap_end = (uint32_t)((addr + size > sector_addr + sector_size) ? sector_size : (addr + size - sector_addr));
    if (overlap_end > overlap_start) {
      /* copy data into sector buffer */
      uint32_t copy_len = overlap_end - overlap_start;
      memcpy(sector_buf + overlap_start, buf + buf_off, copy_len);
      buf_off += copy_len;
    }

    /* Erase sector */
    if (W25Q_EraseSector(&w25q, sector_addr) != 0) {
      free(sector_buf);
      return (USBD_FAIL);
    }

    /* Program sector page-by-page */
    for (uint32_t p = 0; p < sector_size; p += page_size) {
      if (W25Q_WritePage(&w25q, sector_buf + p, sector_addr + p, page_size) != 0) {
        free(sector_buf);
        return (USBD_FAIL);
      }
    }

    free(sector_buf);
  }

  return (USBD_OK);
  /* USER CODE END 7 */
}

/**
  * @brief  Returns the Max Supported LUNs.
  * @param  None
  * @retval Lun(s) number.
  */
int8_t STORAGE_GetMaxLun_FS(void)
{
  /* USER CODE BEGIN 8 */
  return (STORAGE_LUN_NBR - 1);
  /* USER CODE END 8 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

