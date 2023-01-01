/**
 *******************************************************************************
 * @file    txz_fuart_ex.h
 * @brief   This file provides all the functions prototypes for FUART driver.
 * @brief   Extended functionality.
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
#ifndef __FUART_EX_H
#define __FUART_EX_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"
#include "txz_fuart.h"

/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/**
 *  @addtogroup FUART
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Exported_define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Exported_define
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Exported_define */


/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Exported_typedef
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group FUART_Exported_typedef */


/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup FUART_Exported_functions
 *  @{
 */
TXZ_Result fuart_send_break(fuart_t *p_obj);
TXZ_Result fuart_stop_break(fuart_t *p_obj);
/**
 *  @}
 */ /* End of group FUART_Exported_functions */

/**
 *  @}
 */ /* End of group FUART */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __FUART_EX_H */


