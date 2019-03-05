/** 
 *******************************************************************************
 * @file    adc.h
 * @brief   This file provides all the functions prototypes for ADC driver.
 * @version V1.0.0.0
 * $Date:: 2017-09-12 13:52:12 #$
 * 
 * SPDX-License-Identifier: Apache-2.0
 * 
 * (C)Copyright TOSHIBA MICROELECTRONICS CORPORATION 2017 All rights reserved
 *******************************************************************************
 */
/*------------------------------------------------------------------------------*/
/*  Define to prevent recursive inclusion                                       */
/*------------------------------------------------------------------------------*/
#ifndef __ADC_H
#define __ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------------------------------------------*/
/*  Includes                                                                    */
/*------------------------------------------------------------------------------*/
#include "txz_driver_def.h"
#include "adc_ch.h"
/**
 *  @addtogroup Periph_Driver Peripheral Driver
 *  @{
 */

/** 
 *  @defgroup ADC ADC
 *  @brief    ADC Driver.
 *  @{
 */
/*------------------------------------------------------------------------------*/
/*  Macro Definition                                                            */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Exported_define ADC Exported Define
 *  @{
 */

/** 
 *  @defgroup ADC_ChannelMax Channel Num Max
 *  @brief    Max Num of channel.
 *  @{
 */

    #define ADC_NUM_MAX                 ((uint32_t)24)            /*!< Max Num of conversion. */
/**
 *  @}
 */ /* End of group ADC_ChannelMax */
/**
 *  @}
 */ /* End of group ADC_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Enumerated Type Definition                                                  */
/*------------------------------------------------------------------------------*/
/** 
    @defgroup ADC_Exported_define ADC Exported Define
    @{
 */
/** @enum adc_sampling_period0_t
    @brief Outside AIN sampling period.
 */
typedef enum
{
    ADC_SAMPLING_PERIOD0_XN = 0x00,     /*!< SCLK Period (1/SCLK)xN */
    ADC_SAMPLING_PERIOD0_X2N = 0x08,     /*!< SCLK Period (1/SCLK)x2N */
    ADC_SAMPLING_PERIOD0_X3N = 0x10,     /*!< SCLK Period (1/SCLK)x3N */
    ADC_SAMPLING_PERIOD0_X4N = 0x18,     /*!< SCLK Period (1/SCLK)x4N */
    ADC_SAMPLING_PERIOD0_X16N = 0x28,    /*!< SCLK Period (1/SCLK)x16N */
    ADC_SAMPLING_PERIOD0_X64N = 0x38,   /*!< SCLK Period (1/SCLK)x64N */
}adc_sampling_period0_t;
/** @enum adc_sampling_period1_t
    @brief Outside AIN sampling period.
 */
typedef enum
{
    ADC_SAMPLING_PERIOD1_XN = 0x000,     /*!< SCLK Period (1/SCLK)xN */
    ADC_SAMPLING_PERIOD1_X2N = 0x100,     /*!< SCLK Period (1/SCLK)x2N */
    ADC_SAMPLING_PERIOD1_X3N = 0x200,     /*!< SCLK Period (1/SCLK)x3N */
    ADC_SAMPLING_PERIOD1_X4N = 0x300,     /*!< SCLK Period (1/SCLK)x4N */
    ADC_SAMPLING_PERIOD1_X16N = 0x500,    /*!< SCLK Period (1/SCLK)x16N */
    ADC_SAMPLING_PERIOD1_X64N = 0x700,   /*!< SCLK Period (1/SCLK)x64N */
}adc_sampling_period1_t;

/*! @enum adc_sclk_t
    @brief Select AD prescaler output (SCLK).
 */
typedef enum
{
    ADC_SCLK_1  = (0x00000000U),                /*!< ADCLK/1 */
    ADC_SCLK_2  = (0x00000001U),                /*!< ADCLK/2 */
    ADC_SCLK_4  = (0x00000002U),                /*!< ADCLK/4 */
    ADC_SCLK_8  = (0x00000003U),                /*!< ADCLK/8 */
    ADC_SCLK_16 = (0x00000004U),                /*!< ADCLK/16 */
}adc_sclk_t;

/*! @enum adc_mod1_t
    @brief Select SCLK Frequency Band (MOD1).
 */
typedef enum
{
	ADC_MOD1_SCLK_1	= (0x00001000U),		/*!< SCLK =< 40MHz > */
	ADC_MOD1_SCLK_2	= (0x00003000U),		/*!< 40MHz < SCLK =< 50MHz > */
	ADC_MOD1_SCLK_3 = (0x00004000U),		/*!< 50MHz < SCLK =< 60MHz > */
	ADC_MOD1_SCLK_4 = (0x00106011U),		/*!< 60MHz < SCLK =< 80MHz > */
}adc_mod1_t;

/*! @enum adc_mod2_t
    @brief Select ADC Product Setting Value (MOD2).
 */
typedef enum
{
    ADC_MOD2_TMPM4G9 = (0x00000000U),          /*!< TMPM4G9 */
    ADC_MOD2_CLEAR   = (0x00000000U),          /*!< Reset Value */
}adc_mod2_t;

/*! @enum adc_int_t
    @brief Select Interrupt Enable/Disable.
 */
typedef enum
{
    ADC_INT_DISABLE = (0x00000000U),           /*!< Disable. */
    ADC_INT_ENABLE  = (0x00000080U),           /*!< Enable. */
}adc_int_t;

/*! @enum adc_conversion_t
    @brief Select conversion method.
 */
typedef enum
{
    ADC_CONVERSION_DISABLE = (0x00000000U),    /*!< Disable. */
    ADC_CONVERSION_CNT     = (0x00000100U),    /*!< Continuation. */
    ADC_CONVERSION_SGL     = (0x00000200U),    /*!< Single. */
    ADC_CONVERSION_TRG     = (0x00000300U),    /*!< Universal Trigger. */
    ADC_CONVERSION_HPTG     = (0x00000400U),    /*!< High Priority Trigger. */
}adc_conversion_t;

/*! @enum adc_dma_int_t
    @brief Select DMA interrupt method.
 */
typedef enum
{
    ADC_DMA_INT_SGL_DISABLE = (0x00000000U),    /*!< Disable. */
    ADC_DMA_INT_SGL_ENABLE = (0x00000020U),    /*!< DMA Single interrupt Enable. */
    ADC_DMA_INT_CNT_DISABLE = (0x00000000U),    /*!< Disable. */
    ADC_DMA_INT_CNT_ENABLE = (0x00000040U),    /*!< DMA Continuation interrupt Enable. */
    ADC_DMA_INT_TRG_DISABLE = (0x00000000U),   /*!< Disable. */
    ADC_DMA_INT_TRG_ENABLE = (0x00000010U),    /*!< DMA Universal Trigger interrupt Enable. */
    ADC_TRG_DISABLE = (0x00000000U),           /*!< Universal Trigger Disable. */
    ADC_TRG_ENABLE = (0x00000001U),            /*!< Universal Trigger Enable. */
    ADC_HPTG_DISABLE = (0x00000000U),          /*!< High Priority Trigger Disable. */
    ADC_HPTG_ENABLE = (0x00000002U),           /*!< High Priority Trigger Enable. */
}adc_dma_int_t;

/*! @enum adc_ain_range_t
    @brief Range of AIN Macro Definisiton.
           Range of AIN be set "(ADC_AIN_RANGE_MIN <= Value <= ADC_AIN_RANGE_MAX)".
 */
typedef enum
{
    ADC_AIN_RANGE_MIN = (0x00000000U),         /*!< Minimum  Value :AINx00 */
    ADC_AIN_RANGE_MAX = (0x00000017U),         /*!< Maximum  Value :AINx23 */
}adc_ain_range_t;

/*! @enum adc_status_t
    @brief AD Running Status.
 */
typedef enum
{
    ADC_STATUS_MASK = (0x00000080U),           /*!< for Mask. */
    ADC_STATUS_SLEEP = (0x00000000U),          /*!< Sleep. */
    ADC_STATUS_RUNNING = (0x00000080U),        /*!< Running. */
}adc_status_t;

/*! @enum adc_cnt_status_t
    @brief Continuity Conversion Running Status.
 */
typedef enum
{
    ADC_CNT_STATUS_MASK = (0x00000008U),       /*!< for Mask. */
    ADC_CNT_STATUS_SLEEP = (0x00000000U),      /*!< Sleep. */
    ADC_CNT_STATUS_RUNNING = (0x00000008U),    /*!< Running. */
}adc_cnt_status_t;

/*! @enum adc_sgl_status_t
    @brief Single Conversion Running Status.
 */
typedef enum
{
    ADC_SGL_STATUS_MASK = (0x00000004U),       /*!< for Mask. */
    ADC_SGL_STATUS_SLEEP = (0x00000000U),      /*!< Sleep. */
    ADC_SGL_STATUS_RUNNING = (0x00000004U),    /*!< Running. */
}adc_sgl_status_t;

/*! @enum adc_trg_status_t
    @brief Trigger Conversion Running Status.
 */
typedef enum
{
    ADC_TRG_STATUS_MASK = (0x00000002U),       /*!< for Mask. */
    ADC_TRG_STATUS_SLEEP = (0x00000000U),      /*!< Sleep. */
    ADC_TRG_STATUS_RUNNING = (0x00000002U),    /*!< Running. */
}adc_trg_status_t;

/*! @enum adc_hpri_status_t
    @brief Trigger Conversion Running Status.
 */
typedef enum
{
    ADC_HPTG_STATUS_MASK = (0x00000001U),       /*!< for Mask. */
    ADC_HPTG_STATUS_SLEEP = (0x00000000U),      /*!< Sleep. */
    ADC_HPTG_STATUS_RUNNING = (0x00000001U),    /*!< Running. */
}adc_hpri_status_t;

/*! @enum adcmpxen_t
    @brief Select Enable, Disable setting(ADxCMPEN).
 */
typedef enum
{
    ADCMP3EN_DISABLE = (0x00000000U),          /*!< Disable. */
    ADCMP3EN_ENABLE = (0x00000008U),           /*!< Enable. */
    ADCMP2EN_DISABLE = (0x00000000U),          /*!< Disable. */
    ADCMP2EN_ENABLE = (0x00000004U),           /*!< Enable. */
    ADCMP1EN_DISABLE = (0x00000000U),          /*!< Disable. */
    ADCMP1EN_ENABLE = (0x00000002U),           /*!< Enable. */
    ADCMP0EN_DISABLE = (0x00000000U),          /*!< Disable. */
    ADCMP0EN_ENABLE = (0x00000001U),           /*!< Enable. */
}adcmpxen_t;

/*! @enum adcmpcnt_t
    @brief Select Compare count num.
 */
typedef enum
{
    ADCMPCNT_1 = (0x00000000U),                /*!< 1 time */
    ADCMPCNT_2 = (0x00000100U),                /*!< 2 times */
    ADCMPCNT_3 = (0x00000200U),                /*!< 3 times */
    ADCMPCNT_4 = (0x00000300U),                /*!< 4 times */
    ADCMPCNT_5 = (0x00000400U),                /*!< 5 times */
    ADCMPCNT_6 = (0x00000500U),                /*!< 6 times */
    ADCMPCNT_7 = (0x00000600U),                /*!< 7 times */
    ADCMPCNT_8 = (0x00000700U),                /*!< 8 times */
    ADCMPCNT_9 = (0x00000800U),                /*!< 9 times */
    ADCMPCNT_10 = (0x00000900U),               /*!< 10 times */
    ADCMPCNT_11 = (0x00000a00U),               /*!< 11 times */
    ADCMPCNT_12 = (0x00000b00U),               /*!< 12 times */
    ADCMPCNT_13 = (0x00000c00U),               /*!< 13 times */
    ADCMPCNT_14 = (0x00000d00U),               /*!< 14 times */
    ADCMPCNT_15 = (0x00000e00U),               /*!< 15 times */
    ADCMPCNT_16 = (0x00000f00U),               /*!< 16 times */
}adcmpcnt_t;

/*! @enum adcmpcond_t
    @brief Compare condition
 */
typedef enum
{
    ADCMPCond_CNT = (0x00000000U),             /*!< Continuous */
    ADCMPCond_ACC = (0x00000040U),             /*!< Accumulation */
}adcmpcond_t;

/*! @enum adcmpbigsml_t
    @brief Compare Big, Small condition
 */
typedef enum
{
    ADCMPBigSml_Big = (0x00000000U),           /*!< Big */
    ADCMPBigSml_Sml = (0x00000020U),           /*!< Small */
}adcmpbigsml_t;

/*! @enum adcmpstr_t
    @brief Select Compare Store register
 */
typedef enum
{
    ADCMPStr_Reg0 = (0x00000000U),                /*!< ADxREG0 */
    ADCMPStr_Reg1 = (0x00000001U),                /*!< ADxREG1 */
    ADCMPStr_Reg2 = (0x00000002U),                /*!< ADxREG2 */
    ADCMPStr_Reg3 = (0x00000003U),                /*!< ADxREG3 */
    ADCMPStr_Reg4 = (0x00000004U),                /*!< ADxREG4 */
    ADCMPStr_Reg5 = (0x00000005U),                /*!< ADxREG5 */
    ADCMPStr_Reg6 = (0x00000006U),                /*!< ADxREG6 */
    ADCMPStr_Reg7 = (0x00000007U),                /*!< ADxREG7 */
    ADCMPStr_Reg8 = (0x00000008U),                /*!< ADxREG8 */
    ADCMPStr_Reg9 = (0x00000009U),                /*!< ADxREG9 */
    ADCMPStr_Reg10 = (0x0000000aU),               /*!< ADxREG10 */
    ADCMPStr_Reg11 = (0x0000000bU),               /*!< ADxREG11 */
    ADCMPStr_Reg12 = (0x0000000cU),               /*!< ADxREG12 */
    ADCMPStr_Reg13 = (0x0000000dU),               /*!< ADxREG13 */
    ADCMPStr_Reg14 = (0x0000000eU),               /*!< ADxREG14 */
    ADCMPStr_Reg15 = (0x0000000fU),               /*!< ADxREG15 */
    ADCMPStr_Reg16 = (0x00000010U),               /*!< ADxREG16 */
    ADCMPStr_Reg17 = (0x00000011U),               /*!< ADxREG17 */
    ADCMPStr_Reg18 = (0x00000012U),               /*!< ADxREG18 */
    ADCMPStr_Reg19 = (0x00000013U),               /*!< ADxREG19 */
    ADCMPStr_Reg20 = (0x00000014U),               /*!< ADxREG20 */
    ADCMPStr_Reg21 = (0x00000015U),               /*!< ADxREG21 */
    ADCMPStr_Reg22 = (0x00000016U),               /*!< ADxREG22 */
    ADCMPStr_Reg23 = (0x00000017U),               /*!< ADxREG23 */
}adcmpstr_t;
/*! @enum adcexazain_t
    @brief Select AIN no.
 */

typedef enum
{
	ADCEXAZSEL_AIN0 = 0,		/*!< AIN0 */
	ADCEXAZSEL_AIN1,			/*!< AIN1 */
	ADCEXAZSEL_AIN2,			/*!< AIN2 */
	ADCEXAZSEL_AIN3,			/*!< AIN3 */
	ADCEXAZSEL_AIN4,			/*!< AIN4 */
	ADCEXAZSEL_AIN5,			/*!< AIN5 */
	ADCEXAZSEL_AIN6,			/*!< AIN6 */
	ADCEXAZSEL_AIN7,			/*!< AIN7 */
	ADCEXAZSEL_AIN8,			/*!< AIN8 */
	ADCEXAZSEL_AIN9,			/*!< AIN9 */
	ADCEXAZSEL_AIN10,			/*!< AIN10 */
	ADCEXAZSEL_AIN11,			/*!< AIN11 */
	ADCEXAZSEL_AIN12,			/*!< AIN12 */
	ADCEXAZSEL_AIN13,			/*!< AIN13 */
	ADCEXAZSEL_AIN14,			/*!< AIN14 */
	ADCEXAZSEL_AIN15,			/*!< AIN15 */
	ADCEXAZSEL_AIN16,			/*!< AIN16 */
	ADCEXAZSEL_AIN17,			/*!< AIN17 */
	ADCEXAZSEL_AIN18,			/*!< AIN18 */
	ADCEXAZSEL_AIN19,			/*!< AIN19 */
	ADCEXAZSEL_AIN20,			/*!< AIN20 */
	ADCEXAZSEL_AIN21,			/*!< AIN21 */
	ADCEXAZSEL_AIN22,			/*!< AIN22 */
	ADCEXAZSEL_AIN23,			/*!< AIN23 */
}adcexazain_t;
/*! @enum adcexazsel_t
    @brief Select sampling period, EXAS0 or EXAS1.
 */
typedef enum
{
	ADCEXAZSEL_EXAZ0 = (0x00000000),			/*< EXAZ0 */
	ADCEXAZSEL_EXAZ1 = (0x00000001),			/*< EXAZ1 */
}adcexazsel_t;

/*!
    @}
 */ /* End of group ADC_Exported_define */

/*------------------------------------------------------------------------------*/
/*  Structure Definition                                                        */
/*------------------------------------------------------------------------------*/
/*! 
    @defgroup ADC_Exported_typedef ADC Exported Typedef
    @{
 */

/*----------------------------------*/
/*! @struct adc_clock_t
    @brief  Clock information structure definenition.
*/
/*----------------------------------*/
typedef struct
{
   adc_sampling_period0_t exaz0;      /*!< Outside AIN0 Sampling period. 
                                            : Use @ref adc_sampling_period0_t.   */
   adc_sampling_period1_t exaz1;      /*!< Outside AIN1 Sampling period. 
                                            : Use @ref adc_sampling_period1_t.   */
   adc_sclk_t vadcld;               /*!< Select AD prescaler output (SCLK).
                                            : Use @ref adc_sclk_t.              */
   uint32_t   sampling_select;      /*!< Sampling period select. : bit0-bit23 */
} adc_clock_t;

/*----------------------------------*/
/*! @struct adc_cmpx_t
    @brief  Clock information structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t CMPEN;             /*!< Enable Register status */
    uint32_t CMPCNT;            /*!< Compare count num. */
    uint32_t CMPCond;           /*!< Compare condition */
    uint32_t CMPBigSml;         /*!< Compare Big/Small condition */
    uint32_t StrReg;            /*!< Compare Store register */
    uint32_t ADComp;            /*!< ADxCMP0 register data */
    void (*handle)(uint32_t id, TXZ_Result result);        /*!< Notity Compare Done.       */
} adc_cmpx_t;

/*----------------------------------*/
/*! @struct adc_initial_setting_t
    @brief  Initial setting structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    uint32_t        id;     /*!< ID: User value. */
    adc_clock_t  clk;    /*!< Clock setting.
                                : Use @ref adc_clock_t. */
    uint32_t        mod1;   /*!< AVDD3 voltage band setting.
                                : Use @ref adc_mod1_t. */
    uint32_t        mod2;   /*!< Product info setting.
                                : Use @ref adc_mod2_t. */
    adc_cmpx_t   CMPxEN[4]; /*!< adc_cmpx_t. */


} adc_initial_setting_t;

/*----------------------------------*/
/*! @struct adc_channel_setting_t
    @brief  Channel Setting. \n
*/
/*----------------------------------*/
typedef struct
{
    uint32_t interrupt; /*!< Interrupt Enable/Disable.
                                : Use @ref adc_dma_int_t.       */
    uint32_t type;      /*!< Conversion Type.
                                : Use @ref adc_conversion_t.    */
    uint32_t ain;       /*!< AIN.                               */
} adc_channel_setting_t;

/*----------------------------------*/
/*! @struct adc_internal_info_t
    @brief  Driver internal information structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    adc_ch_t ch[ADC_NUM_MAX];   /*!< Channel Instance. */
} adc_internal_info_t;

/*----------------------------------*/
/*! @struct adc_t
    @brief  ADC handle structure definenition.
*/
/*----------------------------------*/
typedef struct
{
    TSB_AD_TypeDef              *p_instance;     /*!< Registers base address.           */
    adc_initial_setting_t    init;            /*!< Initial setting.                  */
    adc_internal_info_t      info;            /*!< Driver internal information.      */
    struct
    {
        void (*single)(uint32_t id, TXZ_Result result);        /*!< Notity Single Conversion Done.       */
        void (*continuity)(uint32_t id, TXZ_Result result);    /*!< Notity Continuity Conversion Done.   */
        void (*trigger)(uint32_t id, TXZ_Result result);       /*!< Notity Trigger Conversion Done.      */
        void (*highpriority)(uint32_t id, TXZ_Result result);       /*!< Notity HighPriority Conversion Done.      */
    } handler;                                                 /*!< Handler structure. */
} adc_t;

/**
 *  @}
 */ /* End of group ADC_Exported_typedef */

/*------------------------------------------------------------------------------*/
/*  Functions                                                                   */
/*------------------------------------------------------------------------------*/
/** 
 *  @defgroup ADC_Exported_functions ADC Exported Functions
 *  @{
 */
TXZ_Result adc_init(adc_t *p_obj);
TXZ_Result adc_deinit(adc_t *p_obj);
TXZ_Result adc_mode1_setting(void);
TXZ_Result adc_channel_setting(adc_t *p_obj, uint32_t ch, adc_channel_setting_t *p_setting);
TXZ_Result adc_channel_clear(adc_t *p_obj, uint32_t ch);
TXZ_Result adc_cmp_init(adc_t *p_obj, adc_cmpx_t *p_cmpx_t);
TXZ_Result adc_cmp_deinit(adc_t *p_obj, adc_cmpx_t *p_cmpx_t);
TXZ_Result adc_channel_get_value(adc_t *p_obj, uint32_t ch, uint32_t *p_value);
TXZ_Result adc_start(adc_t *p_obj);
TXZ_Result adc_stop(adc_t *p_obj);
TXZ_Result adc_get_status(adc_t *p_obj, uint32_t *p_status);
TXZ_WorkState adc_poll_conversion(adc_t *p_obj, uint32_t timeout);
void adc_compa_irq_handler( void );
void adc_compb_irq_handler( void );
void adc_single_irq_handler( void );
void adc_continuity_irq_handler( void );
void adc_trigger_irq_handler( void );
/**
 *  @}
 */ /* End of group ADC_Exported_functions */

/**
 *  @}
 */ /* End of group ADC */

/**
 *  @}
 */ /* End of group Periph_Driver */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ADC_H */


