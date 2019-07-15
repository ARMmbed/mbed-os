/**
 *******************************************************************************
 * @file    tmpm3hq_fc.c
 * @brief   Flash_Userboot Sample Application.
 * @version V1.0.1.0
 * $Date:: 2016-11-28 08:16:17 #$
 *
 * DO NOT USE THIS SOFTWARE WITHOUT THE SOFTWARE LICENSE AGREEMENT.
 *
 * (C)Copyright TOSHIBA CORPORATION 2016 All rights reserved
 *******************************************************************************
 */
#ifdef __cplusplus
 extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include <string.h>
#include "tmpm3hq_fc.h"
#include "TMPM3HQ.h"

/**
 *  @addtogroup Example
 *  @{
 */

/**
 *  @defgroup Flash_Userboot Flash_Userboot Sample Appli
 *  @{
 */


/*------------------------------------------------------------------------------*/
/*  Macro Function                                                              */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup Flash_Userboot_Private_macro Flash_Userboot Private Macro
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group Flash_Userboot_Private_macro */


/*------------------------------------------------------------------------------*/
/*  Configuration                                                               */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup Flash_Userboot_Private_define Flash_Userboot Private Define
 *  @{
 */

/**
 *  @}
 */ /* End of group Flash_Userboot_Private_define */


/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup Flash_Userboot_Private_define Flash_Userboot Private Define
 *  @{
 */
#define FC_KCR_KEYCODE      (0xA74A9D23UL)              /*!< The specific code that writes the FCKCR register. */
#define FC_BRANK_VALUE      (uint32_t)(0xFFFFFFFFUL)    /*!< Brank value */
#define FC_MAX_PAGES        (uint8_t)(0x20)             /*!< Maxmum pages */
#define FC_CMD_ADDRESS_MASK (uint32_t)(0xFFFF0000UL)    /*!< Upper address mask of the upper address */
#define FC_CMD_BC1_ADDR     (0x00000550UL)           /*!< The lower address of the first bus cycle when uses commans */
#define FC_CMD_BC2_ADDR     (0x00000AA0UL)           /*!< The lower address of the second bus cycle when uses commans */

/* FCSR0 register */
#define     FC_AREASEL_EXPECT_AREA0 (uint32_t)(0x00000000UL)    /*!< RW, Selects expect area0       */
#define     FC_AREASEL_AREA0        (uint32_t)(0x00000007UL)    /*!< RW, Selects area0              */
#define     FC_AREASEL_MASK_AREA0   (uint32_t)(0xFFF8FFF8UL)    /*!< RW, Selects area0              */
#define     FC_AREASEL_WRITE_MODE   (uint32_t)(0x04000000UL)    /*!< R,  Write Mode                 */

static uint32_t fc_const_code_flash_address[FC_MAX_PAGES] = {
    (0x5E000000UL),   /*!< CODE FLASH Page0 */
    (0x5E001000UL),   /*!< CODE FLASH Page1 */
    (0x5E002000UL),   /*!< CODE FLASH Page2 */
    (0x5E003000UL),   /*!< CODE FLASH Page3 */
    (0x5E004000UL),   /*!< CODE FLASH Page4 */
    (0x5E005000UL),   /*!< CODE FLASH Page5 */
    (0x5E006000UL),   /*!< CODE FLASH Page6 */
    (0x5E007000UL),   /*!< CODE FLASH Page7 */
    (0x5E008000UL),   /*!< CODE FLASH Page8 */
    (0x5E009000UL),   /*!< CODE FLASH Page9 */
    (0x5E00A000UL),   /*!< CODE FLASH Page10 */
    (0x5E00B000UL),   /*!< CODE FLASH Page11 */
    (0x5E00C000UL),   /*!< CODE FLASH Page12 */
    (0x5E00D000UL),   /*!< CODE FLASH Page13 */
    (0x5E00E000UL),   /*!< CODE FLASH Page14 */
    (0x5E00F000UL),   /*!< CODE FLASH Page15 */
    (0x5E010000UL),   /*!< CODE FLASH Page16 */
    (0x5E011000UL),   /*!< CODE FLASH Page17 */
    (0x5E012000UL),   /*!< CODE FLASH Page18 */
    (0x5E013000UL),   /*!< CODE FLASH Page19 */
    (0x5E014000UL),   /*!< CODE FLASH Page20 */
    (0x5E015000UL),   /*!< CODE FLASH Page21 */
    (0x5E016000UL),   /*!< CODE FLASH Page22 */
    (0x5E017000UL),   /*!< CODE FLASH Page23 */
    (0x5E018000UL),   /*!< CODE FLASH Page24 */
    (0x5E019000UL),   /*!< CODE FLASH Page25 */
    (0x5E01A000UL),   /*!< CODE FLASH Page26 */
    (0x5E01B000UL),   /*!< CODE FLASH Page27 */
    (0x5E01C000UL),   /*!< CODE FLASH Page28 */
    (0x5E01D000UL),   /*!< CODE FLASH Page29 */
    (0x5E01E000UL),   /*!< CODE FLASH Page30 */
    (0x5E01F000UL)    /*!< CODE FLASH Page31 */
};

/**
 *  @}
 */ /* End of group Flash_Userboot_Private_define */


/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup Flash_Userboot_Private_define Flash_Userboot Private Define
 *  @{
 */
/**
 *  @defgroup Flash_Userboot_Private_typedef Flash_Userboot Private Typedef
 *  @{
 */

/**
 *  @}
 */ /* End of group Flash_Userboot_Private_typedef */


/*------------------------------------------------------------------------------*/
/*  Private Member                                                              */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup Flash_Userboot_Private_variables Flash_Userboot Private Variables
 *  @{
 */
/**
 *  @}
 */ /* End of group Flash_Userboot_Private_variables */


/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup Flash_Userboot_Private_fuctions Flash_Userboot Private Fuctions
 *  @{
 */
__STATIC_INLINE TXZ_Result fc_enable_areasel(void);
__STATIC_INLINE TXZ_Result fc_disable_areasel(void);
static void fc_write_command(uint32_t* src_address, uint32_t* dst_address, uint32_t size);
static TXZ_Result fc_verify_check(uint32_t* src_address, uint32_t* dst_address, uint32_t size);
static void fc_erase_command(uint32_t* flash_top_address, uint32_t* erase_top_address, fc_erase_kind_t kind);

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
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
__STATIC_INLINE TXZ_Result fc_enable_areasel(void)
{
    TXZ_Result retval = TXZ_ERROR;

    /* Writes the FCKER register the KEYCODE. */
    TSB_FC->KCR = FC_KCR_KEYCODE;

    /* Selects the area0 */
    {
        uint32_t reg = TSB_FC->AREASEL & FC_AREASEL_MASK_AREA0;
        TSB_FC->AREASEL = reg | FC_AREASEL_AREA0;
    }

    /* Confirms the FCAREASEL register the SSF0 was set. */
    while(1){
        uint32_t i = TSB_FC->AREASEL;
        if((i & FC_AREASEL_WRITE_MODE) == FC_AREASEL_WRITE_MODE){
            retval = TXZ_SUCCESS;
            break;
        }
    }

    return(retval);
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
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
__STATIC_INLINE TXZ_Result fc_disable_areasel(void)
{
    TXZ_Result retval = TXZ_ERROR;

    /* Writes the FCKER register the KEYCODE. */
    TSB_FC->KCR = FC_KCR_KEYCODE;

    /* Selects the area0 */
    {
        uint32_t reg = TSB_FC->AREASEL & FC_AREASEL_MASK_AREA0;
        TSB_FC->AREASEL = reg | FC_AREASEL_EXPECT_AREA0;
    }

    /* Confirms the SSF0 of the FCAREASEL  register is not set. */
    while(1){
        uint32_t i = TSB_FC->AREASEL;
        if((i & FC_AREASEL_WRITE_MODE) != FC_AREASEL_WRITE_MODE){
            retval = TXZ_SUCCESS;
            break;
        }
    }

    return(retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Writes data of the Flash ROM.
  * @param  uint32_t*   : src_address
  * @param  uint32_t*   : dst_address
  * @param  uint32_t    : size
  * @return -
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
static void fc_write_command(uint32_t* src_address, uint32_t* dst_address, uint32_t size)
{
    TXZ_Result retval;
    volatile uint32_t* addr1 = (uint32_t *) (FC_CODE_FLASH_ADDRESS_TOP + FC_CMD_BC1_ADDR);
    volatile uint32_t* addr2 = (uint32_t *) (FC_CODE_FLASH_ADDRESS_TOP + FC_CMD_BC2_ADDR);
    volatile uint32_t* addr3 = (uint32_t *) ((uint32_t)dst_address + FC_CODE_FLASH_ADDRESS_TOP);
    uint32_t* source = (uint32_t *) src_address;

    /* Enables the AREA0. Write Mode. */
    retval = fc_enable_areasel();

    if(retval == TXZ_SUCCESS){
        uint32_t i;

        *addr1 = (0x000000AAUL); /* bus cycle 1 */
        *addr2 = (0x00000055UL); /* bus cycle 2 */
        *addr1 = (0x000000A0UL); /* bus cycle 3 */
        for(i=(0UL); i<size; i+=(0x4UL)){
            *addr3 = *source;
            source++;
        }

        /* Confirmation of the works start of ROM. */
        while(fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE){
        };

        /* Waits for a finish of the works in the code Flash ROM. */
        while(fc_get_status(FC_SR0_RDYBSY) == TXZ_BUSY){
        };
    }

    /* Disables the AREA0. Read Mode. */
    retval = fc_disable_areasel();
}

/*--------------------------------------------------*/
/**
  * @brief  Verifies data of the Flash ROM.
  * @param  uint32_t*   : src_address
  * @param  uint32_t*   : dst_address
  * @param  uint32_t    : size
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
static TXZ_Result fc_verify_check(uint32_t* src_address, uint32_t* dst_address, uint32_t size)
{
    TXZ_Result retval = TXZ_ERROR;
    int res = memcmp(src_address, dst_address, size);
    if(res == 0){
        retval = TXZ_SUCCESS;
    }

    return(retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auro page erase command of the flash ROM.
  * @param  uint32_t* flash_top_address : flash top address
  * @param  uint32_t* erase_top_address : erase top address
  * @param  fc_erase_kind_t kind        : Chip, Area, Block, Page, etc.
  * @return -
  * @note   This function erases specified place of the flash ROM.
  */
/*--------------------------------------------------*/
//static void fc_erase_pages_flash(uint32_t* top_address, uint32_t* erase_top_address)
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
static void fc_erase_command(uint32_t* flash_top_address, uint32_t* erase_top_address, fc_erase_kind_t kind)
{
    TXZ_Result retval;
    volatile uint32_t *addr1 = (uint32_t *) ((uint32_t)flash_top_address + FC_CMD_BC1_ADDR);
    volatile uint32_t *addr2 = (uint32_t *) ((uint32_t)flash_top_address + FC_CMD_BC2_ADDR);
    volatile uint32_t *addr3 = (uint32_t *) erase_top_address;

    /* Enables the AREA0. Write Mode. */
    retval = fc_enable_areasel();

    if(retval == TXZ_SUCCESS){
        *addr1 = (0x000000AAUL);
        *addr2 = (0x00000055UL);
        *addr1 = (0x00000080UL);
        *addr1 = (0x000000AAUL);
        *addr2 = (0x00000055UL);
        *addr3 = kind;

        /* Confirmation of the works start of ROM. */
        while(fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE){
        };

        /* Waits for a finish of the works in the code Flash ROM. */
        while(fc_get_status(FC_SR0_RDYBSY) == TXZ_BUSY){
        };
    }

    /* Disables the AREA0. Read Mode. */
    retval = fc_disable_areasel();

}

/*--------------------------------------------------*/
/**
  * @brief  Checks a blank of the Flash ROM every 4bytes.
  * @param  uint32_t*   : addrress
  * @param  uint32_t    : size
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
static TXZ_Result fc_blank_check(uint32_t* address, uint32_t size)
{
    TXZ_Result retval = TXZ_SUCCESS;

    uint32_t i;

    for(i=0; i<(size/sizeof(uint32_t)); i++){
        uint32_t* addr = &address[i];
        if(*addr != FC_BRANK_VALUE){
            retval = TXZ_ERROR;
            break;
        }
    }

    return (retval);
}

/**
 *  @}
 */ /* End of group Flash_Userboot_Private_fuctions */


/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/**
 *  @defgroup Flash_Userboot_Exported_functions Flash_Userboot Exported Functions
 *  @{
 */
/*--------------------------------------------------*/
/**
  * @brief  Get the status of the flash auto operation.
  * @param  fc_sr0_t : status
  * @return Result.
  * @retval TXZ_BUSY : Busy.
  * @retval TXZ_DONE : Done.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
TXZ_WorkState fc_get_status(fc_sr0_t status)
{
    TXZ_WorkState retval = TXZ_BUSY;
    uint32_t work32;

    /* Reads the FCSR0. Masks the other specfic status */
    work32 = TSB_FC->SR0 & (uint32_t)status;

    /* Confirms the specific status of the flash ROM */
    if(work32 == (uint32_t)status){
        retval = TXZ_DONE;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auto write command of the code flash ROM.
  * @param  uint32_t*   : src_address
  * @param  uint32_t*   : dst_address
  * @param  uint32_t    : size
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
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
TXZ_Result fc_write_code_flash(uint32_t* src_address, uint32_t* dst_address, uint32_t size)
{
    TXZ_Result retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if(fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE){

        uint32_t i;
        /* Checks the code Flash ROM status */
        for(i=0;i<size;i+=(uint32_t)(0x10UL)){
            /* Writes 16bytes data. */
            fc_write_command((uint32_t*)((uint32_t)src_address+i), (uint32_t*)((uint32_t)dst_address+i), (uint32_t)(0x10UL));
        }

        /* Verifies user data and data of the Flash ROM. */
        retval = fc_verify_check(src_address, dst_address, size);
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Auro page erase command of the code flash ROM.
  * @param  first_page      : The first page to erase
  * @param  num_of_pages    : The number of pages to erase.
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  *         This function erases specified page of the code Flash ROM and checks a blank.
  */
/*--------------------------------------------------*/
TXZ_Result fc_erase_page_code_flash(fc_code_flash_page_number_t first_page, uint8_t num_of_pages)
{
    TXZ_Result retval = TXZ_SUCCESS;

    /* Checks the code Flash ROM status */
    if(fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE){
        /* Checks the number of maximum pages. */
        if((first_page + num_of_pages) <= FC_MAX_PAGES){
            uint8_t i;
            for(i=0; i<num_of_pages ; i++){
                /* Erases the specific page. */
                fc_erase_command((uint32_t*)FC_CODE_FLASH_ADDRESS_TOP,
                                (uint32_t*)fc_const_code_flash_address[first_page+i],
                                FC_ERASE_KIND_PAGE);
            }
            /* Checks a blank of the specific page. */
            if(fc_blank_check((uint32_t*)fc_const_code_flash_address[first_page],
                              FC_PAGE_SIZE*(uint32_t)num_of_pages) == TXZ_ERROR){
                retval = TXZ_ERROR;
            }
        }
        else{
            retval = TXZ_ERROR;
        }
    }
    else {
        retval = TXZ_ERROR;
    }

    return (retval);
}

/*--------------------------------------------------*/
/**
  * @brief  Checks a blank of the code Flash ROM of specified pages.
  * @param  first_page      : The first page which checks a blank.
  * @param  last_page       : The last page which checks a blank..
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
TXZ_Result fc_blank_check_page_code_flash(fc_code_flash_page_number_t first_page, fc_code_flash_page_number_t last_page)
{
    TXZ_Result retval;

    uint32_t* address = (uint32_t*)fc_const_code_flash_address[first_page];
    uint32_t size = ((uint32_t)(last_page - first_page + 1) * (uint32_t)FC_PAGE_SIZE);

    retval = fc_blank_check(address, size);

    return (retval);
}

/*--------------------------------------------------*/
/***************  written by TSIP  ******************/
/**
  * @brief  Erases the entire block of code Flash ROM of specified address.
  * @param  uint32_t *top_addr      : top address of Flash ROM.
  * @param  uint32_t *blk_addr      : start address of block to be erased.
  * @return Result.
  * @retval TXZ_SUCCESS :Success.
  * @retval TXZ_ERROR   :Failure.
  * @note   It works in the inner RAM.
  */
/*--------------------------------------------------*/
#if defined ( __GNUC__ )    /* GCC Compiler */
__attribute__((section(".ram_func")))
#endif
#if defined ( __ICCARM__ )      // IAR Compiler
__ramfunc
#endif
TXZ_Result fc_erase_block_code_flash(uint32_t *top_addr, uint32_t *blk_addr)
{
    TXZ_Result retval = TXZ_SUCCESS;
    /* Checks the code Flash ROM status */
    if(fc_get_status(FC_SR0_RDYBSY) == TXZ_DONE){
        /* Erases the specific block. */
        fc_erase_command((uint32_t*)FC_CODE_FLASH_ADDRESS_TOP, blk_addr, FC_ERASE_KIND_BLOCK);
        /* Checks a blank of the specific block. */
        if(fc_blank_check(blk_addr, (uint32_t)0x8000) == TXZ_ERROR){
            retval = TXZ_ERROR;
        } else{
       // do nothing
        }
    }
    else {
        retval = TXZ_ERROR;
    }

    return (retval);
}


/**
 *  @}
 */ /* End of group Flash_Userboot_Exported_functions */

/**
 *  @}
 */ /* End of group Flash_Userboot */

/**
 *  @}
 */ /* End of group Example */

#ifdef __cplusplus
}
#endif /* __cplusplus */
