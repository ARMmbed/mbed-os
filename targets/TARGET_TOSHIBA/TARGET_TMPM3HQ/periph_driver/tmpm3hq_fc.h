/**
 *******************************************************************************
 * @file     tmpm3hq_fc.h
 * @brief    Flash_Userboot Sample Application.
 * @version  V1.0.0.0
 * $date::                 $
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA CORPORATION 2016 All rights reserved
 *******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FC_H
#define __FC_H

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "txz_driver_def.h"

#if defined(__FC_H)
/** @addtogroup Periph driver
  * @{
  */

/** @defgroup fc
  * @brief fc
  * @{
  */


/** @defgroup FlashApi_Exported_Types
  * @{
  */

/**
 *  @enum   fc_sr0_t
 *  @brief  Enumerated type definition of the FCSR0 register.
 */
typedef enum {
    FC_SR0_RDYBSY       = (0x00000001UL)    /*!< 0:Busy, 1:Ready all flash      */
} fc_sr0_t;

/*----------------------------------*/
/**
 *  @enum   fc_erase_kind_t
 *  @brief  FC Erase Flash Kind structure definenition.
*/
/*----------------------------------*/
typedef enum {
    FC_ERASE_KIND_PAGE      = (0x00000040UL),   /*!< Page Erase                     */
    FC_ERASE_KIND_BLOCK     = (0x00000030UL)    /*!< Page Erase                     */
} fc_erase_kind_t;

typedef enum {
    FC_CODE_FLASH_PAGE0  = 0,       /*!< (0x5E000000UL), CODE FLASH Page0 */
    FC_CODE_FLASH_PAGE1,        /*!< (0x5E001000UL), CODE FLASH Page1 */
    FC_CODE_FLASH_PAGE2,        /*!< (0x5E002000UL), CODE FLASH Page2 */
    FC_CODE_FLASH_PAGE3,        /*!< (0x5E003000UL), CODE FLASH Page3 */
    FC_CODE_FLASH_PAGE4,        /*!< (0x5E004000UL), CODE FLASH Page4 */
    FC_CODE_FLASH_PAGE5,        /*!< (0x5E005000UL), CODE FLASH Page5 */
    FC_CODE_FLASH_PAGE6,        /*!< (0x5E006000UL), CODE FLASH Page6 */
    FC_CODE_FLASH_PAGE7,        /*!< (0x5E007000UL), CODE FLASH Page7 */
    FC_CODE_FLASH_PAGE8,        /*!< (0x5E008000UL), CODE FLASH Page8 */
    FC_CODE_FLASH_PAGE9,        /*!< (0x5E009000UL), CODE FLASH Page9 */
    FC_CODE_FLASH_PAGE10,       /*!< (0x5E00A000UL), CODE FLASH Page10 */
    FC_CODE_FLASH_PAGE11,       /*!< (0x5E00B000UL), CODE FLASH Page11 */
    FC_CODE_FLASH_PAGE12,       /*!< (0x5E00C000UL), CODE FLASH Page12 */
    FC_CODE_FLASH_PAGE13,       /*!< (0x5E00D000UL), CODE FLASH Page13 */
    FC_CODE_FLASH_PAGE14,       /*!< (0x5E00E000UL), CODE FLASH Page14 */
    FC_CODE_FLASH_PAGE15,       /*!< (0x5E00F000UL), CODE FLASH Page15 */
    FC_CODE_FLASH_PAGE16,       /*!< (0x5E010000UL), CODE FLASH Page16 */
    FC_CODE_FLASH_PAGE17,       /*!< (0x5E011000UL), CODE FLASH Page17 */
    FC_CODE_FLASH_PAGE18,       /*!< (0x5E012000UL), CODE FLASH Page18 */
    FC_CODE_FLASH_PAGE19,       /*!< (0x5E013000UL), CODE FLASH Page19 */
    FC_CODE_FLASH_PAGE20,       /*!< (0x5E014000UL), CODE FLASH Page20 */
    FC_CODE_FLASH_PAGE21,       /*!< (0x5E015000UL), CODE FLASH Page21 */
    FC_CODE_FLASH_PAGE22,       /*!< (0x5E016000UL), CODE FLASH Page22 */
    FC_CODE_FLASH_PAGE23,       /*!< (0x5E017000UL), CODE FLASH Page23 */
    FC_CODE_FLASH_PAGE24,       /*!< (0x5E018000UL), CODE FLASH Page24 */
    FC_CODE_FLASH_PAGE25,       /*!< (0x5E019000UL), CODE FLASH Page25 */
    FC_CODE_FLASH_PAGE26,       /*!< (0x5E01A000UL), CODE FLASH Page26 */
    FC_CODE_FLASH_PAGE27,       /*!< (0x5E01B000UL), CODE FLASH Page27 */
    FC_CODE_FLASH_PAGE28,       /*!< (0x5E01C000UL), CODE FLASH Page28 */
    FC_CODE_FLASH_PAGE29,       /*!< (0x5E01D000UL), CODE FLASH Page29 */
    FC_CODE_FLASH_PAGE30,       /*!< (0x5E01E000UL), CODE FLASH Page30 */
    FC_CODE_FLASH_PAGE31        /*!< (0x5E01F000UL), CODE FLASH Page31 */
} fc_code_flash_page_number_t;

/** @} */
/* End of group FlashApi_Exported_Types */

/** @defgroup FlashApi_Exported_Constants
  * @{
  */

#define FC_CODE_FLASH_ADDRESS_TOP   (0x5E000000UL)              /*!< CODE FLASH Address Top */
#define FC_PAGE_SIZE                (0x1000)                    /*!< The number of bytes in a page. */
#define FC_CODE_FLASH_WRITE_SIZE    (sizeof(uint32_t)*4)        /*!< CODE FLASH WRITE SIZE */

/** @} */
/* End of group FlashApi_Exported_Constants */



/** @weakgroup FlashApi_Exported_FunctionPrototypes
  * @{
  */
TXZ_WorkState fc_get_status(fc_sr0_t status);
TXZ_Result fc_write_code_flash(uint32_t* src_address, uint32_t* dst_address, uint32_t size);
TXZ_Result fc_erase_page_code_flash(fc_code_flash_page_number_t first_page, uint8_t num_of_pages);
TXZ_Result fc_blank_check_page_code_flash(fc_code_flash_page_number_t first_page, fc_code_flash_page_number_t lasr_page);

/** @} */
/* End of group FlashApi_Exported_FunctionPrototypes */


/** @} */
/* End of group fc */

/** @} */
/* End of group Periph_driver */

#endif                          /* defined(__FC_H) */

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* __FC_H */
