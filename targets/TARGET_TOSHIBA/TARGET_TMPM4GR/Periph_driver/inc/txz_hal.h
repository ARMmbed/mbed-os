/**
 *******************************************************************************
 * @file    txz_hal.h
 * @brief   This file provides all the functions prototypes for driver common part.
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __HAL_H
#define __HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"

/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/**
 *  @defgroup HAL HAL
 *  @brief    HAL Driver.
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Exported_macro HAL Exported Macro
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Exported_macro */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Exported_define HAL Exported Define
 *  @{
 */
#if defined ( __CC_ARM   )      /* RealView Compiler */
extern uint32_t Load$$FLASH_CODE_RAM$$Base;
extern uint32_t Image$$FLASH_CODE_RAM$$Base;
extern uint32_t Load$$FLASH_CODE_RAM$$Length;

#elif defined ( __ICCARM__ )    /* IAR Compiler */
#pragma section = "FLASH_CODE_RAM"
#pragma section = "FLASH_CODE_ROM"
#endif

#if defined ( __CC_ARM   )      /* RealView Compiler */
#define FLASH_API_ROM           (uint32_t *)&Load$$FLASH_CODE_RAM$$Base
#define FLASH_API_RAM           (uint32_t *)&Image$$FLASH_CODE_RAM$$Base
#define SIZE_FLASH_API          (uint32_t)&Load$$FLASH_CODE_RAM$$Length

#elif defined ( __ICCARM__ )    /* IAR Compiler */
#define FLASH_API_ROM           ((uint32_t *)__section_begin("FLASH_CODE_ROM"))
#define FLASH_API_RAM           ((uint32_t *)__section_begin("FLASH_CODE_RAM"))
#define SIZE_FLASH_API          ((uint32_t)__section_size("FLASH_CODE_ROM"))
#endif

/* no define */

/**
 *  @}
 */ /* End of group HAL_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Exported_define HAL Exported Define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Exported_typedef HAL Exported Typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group HAL_Exported_typedef */

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup HAL_Exported_functions HAL Exported Functions
 *  @{
 */

void hal_inc_tick(void);
uint32_t hal_get_tick(void);
void wait(uint32_t count);
void Copy_Routine(uint32_t *dest, uint32_t *source, uint32_t size);
void fc_ram_con_reg_set(uint32_t sysclock);

/**
 *  @}
 */ /* End of group HAL_Exported_functions */

/**
 *  @}
 */ /* End of group HAL */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __HAL_H */


