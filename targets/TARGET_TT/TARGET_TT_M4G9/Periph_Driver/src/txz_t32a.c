    /**
 *******************************************************************************
 * @file    txz_t32a.c
 * @brief   This file provides API functions for T32A driver.
 * @version V1.0.0.7
 * $Date:: 2018-03-30 13:56:50 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */

#ifdef __cplusplus
 extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_t32a.h"
     
#if defined(__T32A_H)
/**
 *  @addtogroup Periph_Driver
 *  @{
 */

/** 
 *  @addtogroup T32A
 *  @{
 */

/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup T32A_Private_define T32A Private Define
 *  @{
 */
/** 
 *  @name  Parameter Result
 *  @brief Whether the parameter is specified or not.
 *  @{
 */
#define PARAM_OK    ((int32_t)1)    /*!< Parameter is valid(specified).         */
#define PARAM_NG    ((int32_t)0)    /*!< Parameter is invalid(not specified).   */
/**
 *  @}
 */ /* End of name Parameter Result */
/**
 *  @}
 */ /* End of group T32A_Private_typedef */
/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup T32A_Private_define T32A Private Define
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group T32A_Private_define */
/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup T32A_Private_typedef T32A Private Typedef
 *  @{
 */

/* no define */

/** 
 *  @}
 */ /* End of group T32A_Private_typedef */

/*------------------------------------------------------------------------------*/
/*  Private Member                                                              */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup T32A_Private_member T32A Private Member
 *  @{
 */

/* no define */

/**
 *  @}
 */ /* End of group T32A_Private_member */

/*------------------------------------------------------------------------------*/
/*  Private Function                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup T32A_Private_fuctions TSPI Private Fuctions
 *  @{
 */
#ifdef DEBUG
    __INLINE static int32_t check_param_mode_halt(uint32_t param);
    __INLINE static int32_t check_param_mode_mode32(uint32_t param);
    __INLINE static int32_t check_param_runx_sftstpx(uint32_t param);
    __INLINE static int32_t check_param_runx_sftstax(uint32_t param);
    __INLINE static int32_t check_param_runx_runx(uint32_t param);
    __INLINE static int32_t check_param_crx_prsclx(uint32_t param);
    __INLINE static int32_t check_param_crx_clkx(uint32_t param);
    __INLINE static int32_t check_param_crx_wbfx(uint32_t param);
    __INLINE static int32_t check_param_crx_updnx(uint32_t param);
    __INLINE static int32_t check_param_crx_reldx(uint32_t param);
    __INLINE static int32_t check_param_crx_stopx(uint32_t param);
    __INLINE static int32_t check_param_crx_startx(uint32_t param);
    __INLINE static int32_t check_param_outcrx0_ocrx(uint32_t param);
    __INLINE static int32_t check_param_outcrx1_ocrcapx1(uint32_t param);
    __INLINE static int32_t check_param_outcrx1_ocrcapx0(uint32_t param);
    __INLINE static int32_t check_param_outcrx1_ocrcmpx1(uint32_t param);
    __INLINE static int32_t check_param_outcrx1_ocrcmpx0(uint32_t param);
    __INLINE static int32_t check_param_capcrx_capmx1(uint32_t param);
    __INLINE static int32_t check_param_capcrx_capmx0(uint32_t param);
    __INLINE static int32_t check_param_rgx0_rgx0(uint32_t param);
    __INLINE static int32_t check_param_rgx1_rgx1(uint32_t param);
    __INLINE static int32_t check_param_reldx_reld(uint32_t param);
    __INLINE static int32_t check_param_imx_imsterr(uint32_t param);
    __INLINE static int32_t check_param_imx_imufx(uint32_t param);
    __INLINE static int32_t check_param_imx_imofx(uint32_t param);
    __INLINE static int32_t check_param_imx_imx1(uint32_t param);
    __INLINE static int32_t check_param_imx_imx0(uint32_t param);
    __INLINE static int32_t check_param_dma_req_dmaenx2(uint32_t param);
    __INLINE static int32_t check_param_dma_req_dmaenx1(uint32_t param);
    __INLINE static int32_t check_param_dma_req_dmaenx0(uint32_t param);
    __INLINE static int32_t check_param_pls_cr_pdn(uint32_t param);
    __INLINE static int32_t check_param_pls_cr_pup(uint32_t param);
    __INLINE static int32_t check_param_pls_cr_nf(uint32_t param);
    __INLINE static int32_t check_param_pls_cr_pdir(uint32_t param);
    __INLINE static int32_t check_param_pls_cr_pmode(uint32_t param);
#endif

#ifdef DEBUG
/*--------------------------------------------------*/
/** 
  * @brief  Check the Mode HALT's parameter.
  * @param  param    :Mode HALT's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_HALT
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_mode_halt(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_DBG_HALT_RUN:
        case T32A_DBG_HALT_STOP:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Mode mode32's parameter.
  * @param  param    :Mode mode32's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_MODE32
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_mode_mode32(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_MODE_16:
        case T32A_MODE_32:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the SW Counter STOP Control's parameter.
  * @param  param    :SW Counter STOP Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_SFTSTPx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_runx_sftstpx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_COUNT_DONT_STOP:
        case T32A_COUNT_STOP:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the SW START Control's parameter.
  * @param  param    :SW START Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_SFTSTAx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_runx_sftstax(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_COUNT_DONT_START:
        case T32A_COUNT_START:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A RUN Disable/Enable Control's parameter.
  * @param  param    :T32A RUN Disable/Enable Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_PRSCLx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_runx_runx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_RUN_DISABLE:
        case T32A_RUN_ENABLE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A PRESCALER Control's parameter.
  * @param  param    :T32A PRESCALER Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_PRSCLx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_crx_prsclx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_PRSCLx_1:
        case T32A_PRSCLx_2:
        case T32A_PRSCLx_8:
        case T32A_PRSCLx_32:
        case T32A_PRSCLx_128:
        case T32A_PRSCLx_256:
        case T32A_PRSCLx_512:
        case T32A_PRSCLx_1024:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A PRESCALER Control's parameter.
  * @param  param    :T32A PRESCALER Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_CLKx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_crx_clkx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_CLKx_PRSCLx:
        case T32A_CLKx_INTRG:
        case T32A_CLKx_TIM_RISING_EDGE:
        case T32A_CLKx_TIM_TRAILING_EDGE:
        case T32A_CLKx_EXTTRG_RISING_EDGE:
        case T32A_CLKx_EXTTRG_TRAILING_EDGE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the Double Buffer Disable/Enable Control's parameter.
  * @param  param    :Double Buffer Disable/Enable Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_WBFx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_crx_wbfx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_WBF_DISABLE:
        case T32A_WBF_ENABLE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Counter Up/Down Control's parameter.
  * @param  param    :T32A Counter Up/Down Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_UPDNx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_crx_updnx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_COUNT_UP:
        case T32A_COUNT_DOWN:
        case T32A_COUNT_UPDOWN:
        case T32A_COUNT_PLS:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Counter Reload Control's parameter.
  * @param  param    :T32A Counter Reload Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_RELDx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_crx_reldx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_RELOAD_NON:
        case T32A_RELOAD_INTRG:
        case T32A_RELOAD_EXTTRG_RISING_EDGE:
        case T32A_RELOAD_EXTTRG_TRAILING_EDGE:
        case T32A_RELOAD_TIM_RISING_EDGE:
        case T32A_RELOAD_TIM_TRAILING_EDGE:
        case T32A_RELOAD_SYNC:
        case T32A_RELOAD_TREGx:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Counter Stop Control's parameter.
  * @param  param    :T32A Counter Stop Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_STOPx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_crx_stopx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_STOP_NON:
        case T32A_STOP_INTRG:
        case T32A_STOP_EXTTRG_RISING_EDGE:
        case T32A_STOP_EXTTRG_TRAILING_EDGE:
        case T32A_STOP_TIM_RISING_EDGE:
        case T32A_STOP_TIM_TRAILING_EDGE:
        case T32A_STOP_SYNC:
        case T32A_STOP_TREGx:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Counter Start Control's parameter.
  * @param  param    :T32A Counter Start Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_STARTx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_crx_startx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_START_NON:
        case T32A_START_INTRG:
        case T32A_START_EXTTRG_RISING_EDGE:
        case T32A_START_EXTTRG_TRAILING_EDGE:
        case T32A_START_TIM_RISING_EDGE:
        case T32A_START_TIM_TRAILING_EDGE:
        case T32A_START_SYNC:
            result = PARAM_OK;
            break;
        case T32A_START_Rsvd:
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32AxOUTA Control's parameter.
  * @param  param    :T32AxOUTA Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_OCRx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_outcrx0_ocrx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_OCR_DISABLE:
        case T32A_OCR_SET:
        case T32A_OCR_CLR:
        case T32A_OCR_INVERSION:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32AxOUTA Control of T32AxCAPx1 T32AxRGx1's parameter.
  * @param  param    :T32AxOUTA Control of T32AxCAPx1 T32AxRGx1's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_OCRCAPx1
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_outcrx1_ocrcapx1(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_OCRCAPx1_DISABLE:
        case T32A_OCRCAPx1_SET:
        case T32A_OCRCAPx1_CLR:
        case T32A_OCRCAPx1_INVERSION:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32AxOUTA Control of T32AxCAPx0 T32AxRGx0's parameter.
  * @param  param    :T32AxOUTA Control of T32AxCAPx0 T32AxRGx0's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_OCRCAPx0
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_outcrx1_ocrcapx0(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_OCRCAPx0_DISABLE:
        case T32A_OCRCAPx0_SET:
        case T32A_OCRCAPx0_CLR:
        case T32A_OCRCAPx0_INVERSION:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32AxOUTA Control of T32AxRGx1 Counter Value's parameter.
  * @param  param    :T32AxOUTA Control of T32AxRGx1 Counter Value's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_OCRCMPx1
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_outcrx1_ocrcmpx1(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_OCRCMPx1_DISABLE:
        case T32A_OCRCMPx1_SET:
        case T32A_OCRCMPx1_CLR:
        case T32A_OCRCMPx1_INVERSION:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32AxOUTA Control of T32AxRGx1 Counter Value's parameter.
  * @param  param    :T32AxOUTA Control of T32AxRGx1 Counter Value's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_OCRCMPx0
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_outcrx1_ocrcmpx0(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_OCRCMPx0_DISABLE:
        case T32A_OCRCMPx0_SET:
        case T32A_OCRCMPx0_CLR:
        case T32A_OCRCMPx0_INVERSION:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Capture Control Register A1's parameter.
  * @param  param    :T32A Capture Control Register A1's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_CAPMx1
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_capcrx_capmx1(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_CAPMx1_DISABLE:
        case T32A_CAPMx1_INTRG:
        case T32A_CAPMx1_INx0_RISING_EDGE:
        case T32A_CAPMx1_INx0_TRAILING_EDGE:
        case T32A_CAPMx1_INx1_RISING_EDGE:
        case T32A_CAPMx1_INx1_TRAILING_EDGE:
        case T32A_CAPMx1_TIM_RISING_EDGE:
        case T32A_CAPMx1_TIM_TRAILING_EDGE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Capture Control Register A0's parameter.
  * @param  param    :T32A Capture Control Register A0's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_CAPMx0
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_capcrx_capmx0(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_CAPMx0_DISABLE:
        case T32A_CAPMx0_INTRG:
        case T32A_CAPMx0_INx0_RISING_EDGE:
        case T32A_CAPMx0_INx0_TRAILING_EDGE:
        case T32A_CAPMx0_INx1_RISING_EDGE:
        case T32A_CAPMx0_INx1_TRAILING_EDGE:
        case T32A_CAPMx0_TIM_RISING_EDGE:
        case T32A_CAPMx0_TIM_TRAILING_EDGE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Timer Register A0's parameter.
  * @param  param    :T32A Timer Register A0's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_RGx0
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_rgx0_rgx0(uint32_t param)
{
    int32_t result = PARAM_NG;

    if (param <= T32A_RGx0_MASK){
        result = PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Timer Register A1's parameter.
  * @param  param    :T32A Timer Register A1's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_RGx1
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_rgx1_rgx1(uint32_t param)
{
    int32_t result = PARAM_NG;

    if (param <= T32A_RGx1_MASK){
        result = PARAM_OK;
    }

    return (result);
}


/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Counter Reload Register A's parameter.
  * @param  param    :T32A Counter Reload Register A's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_RELD
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_reldx_reld(uint32_t param)
{
    int32_t result = PARAM_NG;

    if (param <= T32A_RELDx_MASK){
        result = PARAM_OK;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Statuserr Interrupt Request MASK's parameter.
  * @param  param    :T32A Statuserr Interrupt Request MASK's parameter.
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_IMSTEER
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_imx_imsterr(uint32_t param)
{
    int32_t result = PARAM_NG;
    
    switch (param) 
    {
        case T32A_IMSTERR_MASK_NOREQ:
        case T32A_IMSTERR_MASK_REQ:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Overflow Interrupt Request MASK's parameter.
  * @param  param    :T32A Overflow Interrupt Request MASK's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_IMUFx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_imx_imufx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_IMOFx_MASK_NOREQ:
        case T32A_IMOFx_MASK_REQ:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Overflow Interrupt Request MASK's parameter.
  * @param  param    :T32A Overflow Interrupt Request MASK's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_IMOFx
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_imx_imofx(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_IMOFx_MASK_NOREQ:
        case T32A_IMOFx_MASK_REQ:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Match Up T32AxRGx1 Interrupt Request MASK's parameter.
  * @param  param    :T32A Match Up T32AxRGx1 Interrupt Request MASK's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_IMx1
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_imx_imx1(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_IMx1_MASK_NOREQ:
        case T32A_IMx1_MASK_REQ:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Match Up T32AxRGx0 Interrupt Request MASK's parameter.
  * @param  param    :T32A Match Up T32AxRGx0 Interrupt Request MASK's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_IMx0
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_imx_imx0(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_IMx0_MASK_NOREQ:
        case T32A_IMx0_MASK_REQ:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A DMA Converter1 Request control's parameter.
  * @param  param    :T32A DMA Converter1 Request control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_DMAENx2
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_dma_req_dmaenx2(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_DMAENx2_DISABLE:
        case T32A_DMAENx2_ENABLE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A DMA InputCapture1 Request control's parameter.
  * @param  param    :T32A DMA InputCapture1 Request control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_DMAENx1
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_dma_req_dmaenx1(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_DMAENx1_DISABLE:
        case T32A_DMAENx1_ENABLE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A DMA InputCapture0 Request control's parameter.
  * @param  param    :T32A DMA InputCapture0 Request control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_DMAENx0
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_dma_req_dmaenx0(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_DMAENx0_DISABLE:
        case T32A_DMAENx0_ENABLE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}


/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Pulse Mode Count Down Control's parameter.
  * @param  param    :T32A Pulse Mode Count Down Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_PDN
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_pls_cr_pdn(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_PDN_NON0:
        case T32A_PDN_NON1:
        case T32A_PDN_INC0_RISING_EDGE:
        case T32A_PDN_INC0_TRAILING_EDGE:
        case T32A_PDN_INC1_RISING_EDGE:
        case T32A_PDN_INC1_TRAILING_EDGE:
        case T32A_PDN_INC0_BOTH_EDGE:
        case T32A_PDN_INC1_BOTH_EDGE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Pulse Mode Count UP Control's parameter.
  * @param  param    :T32A Pulse Mode Count UP Control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_PUP
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_pls_cr_pup(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_PUP_NON0:
        case T32A_PUP_NON1:
        case T32A_PUP_INC0_RISING_EDGE:
        case T32A_PUP_INC0_TRAILING_EDGE:
        case T32A_PUP_INC1_RISING_EDGE:
        case T32A_PUP_INC1_TRAILING_EDGE:
        case T32A_PUP_INC0_BOTH_EDGE:
        case T32A_PUP_INC1_BOTH_EDGE:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Noise Filter control's parameter.
  * @param  param    :T32A Noise Filter control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_NF
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_pls_cr_nf(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_NF_NON:
        case T32A_NF_2:
        case T32A_NF_4:
        case T32A_NF_8:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}

/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Phase 2 Pulse Direction control's parameter.
  * @param  param    :T32A Phase 2 Pulse Direction control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_PDIR
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_pls_cr_pdir(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_PDIR_FORWARD:
        case T32A_PDIR_BACKWARD:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}


/*--------------------------------------------------*/
/** 
  * @brief  Check the T32A Pulse Count Mode control's parameter.
  * @param  param    :T32A Pulse Count Mode control's parameter
  * @retval PARAM_OK :Valid
  * @retval PARAM_NG :Invalid
  * @note   Macro definition is @ref T32A_PMODE
  */
/*--------------------------------------------------*/
__INLINE static int32_t check_param_pls_cr_pmode(uint32_t param)
{
    int32_t result = PARAM_NG;

    switch (param) 
    {
        case T32A_PMODE_PHASE_2:
        case T32A_PMODE_PHASE_1:
            result = PARAM_OK;
            break;
        default:
            /* no process */
            break;
    }

    return (result);
}




#endif
/**
 *  @}
 */ /* End of group T32A_Private_functions */


/*------------------------------------------------------------------------------*/
/*  Public Function                                                             */
/*------------------------------------------------------------------------------*/
/** 
 *  @addtogroup T32A_Exported_functions

 */ 
/*--------------------------------------------------*/
/**
  * @brief     Mode Initialize the T32A object.
  * @param     p_obj :T32A object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result t32a_mode_init(t32a_t *p_obj)
{
    TXZ_Result result = TXZ_SUCCESS;
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    check_param_mode_halt(p_obj->init_mode.mode.halt);
    check_param_mode_mode32(p_obj->init_mode.mode.mode);
#endif /* DEBUG */
    /* Timer Mode Set */
    p_obj->p_instance->MOD = 0;
    p_obj->p_instance->MOD =  (p_obj->init_mode.mode.halt | p_obj->init_mode.mode.mode);
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Initialize the T32A object.
  * @param     p_obj :T32A object.
  * @param     type :T32A Timer Type.  : Use @ref t32_type_t
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result t32a_timer_init(t32a_t *p_obj,uint32_t type)
{
    TXZ_Result result = TXZ_SUCCESS;
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    /* Check the parameter of TimerA Mode Set */
    check_param_mode_halt(p_obj->init_mode.mode.halt);
    check_param_mode_mode32(p_obj->init_mode.mode.mode);
    /* Check the parameter of TimerA Run Control Set */
    check_param_runx_sftstpx(p_obj->init[type].runx.sftstp);
    check_param_runx_sftstax(p_obj->init[type].runx.sftsta);
    check_param_runx_runx(p_obj->init[type].runx.run);
    /* Check the parameter of Counter Register Control Set */
    check_param_crx_prsclx(p_obj->init[type].crx.prscl);
    check_param_crx_clkx(p_obj->init[type].crx.clk);
    check_param_crx_wbfx(p_obj->init[type].crx.wbf);
    check_param_crx_updnx(p_obj->init[type].crx.updn);
    check_param_crx_reldx(p_obj->init[type].crx.reld);
    check_param_crx_stopx(p_obj->init[type].crx.stop);
    check_param_crx_startx(p_obj->init[type].crx.start);
    /* Check the parameter of TimerA Output Control Set */
    check_param_outcrx0_ocrx(p_obj->init[type].outcrx0.ocr);
    /* Check the parameter of T32AxOUTA Control Set */
    check_param_outcrx1_ocrcapx1(p_obj->init[type].outcrx1.ocrcap1);
    check_param_outcrx1_ocrcapx0(p_obj->init[type].outcrx1.ocrcap0);
    check_param_outcrx1_ocrcmpx1(p_obj->init[type].outcrx1.ocrcmp1);
    check_param_outcrx1_ocrcmpx0(p_obj->init[type].outcrx1.ocrcmp0);
    /*  Check the parameter of Capture Control Set */
    check_param_capcrx_capmx1(p_obj->init[type].capcrx.capmx1);
    check_param_capcrx_capmx0(p_obj->init[type].capcrx.capmx0);
    /* Check the parameter of T32A Timer Register 0 Set */
    check_param_rgx0_rgx0(p_obj->init[type].rgx0.rgx0);
    /* Check the parameter of T32A Timer Register 1 Set */
    check_param_rgx1_rgx1(p_obj->init[type].rgx1.rgx1);
    /* Check the parameter of  T32A Counter Reload Register Set */
    check_param_reldx_reld(p_obj->init[type].reldx.reld);
    /* Check the parameter of  Interrupt mask register Set */
    check_param_imx_imsterr(p_obj->init[type].imx.imsterr);
    check_param_imx_imufx(p_obj->init[type].imx.imuf);
    check_param_imx_imofx(p_obj->init[type].imx.imof);
    check_param_imx_imx1(p_obj->init[type].imx.imx1);
    check_param_imx_imx0(p_obj->init[type].imx.imx0);                                  
    /* Check the parameter of  DMA Request register Set */
    check_param_dma_req_dmaenx2(p_obj->init[type].dma_req.dmaenx2);
    check_param_dma_req_dmaenx1(p_obj->init[type].dma_req.dmaenx1);
    check_param_dma_req_dmaenx0(p_obj->init[type].dma_req.dmaenx0);
#endif

    switch (type) 
    {
        case T32A_TIMERA:
            /* Timer A */
            if(p_obj->init_mode.mode.mode != T32A_MODE_16) {
                result = TXZ_ERROR;
                return (result);
            }
            /* TimerA Run Control Disable */
            p_obj->p_instance->RUNA = 0;
            /* Counter Register Control Set */
            p_obj->p_instance->CRA = 0;
            p_obj->p_instance->CRA =  (p_obj->init[type].crx.prscl | p_obj->init[type].crx.clk | p_obj->init[type].crx.wbf | p_obj->init[type].crx.updn | \
                                       p_obj->init[type].crx.reld | p_obj->init[type].crx.stop |p_obj->init[type].crx.start );
            /* TimerA Output Control Set */
            p_obj->p_instance->OUTCRA0 = 0;
            p_obj->p_instance->OUTCRA0 = p_obj->init[type].outcrx0.ocr;
            /* T32AxOUTA Control Set */
            p_obj->p_instance->OUTCRA1 = 0;
            p_obj->p_instance->OUTCRA1 = (p_obj->init[type].outcrx1.ocrcap1 | p_obj->init[type].outcrx1.ocrcap0 | p_obj->init[type].outcrx1.ocrcmp1 | \
                                          p_obj->init[type].outcrx1.ocrcmp0 );
            /* T32A Timer Register A0 Set */
            p_obj->p_instance->RGA0 = 0;
            p_obj->p_instance->RGA0 = p_obj->init[type].rgx0.rgx0;
            /* T32A Timer Register A1 Set */
            p_obj->p_instance->RGA1 = 0;
            p_obj->p_instance->RGA1 = p_obj->init[type].rgx1.rgx1;
            /* T32A Counter Reload Register Set */
            p_obj->p_instance->RELDA = 0;
            p_obj->p_instance->RELDA = p_obj->init[type].reldx.reld;
            /* TimerB Capture Control Set */
            p_obj->p_instance->CAPCRA = (p_obj->init[type].capcrx.capmx0 | p_obj->init[type].capcrx.capmx1);
            /* Interrupt mask register Set */
            p_obj->p_instance->IMA = 0;
            p_obj->p_instance->IMA = (p_obj->init[type].imx.imuf | p_obj->init[type].imx.imof | p_obj->init[type].imx.imx1 | \
                                          p_obj->init[type].imx.imx0 );
            /* DMA Request register Set */
            p_obj->p_instance->DMAA = 0;
            p_obj->p_instance->DMAA = (p_obj->init[type].dma_req.dmaenx2 | p_obj->init[type].dma_req.dmaenx1 | p_obj->init[type].dma_req.dmaenx0 );
            /* TimerA Run Control Set */
            p_obj->p_instance->RUNA =  (p_obj->init[type].runx.sftstp | p_obj->init[type].runx.sftsta | p_obj->init[type].runx.run);
            break;
        case T32A_TIMERB:
            /* Timer B */
            if(p_obj->init_mode.mode.mode != T32A_MODE_16) {
                result = TXZ_ERROR;
                return (result);
            }
            /* TimerB Run Control Disable */
            p_obj->p_instance->RUNB = 0;
            /* Counter Register Control Set */
            p_obj->p_instance->CRB = 0;
            p_obj->p_instance->CRB =  (p_obj->init[type].crx.prscl | p_obj->init[type].crx.clk | p_obj->init[type].crx.wbf | p_obj->init[type].crx.updn | \
                                       p_obj->init[type].crx.reld | p_obj->init[type].crx.stop |p_obj->init[type].crx.start );
            /* TimerB Output Control Set */
            p_obj->p_instance->OUTCRB0 = 0;
            p_obj->p_instance->OUTCRB0 = p_obj->init[type].outcrx0.ocr;
            /* T32AxOUTB Control Set */
            p_obj->p_instance->OUTCRB1 = 0;
            p_obj->p_instance->OUTCRB1 = (p_obj->init[type].outcrx1.ocrcap1 | p_obj->init[type].outcrx1.ocrcap0 | p_obj->init[type].outcrx1.ocrcmp1 | \
                                            p_obj->init[type].outcrx1.ocrcmp0 );
            /* T32A Timer Register B0 Set */
            p_obj->p_instance->RGB0 = 0;
            p_obj->p_instance->RGB0 = p_obj->init[type].rgx0.rgx0;
            /* T32A Timer Register B1 Set */
            p_obj->p_instance->RGB1 = 0;
            p_obj->p_instance->RGB1 = p_obj->init[type].rgx1.rgx1;
            /* T32A Counter Reload Register Set */
            p_obj->p_instance->RELDB = 0;
            p_obj->p_instance->RELDB = p_obj->init[type].reldx.reld;
            /* TimerB Capture Control Set */
            p_obj->p_instance->CAPCRB = (p_obj->init[type].capcrx.capmx0 | p_obj->init[type].capcrx.capmx1);
            /* Interrupt mask register Set */
            p_obj->p_instance->IMB = 0;
            p_obj->p_instance->IMB = (p_obj->init[type].imx.imuf | p_obj->init[type].imx.imof | p_obj->init[type].imx.imx1 | \
                                          p_obj->init[type].imx.imx0 );
            /* DMA Request register Set */
            p_obj->p_instance->DMAB = 0;
            p_obj->p_instance->DMAB = (p_obj->init[type].dma_req.dmaenx2 | p_obj->init[type].dma_req.dmaenx1 | p_obj->init[type].dma_req.dmaenx0 );
            /* TimerB Run Control Set */
            p_obj->p_instance->RUNB =  (p_obj->init[type].runx.sftstp | p_obj->init[type].runx.sftsta | p_obj->init[type].runx.run);
            break;
        case T32A_TIMERC:
            /* Timer C */
            if(p_obj->init_mode.mode.mode != T32A_MODE_32) {
                result = TXZ_ERROR;
                return (result);
            }
            /* TimerC Run Control Disable */
            p_obj->p_instance->RUNC = 0;
        #ifdef DEBUG
            /* Pulse Count Control register Set */
            check_param_pls_cr_pdn(p_obj->init[type].pls_cr.pdn);
            check_param_pls_cr_pup(p_obj->init[type].pls_cr.pup);
            check_param_pls_cr_nf(p_obj->init[type].pls_cr.nf);
            check_param_pls_cr_pdir(p_obj->init[type].pls_cr.pdir);
            check_param_pls_cr_pmode(p_obj->init[type].pls_cr.pmode);
        #endif
            /* Counter Register Control Set */
            p_obj->p_instance->CRC = 0;
            p_obj->p_instance->CRC =  (p_obj->init[type].crx.prscl | p_obj->init[type].crx.clk | p_obj->init[type].crx.wbf | p_obj->init[type].crx.updn | \
                                       p_obj->init[type].crx.reld | p_obj->init[type].crx.stop |p_obj->init[type].crx.start );
            /* TimerC Output Control Set */
            p_obj->p_instance->OUTCRC0 = 0;
            p_obj->p_instance->OUTCRC0 = p_obj->init[type].outcrx0.ocr;
            /* T32AxOUTC Control Set */
            p_obj->p_instance->OUTCRC1 = 0;
            p_obj->p_instance->OUTCRC1 = (p_obj->init[type].outcrx1.ocrcap1 | p_obj->init[type].outcrx1.ocrcap0 | p_obj->init[type].outcrx1.ocrcmp1 | \
                                            p_obj->init[type].outcrx1.ocrcmp0 );
            /* T32A Timer Register C0 Set */
            p_obj->p_instance->RGC0 = 0;
            p_obj->p_instance->RGC0 = p_obj->init[type].rgx0.rgx0;
            /* T32A Timer Register C1 Set */
            p_obj->p_instance->RGC1 = 0;
            p_obj->p_instance->RGC1 = p_obj->init[type].rgx1.rgx1;
            /* T32A Counter Reload Register Set */
            p_obj->p_instance->RELDC = 0;
            p_obj->p_instance->RELDC = p_obj->init[type].reldx.reld;
            /* TimerC Capture Control Set */
            p_obj->p_instance->CAPCRC = (p_obj->init[type].capcrx.capmx0 | p_obj->init[type].capcrx.capmx1);
            /* Interrupt mask register Set */
            p_obj->p_instance->IMC = 0;
            p_obj->p_instance->IMC = (p_obj->init[type].imx.imuf | p_obj->init[type].imx.imof | p_obj->init[type].imx.imx1 | \
                                          p_obj->init[type].imx.imx0 );
            /* DMA Request register Set */
            p_obj->p_instance->DMAC = 0;
            p_obj->p_instance->DMAC = (p_obj->init[type].dma_req.dmaenx2 | p_obj->init[type].dma_req.dmaenx1 | p_obj->init[type].dma_req.dmaenx0 );
            /* Pulse Count Control register Set */
            p_obj->p_instance->PLSCR = 0;
            p_obj->p_instance->PLSCR = (p_obj->init[type].pls_cr.pdn | p_obj->init[type].pls_cr.pup | p_obj->init[type].pls_cr.nf | \
                                          p_obj->init[type].pls_cr.pdir |  p_obj->init[type].pls_cr.pmode);
            /* TimerC Run Control Set */
            p_obj->p_instance->RUNC =  (p_obj->init[type].runx.sftstp | p_obj->init[type].runx.sftsta | p_obj->init[type].runx.run);
            break;
        default:
            result = TXZ_ERROR;
            return (result);
    }
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Release the T32A object.
  * @param     p_obj :T32A object.
  * @param     type :T32A Timer Type.  : Use @ref t32_type_t
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result t32a_deinit(t32a_t *p_obj, uint32_t type)
{
    TXZ_Result result = TXZ_SUCCESS;
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    switch (type) 
    {
        case T32A_TIMERA:
            /* Timer A */
            /* Disable the selected T32A peripheral  */
            p_obj->p_instance->RUNA = T32A_RUN_DISABLE;
            break;
        case T32A_TIMERB:
            /* Timer B */
            /* Disable the selected T32A peripheral  */
            p_obj->p_instance->RUNB = T32A_RUN_DISABLE;
            break;
        case T32A_TIMERC:
            /* Timer C */
            /* Disable the selected T32A peripheral  */
            p_obj->p_instance->RUNC = T32A_RUN_DISABLE;
            break;
        default:
            result = TXZ_ERROR;
            return (result);
    }
    
    return (result);
}


/*--------------------------------------------------*/
/**
  * @brief     Timer Start in interrupt mode.
  * @param     p_obj :T32A object.
  * @param     type :T32A Timer Type.  : Use @ref t32_type_t
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result t32a_timer_startIT(t32a_t *p_obj, uint32_t type)
{
    TXZ_Result result = TXZ_SUCCESS;
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    switch (type) 
    {
        case T32A_TIMERA:
            if(((p_obj->p_instance->RUNA) & T32A_RUNFLG_RUN) == 0){
                /* Timer A RUN */
                p_obj->p_instance->RUNA |= T32A_RUN_ENABLE;
            }else{
                result = TXZ_ERROR;
                return (result);
            }
            break;
        case T32A_TIMERB:
            if(((p_obj->p_instance->RUNB) & T32A_RUNFLG_RUN) == 0){
                /* Timer B RUN */
                p_obj->p_instance->RUNB |= T32A_RUN_ENABLE;
            }else{
                result = TXZ_ERROR;
                return (result);
            }
            break;
        case T32A_TIMERC:
            if(((p_obj->p_instance->RUNC) & T32A_RUNFLG_RUN) == 0){
                /* Timer C RUN */
                p_obj->p_instance->RUNC |= T32A_RUN_ENABLE;
            }else{
                result = TXZ_ERROR;
                return (result);
            }
            break;
        default:
            result = TXZ_ERROR;
            return (result);
    }
    
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Timer Stop in interrupt mode.
  * @param     p_obj :T32A object.
  * @param     type :T32A Timer Type.  : Use @ref t32_type_t
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result t32a_timer_stopIT(t32a_t *p_obj, uint32_t type)
{
    TXZ_Result result = TXZ_SUCCESS;
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    switch (type) 
    {
        case T32A_TIMERA:
            /* Timer A Stop */
            p_obj->p_instance->RUNA = T32A_RUN_DISABLE;
            break;
        case T32A_TIMERB:
            /* Timer B Stop */
            p_obj->p_instance->RUNB = T32A_RUN_DISABLE;
            break;
        case T32A_TIMERC:
            /* SW Counter Stop & Timer C Stop */
            p_obj->p_instance->RUNC = T32A_RUN_DISABLE;
            break;
        default:
            result = TXZ_ERROR;
            return (result);
    }
    
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Timer Start in interrupt mode.
  * @param     p_obj :T32A object.
  * @param     type :T32A Timer Type.  : Use @ref t32_type_t
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result t32a_SWcounter_start(t32a_t *p_obj, uint32_t type)
{
    TXZ_Result result = TXZ_SUCCESS;
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    switch (type) 
    {
        case T32A_TIMERA:
            if(((p_obj->p_instance->RUNA) & T32A_RUNFLG_RUN) == 0){
                /* Timer A SW Counter start */
                p_obj->p_instance->RUNA |= T32A_COUNT_START;
            }else{
                result = TXZ_ERROR;
                return (result);
            }
            break;
        case T32A_TIMERB:
            if(((p_obj->p_instance->RUNB) & T32A_RUNFLG_RUN) == 0){
                /* Timer SW Counter start */
                p_obj->p_instance->RUNB |= T32A_COUNT_START;
            }else{
                result = TXZ_ERROR;
                return (result);
            }
            break;
        case T32A_TIMERC:
            if(((p_obj->p_instance->RUNC) & T32A_RUNFLG_RUN) == 0){
                /* Timer C SW Counter start */
                p_obj->p_instance->RUNC |= T32A_COUNT_START;
            }else{
                result = TXZ_ERROR;
                return (result);
            }
            break;
        default:
            result = TXZ_ERROR;
            return (result);
    }
    
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Timer Stop in interrupt mode.
  * @param     p_obj :T32A object.
  * @param     type :T32A Timer Type.  : Use @ref t32_type_t
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result t32a_SWcounter_stop(t32a_t *p_obj, uint32_t type)
{
    TXZ_Result result = TXZ_SUCCESS;
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    switch (type) 
    {
        case T32A_TIMERA:
            /* TimerA SW Counter Stop */
            p_obj->p_instance->RUNA = T32A_COUNT_STOP;
            break;
        case T32A_TIMERB:
            /* Timer B SW Counter Stop */
            p_obj->p_instance->RUNB = T32A_COUNT_STOP;
            break;
        case T32A_TIMERC:
            /* Timer C SW Counter Stop */
            p_obj->p_instance->RUNC = T32A_COUNT_STOP;
            break;
        default:
            result = TXZ_ERROR;
            return (result);
    }
    
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     Timer Register Value Setting
  * @param     p_obj :T32A object.
  * @param     type :T32A Timer Type.  : Use @ref t32_type_t
  * @param     num :T32A Register Number.  : Use @ref t32_regnum_t
  * @param     value :Setting Value.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result t32a_reg_set(t32a_t *p_obj, uint32_t type, uint32_t num, uint32_t value)
{
    TXZ_Result result = TXZ_SUCCESS;
#ifdef DEBUG
    /* Check the NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
#endif
    switch (type) 
    {
        case T32A_TIMERA:
            /* Timer A */
            if(num == T32A_REG0) {p_obj->p_instance->RGA0 = value;}
            else if(num == T32A_REG1) {p_obj->p_instance->RGA1 = value;}
            else if(num == T32A_RELOAD) {p_obj->p_instance->RELDA = value;}
            break;
        case T32A_TIMERB:
            /* Timer B */
            if(num == T32A_REG0) {p_obj->p_instance->RGB0 = value;}
            else if(num == T32A_REG1) {p_obj->p_instance->RGB1 = value;}
            else if(num == T32A_RELOAD) {p_obj->p_instance->RELDB = value;}
            break;
        case T32A_TIMERC:
            /* Timer C */
            if(num == T32A_REG0) {p_obj->p_instance->RGC0 = value;}
            else if(num == T32A_REG1) {p_obj->p_instance->RGC1 = value;}
            else if(num == T32A_RELOAD) {p_obj->p_instance->RELDC = value;}
            break;
        default:
            result = TXZ_ERROR;
            return (result);
    }
    
    return (result);
}
/*--------------------------------------------------*/
/**
  * @brief     Timer Register Value Read
  * @param     p_obj :T32A object.
  * @param     type :T32A Timer Type.  : Use @ref t32_type_t
  * @param     p_val :Save area for register value.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
TXZ_Result t32a_tmr_read(t32a_t *p_obj, uint32_t type, uint32_t *p_val)
{
    TXZ_Result result = TXZ_SUCCESS;
    switch (type) 
    {
        case T32A_TIMERA:
            /* Timer A */
            *p_val = p_obj->p_instance->TMRA;
            break;
        case T32A_TIMERB:
            /* Timer B */
            *p_val = p_obj->p_instance->TMRB;
            break;
        case T32A_TIMERC:
            /* Timer C */
            *p_val = p_obj->p_instance->TMRC;
            break;
        default:
            result = TXZ_ERROR;
            return (result);
    }
    
    return (result);
}
/*--------------------------------------------------*/
/**
  * @brief  Get status.
  * @details  Status bits.
  * | Bit   | Bit Symbol | Function                                                            |
  * | :---  | :---       | :---                                                                |
  * | 31-4  | -          | -                                                                   |
  * | 3     | INTUFA     | Under Flow Intterrupt. Use @ref T32A_INTOFx_FLG_MASK.               |
  * | 2     | INTOFA     | Over Flow Intterrupt. Use @ref T32A_INTOFx_FLG_MASK.                |
  * | 1     | INTx1      | Match up TimerRegister x1 Intterrupt. Use @ref T32A_INTx1_FLG_MASK. |
  * | 0     | INTx0      | Match up TimerRegister x0 Intterrupt. Use @ref T32A_INTx0_FLG_MASK. |
  * 
  * @param  p_obj    :T32A object.
  * @param  p_status :Save area for status.
  * @param     type :T32A Timer Type.  : Use @ref t32_type_t
  * @retval TXZ_SUCCESS  :Success.
  * @retval TXZ_ERROR    :Failure.
  * @note   -
  */
/*--------------------------------------------------*/
TXZ_Result t32a_get_status(t32a_t *p_obj, uint32_t *p_status, uint32_t type)
{
    TXZ_Result result = TXZ_SUCCESS;

    /*------------------------------*/
    /*  Parameter Check             */
    /*------------------------------*/
#ifdef DEBUG
    /* Check the UART_NULL of address. */
    assert_param(IS_POINTER_NOT_NULL(p_obj));
    assert_param(IS_POINTER_NOT_NULL(p_obj->p_instance));
    assert_param(IS_POINTER_NOT_NULL(p_status));
#endif /* #ifdef DEBUG */
    /*------------------------------*/
    /*  Status Read                 */
    /*------------------------------*/
    switch (type) 
    {
        case T32A_TIMERA:
            /* Timer A */
            *p_status = p_obj->p_instance->STA;
            break;
        case T32A_TIMERB:
            /* Timer B */
            *p_status = p_obj->p_instance->STB;
            break;
        case T32A_TIMERC:
            /* Timer C */
            *p_status = p_obj->p_instance->STC;
            break;
        default:
            result = TXZ_ERROR;
            return (result);
    }
    
    return (result);
}

/*--------------------------------------------------*/
/**
  * @brief     IRQ Handler for Timer interrupt.
  * @param     p_obj :T32A object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
void t32a_timer_IRQHandler(t32a_t *p_obj)
{
    uint32_t status_a, status_b, status_c;
    /*------------------------------*/
    /*  Get Status                  */
    /*------------------------------*/
    (void)t32a_get_status(p_obj, &status_a, T32A_TIMERA);
    (void)t32a_get_status(p_obj, &status_b, T32A_TIMERB);
    (void)t32a_get_status(p_obj, &status_c, T32A_TIMERC);

  if(status_a != 0){
      /*------------------------------*/
      /* Call Handler Timer A         */
      /*------------------------------*/
      if (p_obj->init[T32A_TIMERA].handler_T != T32A_NULL)
      {
          /* Call the handler with Status Register Value & SUCCESS. */
          p_obj->init[T32A_TIMERA].handler_T(p_obj->init[T32A_TIMERA].id, status_a, TXZ_SUCCESS);
      }
  }
  if(status_b != 0){
      /*------------------------------*/
      /* Call Handler Timer B         */
      /*------------------------------*/
      if (p_obj->init[T32A_TIMERB].handler_T != T32A_NULL)
      {
          /* Call the handler with Status Register Value & SUCCESS. */
          p_obj->init[T32A_TIMERB].handler_T(p_obj->init[T32A_TIMERB].id, status_b, TXZ_SUCCESS);
      }
  }
  if(status_c != 0){
      /*------------------------------*/
      /* Call Handler Timer C         */
      /*------------------------------*/
      if (p_obj->init[T32A_TIMERC].handler_T != T32A_NULL)
      {
          /* Call the handler with Status Register Value & SUCCESS. */
          p_obj->init[T32A_TIMERC].handler_T(p_obj->init[T32A_TIMERC].id, status_c,TXZ_SUCCESS);
      }
  }
}

/*--------------------------------------------------*/
/**
  * @brief     IRQ Timer Capture0 Handler for Timer Capture0 interrupt.
  * @param     p_obj :T32A object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
void t32a_timer_cap0_IRQHandler(t32a_t *p_obj)
{
    uint32_t status_a, status_b, status_c;
    /*------------------------------*/
    /*  Get Status                  */
    /*------------------------------*/
    (void)t32a_get_status(p_obj, &status_a, T32A_TIMERA);
    (void)t32a_get_status(p_obj, &status_b, T32A_TIMERB);
    (void)t32a_get_status(p_obj, &status_c, T32A_TIMERC);

  if(status_a != 0){
      /*------------------------------*/
      /* Call Handler Timer A         */
      /*------------------------------*/
      if (p_obj->init[T32A_TIMERA].handler_TC0 != T32A_NULL)
      {
          /* Call the handler with Status Register Value & SUCCESS. */
          p_obj->init[T32A_TIMERA].handler_TC0(p_obj->init[T32A_TIMERA].id, status_a, TXZ_SUCCESS);
      }
  }
  if(status_b != 0){
      /*------------------------------*/
      /* Call Handler Timer B         */
      /*------------------------------*/
      if (p_obj->init[T32A_TIMERB].handler_TC0 != T32A_NULL)
      {
          /* Call the handler with Status Register Value & SUCCESS. */
          p_obj->init[T32A_TIMERB].handler_TC0(p_obj->init[T32A_TIMERB].id, status_b, TXZ_SUCCESS);
      }
  }
  if(status_c != 0){
      /*------------------------------*/
      /* Call Handler Timer C         */
      /*------------------------------*/
      if (p_obj->init[T32A_TIMERC].handler_TC0 != T32A_NULL)
      {
          /* Call the handler with Status Register Value & SUCCESS. */
          p_obj->init[T32A_TIMERC].handler_TC0(p_obj->init[T32A_TIMERC].id, status_c,TXZ_SUCCESS);
      }
  }
}

/*--------------------------------------------------*/
/**
  * @brief     IRQ Timer Capture1 Handler for Timer Capture1 interrupt.
  * @param     p_obj :T32A object.
  * @retval    TXZ_SUCCESS :Success.
  * @retval    TXZ_ERROR :Failure.
  * @note      When p_obj is NULL, "Failure" is returned.
  * @note      An initial value of default is set at the
               reset status value. If needed, please
               rewrite and use an initial value.
  * @attention This function is not available in interrupt.
  */
/*--------------------------------------------------*/
void t32a_timer_cap1_IRQHandler(t32a_t *p_obj)
{
    uint32_t status_a, status_b, status_c;
    /*------------------------------*/
    /*  Get Status                  */
    /*------------------------------*/
    (void)t32a_get_status(p_obj, &status_a, T32A_TIMERA);
    (void)t32a_get_status(p_obj, &status_b, T32A_TIMERB);
    (void)t32a_get_status(p_obj, &status_c, T32A_TIMERC);

  if(status_a != 0){
      /*------------------------------*/
      /* Call Handler Timer A         */
      /*------------------------------*/
      if (p_obj->init[T32A_TIMERA].handler_TC1 != T32A_NULL)
      {
          /* Call the handler with Status Register Value & SUCCESS. */
          p_obj->init[T32A_TIMERA].handler_TC1(p_obj->init[T32A_TIMERA].id, status_a, TXZ_SUCCESS);
      }
  }
  if(status_b != 0){
      /*------------------------------*/
      /* Call Handler Timer B         */
      /*------------------------------*/
      if (p_obj->init[T32A_TIMERB].handler_TC1 != T32A_NULL)
      {
          /* Call the handler with Status Register Value & SUCCESS. */
          p_obj->init[T32A_TIMERB].handler_TC1(p_obj->init[T32A_TIMERB].id, status_b, TXZ_SUCCESS);
      }
  }
  if(status_c != 0){
      /*------------------------------*/
      /* Call Handler Timer C         */
      /*------------------------------*/
      if (p_obj->init[T32A_TIMERC].handler_TC1 != T32A_NULL)
      {
          /* Call the handler with Status Register Value & SUCCESS. */
          p_obj->init[T32A_TIMERC].handler_TC1(p_obj->init[T32A_TIMERC].id, status_c,TXZ_SUCCESS);
      }
  }
}

/*--------------------------------------------------*/
/** 
  * @brief  Calculate timer value to set timer register. 
  * @param  p_value: time value store pointer.
  * @param  time: The require period which the uint is us. 
  * @param  prescaler: System Clock Freq
  * @param  prscl: Select the division for source clock  @ref T32A_PRSCLx. 
  * @retval the value set to Tmrb timer register. 
  */ 
/*--------------------------------------------------*/
TXZ_Result t32a_Calculator(uint32_t *p_value, uint32_t time, uint32_t prescaler, uint32_t prscl) 
{      
    TXZ_Result result = TXZ_SUCCESS;
    uint64_t denominator;
    uint64_t numerator;
    uint32_t div;

    /* div */
    switch (prscl)
    {
        case T32A_PRSCLx_1:
        div = 1;
        break;
        case T32A_PRSCLx_2:
        div = 2;
        break;
    case T32A_PRSCLx_8:
        div = 8;
        break;
    case T32A_PRSCLx_32:
        div = 32;
        break;
    case T32A_PRSCLx_128:
        div = 128;
        break;
    case T32A_PRSCLx_256:
        div = 256;
        break;
    case T32A_PRSCLx_512:
        div = 512;
        break;
    case T32A_PRSCLx_1024:
        div = 1024;
        break;
    default:
        div = 1;
        break;
    }
    /*-----------------------------------------------*/
    /* "1"counter  (s) = 1 / fs                      */
    /* "1"counter  (s) = 1 / (prescaler / div)       */
    /* "1"counter (us) = (10^6) / (prescaler / div)  */
    /* "1"counter (us) = ((10^6) * div)/prescaler    */
    /* "x"counter (us) = time                        */
    /*-----------------------------------------------*/
    /* x : time = 1 : ((10^6) * div)/prescaler       */
    /*-----------------------------------------------*/
    /* x = time / (((10^6) * div)/prescaler)         */
    /*   = (prescaler * time) / ((10^6) * div)       */
    /*-----------------------------------------------*/
    denominator = (uint64_t)((uint64_t)(prescaler) * (uint64_t)(time));
    numerator   = (uint64_t)((uint64_t)(1000000) * (uint64_t)div);
    denominator = (uint64_t)(denominator / numerator);
    /* result */
    if ((denominator == (uint64_t)(0)) || (denominator > (uint64_t)(0xFFFF)))
    {
        result = TXZ_ERROR;
    }
    else
    {
        *p_value = (uint32_t)denominator;
    }

    return (result); 
}

/**
 *  @}
 */ /* End of group T32A_Exported_functions */

/**
 *  @} 
 */ /* End of group T32A */

/**
 *  @} 
 */ /* End of group Periph_Driver */

#endif /* defined(__T32A_H)  */

#ifdef __cplusplus
}
#endif /* __cplusplus */
