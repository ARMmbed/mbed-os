/**
 *******************************************************************************
 * @file    rda_flash512ud32_b.c
 * @brief   This file provides API functions for FLASH rda.\n
 * @version V1.0.0
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * Copyright(C) Toshiba Electronic Device Solutions Corporation 2020
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
#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "flash.h"
#include "TMPM4KNA.h"

#if defined(__FLASH_H)
/**
 *  @addtogroup Periph_Driver Peripheral Driver
 *  @{
 */

/**
 *  @addtogroup RDA_FLASH512UD32 RDA_FLASH512UD32
 *  @{
 */


/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup RDA_FLASH512UD32_Private_config RDA_FLASH512UD32 Private Config
 *  @{
 */


/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_Private_config */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup RDA_FLASH512UD32_Private_define RDA_FLASH512UD32 Private Define
 *  @{
 */
#define FC_KCR_KEYCODE          (0xA74A9D23UL)              /*!< The specific code that writes the FCKCR register. */
#define FC_BRANK_VALUE          (uint32_t)(0xFFFFFFFFUL)    /*!< Brank value */
#define FC_MAX_PAGES            (uint8_t)(0x80)             /*!< Maxmum pages */
#define FC_MAX_BLOCKS           (uint8_t)(0x16)             /*!< Maxmum blocks */
#define FC_MAX_AREAS            (uint8_t)(0x1)              /*!< Maxmum areas */
#define FC_MAX_DATA_PAGES       (uint8_t)(0x21)             /*!< Maxmum pages */
#define FC_MAX_DATA_BLOCKS      (uint8_t)(0x8)              /*!< Maxmum blocks */
#define FC_CMD_ADDRESS_MASK     (uint32_t)(0xFFFF0000UL)    /*!< Upper address mask of the upper address */
#define FC_CMD_BC1_ADDR         (0x00000550UL)              /*!< The lower address of the first bus cycle when uses commans */
#define FC_CMD_BC2_ADDR         (0x00000AA0UL)              /*!< The lower address of the second bus cycle when uses commans */
#define FC_BANK_USER_INFO       (0x00000007UL)              /*!< Bank Change User Information Area */
#define FC_BANK_CODE_FLASH      (0x00000000UL)              /*!< Bank Change Code Flash */
#define FC_BUFFER_DISABLE       (0x00000007UL)              /*!< Flash Buffer Disable nad Clear */
#define FC_BUFFER_ENABLE        (0x00000000UL)              /*!< Flash Buffer Enable */

#define FC_PROTECT_ADDR         (0x00002000UL)              /*!< The lower address of protect command */
#define FC_SWAP_ADDR            (0x00003000UL)              /*!< The lower address of swap command */
#define FC_BUSY_MAX_WAIT        (0x00001000UL)              /*!< Busy wait count */

#define FC_ACCR_FDLC_4          (0x00000300UL)              /*!< Data Flash read clock 4clock   */
#define FC_ACCR_FDLC_5          (0x00000400UL)              /*!< Data Flash read clock 5clock   */
#define FC_ACCR_FDLC_6          (0x00000500UL)              /*!< Data Flash read clock 6clock   */
#define FC_ACCR_FCLC_1          (0x00000000UL)              /*!< Code Flash read clock 1clock   */
#define FC_ACCR_FCLC_2          (0x00000001UL)              /*!< Code Flash read clock 2clock   */
#define FC_ACCR_FCLC_3          (0x00000002UL)              /*!< Code Flash read clock 3clock   */
#define FC_ACCR_FCLC_4          (0x00000003UL)              /*!< Code Flash read clock 4clock   */
#define FC_ACCR_FCLC_5          (0x00000004UL)              /*!< Code Flash read clock 5clock   */
#define FC_ACCR_FCLC_6          (0x00000005UL)              /*!< Code Flash read clock 6clock   */
#define SYSCORECLOCK_80M        (80000000UL)                /*!< 80MHz                          */

/* FCSR0 register */
#define FC_AREASEL_EXPECT_AREA0 (uint32_t)(0x00000000UL)    /*!< RW, Selects expect area0       */
#define FC_AREASEL_EXPECT_AREA4 (uint32_t)(0x00000000UL)    /*!< RW, Selects expect area4       */
#define FC_AREASEL_AREA0        (uint32_t)(0x00000007UL)    /*!< RW, Selects area0              */
#define FC_AREASEL_AREA4        (uint32_t)(0x00070000UL)    /*!< RW, Selects area4              */
#define FC_AREASEL_MASK_AREA0   (uint32_t)(0xFF8F0FF8UL)    /*!< RW, Selects area0              */
#define FC_AREASEL_MASK_AREA4   (uint32_t)(0xFF880FFFUL)    /*!< RW, Selects area4              */
#define FC_AREASEL_WRITE_MODE   (uint32_t)(0x04000000UL)    /*!< R,  Write Mode                 */
#define FC_AREASEL4_WRITE_MODE  (uint32_t)(0x40000000UL)    /*!< R,  Write Mode                 */

#define FC_SEC_MASK             (uint32_t)(0x00000001UL)    /*!< Flash security bit mask  */
#define FC_SEC_ENABLE           (uint32_t)(0x00000001UL)    /*!< Flash security enable  */

static uint32_t fc_const_code_flash_address[FC_MAX_PAGES] = {
    (0x5E000000UL),   /*!< CODE FLASH Page0   */
    (0x5E001000UL),   /*!< CODE FLASH Page1   */
    (0x5E002000UL),   /*!< CODE FLASH Page2   */
    (0x5E003000UL),   /*!< CODE FLASH Page3   */
    (0x5E004000UL),   /*!< CODE FLASH Page4   */
    (0x5E005000UL),   /*!< CODE FLASH Page5   */
    (0x5E006000UL),   /*!< CODE FLASH Page6   */
    (0x5E007000UL),   /*!< CODE FLASH Page7   */
    (0x5E008000UL),   /*!< CODE FLASH Page8   */
    (0x5E009000UL),   /*!< CODE FLASH Page9   */
    (0x5E00A000UL),   /*!< CODE FLASH Page10  */
    (0x5E00B000UL),   /*!< CODE FLASH Page11  */
    (0x5E00C000UL),   /*!< CODE FLASH Page12  */
    (0x5E00D000UL),   /*!< CODE FLASH Page13  */
    (0x5E00E000UL),   /*!< CODE FLASH Page14  */
    (0x5E00F000UL),   /*!< CODE FLASH Page15  */
    (0x5E010000UL),   /*!< CODE FLASH Page16  */
    (0x5E011000UL),   /*!< CODE FLASH Page17  */
    (0x5E012000UL),   /*!< CODE FLASH Page18  */
    (0x5E013000UL),   /*!< CODE FLASH Page19  */
    (0x5E014000UL),   /*!< CODE FLASH Page20  */
    (0x5E015000UL),   /*!< CODE FLASH Page21  */
    (0x5E016000UL),   /*!< CODE FLASH Page22  */
    (0x5E017000UL),   /*!< CODE FLASH Page23  */
    (0x5E018000UL),   /*!< CODE FLASH Page24  */
    (0x5E019000UL),   /*!< CODE FLASH Page25  */
    (0x5E01A000UL),   /*!< CODE FLASH Page26  */
    (0x5E01B000UL),   /*!< CODE FLASH Page27  */
    (0x5E01C000UL),   /*!< CODE FLASH Page28  */
    (0x5E01D000UL),   /*!< CODE FLASH Page29  */
    (0x5E01E000UL),   /*!< CODE FLASH Page30  */
    (0x5E01F000UL)    /*!< CODE FLASH Page31  */
};

static uint32_t fc_const_code_flash_block_address[FC_MAX_BLOCKS] = {
    (0x5E000000UL),   /*!< CODE FLASH Block0  */
    (0x5E008000UL),   /*!< CODE FLASH Block1  */
    (0x5E010000UL),   /*!< CODE FLASH Block2  */
    (0x5E018000UL),   /*!< CODE FLASH Block3  */
    (0x5E020000UL),   /*!< CODE FLASH Block4  */
    (0x5E028000UL),   /*!< CODE FLASH Block5  */
    (0x5E030000UL),   /*!< CODE FLASH Block6  */
    (0x5E038000UL),   /*!< CODE FLASH Block7  */
    (0x5E040000UL),   /*!< CODE FLASH Block8  */
    (0x5E048000UL),   /*!< CODE FLASH Block9  */
    (0x5E050000UL),   /*!< CODE FLASH Block10 */
    (0x5E058000UL),   /*!< CODE FLASH Block11 */
    (0x5E060000UL),   /*!< CODE FLASH Block12 */
    (0x5E068000UL),   /*!< CODE FLASH Block13 */
    (0x5E070000UL),   /*!< CODE FLASH Block14 */
    (0x5E078000UL)    /*!< CODE FLASH Block15 */
};

static uint32_t fc_const_data_flash_address[FC_MAX_DATA_PAGES] = {
    (0x30000000UL),   /*!< DATA FLASH Page0   */
    (0x30000100UL),   /*!< DATA FLASH Page1   */
    (0x30000200UL),   /*!< DATA FLASH Page2   */
    (0x30000300UL),   /*!< DATA FLASH Page3   */
    (0x30000400UL),   /*!< DATA FLASH Page4   */
    (0x30000500UL),   /*!< DATA FLASH Page5   */
    (0x30000600UL),   /*!< DATA FLASH Page6   */
    (0x30000700UL),   /*!< DATA FLASH Page7   */
    (0x30000800UL),   /*!< DATA FLASH Page8   */
    (0x30000900UL),   /*!< DATA FLASH Page9   */
    (0x30000A00UL),   /*!< DATA FLASH Page10  */
    (0x30000B00UL),   /*!< DATA FLASH Page11  */
    (0x30000C00UL),   /*!< DATA FLASH Page12  */
    (0x30000D00UL),   /*!< DATA FLASH Page13  */
    (0x30000E00UL),   /*!< DATA FLASH Page14  */
    (0x30000F00UL),   /*!< DATA FLASH Page15  */
    (0x30001000UL),   /*!< DATA FLASH Page16  */
    (0x30001100UL),   /*!< DATA FLASH Page17  */
    (0x30001200UL),   /*!< DATA FLASH Page18  */
    (0x30001300UL),   /*!< DATA FLASH Page19  */
    (0x30001400UL),   /*!< DATA FLASH Page20  */
    (0x30001500UL),   /*!< DATA FLASH Page21  */
    (0x30001600UL),   /*!< DATA FLASH Page22  */
    (0x30001700UL),   /*!< DATA FLASH Page23  */
    (0x30001800UL),   /*!< DATA FLASH Page24  */
    (0x30001900UL),   /*!< DATA FLASH Page25  */
    (0x30001A00UL),   /*!< DATA FLASH Page26  */
    (0x30001B00UL),   /*!< DATA FLASH Page27  */
    (0x30001C00UL),   /*!< DATA FLASH Page28  */
    (0x30001D00UL),   /*!< DATA FLASH Page29  */
    (0x30001E00UL),   /*!< DATA FLASH Page30  */
    (0x30001F00UL),   /*!< DATA FLASH Page31  */
    (0x30002000UL)    /*!< DATA FLASH Page32  */
};

static uint32_t fc_const_data_flash_block_address[FC_MAX_DATA_BLOCKS] = {
    (0x30000000UL),   /*!< DATA FLASH Block0 */
    (0x30001000UL),   /*!< DATA FLASH Block1 */
    (0x30002000UL),   /*!< DATA FLASH Block2 */
    (0x30003000UL),   /*!< DATA FLASH Block3 */
    (0x30004000UL),   /*!< DATA FLASH Block4 */
    (0x30005000UL),   /*!< DATA FLASH Block5 */
    (0x30006000UL),   /*!< DATA FLASH Block6 */
    (0x30007000UL)    /*!< DATA FLASH Block7 */
};

static uint32_t fc_const_data_flash_area_address[FC_MAX_AREAS] = {
    (0x30000000UL)   /*!< DATA FLASH AREA0 */
};

/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup RDA_FLASH512UD32_Private_enum RDA_FLASH512UD32 Private Enum
 *  @{
 */

/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_Private_enum */


/*------------------------------------------------------------------------------*/
/*  Private Variables                                                           */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup RDA_FLASH512UD32_Private_variables RDA_FLASH512UD32 Private Variables
 *  @{
 */

/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_Private_variables */


/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @addtogroup RDA_FLASH512UD32_Private_functions RDA_FLASH512UD32 Private Functions
 *  @{
 */
__STATIC_INLINE uint32_t fc_enable_areasel(void);
__STATIC_INLINE uint32_t fc_disable_areasel(void);
__STATIC_INLINE uint32_t fc_enable_areasel4(void);
__STATIC_INLINE uint32_t fc_disable_areasel4(void);

static void     fc_write_command(uint32_t *src_address, uint32_t *dst_address, uint32_t size);
static uint32_t fc_write_data_flash(uint32_t *src_address, uint32_t *dst_address, uint32_t size);

static void     fc_erase_command(uint32_t *flash_top_address, uint32_t *erase_top_address, fc_erase_kind_t kind);

static uint32_t fc_get_status(fc_sr0_t status);
static uint32_t fc_verify_check(uint32_t *src_address, uint32_t *dst_address, uint32_t size);
static uint32_t fc_blank_check(uint32_t *address, uint32_t size);

static void     Copy_Routine(uint32_t *dest, uint32_t *source, uint32_t size);

static void     fc_protect_program_command(uint32_t protect_bit, fc_protect_kind_t kind);
static void     fc_protect_erase_command(void);

static void     fc_bank_change(uint32_t bank);

__STATIC_INLINE uint32_t fc_protect_mask_set(uint32_t protect_bit, fc_protect_kind_t kind);

/*--------------------------------------------------*/
/**
  * @brief  Read clock set.
  * @param  -
  * @retval none.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
void fc_read_clock_set(void)
{
    uint32_t regval = 0;

    SystemCoreClockUpdate();
    if (SystemCoreClock > SYSCORECLOCK_80M) {
        regval = (uint32_t)(FC_ACCR_FDLC_5 | FC_ACCR_FCLC_5);
        TSB_FC->KCR = FC_KCR_KEYCODE;
        TSB_FC->FCACCR = regval;
        while (TSB_FC->FCACCR != (uint32_t)(FC_ACCR_FDLC_5 | FC_ACCR_FCLC_5)) {
            /* no processing */
        }
    } else {
        regval = (uint32_t)(FC_ACCR_FDLC_4 | FC_ACCR_FCLC_4);
        TSB_FC->KCR = FC_KCR_KEYCODE;
        TSB_FC->FCACCR = regval;
        while (TSB_FC->FCACCR != (uint32_t)(FC_ACCR_FDLC_4 | FC_ACCR_FCLC_4)) {
            /* no processing */
        }
    }
}

/*--------------------------------------------------*/
/**
  * @brief  fc fixed clock set.
  * @param  sysclock    : system clock (Hz)
  * @retval none.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
void fc_fixed_clock_set(uint32_t sysclock)
{
    uint32_t regval = 0;

    if (sysclock > SYSCORECLOCK_80M) {
        regval = (uint32_t)(FC_ACCR_FDLC_5 | FC_ACCR_FCLC_5);
        TSB_FC->KCR = FC_KCR_KEYCODE;
        TSB_FC->FCACCR = regval;
        while (TSB_FC->FCACCR != (uint32_t)(FC_ACCR_FDLC_5 | FC_ACCR_FCLC_5)) {
            /* no processing */
        }
    } else {
        regval = (uint32_t)(FC_ACCR_FDLC_4 | FC_ACCR_FCLC_4);
        TSB_FC->KCR = FC_KCR_KEYCODE;
        TSB_FC->FCACCR = regval;
        while (TSB_FC->FCACCR != (uint32_t)(FC_ACCR_FDLC_4 | FC_ACCR_FCLC_4)) {
            /* no processing */
        }
    }
}

/*--------------------------------------------------*/
/**
  * @brief  Check FCSR0_RDYBSY.
  * @param  -
  * @retval BUSY = 0
  * @note
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t Get_param_FC_RDYBSY(void)
{
    uint32_t retval = 0 ;

    retval = TSB_FC->SR0 & FCSR0_RDYBSY_MASK ;

    return retval ;
}
#if  defined(_CC_ARM)
/*--------------------------------------------------*/
/**
  * @brief  copy fc_func.
  * @param  -
  * @retval -
  * @note
  */
/*--------------------------------------------------*/
void copy_fc_func(void)
{
    Copy_Routine(FLASH_API_RAM, FLASH_API_ROM, SIZE_FLASH_API);         /* copy flash API to RAM */

    return;
}

/*--------------------------------------------------*/
/**
  * @brief  copy user_data.
  * @param  -
  * @retval -
  * @note
  */
/*--------------------------------------------------*/
void copy_user_data(char *data_a, char *data_b, uint32_t size)
{
    /* copy A to RAM  */
    Copy_Routine(DEMO_A_RAM, (uint32_t *)data_a, size);

    /* copy B to RAM  */
    Copy_Routine(DEMO_B_RAM, (uint32_t *)data_b, size);

    return;
}

/*--------------------------------------------------*/
/**
  * @brief  copy user_program.
  * @param  -
  * @retval -
  * @note
  */
/*--------------------------------------------------*/
void copy_user_program(uint32_t size)
{
    /* copy A to RAM  */
    Copy_Routine(DEMO_A_RAM, DEMO_A_FLASH, size);

    /* copy B to RAM  */
    Copy_Routine(DEMO_B_RAM, DEMO_B_FLASH, size);

    return;
}

/*--------------------------------------------------*/
/**
  * @brief  rewrite_user_program.
  * @param  -
  * @retval -
  * @note
  */
/*--------------------------------------------------*/
uint32_t rewrite_user_program(uint32_t size)
{
    uint32_t ret;

    ret = fc_write_code_flash(DEMO_B_RAM, DEMO_A_FLASH, size);
    if (ret !=  TXZ_ERROR) {
        ret = fc_write_code_flash(DEMO_A_RAM, DEMO_B_FLASH, size);
        if (ret != TXZ_ERROR) {
            ret = TXZ_SUCCESS;
        }
    }

    return (ret);
}

#endif
/*--------------------------------------------------*/
/**
  * @brief  Auto protect erase command of the code flash ROM.
  * @param  -
  * @return Result.
  * @retval TXZ_SUCCESS : Success.
  * @retval TXZ_ERROR   : Failure.
  * @note   It works in the inner RAM.
  *         This function protect specified block of the code Flash ROM and checks a blank.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_protect_erase_code_flash(void)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        /* Checks the number of maximum blocks. */
        fc_protect_erase_command();
    } else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  fc_write_page_data_flash.
  * @param  -
  * @retval -
  * @note
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_write_page_data_flash(uint32_t page, uint32_t size)
{
    uint32_t ret;
    uint8_t  SetData[4], Data;
    uint32_t i;

    Data = 0;
    for (i = 0; i < size; i += 0x4U) {
        SetData[0] = Data++;
        SetData[1] = Data++;
        SetData[2] = Data++;
        SetData[3] = Data++;

        ret = fc_write_data_flash((uint32_t *)SetData,
                                  (uint32_t *)(fc_const_data_flash_address[page] + i),
                                  0x4);
    }

    return (ret);
}

/*--------------------------------------------------*/
/**
  * @brief  Auro page erase command of the user information area.
  * @param  -
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function erases specified page of the code Flash ROM and checks a blank.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_erase_user_information_area(uint32_t User_Information_Area)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        fc_bank_change(FC_BANK_USER_INFO);

        /* Erases the specific page. */
        fc_erase_command((uint32_t *)FC_CODE_FLASH_ADDRESS_TOP,
                         (uint32_t *)User_Information_Area,
                         FC_ERASE_KIND_PAGE);
        /* Checks a blank of the specific page. */
        if (fc_blank_check((uint32_t *)User_Information_Area, FC_PAGE_SIZE) == TXZ_ERROR) {
            retval = TXZ_ERROR;
        }

        fc_bank_change(FC_BANK_CODE_FLASH);
    } else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Raed of the user information area.
  * @param  dst_address  :destination address
  * @param  size         :data size
  * @retval -
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
void fc_read_user_information_area(uint32_t *src_address, uint32_t *dst_address, uint32_t size)
{
    fc_bank_change(FC_BANK_USER_INFO);

    {
        char *d = (char *)dst_address;
        char *s = (char *)src_address;
        int   i;
        for (i = 0; i < size; i++) {
            *d++ = *s++;
        }
    }

    fc_bank_change(FC_BANK_CODE_FLASH);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto write command of the user information area.
  * @param  src_address  :source address
  * @param  size         :data size
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function writes 16bytes data to the code Flash ROM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_write_user_information_area(uint32_t *src_address, uint32_t size, uint32_t User_Information_Area)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        uint32_t i;

        fc_bank_change(FC_BANK_USER_INFO);

        /* Checks the code Flash ROM status */
        for (i = 0; i < size; i += (uint32_t)(0x10UL)) {
            /* Writes 16bytes data. */
            fc_write_command((uint32_t *)((uint32_t)src_address + i),
                             (uint32_t *)((uint32_t)User_Information_Area - FC_CODE_FLASH_ADDRESS_TOP + i),
                             (uint32_t)(0x10UL));
        }

        /* Verifies user data and data of the Flash ROM. */
        retval = fc_verify_check(src_address, (uint32_t *)User_Information_Area, size);

        fc_bank_change(FC_BANK_CODE_FLASH);
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Enables the AREA0.
  * @param  -
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function write the FCAREASEL regiset.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
__STATIC_INLINE uint32_t fc_enable_areasel(void)
{
    uint32_t retval = TXZ_ERROR;
    uint32_t reg    = TSB_FC->AREASEL & FC_AREASEL_MASK_AREA0;

    reg |= FC_AREASEL_AREA0;
    /* Writes the FCKER register the KEYCODE. */
    TSB_FC->KCR = FC_KCR_KEYCODE;

    /* Selects the area0 */
    TSB_FC->AREASEL = reg;

    /* Confirms the FCAREASEL register the SSF0 was set. */
    while (1) {
        uint32_t i = TSB_FC->AREASEL;
        if ((i & FC_AREASEL_WRITE_MODE) == FC_AREASEL_WRITE_MODE) {
            retval = TXZ_SUCCESS;
            break;
        }
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Enables the AREA4.
  * @param  -
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function write the FCAREASEL regiset.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
__STATIC_INLINE uint32_t fc_enable_areasel4(void)
{
    uint32_t retval = TXZ_ERROR;
    uint32_t reg    = TSB_FC->AREASEL & FC_AREASEL_MASK_AREA4;

    reg |= FC_AREASEL_AREA4;
    /* Writes the FCKER register the KEYCODE. */
    TSB_FC->KCR = FC_KCR_KEYCODE;

    /* Selects the area4 */
    TSB_FC->AREASEL = reg;

    /* Confirms the FCAREASEL register the SSF4 was set. */
    while (1) {
        uint32_t i = TSB_FC->AREASEL;
        if ((i & FC_AREASEL4_WRITE_MODE) == FC_AREASEL4_WRITE_MODE) {
            retval = TXZ_SUCCESS;
            break;
        }
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Disables the AREA0.
  * @param  -
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function write the FCAREASEL regiset.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
__STATIC_INLINE uint32_t fc_disable_areasel(void)
{
    uint32_t retval = TXZ_ERROR;
    uint32_t reg    = TSB_FC->AREASEL & FC_AREASEL_MASK_AREA0;

    reg |= FC_AREASEL_EXPECT_AREA0;
    /* Writes the FCKER register the KEYCODE. */
    TSB_FC->KCR = FC_KCR_KEYCODE;

    /* Selects the area0 */
    TSB_FC->AREASEL = reg;

    /* Confirms the SSF0 of the FCAREASEL  register is not set. */
    while (1) {
        uint32_t i = TSB_FC->AREASEL;
        if ((i & FC_AREASEL_WRITE_MODE) != FC_AREASEL_WRITE_MODE) {
            retval = TXZ_SUCCESS;
            break;
        }
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Disables the AREA4.
  * @param  -
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function write the FCAREASEL regiset.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
__STATIC_INLINE uint32_t fc_disable_areasel4(void)
{
    uint32_t retval = TXZ_ERROR;


    /* Writes the FCKER register the KEYCODE. */
    TSB_FC->KCR = FC_KCR_KEYCODE;

    /* Selects the area4 */
    TSB_FC->AREASEL = FC_AREASEL_EXPECT_AREA4;

    /* Confirms the SSF0 of the FCAREASEL  register is not set. */
    while (1) {
        uint32_t i = TSB_FC->AREASEL;
        if ((i & FC_AREASEL4_WRITE_MODE) != FC_AREASEL4_WRITE_MODE) {
            retval = TXZ_SUCCESS;
            break;
        }
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Writes data of the Flash ROM.
  * @param  src_address
  * @param  dst_address
  * @param  size
  * @return -
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static void fc_write_command(uint32_t *src_address, uint32_t *dst_address, uint32_t size)
{
    uint32_t retval;
    volatile uint32_t *addr1;
    volatile uint32_t *addr2;
    volatile uint32_t *addr3;
    uint32_t *source = (uint32_t *) src_address;

    if ((uint32_t)dst_address >= 0x30000000) {
        addr1 = (uint32_t *)((uint32_t)FC_CODE_DATA_ADDRESS_TOP + FC_CMD_BC1_ADDR);
        addr2 = (uint32_t *)((uint32_t)FC_CODE_DATA_ADDRESS_TOP + FC_CMD_BC2_ADDR);
        addr3 = (uint32_t *)((uint32_t)dst_address);
        /* Enables the AREA4. Write Mode. */
        retval = fc_enable_areasel4();
    } else {
        addr1 = (uint32_t *)((uint32_t)FC_CODE_FLASH_ADDRESS_TOP + FC_CMD_BC1_ADDR);
        addr2 = (uint32_t *)((uint32_t)FC_CODE_FLASH_ADDRESS_TOP + FC_CMD_BC2_ADDR);
        addr3 = (uint32_t *)((uint32_t)FC_CODE_FLASH_ADDRESS_TOP + (uint32_t)dst_address);
        /* Enables the AREA0. Write Mode. */
        retval = fc_enable_areasel();
    }

    if (retval == TXZ_SUCCESS) {
        uint32_t i;

        *addr1 = (0x000000AAUL); /* bus cycle 1 */
        *addr2 = (0x00000055UL); /* bus cycle 2 */
        if ((uint32_t)dst_address >= 0x30000000) {
            *addr1 = (0x000000C0UL); /* bus cycle 3 */
        } else {
            *addr1 = (0x000000A0UL); /* bus cycle 3 */
        }
        for (i = (0UL); i < size; i += (0x4UL)) {
            *addr3 = *source;
            source++;
        }

        /* Confirmation of the works start of ROM. */
        while (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {};

        /* Waits for a finish of the works in the code Flash ROM. */
        while (fc_get_status(FC_SR0_RDYBSY) == TXZ_BUSY) {};
    }

    if ((uint32_t)addr1 > 0x5E000000) {
        /* Disables the AREA0. Read Mode. */
        retval = fc_disable_areasel();
    } else {
        /* Disables the AREA4. Read Mode. */
        retval = fc_disable_areasel4();
    }
}

/*--------------------------------------------------*/
/**
  * @brief  Verifies data of the Flash ROM.
  * @param  src_address  :source address
  * @param  dst_address  :destination address
  * @param  size         :data size
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static uint32_t fc_verify_check(uint32_t *src_address, uint32_t *dst_address, uint32_t size)
{
    char *s = (char *)src_address;
    char *d = (char *)dst_address;
    uint32_t i;
    for (i = 0; i < size; i++) {
        if (*s++ != *d++) {
            return (TXZ_ERROR);
        }
    }
    return (TXZ_SUCCESS);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto page erase command of the flash ROM.
  * @param  flash top address
  * @param  erase top address
  * @param  kind               : Chip, Area, Block, Page, etc.
  * @return -
  * @note   This function erases specified place of the flash ROM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static void fc_erase_command(uint32_t *flash_top_address, uint32_t *erase_top_address, fc_erase_kind_t kind)
{
    uint32_t retval;
    volatile uint32_t *addr1 = (uint32_t *)((uint32_t)flash_top_address + FC_CMD_BC1_ADDR);
    volatile uint32_t *addr2 = (uint32_t *)((uint32_t)flash_top_address + FC_CMD_BC2_ADDR);
    volatile uint32_t *addr3 = (uint32_t *) erase_top_address;

    if ((uint32_t)addr1 > 0x5E000000) {
        /* Enables the AREA0. Write Mode. */
        retval = fc_enable_areasel();
    } else {
        /* Enables the AREA4. Write Mode. */
        retval = fc_enable_areasel4();
    }

    if (retval == TXZ_SUCCESS) {
        *addr1 = (0x000000AAUL);
        *addr2 = (0x00000055UL);
        *addr1 = (0x00000080UL);
        *addr1 = (0x000000AAUL);
        *addr2 = (0x00000055UL);
        *addr3 = kind;

        /* Confirmation of the works start of ROM. */
        {
            uint32_t busy_count = 0;
            while (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
                if (++busy_count > FC_BUSY_MAX_WAIT) {
                    break;
                }
            };
        }

        /* Waits for a finish of the works in the code Flash ROM. */
        while (fc_get_status(FC_SR0_RDYBSY) == TXZ_BUSY) { };
    }
    if ((uint32_t)addr1 > 0x5E000000) {
        /* Disables the AREA0. Read Mode. */
        retval = fc_disable_areasel();
    } else {
        /* Disables the AREA4. Read Mode. */
        retval = fc_disable_areasel4();
    }
}

/*--------------------------------------------------*/
/**
  * @brief  Checks a blank of the Flash ROM every 4bytes.
  * @param  addrress
  * @param  size
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static uint32_t fc_blank_check(uint32_t *address, uint32_t size)
{
    uint32_t retval = TXZ_SUCCESS;
    uint32_t i;

    for (i = 0; i < (size / sizeof(uint32_t)); i++) {
        uint32_t *addr = &address[i];
        if (*addr != FC_BRANK_VALUE) {
            retval = TXZ_ERROR;
            break;
        }
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Get the status of the flash auto operation.
  * @param  status
  * @return Result.
  * @retval TXZ_BUSY : Busy.
  * @retval TXZ_DONE : Done.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static uint32_t fc_get_status(fc_sr0_t status)
{
    uint32_t retval;
    uint32_t work32;

    /* Reads the FCSR0. Masks the other specfic status */
    work32 = TSB_FC->SR0 & (uint32_t)status;

    /* Confirms the specific status of the flash ROM */
    if (work32 == (uint32_t)status) {
        retval = TXZ_DONE;
    } else {
        retval = TXZ_BUSY;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto write command of the code flash ROM.
  * @param  src_address  :source address
  * @param  dst_address  :destination address
  * @param  size         :data size
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function writes 16bytes data to the code Flash ROM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_write_code_flash(uint32_t *src_address, uint32_t *dst_address, uint32_t size)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        uint32_t i;
        /* Checks the code Flash ROM status */
        for (i = 0; i < size; i += (uint32_t)(0x10UL)) {
            /* Writes 16bytes data. */
            fc_write_command((uint32_t *)((uint32_t)src_address + i),
                             (uint32_t *)((uint32_t)dst_address + i),
                             (uint32_t)(0x10UL));
        }

        /* Verifies user data and data of the Flash ROM. */
        retval = fc_verify_check(src_address, dst_address, size);
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto page erase command of the code flash ROM.
  * @param  first_page    : The first page to erase
  * @param  num_of_pages  : The number of pages to erase.
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function erases specified page of the code Flash ROM and checks a blank.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_erase_page_code_flash(fc_code_flash_page_number_t first_page, uint8_t num_of_pages)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        /* Checks the number of maximum pages. */
        if ((first_page + num_of_pages) <= FC_MAX_PAGES) {
            uint8_t i;
            for (i = 0; i < num_of_pages; i++) {
                /* Erases the specific page. */
                fc_erase_command((uint32_t *)FC_CODE_FLASH_ADDRESS_TOP,
                                 (uint32_t *)fc_const_code_flash_address[first_page + i],
                                 FC_ERASE_KIND_PAGE);
            }
            /* Checks a blank of the specific page. */
            if (fc_blank_check((uint32_t *)fc_const_code_flash_address[first_page],
                               FC_PAGE_SIZE * (uint32_t)num_of_pages) == TXZ_ERROR) {
                retval = TXZ_ERROR;
            }
        } else {
            retval = TXZ_ERROR;
        }
    } else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  copy 32-bit data from source to dest
  * @param  the address of source and dast, the data size
  * @retval None.
  * @note   -
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static void Copy_Routine(uint32_t *dest, uint32_t *source, uint32_t size)
{
    uint32_t *dest_addr, *source_addr, tmpsize;
    uint32_t i, tmps, tmpd, mask;

    dest_addr   = dest;
    source_addr = source;

    tmpsize = size >> 2U;
    for (i = 0U; i < tmpsize; i++) {   /* 32bits copy */
        *dest_addr = *source_addr;
        dest_addr++;
        source_addr++;
    }
    if (size & 0x00000003U) {  /* if the last data size is not 0(maybe 1,2 or 3), copy the last data */
        mask = 0xFFFFFF00U;
        i = size & 0x00000003U;
        tmps = *source_addr;
        tmpd = *dest_addr;
        while (i - 1U) {
            mask = mask << 8U;
            i--;
        }
        tmps = tmps & (~mask);
        tmpd = tmpd & (mask);
        *dest_addr = tmps + tmpd;       /* 32bits copy, but only change the bytes need to be changed */
    } else {
        /* Do nothing */
    }
}

/*--------------------------------------------------*/
/**
  * @brief  Auto page erase command of the data flash.
  * @param  first_page    : The first page to erase
  * @param  num_of_pages  : The number of pages to erase.
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function erases specified page of the data Flash and checks a blank.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_erase_page_data_flash(fc_data_flash_page_number_t first_page, uint8_t num_of_pages)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the data Flash status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        /* Checks the number of maximum pages. */
        if ((first_page + num_of_pages) <= FC_MAX_DATA_PAGES) {
            uint8_t i;
            for (i = 0; i < num_of_pages; i++) {
                /* Erases the specific page. */
                fc_erase_command((uint32_t *)FC_DATA_FLASH_ADDRESS_TOP,
                                 (uint32_t *)fc_const_data_flash_address[first_page + i],
                                 FC_ERASE_KIND_PAGE);
            }
            /* Checks a blank of the specific page. */
            if (fc_blank_check((uint32_t *)fc_const_data_flash_address[first_page],
                               FC_DATA_PAGE_SIZE * (uint32_t)num_of_pages) == TXZ_ERROR) {
                retval = TXZ_ERROR;
            }
        } else {
            retval = TXZ_ERROR;
        }
    } else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto write command of the data flash.
  * @param  src_address  :source address
  * @param  dst_address  :destination address
  * @param  size         :data size
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function writes 16bytes data to the DATA Flash.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static uint32_t fc_write_data_flash(uint32_t *src_address, uint32_t *dst_address, uint32_t size)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the code DATA Flash status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        uint32_t i;
        /* Checks the DATA Flash status */
        for (i = 0; i < size; i += (uint32_t)(0x4UL)) {
            /* Writes 4bytes data. */
            fc_write_command((uint32_t *)((uint32_t)src_address + i),
                             (uint32_t *)((uint32_t)dst_address + i),
                             (uint32_t)(0x4UL));
        }
        /* Verifies user data and data of the Flash ROM. */
        retval = fc_verify_check(src_address, dst_address, size);
    } else {
        retval = TXZ_BUSY;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto block erase command of the data flash.
  * @param  first_block      : The first block to erase
  * @param  num_of_block     : The number of blocks to erase.
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function erases specified block of the data Flash and checks a blank.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_erase_block_data_flash(fc_data_flash_block_number_t first_block, uint8_t num_of_block)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the data Flash status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        if ((first_block + num_of_block) <= FC_MAX_DATA_BLOCKS) {
            uint8_t i;
            for (i = 0; i < num_of_block; i++) {
                /* Erases the specific block. */
                fc_erase_command((uint32_t *)FC_DATA_FLASH_ADDRESS_TOP,
                                 (uint32_t *)fc_const_data_flash_block_address[first_block + i],
                                 FC_ERASE_KIND_BLOCK);
            }
            /* Checks a blank of the specific block. */
            if (fc_blank_check((uint32_t *)fc_const_data_flash_block_address[first_block],
                               FC_DATA_BLOCK_SIZE * (uint32_t)num_of_block) == TXZ_ERROR) {
                retval = TXZ_ERROR;
            }
        } else {
            retval = TXZ_ERROR;
        }
    } else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto area erase command of the data flash.
  * @param  area      : The area block to erase
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function erases specified block of the data Flash and checks a blank.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_erase_area_data_flash(fc_data_flash_area_number_t area)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the data Flash status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        /* Checks the number of maximum blocks. */
        if (area == 0) {
            /* Erases the specific block. */
            fc_erase_command((uint32_t *)FC_DATA_FLASH_ADDRESS_TOP,
                             (uint32_t *)fc_const_data_flash_area_address[area],
                             FC_ERASE_KIND_AREA);
            /* Checks a blank of the specific block. */
            if (fc_blank_check((uint32_t *)fc_const_data_flash_area_address[area], FC_DATA_AREA_SIZE) == TXZ_ERROR) {
                retval = TXZ_ERROR;
            }
        } else {
            retval = TXZ_ERROR;
        }
    } else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto protect page program command of the code flash ROM.
  * @param  page      : The page to protect program
  * @return Result.
  * @retval TXZ_SUCCESS : Success.
  * @retval TXZ_ERROR   : Failure.
  * @note   It works in the inner RAM.
  *         This function protect specified block of the code Flash ROM and checks a blank.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_protect_page_code_flash(fc_code_flash_page_number_t page)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        /* Checks the number of maximum blocks. */
        if (page <= FC_CODE_FLASH_PAGE7) {
            fc_protect_program_command(page, FC_PROTECT_KIND_PAGE);
        } else {
            retval = TXZ_ERROR;
        }
    } else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto protect block program command of the code flash ROM.
  * @param  block      : The block to protect program
  * @return Result.
  * @retval TXZ_SUCCESS : Success.
  * @retval TXZ_ERROR   : Failure.
  * @note   It works in the inner RAM.
  *         This function protect specified block of the code Flash ROM and checks a blank.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_protect_block_code_flash(fc_code_flash_block_number_t block)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        /* Checks the number of maximum blocks. */
        if ((block >= FC_CODE_FLASH_BLOCK1) && (block <= FC_CODE_FLASH_BLOCK3)) {
            fc_protect_program_command(block, FC_PROTECT_KIND_BLOCK);
        } else {
            retval = TXZ_ERROR;
        }
    } else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto ptotect bit program command of the flash ROM.
  * @param  protect_bit
  * @param  kind
  * @return -
  * @note   This function protect specified place of the flash ROM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static void fc_protect_program_command(uint32_t protect_bit, fc_protect_kind_t kind)
{
    uint32_t retval = TXZ_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *)((uint32_t)FC_CODE_FLASH_ADDRESS_TOP + FC_CMD_BC1_ADDR);
    volatile uint32_t *addr2 = (uint32_t *)((uint32_t)FC_CODE_FLASH_ADDRESS_TOP + FC_CMD_BC2_ADDR);
    volatile uint32_t *addr3;

    if (kind == FC_PROTECT_KIND_PAGE) {
        if (protect_bit <= 7) {
            addr3 = (uint32_t *)(FC_CODE_FLASH_ADDRESS_TOP + FC_PROTECT_ADDR + (protect_bit << 4));
        } else {
            retval = TXZ_ERROR;
        }
    } else if (kind == FC_PROTECT_KIND_BLOCK) {
        if ((protect_bit >= 1) && (protect_bit <= 3)) {
            addr3 = (uint32_t *)(FC_CODE_FLASH_ADDRESS_TOP + FC_PROTECT_ADDR +  0x80 + ((protect_bit - 1) << 4));
        } else {
            retval = TXZ_ERROR;
        }
    } else {
        retval = TXZ_ERROR;
    }

    if (retval == TXZ_SUCCESS) {
        retval = fc_protect_mask_set(protect_bit, kind);
        /* Enables the AREA0. Write Mode. */
        retval = fc_enable_areasel();

        if (retval == TXZ_SUCCESS) {
            *addr1 = (0x000000AAUL);
            *addr2 = (0x00000055UL);
            *addr1 = (0x0000009AUL);
            *addr3 = (0x0000009AUL);

            /* Confirmation of the works start of ROM. */
            while (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {};

            /* Waits for a finish of the works in the code Flash ROM. */
            while (fc_get_status(FC_SR0_RDYBSY) == TXZ_BUSY) {};
        }
        /* Disables the AREA0. Read Mode. */
        retval = fc_disable_areasel();
    }
}

/*--------------------------------------------------*/
/**
  * @brief  Auto ptotect bit erase command of the flash ROM.
  * @param  -
  * @return -
  * @note   This function protect specified place of the flash ROM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static void fc_protect_erase_command(void)
{
    uint32_t retval = TXZ_SUCCESS;
    volatile uint32_t *addr1 = (uint32_t *)((uint32_t)FC_CODE_FLASH_ADDRESS_TOP + FC_CMD_BC1_ADDR);
    volatile uint32_t *addr2 = (uint32_t *)((uint32_t)FC_CODE_FLASH_ADDRESS_TOP + FC_CMD_BC2_ADDR);
    volatile uint32_t *addr3 = (uint32_t *)(FC_CODE_FLASH_ADDRESS_TOP           + FC_PROTECT_ADDR);

    {
        /* Enables the AREA0. Write Mode. */
        retval = fc_enable_areasel();

        if (retval == TXZ_SUCCESS) {
            *addr1 = (0x000000AAUL);
            *addr2 = (0x00000055UL);
            *addr1 = (0x00000080UL);
            *addr1 = (0x000000AAUL);
            *addr2 = (0x00000055UL);
            *addr3 = (0x00000060UL);

            /* Confirmation of the works start of ROM. */
            while (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {};

            /* Waits for a finish of the works in the code Flash ROM. */
            while (fc_get_status(FC_SR0_RDYBSY) == TXZ_BUSY) {};
        }
        /* Disables the AREA0. Read Mode. */
        retval = fc_disable_areasel();
    }
}

/*--------------------------------------------------*/
/**
  * @brief  Protect Mask Set.
  * @param  protect_bit
  * @param  kind
  * @return Result.
  * @retval TXZ_SUCCESS : Success.
  * @retval TXZ_ERROR   : Failure.
  * @note   It works in the inner RAM.
  *         This function write the FCAREASEL regiset.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
__STATIC_INLINE uint32_t fc_protect_mask_set(uint32_t protect_bit, fc_protect_kind_t kind)
{
    uint32_t retval = TXZ_ERROR;
    uint32_t reg = 0xFFFFFFFF;

    if (kind == FC_PROTECT_KIND_PAGE) {
        reg = (TSB_FC->PMR0) | (1 << protect_bit);
        /* Writes the FCKER register the KEYCODE. */
        TSB_FC->KCR = FC_KCR_KEYCODE;

        /* Selects the area0 */
        TSB_FC->PMR0 = reg;

        retval = TXZ_SUCCESS;
    } else if (kind == FC_PROTECT_KIND_BLOCK) {
        reg = (TSB_FC->PMR1) | (1 << protect_bit);
        /* Writes the FCKER register the KEYCODE. */
        TSB_FC->KCR = FC_KCR_KEYCODE;

        /* Selects the area0 */
        TSB_FC->PMR1 = reg;

        retval = TXZ_SUCCESS;
    }
    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto block erase command of the code flash ROM.
  * @param  first_block      : The first block to erase
  * @param  num_of_block     : The number of blocks to erase.
  * @return Result.
  * @retval TXZ_SUCCESS : Success.
  * @retval TXZ_ERROR   : Failure.
  * @note   It works in the inner RAM.
  *         This function erases specified block of the code Flash ROM and checks a blank.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
uint32_t fc_erase_block_code_flash(fc_code_flash_block_number_t first_block, uint8_t num_of_block)
{
    uint32_t retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if (fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE) {
        if ((first_block + num_of_block) <= FC_MAX_BLOCKS) {
            uint8_t i;
            for (i = 0; i < num_of_block; i++) {
                /* Erases the specific block. */
                fc_erase_command((uint32_t *)FC_CODE_FLASH_ADDRESS_TOP,
                                 (uint32_t *)fc_const_code_flash_block_address[first_block + i],
                                 FC_ERASE_KIND_BLOCK);
            }
            /* Checks a blank of the specific block. */
            if (fc_blank_check((uint32_t *)fc_const_code_flash_block_address[first_block], FC_BLOCK_SIZE * (uint32_t)num_of_block) == TXZ_ERROR) {
                retval = TXZ_ERROR;
            }
        } else {
            retval = TXZ_ERROR;
        }
    } else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  bank change.
  * @param  bank : FC_BANK_USER_INFO, FC_BANK_CODE_FLASH
  * @return -
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
static void fc_bank_change(uint32_t bank)
{
    if (bank == FC_BANK_USER_INFO) {
        /* Flash Buffer disable */
        TSB_FC->BUFDISCLR = FC_BUFFER_DISABLE;
        while (TSB_FC->BUFDISCLR != FC_BUFFER_DISABLE);
        /* change user information area */
        TSB_FC->BNKCR = FC_BANK_USER_INFO;
        while (TSB_FC->BNKCR != FC_BANK_USER_INFO);
    } else if (bank == FC_BANK_CODE_FLASH) {
        /* change code flash */
        TSB_FC->BNKCR = FC_BANK_CODE_FLASH;
        while (TSB_FC->BNKCR != FC_BANK_CODE_FLASH);
        /* Flash Buffer enable */
        TSB_FC->BUFDISCLR = FC_BUFFER_ENABLE;
        while (TSB_FC->BUFDISCLR != FC_BUFFER_ENABLE);
    } else {
        /* do nothing */
    }
}

/*--------------------------------------------------*/


/**
 *  @}
 */ /* End of group RDA_FLASH512UD32_Private_functions */

/**
 *  @}
 */ /* End of group RDA_FLASH512UD32 */

/**
 *  @}
 */ /* End of group Periph_Driver */

#endif /* defined(__FLASH_H) */

#ifdef __cplusplus
}
#endif /* __cplusplus */
