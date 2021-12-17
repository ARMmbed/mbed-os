/**
 *******************************************************************************
 * @file     rda_flash512ud32_b.h
 * @brief    This file provides rda_flash512ud32 definition.\n
 * @version  V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2021
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RDA_FLASH512UD32_B_H
#define __RDA_FLASH512UD32_B_H

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "flash.h"
//#include "driver_com.h"
#include "TMPM4KNA.h"


/**
 * @addtogroup Periph_Driver Peripheral Driver
 * @{
 */

/**
 * @addtogroup RDA_FLASH512UD32 RDA_FLASH512UD32
 * @{
 */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
#if defined ( __CC_ARM   )      /* RealView Compiler */
extern uint32_t Load$$FLASH_CODE_RAM$$Base;
extern uint32_t Image$$FLASH_CODE_RAM$$Base;
extern uint32_t Load$$FLASH_CODE_RAM$$Length;
extern uint32_t Image$$FLASH_DEMO_A$$Base;
extern uint32_t Load$$FLASH_DEMO_B$$Base;
extern uint32_t Image$$RAM_DEMO_A$$Base;
extern uint32_t Image$$RAM_DEMO_B$$Base;

#elif defined ( __ICCARM__ )    /* IAR Compiler */
#pragma section = "FLASH_CODE_RAM"
#pragma section = "FLASH_CODE_ROM"
#pragma section = "FLASH_DEMO_A"
#pragma section = "FLASH_DEMO_B"
#pragma section = "RAM_DEMO_A"
#pragma section = "RAM_DEMO_B"

#endif

/**
 *  @addtogroup RDA_FLASH512UD32_Private_define RDA_FLASH512UD32 Private Define
 *  @{
 */
#if defined ( __CC_ARM   )      /* RealView Compiler */
#define FLASH_API_ROM           (uint32_t *)&Load$$FLASH_CODE_RAM$$Base

#define FLASH_API_RAM           (uint32_t *)&Image$$FLASH_CODE_RAM$$Base
#define SIZE_FLASH_API          (uint32_t)&Load$$FLASH_CODE_RAM$$Length

#define DEMO_A_FLASH            (uint32_t *)&Image$$FLASH_DEMO_A$$Base
#define DEMO_B_FLASH            (uint32_t *)&Load$$FLASH_DEMO_B$$Base

#define DEMO_A_RAM              (uint32_t *)&Image$$RAM_DEMO_A$$Base
#define DEMO_B_RAM              (uint32_t *)&Image$$RAM_DEMO_B$$Base


#elif defined ( __ICCARM__ )    /* IAR Compiler */
#define FLASH_API_ROM           ((uint32_t *)__section_begin("FLASH_CODE_ROM"))

#define FLASH_API_RAM           ((uint32_t *)__section_begin("FLASH_CODE_RAM"))
#define SIZE_FLASH_API          ((uint32_t)__section_size("FLASH_CODE_ROM"))

#define DEMO_A_FLASH            ((uint32_t *)__section_begin("FLASH_DEMO_A"))
#define DEMO_B_FLASH            ((uint32_t *)__section_begin("FLASH_DEMO_B"))

#define DEMO_A_RAM              ((uint32_t *)__section_begin("RAM_DEMO_A"))
#define DEMO_B_RAM              ((uint32_t *)__section_begin("RAM_DEMO_B"))

#endif

/* code start address */
#define DEMO_START_ADDR         ((uint32_t)DEMO_A_FLASH)

#define USERINFOSIZE      (128)        /*<! User Information data size */

/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_Private_define */


/**
 * @addtogroup RDA_FLASH512UD32_enum
 * @{
 */

/*----------------------------------*/
/**
 *  @enum   fc_sr0_t
 *  @brief  Enumerated type definition of the FCSR0 register.
 */
/*----------------------------------*/
typedef enum {
    FC_SR0_RDYBSY           = (0x00000001UL)      /*!< 0:Busy, 1:Ready all flash     */
} fc_sr0_t;

/*----------------------------------*/
/**
 *  @enum   fc_erase_kind_t
 *  @brief  FC Erase Flash Kind structure definenition.
*/
/*----------------------------------*/
typedef enum {
    FC_ERASE_KIND_PAGE      = (0x00000040UL),    /*!< Page Erase                     */
    FC_ERASE_KIND_BLOCK     = (0x00000030UL),    /*!< Block Erase                    */
    FC_ERASE_KIND_AREA      = (0x00000020UL)     /*!< Area Erase                     */
} fc_erase_kind_t;

typedef enum {
    FC_CODE_FLASH_PAGE0 = 0,    /*!< (0x5E000000UL), CODE FLASH Page0   */
    FC_CODE_FLASH_PAGE1,        /*!< (0x5E001000UL), CODE FLASH Page1   */
    FC_CODE_FLASH_PAGE2,        /*!< (0x5E002000UL), CODE FLASH Page2   */
    FC_CODE_FLASH_PAGE3,        /*!< (0x5E003000UL), CODE FLASH Page3   */
    FC_CODE_FLASH_PAGE4,        /*!< (0x5E004000UL), CODE FLASH Page4   */
    FC_CODE_FLASH_PAGE5,        /*!< (0x5E005000UL), CODE FLASH Page5   */
    FC_CODE_FLASH_PAGE6,        /*!< (0x5E006000UL), CODE FLASH Page6   */
    FC_CODE_FLASH_PAGE7,        /*!< (0x5E007000UL), CODE FLASH Page7   */
    FC_CODE_FLASH_PAGE8,        /*!< (0x5E008000UL), CODE FLASH Page8   */
    FC_CODE_FLASH_PAGE9,        /*!< (0x5E009000UL), CODE FLASH Page9   */
    FC_CODE_FLASH_PAGE10,       /*!< (0x5E00A000UL), CODE FLASH Page10  */
    FC_CODE_FLASH_PAGE11,       /*!< (0x5E00B000UL), CODE FLASH Page11  */
    FC_CODE_FLASH_PAGE12,       /*!< (0x5E00C000UL), CODE FLASH Page12  */
    FC_CODE_FLASH_PAGE13,       /*!< (0x5E00D000UL), CODE FLASH Page13  */
    FC_CODE_FLASH_PAGE14,       /*!< (0x5E00E000UL), CODE FLASH Page14  */
    FC_CODE_FLASH_PAGE15,       /*!< (0x5E00F000UL), CODE FLASH Page15  */
    FC_CODE_FLASH_PAGE16,       /*!< (0x5E010000UL), CODE FLASH Page16  */
    FC_CODE_FLASH_PAGE17,       /*!< (0x5E011000UL), CODE FLASH Page17  */
    FC_CODE_FLASH_PAGE18,       /*!< (0x5E012000UL), CODE FLASH Page18  */
    FC_CODE_FLASH_PAGE19,       /*!< (0x5E013000UL), CODE FLASH Page19  */
    FC_CODE_FLASH_PAGE20,       /*!< (0x5E014000UL), CODE FLASH Page20  */
    FC_CODE_FLASH_PAGE21,       /*!< (0x5E015000UL), CODE FLASH Page21  */
    FC_CODE_FLASH_PAGE22,       /*!< (0x5E016000UL), CODE FLASH Page22  */
    FC_CODE_FLASH_PAGE23,       /*!< (0x5E017000UL), CODE FLASH Page23  */
    FC_CODE_FLASH_PAGE24,       /*!< (0x5E018000UL), CODE FLASH Page24  */
    FC_CODE_FLASH_PAGE25,       /*!< (0x5E019000UL), CODE FLASH Page25  */
    FC_CODE_FLASH_PAGE26,       /*!< (0x5E01A000UL), CODE FLASH Page26  */
    FC_CODE_FLASH_PAGE27,       /*!< (0x5E01B000UL), CODE FLASH Page27  */
    FC_CODE_FLASH_PAGE28,       /*!< (0x5E01C000UL), CODE FLASH Page28  */
    FC_CODE_FLASH_PAGE29,       /*!< (0x5E01D000UL), CODE FLASH Page29  */
    FC_CODE_FLASH_PAGE30,       /*!< (0x5E01E000UL), CODE FLASH Page30  */
    FC_CODE_FLASH_PAGE31,       /*!< (0x5E01F000UL), CODE FLASH Page31  */
    FC_CODE_FLASH_PAGE32,       /*!< (0x5E020000UL), CODE FLASH Page32  */
    FC_CODE_FLASH_PAGE33,       /*!< (0x5E021000UL), CODE FLASH Page33  */
    FC_CODE_FLASH_PAGE34,       /*!< (0x5E022000UL), CODE FLASH Page34  */
    FC_CODE_FLASH_PAGE35,       /*!< (0x5E023000UL), CODE FLASH Page35  */
    FC_CODE_FLASH_PAGE36,       /*!< (0x5E024000UL), CODE FLASH Page36  */
    FC_CODE_FLASH_PAGE37,       /*!< (0x5E025000UL), CODE FLASH Page37  */
    FC_CODE_FLASH_PAGE38,       /*!< (0x5E026000UL), CODE FLASH Page38  */
    FC_CODE_FLASH_PAGE39,       /*!< (0x5E027000UL), CODE FLASH Page39  */
    FC_CODE_FLASH_PAGE40,       /*!< (0x5E028000UL), CODE FLASH Page40  */
    FC_CODE_FLASH_PAGE41,       /*!< (0x5E029000UL), CODE FLASH Page41  */
    FC_CODE_FLASH_PAGE42,       /*!< (0x5E02A000UL), CODE FLASH Page42  */
    FC_CODE_FLASH_PAGE43,       /*!< (0x5E02B000UL), CODE FLASH Page43  */
    FC_CODE_FLASH_PAGE44,       /*!< (0x5E02C000UL), CODE FLASH Page44  */
    FC_CODE_FLASH_PAGE45,       /*!< (0x5E02D000UL), CODE FLASH Page45  */
    FC_CODE_FLASH_PAGE46,       /*!< (0x5E02E000UL), CODE FLASH Page46  */
    FC_CODE_FLASH_PAGE47,       /*!< (0x5E02F000UL), CODE FLASH Page47  */
    FC_CODE_FLASH_PAGE48,       /*!< (0x5E030000UL), CODE FLASH Page48  */
    FC_CODE_FLASH_PAGE49,       /*!< (0x5E031000UL), CODE FLASH Page49  */
    FC_CODE_FLASH_PAGE50,       /*!< (0x5E032000UL), CODE FLASH Page50  */
    FC_CODE_FLASH_PAGE51,       /*!< (0x5E033000UL), CODE FLASH Page51  */
    FC_CODE_FLASH_PAGE52,       /*!< (0x5E034000UL), CODE FLASH Page52  */
    FC_CODE_FLASH_PAGE53,       /*!< (0x5E035000UL), CODE FLASH Page53  */
    FC_CODE_FLASH_PAGE54,       /*!< (0x5E036000UL), CODE FLASH Page54  */
    FC_CODE_FLASH_PAGE55,       /*!< (0x5E037000UL), CODE FLASH Page55  */
    FC_CODE_FLASH_PAGE56,       /*!< (0x5E038000UL), CODE FLASH Page56  */
    FC_CODE_FLASH_PAGE57,       /*!< (0x5E039000UL), CODE FLASH Page57  */
    FC_CODE_FLASH_PAGE58,       /*!< (0x5E03A000UL), CODE FLASH Page58  */
    FC_CODE_FLASH_PAGE59,       /*!< (0x5E03B000UL), CODE FLASH Page59  */
    FC_CODE_FLASH_PAGE60,       /*!< (0x5E03C000UL), CODE FLASH Page60  */
    FC_CODE_FLASH_PAGE61,       /*!< (0x5E03D000UL), CODE FLASH Page61  */
    FC_CODE_FLASH_PAGE62,       /*!< (0x5E03E000UL), CODE FLASH Page62  */
    FC_CODE_FLASH_PAGE63,       /*!< (0x5E03F000UL), CODE FLASH Page63  */
    FC_CODE_FLASH_PAGE64,       /*!< (0x5E040000UL), CODE FLASH Page64  */
    FC_CODE_FLASH_PAGE65,       /*!< (0x5E041000UL), CODE FLASH Page65  */
    FC_CODE_FLASH_PAGE66,       /*!< (0x5E042000UL), CODE FLASH Page66  */
    FC_CODE_FLASH_PAGE67,       /*!< (0x5E043000UL), CODE FLASH Page67  */
    FC_CODE_FLASH_PAGE68,       /*!< (0x5E044000UL), CODE FLASH Page68  */
    FC_CODE_FLASH_PAGE69,       /*!< (0x5E045000UL), CODE FLASH Page69  */
    FC_CODE_FLASH_PAGE70,       /*!< (0x5E046000UL), CODE FLASH Page70  */
    FC_CODE_FLASH_PAGE71,       /*!< (0x5E047000UL), CODE FLASH Page71  */
    FC_CODE_FLASH_PAGE72,       /*!< (0x5E048000UL), CODE FLASH Page72  */
    FC_CODE_FLASH_PAGE73,       /*!< (0x5E049000UL), CODE FLASH Page73  */
    FC_CODE_FLASH_PAGE74,       /*!< (0x5E04A000UL), CODE FLASH Page74  */
    FC_CODE_FLASH_PAGE75,       /*!< (0x5E04B000UL), CODE FLASH Page75  */
    FC_CODE_FLASH_PAGE76,       /*!< (0x5E04C000UL), CODE FLASH Page76  */
    FC_CODE_FLASH_PAGE77,       /*!< (0x5E04D000UL), CODE FLASH Page77  */
    FC_CODE_FLASH_PAGE78,       /*!< (0x5E04E000UL), CODE FLASH Page78  */
    FC_CODE_FLASH_PAGE79,       /*!< (0x5E04F000UL), CODE FLASH Page79  */
    FC_CODE_FLASH_PAGE80,       /*!< (0x5E050000UL), CODE FLASH Page80  */
    FC_CODE_FLASH_PAGE81,       /*!< (0x5E051000UL), CODE FLASH Page81  */
    FC_CODE_FLASH_PAGE82,       /*!< (0x5E052000UL), CODE FLASH Page82  */
    FC_CODE_FLASH_PAGE83,       /*!< (0x5E053000UL), CODE FLASH Page83  */
    FC_CODE_FLASH_PAGE84,       /*!< (0x5E054000UL), CODE FLASH Page84  */
    FC_CODE_FLASH_PAGE85,       /*!< (0x5E055000UL), CODE FLASH Page85  */
    FC_CODE_FLASH_PAGE86,       /*!< (0x5E056000UL), CODE FLASH Page86  */
    FC_CODE_FLASH_PAGE87,       /*!< (0x5E057000UL), CODE FLASH Page87  */
    FC_CODE_FLASH_PAGE88,       /*!< (0x5E058000UL), CODE FLASH Page88  */
    FC_CODE_FLASH_PAGE89,       /*!< (0x5E059000UL), CODE FLASH Page89  */
    FC_CODE_FLASH_PAGE90,       /*!< (0x5E05A000UL), CODE FLASH Page90  */
    FC_CODE_FLASH_PAGE91,       /*!< (0x5E05B000UL), CODE FLASH Page91  */
    FC_CODE_FLASH_PAGE92,       /*!< (0x5E05C000UL), CODE FLASH Page92  */
    FC_CODE_FLASH_PAGE93,       /*!< (0x5E05D000UL), CODE FLASH Page93  */
    FC_CODE_FLASH_PAGE94,       /*!< (0x5E05E000UL), CODE FLASH Page94  */
    FC_CODE_FLASH_PAGE95,       /*!< (0x5E05F000UL), CODE FLASH Page95  */
    FC_CODE_FLASH_PAGE96,       /*!< (0x5E060000UL), CODE FLASH Page96  */
    FC_CODE_FLASH_PAGE97,       /*!< (0x5E061000UL), CODE FLASH Page97  */
    FC_CODE_FLASH_PAGE98,       /*!< (0x5E062000UL), CODE FLASH Page98  */
    FC_CODE_FLASH_PAGE99,       /*!< (0x5E063000UL), CODE FLASH Page99  */
    FC_CODE_FLASH_PAGE100,      /*!< (0x5E064000UL), CODE FLASH Page100 */
    FC_CODE_FLASH_PAGE101,      /*!< (0x5E065000UL), CODE FLASH Page101 */
    FC_CODE_FLASH_PAGE102,      /*!< (0x5E066000UL), CODE FLASH Page102 */
    FC_CODE_FLASH_PAGE103,      /*!< (0x5E067000UL), CODE FLASH Page103 */
    FC_CODE_FLASH_PAGE104,      /*!< (0x5E068000UL), CODE FLASH Page104 */
    FC_CODE_FLASH_PAGE105,      /*!< (0x5E069000UL), CODE FLASH Page105 */
    FC_CODE_FLASH_PAGE106,      /*!< (0x5E06A000UL), CODE FLASH Page106 */
    FC_CODE_FLASH_PAGE107,      /*!< (0x5E06B000UL), CODE FLASH Page107 */
    FC_CODE_FLASH_PAGE108,      /*!< (0x5E06C000UL), CODE FLASH Page108 */
    FC_CODE_FLASH_PAGE109,      /*!< (0x5E06D000UL), CODE FLASH Page109 */
    FC_CODE_FLASH_PAGE110,      /*!< (0x5E06E000UL), CODE FLASH Page110 */
    FC_CODE_FLASH_PAGE111,      /*!< (0x5E06F000UL), CODE FLASH Page111 */
    FC_CODE_FLASH_PAGE112,      /*!< (0x5E070000UL), CODE FLASH Page112 */
    FC_CODE_FLASH_PAGE113,      /*!< (0x5E071000UL), CODE FLASH Page113 */
    FC_CODE_FLASH_PAGE114,      /*!< (0x5E072000UL), CODE FLASH Page114 */
    FC_CODE_FLASH_PAGE115,      /*!< (0x5E073000UL), CODE FLASH Page115 */
    FC_CODE_FLASH_PAGE116,      /*!< (0x5E074000UL), CODE FLASH Page116 */
    FC_CODE_FLASH_PAGE117,      /*!< (0x5E075000UL), CODE FLASH Page117 */
    FC_CODE_FLASH_PAGE118,      /*!< (0x5E076000UL), CODE FLASH Page118 */
    FC_CODE_FLASH_PAGE119,      /*!< (0x5E077000UL), CODE FLASH Page119 */
    FC_CODE_FLASH_PAGE120,      /*!< (0x5E078000UL), CODE FLASH Page120 */
    FC_CODE_FLASH_PAGE121,      /*!< (0x5E079000UL), CODE FLASH Page121 */
    FC_CODE_FLASH_PAGE122,      /*!< (0x5E07A000UL), CODE FLASH Page122 */
    FC_CODE_FLASH_PAGE123,      /*!< (0x5E07B000UL), CODE FLASH Page123 */
    FC_CODE_FLASH_PAGE124,      /*!< (0x5E07C000UL), CODE FLASH Page124 */
    FC_CODE_FLASH_PAGE125,      /*!< (0x5E07D000UL), CODE FLASH Page125 */
    FC_CODE_FLASH_PAGE126,      /*!< (0x5E07E000UL), CODE FLASH Page126 */
    FC_CODE_FLASH_PAGE127       /*!< (0x5E07F000UL), CODE FLASH Page127 */
} fc_code_flash_page_number_t;

typedef enum {
    FC_CODE_FLASH_BLOCK0  = 0,  /*!< (0x5E000000UL), CODE FLASH Block0  */
    FC_CODE_FLASH_BLOCK1,       /*!< (0x5E008000UL), CODE FLASH Block1  */
    FC_CODE_FLASH_BLOCK2,       /*!< (0x5E010000UL), CODE FLASH Block2  */
    FC_CODE_FLASH_BLOCK3,       /*!< (0x5E018000UL), CODE FLASH Block3  */
    FC_CODE_FLASH_BLOCK4,       /*!< (0x5E020000UL), CODE FLASH Block4  */
    FC_CODE_FLASH_BLOCK5,       /*!< (0x5E028000UL), CODE FLASH Block5  */
    FC_CODE_FLASH_BLOCK6,       /*!< (0x5E030000UL), CODE FLASH Block6  */
    FC_CODE_FLASH_BLOCK7,       /*!< (0x5E038000UL), CODE FLASH Block7  */
    FC_CODE_FLASH_BLOCK8,       /*!< (0x5E040000UL), CODE FLASH Block8  */
    FC_CODE_FLASH_BLOCK9,       /*!< (0x5E048000UL), CODE FLASH Block9  */
    FC_CODE_FLASH_BLOCK10,      /*!< (0x5E050000UL), CODE FLASH Block10 */
    FC_CODE_FLASH_BLOCK11,      /*!< (0x5E058000UL), CODE FLASH Block11 */
    FC_CODE_FLASH_BLOCK12,      /*!< (0x5E060000UL), CODE FLASH Block12 */
    FC_CODE_FLASH_BLOCK13,      /*!< (0x5E068000UL), CODE FLASH Block13 */
    FC_CODE_FLASH_BLOCK14,      /*!< (0x5E070000UL), CODE FLASH Block14 */
    FC_CODE_FLASH_BLOCK15       /*!< (0x5E078000UL), CODE FLASH Block15 */
} fc_code_flash_block_number_t;

typedef enum {
    FC_DATA_FLASH_PAGE0  = 0,   /*!< (0x30000000UL), DATA FLASH Page0   */
    FC_DATA_FLASH_PAGE1,        /*!< (0x30000100UL), DATA FLASH Page1   */
    FC_DATA_FLASH_PAGE2,        /*!< (0x30000200UL), DATA FLASH Page2   */
    FC_DATA_FLASH_PAGE3,        /*!< (0x30000300UL), DATA FLASH Page3   */
    FC_DATA_FLASH_PAGE4,        /*!< (0x30000400UL), DATA FLASH Page4   */
    FC_DATA_FLASH_PAGE5,        /*!< (0x30000500UL), DATA FLASH Page5   */
    FC_DATA_FLASH_PAGE6,        /*!< (0x30000600UL), DATA FLASH Page6   */
    FC_DATA_FLASH_PAGE7,        /*!< (0x30000700UL), DATA FLASH Page7   */
    FC_DATA_FLASH_PAGE8,        /*!< (0x30000800UL), DATA FLASH Page8   */
    FC_DATA_FLASH_PAGE9,        /*!< (0x30000900UL), DATA FLASH Page9   */
    FC_DATA_FLASH_PAGE10,       /*!< (0x30000A00UL), DATA FLASH Page10  */
    FC_DATA_FLASH_PAGE11,       /*!< (0x30000B00UL), DATA FLASH Page11  */
    FC_DATA_FLASH_PAGE12,       /*!< (0x30000C00UL), DATA FLASH Page12  */
    FC_DATA_FLASH_PAGE13,       /*!< (0x30000D00UL), DATA FLASH Page13  */
    FC_DATA_FLASH_PAGE14,       /*!< (0x30000E00UL), DATA FLASH Page14  */
    FC_DATA_FLASH_PAGE15,       /*!< (0x30000F00UL), DATA FLASH Page15  */
    FC_DATA_FLASH_PAGE16,       /*!< (0x30001000UL), DATA FLASH Page16  */
    FC_DATA_FLASH_PAGE17,       /*!< (0x30001100UL), DATA FLASH Page17  */
    FC_DATA_FLASH_PAGE18,       /*!< (0x30001200UL), DATA FLASH Page18  */
    FC_DATA_FLASH_PAGE19,       /*!< (0x30001300UL), DATA FLASH Page19  */
    FC_DATA_FLASH_PAGE20,       /*!< (0x30001400UL), DATA FLASH Page20  */
    FC_DATA_FLASH_PAGE21,       /*!< (0x30001500UL), DATA FLASH Page21  */
    FC_DATA_FLASH_PAGE22,       /*!< (0x30001600UL), DATA FLASH Page22  */
    FC_DATA_FLASH_PAGE23,       /*!< (0x30001700UL), DATA FLASH Page23  */
    FC_DATA_FLASH_PAGE24,       /*!< (0x30001800UL), DATA FLASH Page24  */
    FC_DATA_FLASH_PAGE25,       /*!< (0x30001900UL), DATA FLASH Page25  */
    FC_DATA_FLASH_PAGE26,       /*!< (0x30001A00UL), DATA FLASH Page26  */
    FC_DATA_FLASH_PAGE27,       /*!< (0x30001B00UL), DATA FLASH Page27  */
    FC_DATA_FLASH_PAGE28,       /*!< (0x30001C00UL), DATA FLASH Page28  */
    FC_DATA_FLASH_PAGE29,       /*!< (0x30001D00UL), DATA FLASH Page29  */
    FC_DATA_FLASH_PAGE30,       /*!< (0x30001E00UL), DATA FLASH Page30  */
    FC_DATA_FLASH_PAGE31,       /*!< (0x30001F00UL), DATA FLASH Page31  */
    FC_DATA_FLASH_PAGE32        /*!< (0x30002000UL), DATA FLASH Page32  */
} fc_data_flash_page_number_t;

typedef enum {
    FC_CODE_FLASH_AREA0  = 0    /*!< (0x5E000000UL), CODE FLASH AREA0   */
} fc_code_flash_area_number_t;


typedef enum {
    FC_DATA_FLASH_BLOCK0  = 0,  /*!< (0x30000000UL), DATA FLASH Block0  */
    FC_DATA_FLASH_BLOCK1,       /*!< (0x30001000UL), DATA FLASH Block1  */
    FC_DATA_FLASH_BLOCK2,       /*!< (0x30002000UL), DATA FLASH Block2  */
    FC_DATA_FLASH_BLOCK3,       /*!< (0x30003000UL), DATA FLASH Block3  */
    FC_DATA_FLASH_BLOCK4,       /*!< (0x30004000UL), DATA FLASH Block4  */
    FC_DATA_FLASH_BLOCK5,       /*!< (0x30005000UL), DATA FLASH Block5  */
    FC_DATA_FLASH_BLOCK6,       /*!< (0x30006000UL), DATA FLASH Block6  */
    FC_DATA_FLASH_BLOCK7        /*!< (0x30007000UL), DATA FLASH Block7  */
} fc_data_flash_block_number_t;

typedef enum {
    FC_DATA_FLASH_AREA4  = 0    /*!< (0x30000000UL), DATA FLASH AREA4   */
} fc_data_flash_area_number_t;

typedef enum {
    FC_PROTECT_KIND_PAGE  = (0x00000000UL),    /*!< Page Protect        */
    FC_PROTECT_KIND_BLOCK = (0x00000001UL),    /*!< Block Protect       */
} fc_protect_kind_t;

/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_enum */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup RDA_FLASH512UD32_Exported_define RDA_FLASH512UD32 Exported Define
 *  @{
 */
#define FC_CODE_FLASH_ADDRESS_TOP   (0x5E000000UL)              /*!< CODE FLASH Address Top */
#define FC_CODE_DATA_ADDRESS_TOP    (0x30000000UL)              /*!< CODE FLASH Address Top */
#define FC_PAGE_SIZE                (0x1000)                    /*!< The number of bytes in a page. */
#define FC_BLOCK_SIZE               (0x8000)                    /*!< The number of bytes in a block. */
#define FC_AREA_SIZE                (0x320000)                  /*!< The number of bytes in a area. */
#define FC_DATA_FLASH_ADDRESS_TOP   (0x30000000UL)              /*!< DATA FLASH Address Top */
#define FC_DATA_PAGE_SIZE           (0x100)                     /*!< The number of bytes in a page. */
#define FC_DATA_BLOCK_SIZE          (0x1000)                    /*!< The number of bytes in a block. */
#define FC_DATA_AREA_SIZE           (0x8000)                    /*!< The number of bytes in a area. */
#define FC_DATA_FLASH_WRITE_SIZE    (sizeof(uint32_t)*4)        /*!< CDATA FLASH WRITE SIZE */
#define FC_CODE_FLASH_WRITE_SIZE    (sizeof(uint32_t)*4)        /*!< CODE FLASH WRITE SIZE */
#define FC_CODE_FLASH_PG            (0x00000000UL)              /*!< CODE FLASH PG */
#define FC_CODE_FLASH_BLK           (0x00000001UL)              /*!< CODE FLASH BLK */
#define FC_DATA_FLASH               (0x00000002UL)              /*!< DATA FLASH */

#define FC_PROTECT_PM0              (0x00000001UL)              /*!< CODE FLASH PM0   protect */
#define FC_PROTECT_PM1              (0x00000002UL)              /*!< CODE FLASH PM1   protect */
#define FC_PROTECT_PM2              (0x00000004UL)              /*!< CODE FLASH PM2   protect */
#define FC_PROTECT_PM3              (0x00000008UL)              /*!< CODE FLASH PM3   protect */
#define FC_PROTECT_PM4              (0x00000010UL)              /*!< CODE FLASH PM4   protect */
#define FC_PROTECT_PM5              (0x00000020UL)              /*!< CODE FLASH PM5   protect */
#define FC_PROTECT_PM6              (0x00000040UL)              /*!< CODE FLASH PM6   protect */
#define FC_PROTECT_PM7              (0x00000080UL)              /*!< CODE FLASH PM7   protect */
#define FC_PROTECT_MSK1             (0x00000002UL)              /*!< CODE FLASH MSK1  protect */
#define FC_PROTECT_MSK2             (0x00000004UL)              /*!< CODE FLASH MSK2  protect */
#define FC_PROTECT_MSK3             (0x00000008UL)              /*!< CODE FLASH MSK3  protect */
#define FC_PROTECT_MSK4             (0x00000010UL)              /*!< CODE FLASH MSK4  protect */
#define FC_PROTECT_MSK5             (0x00000020UL)              /*!< CODE FLASH MSK5  protect */
#define FC_PROTECT_MSK6             (0x00000040UL)              /*!< CODE FLASH MSK6  protect */
#define FC_PROTECT_MSK7             (0x00000080UL)              /*!< CODE FLASH MSK7  protect */
#define FC_PROTECT_MSK8             (0x00000100UL)              /*!< CODE FLASH MSK8  protect */
#define FC_PROTECT_MSK9             (0x00000200UL)              /*!< CODE FLASH MSK9  protect */
#define FC_PROTECT_MSK10            (0x00000400UL)              /*!< CODE FLASH MSK10 protect */
#define FC_PROTECT_MSK11            (0x00000800UL)              /*!< CODE FLASH MSK11 protect */
#define FC_PROTECT_MSK12            (0x00001000UL)              /*!< CODE FLASH MSK12 protect */
#define FC_PROTECT_MSK13            (0x00002000UL)              /*!< CODE FLASH MSK13 protect */
#define FC_PROTECT_MSK14            (0x00004000UL)              /*!< CODE FLASH MSK14 protect */
#define FC_PROTECT_MSK15            (0x00008000UL)              /*!< CODE FLASH MSK15 protect */
#define FC_PROTECT_DMSK0            (0x00000001UL)              /*!< DATA FLASH DMSK0 protect */
#define FC_PROTECT_DMSK1            (0x00000002UL)              /*!< DATA FLASH DMSK1 protect */
#define FC_PROTECT_DMSK2            (0x00000004UL)              /*!< DATA FLASH DMSK2 protect */
#define FC_PROTECT_DMSK3            (0x00000008UL)              /*!< DATA FLASH DMSK3 protect */
#define FC_PROTECT_DMSK4            (0x00000010UL)              /*!< DATA FLASH DMSK4 protect */
#define FC_PROTECT_DMSK5            (0x00000020UL)              /*!< DATA FLASH DMSK5 protect */
#define FC_PROTECT_DMSK6            (0x00000040UL)              /*!< DATA FLASH DMSK6 protect */
#define FC_PROTECT_DMSK7            (0x00000080UL)              /*!< DATA FLASH DMSK7 protect */

/**
 *  @addtogroup  FCSR0  FCSR0 Register
 *  @brief     Flash Status Register 0.
 *  @details   Detail.
 *  | Bit   | Bit Symbol |
 *  | :---  | :---       |
 *  | 31:11 | -          |
 *  | 10    | RDYBSY2    |
 *  | 9     | -          |
 *  | 8     | RDYBSY0    |
 *  | 7:1   | -          |
 *  | 0     | RDYBSY     |
 *  @{
 */
/* RDYBSY */
#define FCSR0_RDYBSY_MASK           ((uint32_t)0x00000001)    /*!< RDYBSY :Mask            */
#define FCSR0_RDYBSY_R_RUNNING      ((uint32_t)0x00000000)    /*!< RDYBSY :[R] :Running    */
#define FCSR0_RDYBSY_R_DONE         ((uint32_t)0x00000001)    /*!< RDYBSY :[R] :Done       */
/**
 *  @}
 */ /* End of group FCSR0 */


/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup RDA_FLASH512UD32_Private_typedef RDA_FLASH512UD32 Private Typedef
 *  @{
 */

/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_Private_typedef */


/*------------------------------------------------------------------------------*/
/*  Inline Functions                                                             */
/*------------------------------------------------------------------------------*/
/**
 * @addtogroup RDA_FLASH512UD32_Exported_function RDA_FLASH512UD32 Exported Function
 * @{
 */

void     fc_read_clock_set(void);
void     fc_fixed_clock_set(uint32_t);
uint32_t Get_param_FC_RDYBSY(void);

void     copy_fc_func(void);
void     copy_user_program(uint32_t size);
void     copy_user_data(char *data_a, char *data_b, uint32_t size);

uint32_t rewrite_user_program(uint32_t size);

void     fc_read_user_information_area(uint32_t *src_address, uint32_t *dst_address, uint32_t size);

uint32_t fc_write_page_data_flash(uint32_t page, uint32_t size);
uint32_t fc_write_code_flash(uint32_t *src_address, uint32_t *dst_address, uint32_t size);
uint32_t fc_write_user_information_area(uint32_t *src_address, uint32_t size, uint32_t User_Information_Area);


uint32_t fc_erase_area_data_flash(fc_data_flash_area_number_t area);
uint32_t fc_erase_page_data_flash(fc_data_flash_page_number_t first_page, uint8_t num_of_pages);
uint32_t fc_erase_page_code_flash(fc_code_flash_page_number_t first_page, uint8_t num_of_pages);
uint32_t fc_erase_block_data_flash(fc_data_flash_block_number_t first_block, uint8_t num_of_block);
uint32_t fc_erase_block_code_flash(fc_code_flash_block_number_t first_block, uint8_t num_of_block);
uint32_t fc_erase_user_information_area(uint32_t User_Information_Area);

uint32_t fc_protect_page_code_flash(fc_code_flash_page_number_t page);
uint32_t fc_protect_block_code_flash(fc_code_flash_block_number_t block);
uint32_t fc_protect_erase_code_flash(void);


/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_Exported_function */


/**
 *  @}
 */ /* End of RDA_FLASH512UD32 */

/**
 *  @}
 */ /* End of group Periph_driver */

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __RDA_FLASH512UD32_B_H */

