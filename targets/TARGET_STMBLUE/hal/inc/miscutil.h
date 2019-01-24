/**
  ******************************************************************************
  * @file    miscutil.h
  * @author  AMG - RF Application team
  * @version V1.1.0
  * @date    3-April-2018
  * @brief   Header file for miscellaneous utilities.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2017 STMicroelectronics</center></h2>
  ******************************************************************************
  */
#ifndef __MISCUTIL_H__
#define __MISCUTIL_H__

#include <stdint.h>
#include "crash_handler.h"
#include "compiler.h"


#define DIE_ID_BLUENRG2     ((uint8_t)2)
#define DIE_ID_BLUENRG1     ((uint8_t)1)


/**
 * @brief A structure that represents part information details
 *
 */
typedef struct PartInfoS {
  /** DIE ID number */
  uint8_t  die_id;
  /** Die major number */
  uint8_t  die_major;
  /** Die cut number */
  uint8_t  die_cut;
  /** JTAG ID */
  uint32_t jtag_id_code;
  /** Flash size in bytes */
  uint32_t flash_size;
} PartInfoType;

/** 
 * @brief This function return a structure with information about the device
 * 
 * @param[out] partInfo Pointer to a PartInfoType structure
 *
 * @retval None
 */
void HAL_GetPartInfo(PartInfoType *partInfo);
/**
 * @brief Get Crash Information utility
 * 
 * This function return the crash information that are stored in RAM, by hard
 * handler, nmi handler and assert handler.
 * This function reset the crash information stored in RAM before it returns.
 * So it avoid to report the same crash information after a normal reboot.
 * 
 * @param[out] crashInfo Pointer to a crash_info_t structure
 *
 * @retval None
 */
void HAL_GetCrashInfo(crash_info_t *crashInfo);
/**
 * @brief Set Crash Information utility
 * 
 * This function stores crash information in RAM and reset the device
 * Crash information can be retrieved by using API HAL_GetCrashInfo
 * 
 * @param[in] msp Stack pointer containg crash info
 * @param[out] signature CRash reason signature
 *
 * @retval None
 */
void HAL_CrashHandler(uint32_t msp, uint32_t signature);

/**
 * @brief Get Device ID, Version and Revision numbers
 * 
 * This function returns the device ID, version and revision numbers.
 * To be called by the BLE Stack for ensuring platform independence.
 * 
 * @param[out] device_id Device ID (1 = BlueNRG-1; 2 = BlueNRG-2)
 * @param[out] major_cut Device cut version/major number
 * @param[out] minor_cut Device cut revision/minor number
 *
 * @retval None
 */
void BLEPLAT_get_part_info(uint8_t *device_id, uint8_t *major_cut, uint8_t *minor_cut);

#endif /* __MISCUTIL_H__ */
