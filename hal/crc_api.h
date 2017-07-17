/*************************************************************************//**
 *
 * \file    crc_api.h
 * \brief   ...
 *
 * \note    None
 *
 * \author  Daniel Cesarini <daniel.cesarini@tridonic.com>
 * \version 0.1
 * \date    4 July 2016
 * \bug     None
 *
 * \copyright 2016, Tridonic GmbH & Co KG
 *                  Färbergasse 15 - 6851 Dornbirn, Austria
 *
 *****************************************************************************/

#ifndef YOTTA_MODULES_MBED_HAL_MBED_HAL_CRC_API_H_
#define YOTTA_MODULES_MBED_HAL_MBED_HAL_CRC_API_H_

/* Includes -----------------------------------------------------------------*/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/* Public typedef -----------------------------------------------------------*/
/* Public define ------------------------------------------------------------*/
/* Public macro -------------------------------------------------------------*/
/* Exported variables -------------------------------------------------------*/
/* Public function prototypes -----------------------------------------------*/
int Init();
int Reset();
int CalcSingle(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC);
int CalcAccumulate(uint8_t* pui8StartAddress, uint32_t ui32DataSize, uint32_t* pui32CRC);

#ifdef __cplusplus
}
#endif

#endif /* YOTTA_MODULES_MBED_HAL_MBED_HAL_INTERNAL_FLASH_API_H_ */
