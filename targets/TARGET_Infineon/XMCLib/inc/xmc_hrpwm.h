
/**
 * @file xmc_hrpwm.h
 * @date 2015-06-20
 *
 * @cond
 **********************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without           
 * modification,are permitted provided that the following conditions are met:   
 *                                                                              
 *   Redistributions of source code must retain the above copyright notice,      
 *   this list of conditions and the following disclaimer.                        
 * 
 *   Redistributions in binary form must reproduce the above copyright notice,   
 *   this list of conditions and the following disclaimer in the documentation    
 *   and/or other materials provided with the distribution.                       
 * 
 *   Neither the name of the copyright holders nor the names of its contributors 
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   
 * ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   
 * LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      
 * CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   
 * POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share        
 * modifications, enhancements or bug fixes with Infineon Technologies AG       
 * dave@infineon.com).                                                          
 **********************************************************************************
 *
 * Change History
 * --------------
 *
 * 2015-02-18:
 *     - Driver description updated<br>
 *     - API's are renamed <br>
 *       XMC_HRPWM_HRC_SetResolutionCR1() -> XMC_HRPWM_HRC_SetCompare1()<br>
 *       XMC_HRPWM_HRC_SetResolutionCR2() -> XMC_HRPWM_HRC_SetCompare2()<br>
 *       XMC_HRPWM_HRC_SetDeadTimeDCF()   -> XMC_HRPWM_HRC_SetDeadTimeFalling()<br>
 *       XMC_HRPWM_HRC_SetDeadTimeDCR()   -> XMC_HRPWM_HRC_SetDeadTimeRising()<br>
 *      
 * 2015-05-12:
 *     - XMC_HRPWM_CSG_SelClampingInput() api is added to select the clamping input<br>
 *     - Enum XMC_HRPWM_SHADOW_TX_t is renamed to XMC_HRPWM_SHADOW_TX_DAC_t to represent that shadow transfer is for DAC <br>
 *
 * 2015-06-20: 
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *     - Updated copyright and change history section.
 *
 * @endcond 
 *
 */

#ifndef HRPWM_H
#define HRPWM_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * HEADER FILES
 **********************************************************************************************************************/
#include <xmc_common.h>

#if defined(HRPWM0)
#include <xmc_hrpwm_map.h>

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */
 
/**
 * @addtogroup HRPWM
 * @brief High Resolution PWM Unit (HRPWM) driver for the XMC microcontroller family.<br>
 *
 * The HRPWM extends the capabilities of the associated Capture/Compare Unit(CCU8), that simplifies the design various of SMPS.
 * It allows easy and fast implementation of control loop, reduced total number of external components, avoid susceptibility
 * to environmental and process variations there by reducing the size of the power supply.<br>
 *
 *<b>Comparator Slope Generator(CSG)</b><br>
 * HRPWM module consists 3 Comparator Slope Generator(CSG) units. Each CSG unit comprised of one High Speed Comparator,
 * a dedicated 10 bit 30 MS/s DAC and one hardware controlled Slope Compensation module.<br>
 *
 * <b>CSG features include:</b><br>
 * <OL>
 * <LI>3 High Speed Comparators, that can be use to compare an external signal against the DAC value<br>
 * <LI>3 (30MS/s) 10 bit DAC<br>
 * <LI>3 Slope generation blocks, that are used to generate the DAC input value<br>
 * <LI>different slope generations schemes, for a flexible and automated DAC voltage generation<br>
 * <LI>2 DAC reference values, to allow flexible hysteretic mode control<br>
 * <LI>Input multiplexer for the inverting comparator input, allowing several analog inputs to be connected to each comparator and also dynamic input switching.<br>
 * <LI>blanking compare mode, to avoid premature switch OFF due to noise<br>
 * <LI>a dedicated output per Comparator<br>
 * <LI>programmable clock prescaler<br>
 * <LI>programmable clock pulse swallower for slope linearization with uneven clock scale<br>
 * <LI>different slope generation schemes:<br>
 *     – incrementing mode<br>
 *     – decrementing mode<br>
 *     – triangular mode<br>
 * <LI>shadow transfer for one DAC reference value<br>
 * <LI>external trigger for the DAC<br>
 * <LI>external control for the DAC reference values<br>
 * <LI>four dedicated service request lines<br>
 *</OL>
 *
 *<b>High Resolution Channel unit(HRC)</b><br>
 * It also has 4 High Resolution Channel unit(HRC) that upgrades 4 compare channels of a Capture/Compare unit (CCU8), enabling
 * generation of PWM with 150ps resolution. ie; the rise time and/or fall time of PWM can be changed in steps of 150ps.<br>
 *
 * <b>HRC features include:</b><br>
 * <OL>
 * <LI>Upgrade up to 4 PWM signals of CCU8 outputs for high resolution positioning.<br>
 * <LI>Independent control of PWM set and reset.<br>
 * <LI>Delay the PWM rise time in steps of 150ps. This does not insert dead time.<br>
 * <LI>Extent the fall time of PWM in steps of 150ps. This does not insert dead time.<br>
 * <LI>Dead time insertion on complementary signals<br>
 * <LI>Passive level selection on outputs.<br>
 *</OL>
 * @{
 */
/***********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
#define XMC_HRPWM_CSG0_MEMORY_ADDRESS 0x40020A40 /* CSG0 memory location */
#define XMC_HRPWM_CSG1_MEMORY_ADDRESS 0x40020B40 /* CSG1 memory location */
#define XMC_HRPWM_CSG2_MEMORY_ADDRESS 0x40020C40 /* CSG2 memory location */

#define XMC_HRPWM_COMPARATOR_STATUS (HRPWM0_CSGTRSG_D0STE_Msk + HRPWM0_CSGTRSG_D1STE_Msk + HRPWM0_CSGTRSG_D2STE_Msk)

#define XMC_HRPWM_CHECK_MODULE_PTR(PTR)  ((PTR)== HRPWM0)
#define XMC_HRPWM_CHECK_HRC_PTR(PTR)     ( ((PTR)== HRPWM0_HRC0) || ((PTR)== HRPWM0_HRC1) || ((PTR)== HRPWM0_HRC2) || ((PTR)== HRPWM0_HRC3) )
#define XMC_HRPWM_CHECK_CSG_PTR(PTR)     ( ((PTR)== HRPWM0_CSG0) || ((PTR)== HRPWM0_CSG1) || ((PTR)== HRPWM0_CSG2) )

/***********************************************************************************************************************
 * ENUMS - General
 **********************************************************************************************************************/
/**
 * Return HRPWM driver status
 */
typedef enum XMC_HRPWM_STATUS
{
  XMC_HRPWM_STATUS_OK = 0U,              /**< Driver successfully completed the request */
  XMC_HRPWM_STATUS_BUSY,                 /**< Driver busy, cannot handle request */
  XMC_HRPWM_STATUS_ERROR                 /**< Driver cannot fulfill request, error occurred */
} XMC_HRPWM_STATUS_t;

/***********************************************************************************************************************
 * ENUMS - HRPWM
 **********************************************************************************************************************/
/**
 * HRPWM module clock frequency
 */
typedef enum XMC_HRPWM_CLK_FREQ
{
  XMC_HRPWM_CLK_FREQ_NONE   = 0U,         /**< No clock frequency is selected */
  XMC_HRPWM_CLK_FREQ_180MHZ,              /**< Module clock frequency is 180MHz */
  XMC_HRPWM_CLK_FREQ_120MHZ,              /**< Module clock frequency is 120MHz */
  XMC_HRPWM_CLK_FREQ_80MHZ                /**< Module clock frequency is 80MHz */
} XMC_HRPWM_CLK_FREQ_t;

/***********************************************************************************************************************
 * ENUMS - HRPWM HRC
 **********************************************************************************************************************/
/**
 * HRPWM HRC High Resolution mode configuration
 */
typedef enum XMC_HRPWM_HRC_HR_EDGE
{
  XMC_HRPWM_HRC_HR_EDGE_SEL_RISING = 0U,  /**< Rising edge high resolution signal positioning enabled */
  XMC_HRPWM_HRC_HR_EDGE_SEL_FALLING,      /**< Falling edge high resolution signal positioning enabled */
  XMC_HRPWM_HRC_HR_EDGE_SEL_BOTH,         /**< Both edges high resolution signal positioning enabled */
  XMC_HRPWM_HRC_HR_EDGE_SEL_NONE          /**< No high resolution signal positioning */
} XMC_HRPWM_HRC_HR_EDGE_t;

/**
 * HRPWM HRC source selector input
 */
typedef enum XMC_HRPWM_HRC_SRC_INPUT
{
  XMC_HRPWM_HRC_SRC_INPUT_CCU = 0U,       /**< Source selector is controlled via CCU timer signal */
  XMC_HRPWM_HRC_SRC_INPUT_CSG             /**< Source selector is controlled via CSG output signal */
} XMC_HRPWM_HRC_SRC_INPUT_t;

/**
 * HRPWM HRC source selector - connection of source selector to which CSG unit
 */
typedef enum XMC_HRPWM_HRC_CMP_SEL
{
  XMC_HRPWM_HRC_CMP_SEL_CSG0 = 0U,       /**< Comparator output of CSG0 selected */
  XMC_HRPWM_HRC_CMP_SEL_CSG1,            /**< Comparator output of CSG1 selected */
  XMC_HRPWM_HRC_CMP_SEL_CSG2             /**< Comparator output of CSG2 selected */
} XMC_HRPWM_HRC_CMP_SEL_t;

/**
 * HRPWM HRC source selector - connection of source selector to which CCU timer
 */
typedef enum XMC_HRPWM_HRC_TIMER_SEL
{
  XMC_HRPWM_HRC_TIMER_SEL_CCU_CC0 = 0U,  /**< CCU timer 0 selected */
  XMC_HRPWM_HRC_TIMER_SEL_CCU_CC1,       /**< CCU timer 1 selected */
  XMC_HRPWM_HRC_TIMER_SEL_CCU_CC2,       /**< CCU timer 2 selected */
  XMC_HRPWM_HRC_TIMER_SEL_CCU_CC3        /**< CCU timer 3 selected */
} XMC_HRPWM_HRC_TIMER_SEL_t;

/**
 * HR source selector edge configuration (GSEL)
 */
typedef enum XMC_HRPWM_HRC_SRC_EDGE_SEL
{
  XMC_HRPWM_HRC_SRC_EDGE_SEL_DISABLED = 0U,  /**< source signal generation disabled */
  XMC_HRPWM_HRC_SRC_EDGE_SEL_RISING,         /**< source signal generation on rising edge */
  XMC_HRPWM_HRC_SRC_EDGE_SEL_FALLING,        /**< source signal generation on falling edge */
  XMC_HRPWM_HRC_SRC_EDGE_SEL_BOTH            /**< source signal generation on both edges */
} XMC_HRPWM_HRC_SRC_EDGE_SEL_t;

/**
 * HRPWM function Enable / Disable status
 */
typedef enum XMC_HRPWM_FUNC_STATUS
{
  XMC_HRPWM_FUNC_STATUS_DISABLE = 0U,       /**< Function is disabled  */
  XMC_HRPWM_FUNC_STATUS_ENABLE = 1U         /**< Function is enabled  */
} XMC_HRPWM_FUNC_STATUS_t;

/**
 * HRPWM high resolution module status
 */
typedef enum XMC_HRPWM_HR_LOGIC
{
  XMC_HRPWM_HR_LOGIC_NOT_WORKING = 0U,     /**< High resolution signal path is switched off for all HRC channels */
  XMC_HRPWM_HR_LOGIC_WORKING               /**< High resolution signal path is switched on for all HRC channels */
} XMC_HRPWM_HR_LOGIC_t;

/**
 * High resolution paths for HRC channels
 */
typedef enum XMC_HRPWM_HR_PATH
{
  XMC_HRPWM_HR_PATH_HRC0 = HRPWM0_HRCCFG_HRC0E_Msk,  /**< HRC0 path selected for High resolution */
  XMC_HRPWM_HR_PATH_HRC1 = HRPWM0_HRCCFG_HRC1E_Msk,  /**< HRC1 path selected for High resolution */
  XMC_HRPWM_HR_PATH_HRC2 = HRPWM0_HRCCFG_HRC2E_Msk,  /**< HRC2 path selected for High resolution */
  XMC_HRPWM_HR_PATH_HRC3 = HRPWM0_HRCCFG_HRC3E_Msk,  /**< HRC3 path selected for High resolution */
} XMC_HRPWM_HR_PATH_t;

/**
 * @brief Low resolution paths for HRC channels
 */
typedef enum XMC_HRPWM_LR_PATH
{
  XMC_HRPWM_LR_PATH_HRC0 = HRPWM0_HRCCFG_LRC0E_Msk,  /**< LRC0 path selected for Low resolution */
  XMC_HRPWM_LR_PATH_HRC1 = HRPWM0_HRCCFG_LRC1E_Msk,  /**< LRC0 path selected for Low resolution */
  XMC_HRPWM_LR_PATH_HRC2 = HRPWM0_HRCCFG_LRC2E_Msk,  /**< LRC0 path selected for Low resolution */
  XMC_HRPWM_LR_PATH_HRC3 = HRPWM0_HRCCFG_LRC3E_Msk   /**< LRC0 path selected for Low resolution */
} XMC_HRPWM_LR_PATH_t;

/**
 * Shadow transfer for HRC values
 * The enum is used to access the bitfields of registers HRCSTRG, HRCCTRG, HRCSTSG
 */
typedef enum XMC_HRPWM_HRC_SHADOW_TX
{
  XMC_HRPWM_HRC_SHADOW_TX_HRC0_VALUE      = 0x1U,     /**< HRC0 shadow transfer mask for CR1 & CR2 */
  XMC_HRPWM_HRC_SHADOW_TX_HRC0_DT_VALUE   = 0x2U,     /**< HRC0 shadow transfer mask for DCR & DCRF */
  XMC_HRPWM_HRC_SHADOW_TX_HRC1_VALUE      = 0x10U,    /**< HRC1 shadow transfer mask for CR1 & CR2 */
  XMC_HRPWM_HRC_SHADOW_TX_HRC1_DT_VALUE   = 0x20U,    /**< HRC1 shadow transfer mask for DCR & DCRF */
  XMC_HRPWM_HRC_SHADOW_TX_HRC2_VALUE      = 0x100U,   /**< HRC2 shadow transfer mask for CR1 & CR2 */
  XMC_HRPWM_HRC_SHADOW_TX_HRC2_DT_VALUE   = 0x200U,   /**< HRC2 shadow transfer mask for DCR & DCRF */
  XMC_HRPWM_HRC_SHADOW_TX_HRC3_VALUE      = 0x1000U,  /**< HRC3 shadow transfer mask for CR1 & CR2 */
  XMC_HRPWM_HRC_SHADOW_TX_HRC3_DT_VALUE   = 0x2000U   /**< HRC3 shadow transfer mask for DCR & DCRF */
} XMC_HRPWM_HRC_SHADOW_TX_t;

/**
 * HR source selector
 */
typedef enum XMC_HRPWM_HRC_SOURCE
{
  XMC_HRPWM_HRC_SOURCE_0 = 0U,  /**< High resolution source 0 */
  XMC_HRPWM_HRC_SOURCE_1        /**< High resolution source 1 */
} XMC_HRPWM_HRC_SOURCE_t;

/**
 * HRC dead time shadow transfer trigger selection
 */
typedef enum XMC_HRPWM_HRC_DT_TR_SEL
{
  XMC_HRPWM_HRC_DT_TR_SEL_TIMER = 0U,  /**< Source for shadow transfer trigger is CCU8 timer. */
  XMC_HRPWM_HRC_DT_TR_SEL_OVERFLOW     /**< Source for shadow transfer trigger is dead time timer overflow. */
} XMC_HRPWM_HRC_DT_TR_SEL_t;

/**
 * HRPWM HRC output - Passive level
 */
typedef enum XMC_HRPWM_HRC_OUT_PASSIVE_LVL
{
  XMC_HRPWM_HRC_OUT_PASSIVE_LVL_LOW = 0U,  /**< Passive low output */
  XMC_HRPWM_HRC_OUT_PASSIVE_LVL_HIGH       /**< Passive high output */
} XMC_HRPWM_HRC_OUT_PASSIVE_LVL_t;

/***********************************************************************************************************************
 * ENUMS - HRPWM CSG
 **********************************************************************************************************************/
/**
 * CSG power modes
 */
 typedef enum XMC_HRPWM_CSG_POWER_MODE
{
  XMC_HRPWM_CSG_POWER_MODE_OFF       = 0U << HRPWM0_CSGCFG_C0PM_Pos,   /**< Comparator slope generator turned off */
  XMC_HRPWM_CSG_POWER_MODE_LOW_SPEED = 1U << HRPWM0_CSGCFG_C0PM_Pos,   /**< Comparator slope generator in low speed mode */
  XMC_HRPWM_CSG_POWER_MODE_HI_SPEED  = 3U << HRPWM0_CSGCFG_C0PM_Pos    /**< Comparator slope generator in high speed mode */
} XMC_HRPWM_CSG_POWER_MODE_t;

/**
 * DAC, Comparator start controls & Comparator clamped state control
 * The enum is used to access the bitfields of registers CSGSETG, CSGCLRG, CSGSTATG
 */
typedef enum XMC_HRPWM_CSG_RUN_BIT
{
  XMC_HRPWM_CSG_RUN_BIT_DAC0       = 0x1U,    /**< Start DAC0 */
  XMC_HRPWM_CSG_RUN_BIT_CMP0       = 0x2U,    /**< Start comparator 0 */
  XMC_HRPWM_CSG_RUN_BIT_CMP0_PSL   = 0x4U,    /**< Set comparator 0 output to clamped state */
  XMC_HRPWM_CSG_RUN_BIT_DAC1       = 0x10U,   /**< Start DAC1 */
  XMC_HRPWM_CSG_RUN_BIT_CMP1       = 0x20U,   /**< Start comparator 1 */
  XMC_HRPWM_CSG_RUN_BIT_CMP1_PSL   = 0x40U,   /**< Set comparator 1 output to clamped state */
  XMC_HRPWM_CSG_RUN_BIT_DAC2       = 0x100U,  /**< Start DAC2 */
  XMC_HRPWM_CSG_RUN_BIT_CMP2       = 0x200U,  /**< Start comparator2 */
  XMC_HRPWM_CSG_RUN_BIT_CMP2_PSL   = 0x400U   /**< Set comparator 2 output to clamped state */
} XMC_HRPWM_CSG_RUN_BIT_t;

/**
 * Slope start for DAC units
 */
typedef enum XMC_HRPWM_CSG_SLOPE_START
{
  XMC_HRPWM_CSG_SLOPE_START_DAC0  = HRPWM0_CSGFCG_S0STR_Msk,       /**< Start slope generation for DAC0 */
  XMC_HRPWM_CSG_SLOPE_START_DAC1  = HRPWM0_CSGFCG_S1STR_Msk,       /**< Start slope generation for DAC1 */
  XMC_HRPWM_CSG_SLOPE_START_DAC2  = HRPWM0_CSGFCG_S2STR_Msk        /**< Start slope generation for DAC2 */
} XMC_HRPWM_CSG_SLOPE_START_t;

/**
 * Slope stop for DAC units
 */
typedef enum XMC_HRPWM_CSG_SLOPE_STOP
{
  XMC_HRPWM_CSG_SLOPE_STOP_DAC0  = HRPWM0_CSGFCG_S0STP_Msk,        /**< Stop slope generation for DAC0 */
  XMC_HRPWM_CSG_SLOPE_STOP_DAC1  = HRPWM0_CSGFCG_S1STP_Msk,        /**< Stop slope generation for DAC1 */
  XMC_HRPWM_CSG_SLOPE_STOP_DAC2  = HRPWM0_CSGFCG_S2STP_Msk         /**< Stop slope generation for DAC2 */
} XMC_HRPWM_CSG_SLOPE_STOP_t;

/**
 * Prescaler start in CSG
 */
typedef enum XMC_HRPWM_CSG_PRESCALER_START
{
  XMC_HRPWM_CSG_PRESCALER_START_CSG0  = HRPWM0_CSGFCG_PS0STR_Msk,  /**< Start prescaler of CSG0 */
  XMC_HRPWM_CSG_PRESCALER_START_CSG1  = HRPWM0_CSGFCG_PS1STR_Msk,  /**< Start prescaler of CSG1 */
  XMC_HRPWM_CSG_PRESCALER_START_CSG2  = HRPWM0_CSGFCG_PS2STR_Msk   /**< Start prescaler of CSG2 */
} XMC_HRPWM_CSG_PRESCALER_START_t;

/**
 * Prescaler stop in CSG
 */
typedef enum XMC_HRPWM_CSG_PRESCALER_STOP
{
  XMC_HRPWM_CSG_PRESCALER_STOP_CSG0  = HRPWM0_CSGFCG_PS0STP_Msk,   /**< Stop prescaler of CSG0 */
  XMC_HRPWM_CSG_PRESCALER_STOP_CSG1  = HRPWM0_CSGFCG_PS1STP_Msk,   /**< Stop prescaler of CSG1 */
  XMC_HRPWM_CSG_PRESCALER_STOP_CSG2  = HRPWM0_CSGFCG_PS2STP_Msk    /**< Stop prescaler of CSG2 */
} XMC_HRPWM_CSG_PRESCALER_STOP_t;

/**
 * Clear prescaler in CSG 
 */
typedef enum XMC_HRPWM_CSG_PRESCALER_CLR
{
  XMC_HRPWM_CSG_PRESCALER_CLR_CSG0  = HRPWM0_CSGFCG_PS0CLR_Msk,    /**< Clear prescaler of CSG0 */
  XMC_HRPWM_CSG_PRESCALER_CLR_CSG1  = HRPWM0_CSGFCG_PS1CLR_Msk,    /**< Clear prescaler of CSG1 */
  XMC_HRPWM_CSG_PRESCALER_CLR_CSG2  = HRPWM0_CSGFCG_PS2CLR_Msk     /**< Clear prescaler of CSG2 */
} XMC_HRPWM_CSG_PRESCALER_CLR_t;

/**
 * DAC slope generation status
 */
typedef enum XMC_HRPWM_DAC_SLOPE_GEN_STATUS
{
  XMC_HRPWM_DAC_SLOPE_GEN_STATUS_DAC0  = HRPWM0_CSGFSG_S0RB_Msk,   /**< Slope generation status mask for DAC0 */
  XMC_HRPWM_DAC_SLOPE_GEN_STATUS_DAC1  = HRPWM0_CSGFSG_S1RB_Msk,   /**< Slope generation status mask for DAC1 */
  XMC_HRPWM_DAC_SLOPE_GEN_STATUS_DAC2  = HRPWM0_CSGFSG_S2RB_Msk    /**< Slope generation status mask for DAC2 */
} XMC_HRPWM_DAC_SLOPE_GEN_STATUS_t;

/**
 * CSG prescaler status
 */
typedef enum XMC_HRPWM_CSG_PRESCALER_STATUS
{
  XMC_HRPWM_CSG_PRESCALER_STATUS_CSG0  = HRPWM0_CSGFSG_P0RB_Msk,   /**< Prescaler status in CSG0 */
  XMC_HRPWM_CSG_PRESCALER_STATUS_CSG1  = HRPWM0_CSGFSG_P1RB_Msk,   /**< Prescaler status in CSG1 */
  XMC_HRPWM_CSG_PRESCALER_STATUS_CSG2  = HRPWM0_CSGFSG_P2RB_Msk    /**< Prescaler status in CSG2 */
} XMC_HRPWM_CSG_PRESCALER_STATUS_t;

/**
 * Comparator inputs
 */
typedef enum XMC_HRPWM_CSG_CMP_INPUT
{
  XMC_HRPWM_CSG_CMP_INPUT_CINA = 0U,  /**< Input for comparator is CINA */
  XMC_HRPWM_CSG_CMP_INPUT_CINB        /**< Input for comparator is CINB */
} XMC_HRPWM_CSG_CMP_INPUT_t;

/**
 * CSG comparator input switch request
 */
typedef enum XMC_HRPWM_CSG_SWITCH_CMP_INPUT
{
  XMC_HRPWM_CSG_SWITCH_CMP_INPUT_CMP0 = HRPWM0_CSGTRSG_SW0ST_Msk,  /**< Request to switch the analog input connected to the comparator 0 between CINA and CINB */
  XMC_HRPWM_CSG_SWITCH_CMP_INPUT_CMP1 = HRPWM0_CSGTRSG_SW1ST_Msk,  /**< Request to switch the analog input connected to the comparator 1 between CINA and CINB */
  XMC_HRPWM_CSG_SWITCH_CMP_INPUT_CMP2 = HRPWM0_CSGTRSG_SW2ST_Msk   /**< Request to switch the analog input connected to the comparator 2 between CINA and CINB */
} XMC_HRPWM_CSG_SWITCH_CMP_INPUT_t;

/**
 * CSG comparator input switch request
 */
typedef enum XMC_HRPWM_CSG_CMP_INVERTING_INPUT
{
  XMC_HRPWM_CSG_CMP_INVERTING_INPUT_CMP0 = HRPWM0_CSGTRSG_D0STE_Msk,  /**< Comparator 0 inverting input connection */
  XMC_HRPWM_CSG_CMP_INVERTING_INPUT_CMP1 = HRPWM0_CSGTRSG_D1STE_Msk,  /**< Comparator 1 inverting input connection */
  XMC_HRPWM_CSG_CMP_INVERTING_INPUT_CMP2 = HRPWM0_CSGTRSG_D2STE_Msk   /**< Comparator 2 inverting input connection */
} XMC_HRPWM_CSG_CMP_INVERTING_INPUT_t;

/**
 * Input list to CSG
 */
typedef enum XMC_HRPWM_CSG_INPUT_SEL
{
  XMC_HRPWM_CSG_INPUT_SEL_IA = 0U,  /**< Input selected for blanking or comparator switch: Input-A */
  XMC_HRPWM_CSG_INPUT_SEL_IB,       /**< Input selected for blanking or comparator switch: Input-B */
  XMC_HRPWM_CSG_INPUT_SEL_IC,       /**< Input selected for blanking or comparator switch: Input-C */
  XMC_HRPWM_CSG_INPUT_SEL_ID,       /**< Input selected for blanking or comparator switch: Input-D */
  XMC_HRPWM_CSG_INPUT_SEL_IE,       /**< Input selected for blanking or comparator switch: Input-E */
  XMC_HRPWM_CSG_INPUT_SEL_IF,       /**< Input selected for blanking or comparator switch: Input-F */
  XMC_HRPWM_CSG_INPUT_SEL_IG,       /**< Input selected for blanking or comparator switch: Input-G */
  XMC_HRPWM_CSG_INPUT_SEL_IH,       /**< Input selected for blanking or comparator switch: Input-H */
  XMC_HRPWM_CSG_INPUT_SEL_II,       /**< Input selected for blanking or comparator switch: Input-I */
  XMC_HRPWM_CSG_INPUT_SEL_IJ,       /**< Input selected for blanking or comparator switch: Input-J */
  XMC_HRPWM_CSG_INPUT_SEL_IK,       /**< Input selected for blanking or comparator switch: Input-K */
  XMC_HRPWM_CSG_INPUT_SEL_IL,       /**< Input selected for blanking or comparator switch: Input-L */
  XMC_HRPWM_CSG_INPUT_SEL_IM,       /**< Input selected for blanking or comparator switch: Input-M */
  XMC_HRPWM_CSG_INPUT_SEL_IN,       /**< Input selected for blanking or comparator switch: Input-N */
  XMC_HRPWM_CSG_INPUT_SEL_IO,       /**< Input selected for blanking or comparator switch: Input-O */
  XMC_HRPWM_CSG_INPUT_SEL_IP        /**< Input selected for blanking or comparator switch: Input-P */
} XMC_HRPWM_CSG_INPUT_SEL_t;

/**
 * HRPWM CSG - Selection of edge sensitivity
 */
typedef enum XMC_HRPWM_CSG_EDGE_SEL
{
  XMC_HRPWM_CSG_EDGE_SEL_DISABLED = 0U,  /**< Trigger event not generated */
  XMC_HRPWM_CSG_EDGE_SEL_RISING_EDGE,    /**< Trigger event not generated in rising edge */
  XMC_HRPWM_CSG_EDGE_SEL_FALLING_EDGE,   /**< Trigger event not generated in falling edge */
  XMC_HRPWM_CSG_EDGE_SEL_BOTH_EDGE       /**< Trigger event not generated in both edges */
} XMC_HRPWM_CSG_EDGE_SEL_t;

/**
 * HRPWM CSG - Selection of level sensitivity
 */
typedef enum XMC_HRPWM_CSG_LVL_SEL
{
  XMC_HRPWM_CSG_LVL_SEL_DISABLED = 0U,  /**< Level sensitivity is disabled */
  XMC_HRPWM_CSG_LVL_SEL_HIGH,           /**< Level sensitivity is High */
  XMC_HRPWM_CSG_LVL_SEL_LOW             /**< Level sensitivity is Low */
} XMC_HRPWM_CSG_LVL_SEL_t;

/**
 * HRPWM CSG - Slope Generation clock selection
 */
typedef enum XMC_HRPWM_CSG_CLK_INPUT
{
  XMC_HRPWM_CSG_CLK_INPUT_MCLK = 0U,  /**< Clock for CSG is module clock */
  XMC_HRPWM_CSG_CLK_INPUT_ECLKA,      /**< Clock for CSG is external clock A */
  XMC_HRPWM_CSG_CLK_INPUT_ECLKB,      /**< Clock for CSG is external clock B */
  XMC_HRPWM_CSG_CLK_INPUT_ECLKC       /**< Clock for CSG is external clock C */
} XMC_HRPWM_CSG_CLK_INPUT_t;

/**
 * HRPWM CSG - IRQ Event Id
 * The enum is used to access the bitfields of registers CSGySRE, CSGySRS, CSGySWS, CSGySWC, CSGyISTAT
 */
typedef enum XMC_HRPWM_CSG_IRQ_ID
{
  XMC_HRPWM_CSG_IRQ_ID_VLS1 = 0x1U,   /**< Interrupt on DAC value switch from CSGyDSV1 to CSGyDSV2 interrupt */
  XMC_HRPWM_CSG_IRQ_ID_VLS2 = 0x2U,   /**< Interrupt on DAC value switch from CSGyDSV2 to CSGyDSV1 interrupt */
  XMC_HRPWM_CSG_IRQ_ID_TRGS = 0x4U,   /**< Interrupt on DAC conversion trigger */
  XMC_HRPWM_CSG_IRQ_ID_STRS = 0x8U,   /**< Interrupt on DAC start trigger */
  XMC_HRPWM_CSG_IRQ_ID_STPS = 0x10U,  /**< Interrupt on DAC stop trigger */
  XMC_HRPWM_CSG_IRQ_ID_STD  = 0x20U,  /**< Interrupt on DAC shadow transfer */
  XMC_HRPWM_CSG_IRQ_ID_CRSE = 0x40U,  /**< Interrupt on comparator output rise edge */
  XMC_HRPWM_CSG_IRQ_ID_CFSE = 0x80U,  /**< Interrupt on comparator output fall edge */
  XMC_HRPWM_CSG_IRQ_ID_CSEE = 0x100U  /**< Interrupt on comparator output clamped state */
} XMC_HRPWM_CSG_IRQ_ID_t;

/**
 * HRPWM CSG - Initial DAC start mode
 */
typedef enum XMC_HRPWM_CSG_SWSM
{
  XMC_HRPWM_CSG_SWSM_DSV2_W_TRIGGER = 0U,  /**< DSV2 is used as initial DAC value & conversion trigger is generated */
  XMC_HRPWM_CSG_SWSM_DSV1_W_TRIGGER,       /**< DSV1 is used as initial DAC value & conversion trigger is generated */
  XMC_HRPWM_CSG_SWSM_DSV2_NO_TRIGGER,      /**< DSV2 is used as initial DAC value & no conversion trigger generated */
  XMC_HRPWM_CSG_SWSM_DSV1_NO_TRIGGER       /**< DSV1 is used as initial DAC value & no conversion trigger generated */
} XMC_HRPWM_CSG_SWSM_t;

/**
 * HRPWM CSG - Configuration for Clock disable
 */
typedef enum XMC_HRPWM_CSG_CLK
{
  XMC_HRPWM_CSG_CLK_CSG0 = HRPWM0_CSGCFG_C0CD_Msk,  /**< CSG0 clock mask */
  XMC_HRPWM_CSG_CLK_CSG1 = HRPWM0_CSGCFG_C1CD_Msk,  /**< CSG1 clock mask */
  XMC_HRPWM_CSG_CLK_CSG2 = HRPWM0_CSGCFG_C2CD_Msk   /**< CSG2 clock mask */
} XMC_HRPWM_CSG_CLK_t;

/**
 * HRPWM CSG - DAC shadow transfer values
 */
typedef enum XMC_HRPWM_SHADOW_TX
{
  XMC_HRPWM_SHADOW_TX_DAC0 = HRPWM0_CSGTRC_D0SEC_Msk,  /**< Shadow transfer mask for DAC0 - reference value 1 & Pulse swallow value */
  XMC_HRPWM_SHADOW_TX_DAC1 = HRPWM0_CSGTRC_D1SEC_Msk,  /**< Shadow transfer mask for DAC1 - reference value 1 & Pulse swallow value */
  XMC_HRPWM_SHADOW_TX_DAC2 = HRPWM0_CSGTRC_D2SEC_Msk   /**< Shadow transfer mask for DAC2 - reference value 1 & Pulse swallow value */
} XMC_HRPWM_SHADOW_TX_DAC_t;

/**
 * HRPWM CSG - Service request line
 */
typedef enum XMC_HRPWM_CSG_IRQ_SR_LINE
{
  XMC_HRPWM_CSG_IRQ_SR_LINE_0 = 0U, /**< CSG - Service request SR-0 */
  XMC_HRPWM_CSG_IRQ_SR_LINE_1 = 1U, /**< CSG - Service request SR-1 */
  XMC_HRPWM_CSG_IRQ_SR_LINE_2 = 2U, /**< CSG - Service request SR-2 */
  XMC_HRPWM_CSG_IRQ_SR_LINE_3 = 3U  /**< CSG - Service request SR-3 */
} XMC_HRPWM_CSG_IRQ_SR_LINE_t;

/**
 * HRPWM CSG - Slope Generation control mode
 */
typedef enum XMC_HRPWM_CSG_SLOPE_CTRL_MODE
{
  XMC_HRPWM_CSG_SLOPE_CTRL_MODE_STATIC = 0U,  /**< Slope generation mode - Static mode */
  XMC_HRPWM_CSG_SLOPE_CTRL_MODE_DEC_GEN,      /**< Slope generation mode - Decrementing slope generation */
  XMC_HRPWM_CSG_SLOPE_CTRL_MODE_INC_GEN,      /**< Slope generation mode - Incrementing slope generation */
  XMC_HRPWM_CSG_SLOPE_CTRL_MODE_TRIANGULAR    /**< Slope generation mode - Triangular slope generation */
} XMC_HRPWM_CSG_SLOPE_CTRL_MODE_t;

/**
 * HRPWM CSG - Prescaler external start configuration
 */
typedef enum XMC_HRPWM_CSG_PRESCALER_EXT_START
{
  XMC_HRPWM_CSG_PRESCALER_EXT_START_IGNORE = 0U,  /**< Prescaler operation on external start trigger is: Ignore */
  XMC_HRPWM_CSG_PRESCALER_EXT_START_STRT,         /**< Prescaler operation on external start trigger is: Start prescaler */
  XMC_HRPWM_CSG_PRESCALER_EXT_START_CLR,          /**< Prescaler operation on external start trigger is: Clear prescaler */
  XMC_HRPWM_CSG_PRESCALER_EXT_START_CLR_N_STRT    /**< Prescaler operation on external start trigger is: Clear & Start prescaler */
} XMC_HRPWM_CSG_PRESCALER_EXT_START_t;

/**
 * HRPWM CSG - Prescaler external stop configuration
 */
typedef enum XMC_HRPWM_CSG_PRESCALER_EXT_STOP
{
  XMC_HRPWM_CSG_PRESCALER_EXT_STOP_IGNORE = 0U,  /**< Prescaler operation on external stop trigger is: Ignore */
  XMC_HRPWM_CSG_PRESCALER_EXT_STOP_STP,          /**< Prescaler operation on external stop trigger is: Stop prescaler */
  XMC_HRPWM_CSG_PRESCALER_EXT_STOP_CLR,          /**< Prescaler operation on external stop trigger is: Clear prescaler */
  XMC_HRPWM_CSG_PRESCALER_EXT_STOP_CLR_N_STOP    /**< Prescaler operation on external stop trigger is: Clear & Stop prescaler */
} XMC_HRPWM_CSG_PRESCALER_EXT_STOP_t;

/**
 * HRPWM CSG - Slope Generation external start configuration
 */
typedef enum XMC_HRPWM_CSG_SLOPE_EXT_START
{
  XMC_HRPWM_CSG_SLOPE_EXT_START_IGNORE = 0U,  /**< Slope generation on external start trigger is: Ignore */
  XMC_HRPWM_CSG_SLOPE_EXT_START_STRT,         /**< Slope generation on external start trigger is: Start/restart slope generation */
  XMC_HRPWM_CSG_SLOPE_EXT_START_RESUME,       /**< Slope generation on external start trigger is: Resumes slope generation */
} XMC_HRPWM_CSG_SLOPE_EXT_START_t;

/**
 * HRPWM CGS - Slope Generation external stop configuration
 */
typedef enum XMC_HRPWM_CSG_SLOPE_EXT_STOP
{
  XMC_HRPWM_CSG_SLOPE_EXT_STOP_IGNORE = 0U,  /**< Slope generation on external stop trigger is: Ignore */
  XMC_HRPWM_CSG_SLOPE_EXT_STOP_STP,          /**< Slope generation on external stop trigger is: Stops/Halts the slope generation */
  XMC_HRPWM_CSG_SLOPE_EXT_STOP_FREEZE,       /**< Slope generation on external stop trigger is: Freezes slope generation & feeds constantly
                                                                                                the value programmed in CSGyDSV2 to the DAC */
} XMC_HRPWM_CSG_SLOPE_EXT_STOP_t;

/**
 * HRPWM CSG - Slice numbers
 */
typedef enum XMC_HRPWM_CSG_SLICE
{
  XMC_HRPWM_CSG_SLICE_0 = 0U, /**< CSG slice number is 0 */
  XMC_HRPWM_CSG_SLICE_1,      /**< CSG slice number is 1 */
  XMC_HRPWM_CSG_SLICE_2       /**< CSG slice number is 2 */
} XMC_HRPWM_CSG_SLICE_t;
 
/**
 * HRPWM CSG - Comparator output filter window
 */
typedef enum XMC_HRPWM_CSG_CMP_FILTER_WINDOW
{
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_2_CLK_CYCLES = 0U , /**< Needs to be stable for 2 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_3_CLK_CYCLES,       /**< Needs to be stable for 3 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_4_CLK_CYCLES,       /**< Needs to be stable for 4 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_5_CLK_CYCLES,       /**< Needs to be stable for 5 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_6_CLK_CYCLES,       /**< Needs to be stable for 6 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_7_CLK_CYCLES,       /**< Needs to be stable for 7 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_8_CLK_CYCLES,       /**< Needs to be stable for 8 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_9_CLK_CYCLES,       /**< Needs to be stable for 9 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_10_CLK_CYCLES,      /**< Needs to be stable for 10 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_11_CLK_CYCLES,      /**< Needs to be stable for 11 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_12_CLK_CYCLES,      /**< Needs to be stable for 12 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_13_CLK_CYCLES,      /**< Needs to be stable for 13 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_14_CLK_CYCLES,      /**< Needs to be stable for 14 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_15_CLK_CYCLES,      /**< Needs to be stable for 15 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_16_CLK_CYCLES,      /**< Needs to be stable for 16 clk cycles */
  XMC_HRPWM_CSG_CMP_FILTER_WINDOW_32_CLK_CYCLES       /**< Needs to be stable for 32 clk cycles */
} XMC_HRPWM_CSG_CMP_FILTER_WINDOW_t;
 
/**
 * HRPWM CSG - Slope step gain
 */
typedef enum XMC_HRPWM_CSG_SLOPE_STEP_GAIN
{ 
  XMC_HRPWM_CSG_SLOPE_STEP_GAIN_INC_DEC_BY_1 = 0U,    /**< slope step has an increment/decrement of 1 */
  XMC_HRPWM_CSG_SLOPE_STEP_GAIN_INC_DEC_BY_2,         /**< slope step has an increment/decrement of 2 */
  XMC_HRPWM_CSG_SLOPE_STEP_GAIN_INC_DEC_BY_4,         /**< slope step has an increment/decrement of 4 */
  XMC_HRPWM_CSG_SLOPE_STEP_GAIN_INC_DEC_BY_8          /**< slope step has an increment/decrement of 8 */
} XMC_HRPWM_CSG_SLOPE_STEP_GAIN_t;

/**
 * HRPWM CSG - Slope step gain
 */
typedef enum XMC_HRPWM_CSG_PRESCALER_DIVISION
{
  XMC_HRPWM_CSG_PRESCALER_DIVISION_BY_1 = 0U,         /**< Division by 1 */
  XMC_HRPWM_CSG_PRESCALER_DIVISION_BY_2,              /**< Division by 2 */
  XMC_HRPWM_CSG_PRESCALER_DIVISION_BY_4,              /**< Division by 4 */
  XMC_HRPWM_CSG_PRESCALER_DIVISION_BY_8               /**< Division by 8 */
} XMC_HRPWM_CSG_PRESCALER_DIVISION_t;

/*********************************************************************************************************************
 * DATA STRUCTURES - HRPWM
 ********************************************************************************************************************/
/**
 * Typedef for HRPWM Global registers data structure
 */
typedef HRPWM0_Type XMC_HRPWM_t;

/**
 * Typedef for HRPWM high resolution channel registers data structure
 */
typedef HRPWM0_HRC_Type XMC_HRPWM_HRC_t;

/**
 * Typedef for CSG unit registers data structure
 */
typedef HRPWM0_CSG_Type XMC_HRPWM_CSG_t;

/**
 * HRPWM HRC source path configuration
 */
typedef struct XMC_HRPWM_HRC_SRC_CONFIG
{
  XMC_HRPWM_HRC_HR_EDGE_t       high_res_mode;      /**< high resolution mode configuration */
  XMC_HRPWM_HRC_SRC_INPUT_t     set_config;         /**< Selection of input for set configuration */
  XMC_HRPWM_HRC_SRC_INPUT_t     clear_config;       /**< Selection of input clear configuration */
  XMC_HRPWM_HRC_CMP_SEL_t       cmp_set;            /**< Selection of comparator for set configuration */
  XMC_HRPWM_HRC_CMP_SEL_t       cmp_clear;          /**< Selection of comparator for clear configuration */
  XMC_HRPWM_HRC_TIMER_SEL_t     timer_sel;          /**< Selection of timer */
  XMC_HRPWM_HRC_SRC_EDGE_SEL_t  set_edge_config;    /**< Selection of edge for generating set signal */
  XMC_HRPWM_HRC_SRC_EDGE_SEL_t  clear_edge_config;  /**< Selection of edge for generating clear signal */
  XMC_HRPWM_FUNC_STATUS_t       src_trap_enable;    /**< Selection of source for trap signal generation */
} XMC_HRPWM_HRC_SRC_CONFIG_t;

/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__TASKING__)
  #pragma warning 586
#endif

/**
 * HRPWM HRC configuration
 */
typedef struct XMC_HRPWM_HRC_CONFIG
{
  union
  {
    struct
    {
      uint32_t : 2;
      uint32_t : 2;
      uint32_t : 4;
      uint32_t dt_enable: 1;                   /**< Enables dead time. Accepts enum @ref XMC_HRPWM_FUNC_STATUS_t */
      uint32_t hr_out0_trap_enable: 1;         /**< Enables trap for HROUT0. Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t hr_out1_trap_enable: 1;         /**< Enables trap for HROUT1. Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t hrc_shadow_xfer_linktoCCU8: 1;  /**< Shadow transfer for CR1 and CR2 linked to shadow transfer trigger of CCU8 slice.
                                                    Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t dt_shadow_xfer_linktoCCU8: 1;   /**< Shadow transfer for DCR and DCF linked to shadow transfer trigger of CCU8 slice.
                                                    Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t hr_out0_inv_enable: 1;          /**< Enables inversion of HROUT0 output pin. Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t hr_out1_inv_enable: 1;          /**< Enables inversion of HROUT1 output pin. Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t : 1;
      uint32_t dt_trigger_sel: 1;              /**< Selection of trigger for dead time shadow transfer. Accepts enum XMC_HRPWM_HRC_DT_TR_SEL_t */
      uint32_t : 15;
	};
    uint32_t gc;                               /**< General high resolution channel configuration */
  };

  union
  {
    struct
    {
      uint32_t hr_out0_passive_level_out: 1;   /**< Selection of HROUT0 passive level. Accepts enum XMC_HRPWM_HRC_OUT_PASSIVE_LVL_t */
      uint32_t hr_out1_passive_level_out: 1;   /**< Selection of HROUT0 passive level. Accepts enum XMC_HRPWM_HRC_OUT_PASSIVE_LVL_t */
      uint32_t : 30;
    };
    uint32_t psl;                              /**< Output passive level configuration */
  };
} XMC_HRPWM_HRC_CONFIG_t;

/***********************************************************************************************************************
 * DATA STRUCTURES - CSG
 **********************************************************************************************************************/
/**
 * Configuration data structure of a CSG input selection
 */
typedef struct XMC_HRPWM_CSG_INPUT_CONFIG
{
  XMC_HRPWM_CSG_INPUT_SEL_t  mapped_input; /**< CSG input selection */
  XMC_HRPWM_CSG_EDGE_SEL_t   edge;         /**< Active edge of mapped_input */
  XMC_HRPWM_CSG_LVL_SEL_t    level;        /**< Active level of mapped_input */
} XMC_HRPWM_CSG_INPUT_CONFIG_t;

/**
 *CSG Unit - Comparator configuration
 */
typedef struct XMC_HRPWM_CSG_CMP
{
  union
  {
    struct
    {
      uint32_t : 4;
      uint32_t : 4;
      uint32_t cmp_input_sel: 1;      /**< Comparator input pin selection. Accepts enum XMC_HRPWM_CSG_CMP_INPUT_t */
      uint32_t cmp_input_sw: 2;       /**< Comparator input switching configuration. Accepts enum XMC_HRPWM_CSG_LVL_SEL_t */
      uint32_t cmp_ext_sw_enable: 1;  /**< Enable switching of input between CINA and CINB via external trigger.
                                           Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t cmp_out_inv: 1;        /**< Invert comparator output. Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t : 1;                      /*Enable Comparator output synchronization */
      uint32_t blanking_mode: 2;      /**< Select the edge for blanking. Accepts enum XMC_HRPWM_CSG_EDGE_SEL_t */
      uint32_t blank_ext_enable: 1;   /**< Enable blanking via external trigger. Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t filter_enable: 1;      /**< Enable comparator output filter. Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t filter_window: 4;      /**< Select the comparator output filter window */
      uint32_t : 2;
      uint32_t filter_control: 2;     /**< Select the filter application condition
                                           00B Filtering is always done if enabled
                                           01B Filtering is only done when CSGyDSV1 value is currently fed to the DAC
                                           10B Filtering is only done when the CSGyDSV2 value is currently fed to the DAC */
      uint32_t : 6;
    };
    uint32_t cc;                      /**< Comparator general configuration */
  };

  uint32_t blanking_val;              /**< blanking value, blanking time = blanking_val * module clk freq */

  union
  {
    struct
    {
      uint32_t : 4;
      uint32_t : 4;
      uint32_t clamp_ctrl_lvl: 2;        /**< Select the trigger signal level for clamping the comparator output.
                                              Accepts enum XMC_HRPWM_CSG_LVL_SEL_t */
      uint32_t clamp_level: 1;           /**< Select the comparator output passive level value. */
      uint32_t clamp_exit_sw_config: 1;  /**< Clamped state exit software configuration */
      uint32_t clamp_enter_config: 2;    /**< Clamping level enter configuration */
      uint32_t clamp_exit_config: 2;     /**< Clamping level exit configuration */
      uint32_t : 16;
    };
    uint32_t plc;                        /**< Comparator passive level configuration */
  };
} XMC_HRPWM_CSG_CMP_t;

/**
 * CSG Unit - DAC configuration
 */
typedef struct XMC_HRPWM_CSG_DAC
{
  XMC_HRPWM_CSG_SWSM_t     start_mode;  /**< Initial DAC start mode */
  uint32_t                 dac_dsv1;    /**< DAC reference value 1 */
  uint32_t                 dac_dsv2;    /**< DAC reference value 2 */
} XMC_HRPWM_CSG_DAC_t;

/**
 * CSG Unit - Slope Generation configuration
 */
typedef struct XMC_HRPWM_CSG_SGEN
{
  union
  {
    struct
    {
      uint32_t prescaler_ext_start_mode: 2;  /**< Pre-scaler external start mode. Accepts enum XMC_HRPWM_CSG_PRESCALER_EXT_START_t */
      uint32_t prescaler_ext_stop_mode: 2;   /**< Pre-scaler external stop mode. Accepts enum XMC_HRPWM_CSG_PRESCALER_EXT_STOP_t */
      uint32_t fixed_prescaler_enable: 1;    /**< Fixed pre-scaler, 0:enabled, 1:disabled */
      uint32_t prescaler: 2;                 /**< Pre-scaler division factor */
      uint32_t : 1;
      uint32_t ctrl_mode: 2;                 /**< Slope control mode. Accepts enum XMC_HRPWM_CSG_SLOPE_CTRL_MODE_t */
      uint32_t ext_start_mode: 2;            /**< Slope external start mode. Accepts enum XMC_HRPWM_CSG_SLOPE_EXT_START_t */
      uint32_t ext_stop_mode: 2;             /**< Slope external stop mode. Accepts enum XMC_HRPWM_CSG_SLOPE_EXT_STOP_t */
      uint32_t slope_ref_val_mode: 2;        /**< Slope reference value mode */
      uint32_t : 2; /* start_mode */
      uint32_t step_gain: 2;                 /**< Slope step gain configuration */
      uint32_t static_mode_ist_enable: 1;    /**< Immediate shadow transfer in static mode enabled. Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t pulse_swallow_enable: 1;      /**< Pulse swallow enable / disable. Accepts enum XMC_HRPWM_FUNC_STATUS_t */
      uint32_t : 2;
      uint32_t pulse_swallow_win_mode: 2;    /**< Pulse swallow window mode */
      uint32_t : 6;
    };
    uint32_t sc;                             /**< Slope Generation Configuration */
  };   
  uint32_t   pulse_swallow_val;              /**< Pulse swallow value */
} XMC_HRPWM_CSG_SGEN_t;

/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif

/**
 * HRPWM CSG configuration
 */
typedef struct XMC_HRPWM_CSG_CONFIG
{
  XMC_HRPWM_CSG_CMP_t	  cmp_config;       /**< Comparator set up */
  XMC_HRPWM_CSG_DAC_t     dac_config;       /**< DAC configuration of CSG */
  XMC_HRPWM_CSG_SGEN_t    sgen_config;      /**< Slope generation related configurations */
} XMC_HRPWM_CSG_CONFIG_t;

/***********************************************************************************************************************
 * API PROTOTYPES 
 **********************************************************************************************************************/
/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @return XMC_HRPWM_STATUS_t
 *
 * \par<b>Description </b>
 * <br>HRPWM Init <br>\n
 *
 * This function initializes the HRPWM global registers. It configures the CSG trimming data.
 * This is the first function that needs to be called in initializing HRC or CSG modules.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_SDMMC_TriggerEvent()\n\n\n

 */
XMC_HRPWM_STATUS_t XMC_HRPWM_Init(XMC_HRPWM_t *const hrpwm);

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @return None
 *
 * \par<b>Description </b>
 * <br>Enable global high resolution generation<br>\n
 *
 * Enables global high resolution generation by setting GLBANA.GHREN bit.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_DisableGlobalHR()<br>
 */
void XMC_HRPWM_EnableGlobalHR(XMC_HRPWM_t *const hrpwm);

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @return None
 *
 * \par<b>Description </b>
 * <br>Disable global high resolution generation<br>\n
 *
 * Disables global high resolution generation by clearing GLBANA.GHREN bit.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_EnableGlobalHR()<br>
 */

void XMC_HRPWM_DisableGlobalHR(XMC_HRPWM_t *const hrpwm);

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @return None
 *
 * \par<b>Description </b>
 * <br>Enables the bias generation<br>\n
 *
 * Enables the bias generation of high resolution generation by setting HRBSC.HRBE bit.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_DisableBias()<br>
 */

__STATIC_INLINE void XMC_HRPWM_EnableBias(XMC_HRPWM_t *const hrpwm)
{
  XMC_ASSERT("XMC_HRPWM_EnableBias:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->HRBSC |= HRPWM0_HRBSC_HRBE_Msk;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @return None
 *
 * \par<b>Description </b>
 * <br>Disables the bias generation<br>\n
 *
 * Disables the bias generation of high resolution generation by clearing HRBSC.HRBE bit.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_EnableBias()<br>
 */
__STATIC_INLINE void XMC_HRPWM_DisableBias(XMC_HRPWM_t *const hrpwm)
{
  XMC_ASSERT("XMC_HRPWM_DisableBias:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->HRBSC &= ~(HRPWM0_HRBSC_HRBE_Msk);
}

/***********************************************************************************************************************
 * API PROTOTYPES - HRPWM HRC GLOBAL
 **********************************************************************************************************************/
/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @return XMC_HRPWM_HR_LOGIC_t
 *
 * \par<b>Description </b>
 * <br>Returns the status of the high resolution logic.<br>\n
 *
 * Returns status of the high resolution logic by checking HRGHRS.HRGR bit.
 * The return value should be @ref XMC_HRPWM_HR_LOGIC_WORKING for proper generation of high resolution signal positioning.
 */

XMC_HRPWM_HR_LOGIC_t XMC_HRPWM_GetHRGenReadyStatus(XMC_HRPWM_t *const hrpwm);

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected HRC modules. Use the enum type @ref XMC_HRPWM_HR_PATH_t to generate the mask.
 * @return None
 *
 * \par<b>Description</b>
 * <br>Enables the high resolution path.<br>\n
 *
 * Enables the high resolution path determined by passed mask value, by setting HRCCFG.HRC0E bit.
 * By default signals from source selector 0 are linked to HR path and signals from source selector 1 are linked to LR path.
 * This connections can be reversed at runtime, if bit HRCySC.ST is set to 1.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_HRC_Set_HR_Source()<br>
 *  XMC_HRPWM_DisableHighResolutionPath()<br>
 *  XMC_HRPWM_EnableLowResolutionPath()<br>
 *  XMC_HRPWM_DisableLowResolutionPath()<br>
 *  XMC_HRPWM_EnableHRPowerMode()<br>
 *
 */

__STATIC_INLINE void XMC_HRPWM_EnableHighResolutionPath(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_EnableHighResolutionPath:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->HRCCFG |= mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected HRC modules. Use the enum type @ref XMC_HRPWM_HR_PATH_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Disables the high resolution path<br>\n
 *
 * Disables the high resolution path determined by passed mask value, by clearing HRCCFG.HRC0E bit.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionPath()<br>
 * XMC_HRPWM_EnableLowResolutionPath()<br>
 * XMC_HRPWM_DisableLowResolutionPath()<br>
 *
 */
__STATIC_INLINE void XMC_HRPWM_DisableHighResolutionPath(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_DisableHighResolutionPath:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->HRCCFG &= ~mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected HRC modules. Use the enum type @ref XMC_HRPWM_LR_PATH_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Enables the low resolution path<br>\n
 *
 * Enables the low resolution path determined by passed mask value, by setting HRCCFG.LRC0E bit.
 * By default signals from source selector 0 are linked to HR path and signals from source selector 1 are linked to LR path.
 * This connections can be reversed at runtime, if bit HRCySC.ST is set to 1.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_HRC_Set_HR_Source()<br>
 * XMC_HRPWM_EnableHighResolutionPath()<br>
 * XMC_HRPWM_DisableHighResolutionPath()<br>
 * XMC_HRPWM_DisableLowResolutionPath()<br>
 * XMC_HRPWM_EnableHRPowerMode()<br>
 */

__STATIC_INLINE void XMC_HRPWM_EnableLowResolutionPath(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_EnableLowResolutionPath:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->HRCCFG |= mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected HRC modules. Use the enum type @ref XMC_HRPWM_LR_PATH_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Disables the low resolution path<br>\n
 *
 * Disables the low resolution path determined by passed mask value, by clearing HRCCFG.LRC0E bit.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionPath()<br>
 * XMC_HRPWM_DisableHighResolutionPath()<br>
 * XMC_HRPWM_EnableLowResolutionPath()<br>
 *
 */
 __STATIC_INLINE void XMC_HRPWM_DisableLowResolutionPath(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_DisableLowResolutionPath:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->HRCCFG &= ~mask;
}

 /**
  * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
  * @param mask masked values of selected HRC modules. Use the enum type @ref XMC_HRPWM_HRC_SHADOW_TX_t to generate the mask.
  * @return None
  *
  * \par<b>Description </b>
  * <br>Enables the high resolution shadow transfer<br> \n
  *
  * Enables the high resolution shadow transfer determined by passed mask value, by setting HRCSTRG.H0ES, HRCSTRG.H0DES bits.
  * The input for trigger for shadow transfer needs to be configured correctly.
  *
  * \par<b>Related APIs: </b><br>
  * XMC_HRPWM_DisableHighResolutionShadowTransfer()<br>
  * XMC_HRPWM_GetHighResolutionShadowTransferStatus()<br>
  *
  */

__STATIC_INLINE void XMC_HRPWM_EnableHighResolutionShadowTransfer(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_EnableHighResolutionShadowTransfer:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->HRCSTRG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected HRC modules. Use the enum type @ref XMC_HRPWM_HRC_SHADOW_TX_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Disables the high resolution shadow transfer<br>\n
 *
 * Disables the high resolution shadow transfer determined by passed mask value, by setting HRCCTRG.H0EC, HRCCTRG.H0DEC bits.
 * It cancels shadow transfer request by @ref XMC_HRPWM_EnableHighResolutionShadowTransfer(), provided the shadow transfer has not occurred.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionShadowTransfer()<br>
 *
 */
__STATIC_INLINE void XMC_HRPWM_DisableHighResolutionShadowTransfer(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_DisableHighResolutionShadowTransfer:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->HRCCTRG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected HRC modules. Use the enum type @ref XMC_HRPWM_HRC_SHADOW_TX_t to generate the mask.
 * @return uint32_t
 *
 * \par<b>Description </b>
 * <br>Returns the shadow transfer request status<br>\n
 *
 * Returns the shadow transfer request status, by checking HRCSTSG.H0STE, HRCSTSG.H0DSTE bits.
 * Returns a non zero value if corresponding shadow transfer request has been performed.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionShadowTransfer()<br>
 *
 */
__STATIC_INLINE uint32_t XMC_HRPWM_GetHighResolutionShadowTransferStatus(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_GetHighResolutionShadowTransferStatus:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  return (hrpwm->HRCSTSG & mask);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @return None
 *
 * \par<b>Description </b>
 * <br>Turns ON the power to all HR and LR path<br>\n
 *
 * Turns ON the power to all HR and LR path by setting HRCCFG.HRCPM bit. Enable the HR and LR paths as per requirement by
 * calling following API @ref XMC_HRPWM_EnableHighResolutionPath() and @ref XMC_HRPWM_EnableLowResolutionPath().
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionPath()<br>
 * XMC_HRPWM_EnableLowResolutionPath()<br>
 *
 */
__STATIC_INLINE void XMC_HRPWM_EnableHRPowerMode(XMC_HRPWM_t *const hrpwm)
{
  XMC_ASSERT("XMC_HRPWM_EnableHRPowerMode:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->HRCCFG |= HRPWM0_HRCCFG_HRCPM_Msk;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @return None
 *
 * \par<b>Description </b>
 * <br>Turns OFF the power to all HR and LR path<br>\n
 *
 * Turns OFF the power to all HR and LR path by clearing HRCCFG.HRCPM bit.
 * This disables all HR and LR paths.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHRPowerMode()<br>
 *
 */
__STATIC_INLINE void XMC_HRPWM_DisableHRPowerMode(XMC_HRPWM_t *const hrpwm)
{
  XMC_ASSERT("XMC_HRPWM_DisableHRPowerMode:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));

  /* Turn off high resolution generation logic */
  hrpwm->HRCCFG &= ~(HRPWM0_HRCCFG_HRCPM_Msk);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param clk_freq The operating clock frequency of HRPWM module. Use the enum type @ref XMC_HRPWM_CLK_FREQ_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Configures the clock frequency of operation of HRPWM module<br>\n
 *
 * Configures the clock frequency of operation of HRPWM module by configuring HRCCFG.CLKC bits.
 * The clock is generally selected based on the device type selected.
 *
 */

__STATIC_INLINE void XMC_HRPWM_ModuleClkFreq(XMC_HRPWM_t *const hrpwm, const XMC_HRPWM_CLK_FREQ_t clk_freq)
{
  XMC_ASSERT("XMC_HRPWM_ModuleClkFreq:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  hrpwm->HRCCFG &= ~(HRPWM0_HRCCFG_CLKC_Msk);
  hrpwm->HRCCFG |= (clk_freq << HRPWM0_HRCCFG_CLKC_Pos);
}

/***********************************************************************************************************************
 * API PROTOTYPES - HRPWM CSG GLOBAL
 **********************************************************************************************************************/
/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Enables the operation of comparator <br>\n
 *
 * Enables the operation of comparator by setting CSGSETG.SC0R bit.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_StopComparator()<br>
 * XMC_HRPWM_IsComparatorRunning()<br>
 */

__STATIC_INLINE void XMC_HRPWM_StartComparator(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_StartComparator:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGSETG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Disables the operation of comparator<br>\n
 *
 * Disables the operation of comparator by setting CSGCLRG.CC0R bit.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_StartComparator()<br>
 * XMC_HRPWM_IsComparatorRunning()<br>
 */
__STATIC_INLINE void XMC_HRPWM_StopComparator(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_StopComparator:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGCLRG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return bool
 *
 * \par<b>Description </b>
 * <br>Checks if comparator is enabled<br>\n
 *
 * Checks if comparator is enabled by checking CSGSTATG.C0RB bit.
 * Returns true if comparator run bit is set, else returns false.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_StartComparator()<br>
 * XMC_HRPWM_StopComparator()<br>
 */
__STATIC_INLINE bool XMC_HRPWM_IsComparatorRunning(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  bool status;
  
  XMC_ASSERT("XMC_HRPWM_IsComparatorRunning:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));

  if(hrpwm->CSGSTATG & mask)
  {
    status = true;
  }
  else
  {
    status = false;
  }
  
  return (status);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Enables the operation of CSG DAC<br>\n
 *
 * Enables the operation of CSG DAC by setting CSGSETG.SD0R bit.
 * The DAC operation is enabled. Either the value in DSV1 or DSV2 is sent to DAC, based on configuration.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_StopDac()<br>
 * XMC_HRPWM_IsDacRunning()<br>
 */

__STATIC_INLINE void XMC_HRPWM_StartDac(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_StartDac:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGSETG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Disables the operation of CSG DAC<br>\n
 *
 * Disables the operation of CSG DAC by setting CSGCLRG.CD0R bit.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_StartDac()<br>
 * XMC_HRPWM_IsDacRunning()<br>
 *
 */
__STATIC_INLINE void XMC_HRPWM_StopDac(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_StopDac:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGCLRG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return uint32_t
 *
 * \par<b>Description </b>
 * <br>Checks if CSG DAC is operational<br>\n
 *
 * Checks if CSG DAC is operational by checking CSGSTATG.D0RB bit.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_StartDac()<br>
 * XMC_HRPWM_StopDac()<br>
 */
__STATIC_INLINE uint32_t XMC_HRPWM_IsDacRunning(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_IsDacRunning:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  return (hrpwm->CSGSTATG & mask);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Set the comparator output to clamp state<br> \n
 *
 * Sets the comparator to clamped state via software by setting CSGSETG.SC0P bit. The output of comparator is now not dependent on its inputs pins.
 * The clamped state is defined by comparator output passive level value. Output passive level can be set to high or low.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_UnClampComparatorOutput()<br>
 * XMC_HRPWM_IsComparatorClamped()<br>
 */

__STATIC_INLINE void XMC_HRPWM_ClampComparatorOutput(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_ClampComparatorOutput:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  hrpwm->CSGSETG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Clear the comparator output from clamp state<br>\n
 *
 * Un-clamps the output of comparator from clamped state set via software by setting CSGCLRG.CC0P bit. The output of
 * comparator is now dependent on the inputs of comparator.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_ClampComparatorOutput()<br>
 * XMC_HRPWM_IsComparatorClamped()<br>
 */
__STATIC_INLINE void XMC_HRPWM_UnClampComparatorOutput(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_UnClampComparatorOutput:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  hrpwm->CSGCLRG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return uint32_t
 *
 * \par<b>Description </b>
 * <br>Checks if comparator is in clamped state <br>\n
 *
 * Checks if comparator is in clamped state by checking CSGSTATG.PSLS0 bit.
 * Returns bit encoded status if comparator is set to clamped state via software.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_ClampComparatorOutput()<br>
 * XMC_HRPWM_UnClampComparatorOutput()<br>
 */

__STATIC_INLINE uint32_t XMC_HRPWM_IsComparatorClamped(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_IsComparatorClamped:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  return (hrpwm->CSGSTATG & mask);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return uint32_t
 *
 * \par<b>Description </b>
 * <br>Returns bit encoded status of multiple DACs and Comparators, defined by the mask.<br>\n
 *
 * Returns bit encoded status of multiple DACs and Comparators from register CSGSTATG, defined by the mask.
 * The mask is generated by bitwise ORing multiple Enums.<br>
 * mask = (uint32_t) (XMC_HRPWM_CSG_RUN_BIT_CMP0 | XMC_HRPWM_CSG_RUN_BIT_DAC0 | XMC_HRPWM_CSG_RUN_BIT_CMP0_PSL);
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_IsDacRunning()<br>
 * XMC_HRPWM_IsComparatorClamped()<br>
 */

__STATIC_INLINE uint32_t XMC_HRPWM_GetRunBitStatus(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_GetRunBitStatus:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  return (hrpwm->CSGSTATG & mask);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Start the prescaler & slope generation of DAC<br>\n
 *
 * Start the prescaler & slope generation of DAC by setting CSGFCG.S0STR and CSGFCG.PS0STR bits.
 * The mask is generated by bitwise ORing multiple Enums.<br>
 * mask = (uint32_t) (XMC_HRPWM_CSG_SLOPE_START_DAC0 | XMC_HRPWM_CSG_PRESCALER_START_CSG0);
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_StopSlopeGeneration()<br>
 * XMC_HRPWM_IsSlopeGenerationRunning()<br>
 */

__STATIC_INLINE void XMC_HRPWM_StartSlopeGeneration(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_StartSlopeGeneration:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGFCG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Stops the prescaler & slope generation of DAC<br>\n
 *
 * Stops the prescaler & slope generation of DAC by setting CSGFCG.S0STP and CSGFCG.PS0STP bits.
 * The mask is generated by bitwise ORing multiple Enums.<br>
 * mask = (uint32_t) (XMC_HRPWM_CSG_SLOPE_START_DAC0 | XMC_HRPWM_CSG_PRESCALER_START_CSG0);
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_StartSlopeGeneration()<br>
 * XMC_HRPWM_IsSlopeGenerationRunning()<br>
 */
__STATIC_INLINE void XMC_HRPWM_StopSlopeGeneration(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_StopSlopeGeneration:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGFCG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_RUN_BIT_t to generate the mask.
 * @return bool
 *
 * \par<b>Description </b>
 * <br>Checks if Prescaler & slope generation is running<br>\n
 *
 * Checks if Prescaler & slope generation is running by checking CSGFSG.S0RB CSGFSG.P0RB bits.
 * The mask is generated by bitwise ORing multiple Enums.<br>
 * mask = (uint32_t) (XMC_HRPWM_CSG_SLOPE_START_DAC0 | XMC_HRPWM_CSG_PRESCALER_START_CSG0);
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_StartSlopeGeneration()<br>
 * XMC_HRPWM_StopSlopeGeneration()<br>
 */
__STATIC_INLINE bool XMC_HRPWM_IsSlopeGenerationRunning(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  bool status;
  
  XMC_ASSERT("XMC_HRPWM_IsSlopeGenerationRunning:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  if (hrpwm->CSGFSG & mask)
  {
    status = true;
  }
  else
  {
    status = false;
  }
  
  return (status);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected HRC modules. Use the enum type @ref XMC_HRPWM_SHADOW_TX_DAC_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Enables the shadow transfer of DSV1 and pulse swallow registers of DACs selected by mask<br>\n
 *
 * Enables the shadow transfer of DSV1 and pulse swallow registers of DACs selected by mask by setting CSGTRG.D0SES bit.
 * The transfer is done at the next shadow transfer trigger.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_DisableComparatorShadowTransfer()<br>
 * XMC_HRPWM_GetComparatorShadowTransferStatus()<br>
 */
__STATIC_INLINE void XMC_HRPWM_EnableComparatorShadowTransfer(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_EnableComparatorShadowTransfer:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGTRG = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_SHADOW_TX_DAC_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Cancels the shadow transfer of DSV1 and pulse swallow registers<br>\n
 *
 * Cancels the shadow transfer of DSV1 and pulse swallow registers by setting CSGTRC.D0SEC bit.
 * The transfer request is canceled. Needs to be called before the next shadow transfer trigger.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableComparatorShadowTransfer()<br>
 * XMC_HRPWM_GetComparatorShadowTransferStatus()<br>
 */

__STATIC_INLINE void XMC_HRPWM_DisableComparatorShadowTransfer(XMC_HRPWM_t *const hrpwm, uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_DisableComparatorShadowTransfer:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGTRC = mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @return uint32_t
 *
 * \par<b>Description </b>
 * <br>Gets the shadow transfer status of DSV1 and pulse swallow registers of all the DACs<br>\n
 *
 * Gets the shadow transfer status of DSV1 and pulse swallow registers of all the DACs by checking the register CSGTRSG
 * The return value is not zero if shadow transfer has been requested, but is still pending completion.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableComparatorShadowTransfer()<br>
 * XMC_HRPWM_DisableComparatorShadowTransfer()<br>
 */
__STATIC_INLINE uint32_t XMC_HRPWM_GetComparatorShadowTransferStatus(XMC_HRPWM_t *const hrpwm)
{
  XMC_ASSERT("XMC_HRPWM_GetComparatorShadowTransferStatus:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  return(hrpwm->CSGTRSG & XMC_HRPWM_COMPARATOR_STATUS);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_PRESCALER_STATUS_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Clears the prescaler registers of DACs selected by mask<br>\n
 *
 * Clears the prescaler registers of DACs selected by mask, by setting CSGFCG.PS0CLR bit.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_IsPrescalerRunning()<br>
 */
__STATIC_INLINE void XMC_HRPWM_ClearPreScaler(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_ClearPreScaler:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGFCG |= mask;
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_PRESCALER_STATUS_t to generate the mask.
 * @return uint32_t
 *
 * \par<b>Description </b>
 * <br>Checks the prescaler status of DACs selected by mask<br>\n
 *
 * Checks the prescaler status of DACs selected by mask, by checking  CSGFCG.P0RB bit.
 * Returns the bit encoded status information of prescaler.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_ClearPreScaler()<br>
 * XMC_HRPWM_StartSlopeGeneration()<br>
 */
__STATIC_INLINE uint32_t XMC_HRPWM_IsPrescalerRunning(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_IsPrescalerRunning:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  return (hrpwm->CSGFSG & mask);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_SWITCH_CMP_INPUT_t to generate the mask.
 * @return uint32_t
 *
 * \par<b>Description </b>
 * <br>Returns the bit encoded status of HW pin connected to comparator inverting pin<br>\n
 *
 * Returns the bit encoded status of HW pin connected to comparator inverting pin by checking CSGTRSG.SW0ST bit.
 * The bit position is set to 1 if CINB is connected, else its CINA.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_CSG_SetCMPInput()<br>
 */

__STATIC_INLINE uint32_t XMC_HRPWM_GetCMPInput(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_GetCMPInput:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  return (hrpwm->CSGTRSG & mask);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param slice Slice NO.
 * @param power_mode The mode to be put in.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Sets the DAC in OFF, Low speed or High speed mode<br>\n
 *
 * Sets the DAC in OFF, Low speed or High speed mode, by setting CSGCFG.C0PM bits.
 *
 * \par<b>Related APIs: </b><br>
 *
 */
__STATIC_INLINE void XMC_HRPWM_SetCsgPowerMode(XMC_HRPWM_t *const hrpwm, 
                                                const XMC_HRPWM_CSG_SLICE_t slice, 
                                                const XMC_HRPWM_CSG_POWER_MODE_t power_mode)
{
  XMC_ASSERT("XMC_HRPWM_SetCsgPowerMode:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  
  hrpwm->CSGCFG &= ~(3U << (slice * 2U));
  hrpwm->CSGCFG |= power_mode << (slice * 2U);
}

/**
 * @param hrpwm Constant pointer to XMC_HRPWM_t, pointing to the HRPWM module base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_CLK_t to generate the mask.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Disables the clock of selected CSG subunits<br>\n
 *
 * Disables the clock of selected CSG subunits by setting the CSGCFG.C0CD bit.
 *
 * \par<b>Related APIs: </b><br>
 *
 */

__STATIC_INLINE void XMC_HRPWM_DisableCsgClock(XMC_HRPWM_t *const hrpwm, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_DisableCsgClock:Invalid module pointer", XMC_HRPWM_CHECK_MODULE_PTR(hrpwm));
  hrpwm->CSGCFG |= mask;
}

/***********************************************************************************************************************
 * API PROTOTYPES - HRPWM HRC CHANNEL
 **********************************************************************************************************************/
/**
 * @param hrc Constant pointer to XMC_HRPWM_HRC_t, pointing to the HRC channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Initializes the HRC channel.<br>\n
 *
 * Initializes the HRC channel functionality.<br>
 * These include:<br>
 * 1) Dead time configuration.<br>
 * 3) Trap Configuration.<br>
 * 4) Shadow transfer configuration.<br>
 * 5) Output inversion configuration.<br>
 * 6) Passive levels of HRC outputs.<br>
 */

void XMC_HRPWM_HRC_Init(XMC_HRPWM_HRC_t *const hrc, const XMC_HRPWM_HRC_CONFIG_t *const config);

/**
 * @param hrc Constant pointer to XMC_HRPWM_HRC_t, pointing to the HRC channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Initializes the source 0 of HRC channel.<br>\n
 *
 * Initialize the source 0 functionality of HRC channel.<br>
 * This include:<br>
 * 1) general configuration for source 0 HRC channel.<br>
 * 2) Configuration of which inputs are being used to generate the set and clear for the latch and therefore controlling
 *    the generation of the output PWM signal.<br>
 * 3) Configuration for which timer from the Capture/Compare Unit is used for the Source Selector 0 and Source Selector 1.<br>
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_HRC_ConfigSourceSelect1()<br>
 */


void XMC_HRPWM_HRC_ConfigSourceSelect0(XMC_HRPWM_HRC_t *const hrc, const XMC_HRPWM_HRC_SRC_CONFIG_t *const config);

/**
 * @param hrc Constant pointer to XMC_HRPWM_HRC_t, pointing to the HRC channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Initializes the source 1 of HRC channel.<br>\n
 *
 * Initialize the source 1 functionality of HRC channel.<br>\n
 * This include:<br>
 * 1) general configuration for source 1 HRC channel.<br>
 * 2) Configuration of which inputs are being used to generate the set and clear for the latch and therefore controlling
 *    the generation of the output PWM signal.<br>
 * 3) Configuration for which timer from the Capture/Compare Unit is used for the Source Selector 0 and Source Selector 1.<br>
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_HRC_ConfigSourceSelect0()<br>
 */

void XMC_HRPWM_HRC_ConfigSourceSelect1(XMC_HRPWM_HRC_t *const hrc, const XMC_HRPWM_HRC_SRC_CONFIG_t *const config);

/**
 * @param hrc Constant pointer to XMC_HRPWM_HRC_t, pointing to the HRC channel base address
 * @param cr1_value high resolution positioning value.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Sets the shadow transfer register of high resolution positioning for rising edge<br>\n
 *
 * Call the shadow transfer update API for transfer to CR1 register.
 * A shadow transfer request in corresponding CCU8 slice may also be required.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionShadowTransfer()<br>
 * XMC_HRPWM_HRC_SetCompare2()<br>
 */
__STATIC_INLINE void XMC_HRPWM_HRC_SetCompare1(XMC_HRPWM_HRC_t *const hrc, const uint8_t cr1_value)
{
  XMC_ASSERT("XMC_HRPWM_HRC_SetCompare1:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  hrc->SCR1 = (uint32_t) cr1_value;
}

/**
 * @param hrc Constant pointer to XMC_HRPWM_HRC_t, pointing to the HRC channel base address
 * @param cr2_value high resolution positioning value.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Sets the shadow transfer register of high resolution positioning for falling edge<br>\n
 *
 * Call the shadow transfer update API for transfer to CR2 register.
 * A shadow transfer request in corresponding CCU8 slice may also be required.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionShadowTransfer()<br>
 * XMC_HRPWM_HRC_SetCompare1()<br>
 */
__STATIC_INLINE void XMC_HRPWM_HRC_SetCompare2(XMC_HRPWM_HRC_t *const hrc, const uint8_t cr2_value)
{
  XMC_ASSERT("XMC_HRPWM_HRC_SetCompare2:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  hrc->SCR2 = (uint32_t) cr2_value;
}

/**
 * @param hrc Constant pointer to XMC_HRPWM_HRC_t, pointing to the HRC channel base address
 * @param dcr_value Rising edge dead time value.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Sets the shadow transfer register of rising edge dead time.<br>\n
 *
 * Call the shadow transfer update API for transfer to DCR register.
 * A shadow transfer request in corresponding CCU8 slice may also be required.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionShadowTransfer()<br>
 * XMC_HRPWM_HRC_SetDeadTimeFalling()<br>
 */

__STATIC_INLINE void XMC_HRPWM_HRC_SetDeadTimeRising(XMC_HRPWM_HRC_t *const hrc, uint16_t dcr_value)
{
  XMC_ASSERT("XMC_HRPWM_HRC_SetDeadTimeRising:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  hrc->SDCR = (uint32_t) dcr_value;
}

/**
 * @param hrc Constant pointer to XMC_HRPWM_HRC_t, pointing to the HRC channel base address
 * @param dcf_value Falling edge dead time value.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Sets the shadow transfer register of falling edge dead time.<br>\n
 *
 * Call the shadow transfer update API for transfer to DCR register.
 * A shadow transfer request in corresponding CCU8 slice may also be required.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionShadowTransfer()<br>
 * XMC_HRPWM_HRC_SetDeadTimeRising()<br>
 */
__STATIC_INLINE void XMC_HRPWM_HRC_SetDeadTimeFalling(XMC_HRPWM_HRC_t *const hrc, uint16_t dcf_value)
{
  XMC_ASSERT("XMC_HRPWM_HRC_SetDeadTimeFalling:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  hrc->SDCF = (uint32_t) dcf_value;
}

/**
 * @param hrc Constant pointer to XMC_HRPWM_HRC_t, pointing to the HRC channel base address
 * @param source Source connected to high resolution channel.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Sets the source to high resolution channel<br>\n
 *
 * Sets the shadow transfer register deciding the source connected to high resolution channel.
 * This also affects the CCU8 timer used for linking shadow transfer trigger.
 * Call the shadow transfer update API.
 * A shadow transfer request in corresponding CCU8 slice may also be required.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionShadowTransfer()<br>
 */

__STATIC_INLINE void XMC_HRPWM_HRC_Set_HR_Source(XMC_HRPWM_HRC_t *const hrc, XMC_HRPWM_HRC_SOURCE_t source)
{
  XMC_ASSERT("XMC_HRPWM_HRC_Set_HR_Source:Invalid HRC pointer", XMC_HRPWM_CHECK_HRC_PTR(hrc));
  hrc->SSC = (uint32_t) source;
}
                                   
/***********************************************************************************************************************
 * API PROTOTYPES - HRPWM CSG SLICE
 **********************************************************************************************************************/

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Initializes the CSG channel.<br>\n
 *
 * This function is used to initialize the CSG channel.<br>\n
 * These include:<br>
 * 1) Comparator setup.<br>
 * 2) DAC Configuration.<br>
 * 3) Slope generation configuration.<br>
 */
void XMC_HRPWM_CSG_Init(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_CONFIG_t *const config);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param input Input to comparator. Use the enum type @ref XMC_HRPWM_CSG_CMP_INPUT_t to generate the input.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Configures the input connection to inverting pin of comparator<br>\n
 *
 * Selects the HW pin that gets connected to inverting pin of comparator.<br>
 * Either CINA or CINB can be set.
 * The non-inverting pin is connected to DAC output.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_GetCMPInput()
 */
void XMC_HRPWM_CSG_SetCMPInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_CMP_INPUT_t input);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Configures the input signal to blank the comparator output<br>\n
 *
 * Configures the input signal that is used as trigger signal to blank the comparator output.<br>
 * It configures the signal source, required edge or level.
 * The comparator output is blanked and set to passive level.
 *
 */
void XMC_HRPWM_CSG_SelBlankingInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Configures the input signal to clamp the comparator output<br>\n
 *
 * Configures the input signal that is used as level signal to clamp the comparator output.<br>
 * It configures the signal source and required level.
 *
 */
void XMC_HRPWM_CSG_SelClampingInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Configures the input signal to start the DAC slope generation<br>\n
 *
 * Configures the input signal that is used as trigger signal to start the slope generation.<br>
 * It configures the signal source, required edge or level.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_CSG_StopSlopeGenConfig()<br>
 *
 */

void XMC_HRPWM_CSG_StartSlopeGenConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Configures the input signal to stop the DAC slope generation<br>\n
 *
 * Configures the input that is used as trigger signal to stop the slope generation.<br>
 * It configures the signal source, required edge or level.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_CSG_StartSlopeGenConfig()<br>
 */

void XMC_HRPWM_CSG_StopSlopeGenConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Configures the input signal to trigger the DAC conversion<br>\n
 *
 * Configures the input signal that is used as trigger signal to perform the DAC conversion.<br>
 * It configures the signal source, required edge or level.<br>
 * This is used when DAC is configured in <b>static mode</b>.
 *
 */

void XMC_HRPWM_CSG_TriggerDACConvConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Configure input selection for triggering shadow transfer<br>\n
 *
 * Configure the signal used to triggering shadow transfer.<br>
 * It configures the signal source, required edge or level.
 *
 */

void XMC_HRPWM_CSG_TriggerShadowXferConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param config Pointer to configuration structure.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Configure input selection for switching DAC value between DSV1 and DSV2.<br>\n
 *
 * Configure the signal used to switch DAC value between DSV1 and DSV2.<br>
 * It configures the signal source, required edge or level.
 *
 */


void XMC_HRPWM_CSG_DACRefValSwitchingConfig(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_INPUT_CONFIG_t *const config);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param input_clk Clock selection.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Select the clock for slope generation<br>\n
 *
 * Selects the clock source used for slope generation.<br>
 * These are : <br>
 * module clock      <br>
 * external clock A  <br>
 * external clock B  <br>
 * external clock C  <br>
 *
 */

void XMC_HRPWM_CSG_SelSlopeGenClkInput(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_CLK_INPUT_t input_clk);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param event Event selected for interrupt.
 * @param sr Service request node.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Connects the interrupt request to serve node<br>\n
 *
 * Enables the connection between interrupt request and serve node.<br>
 * Each event may be connected to any of four service node available.
 * Each event/interrupt needs to be enabled individually.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_CSG_EnableEvent()<br>
 *  XMC_HRPWM_CSG_GetEventStatus()<br>
 */

void XMC_HRPWM_CSG_SetSRNode(XMC_HRPWM_CSG_t *const csg, const XMC_HRPWM_CSG_IRQ_ID_t event,
                             const XMC_HRPWM_CSG_IRQ_SR_LINE_t sr);

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param mask masked values of selected CSG modules. Use the enum type @ref XMC_HRPWM_CSG_IRQ_ID_t to generate the input.
 * @return uint32_t
 *
 * \par<b>Description </b>
 * <br>Returns the bit encoded status of selected events<br>\n
 *
 * Checks the status of selected events. The return value is non-zero is the status is set.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_CSG_EnableEvent()<br>
 *  XMC_HRPWM_CSG_SetSRNode()<br>
 *
 */
__STATIC_INLINE uint32_t XMC_HRPWM_CSG_GetEventStatus(XMC_HRPWM_CSG_t *const csg, const uint32_t mask)
{
  XMC_ASSERT("XMC_HRPWM_CSG_GetEventStatus:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  return (csg->ISTAT & mask);
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param value Value to be written to DSV2 register.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Updates the DSV2 register<br>\n
 *
 * Updates the DSV2 register.<br>
 * Note DSV2 register does not have shadow register.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_CSG_UpdateDACRefDSV1()<br>
 */
__STATIC_INLINE void XMC_HRPWM_CSG_UpdateDACRefDSV2(XMC_HRPWM_CSG_t *const csg, uint32_t value)
{
  XMC_ASSERT("XMC_HRPWM_CSG_UpdateDACRefDSV2:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->DSV2 = value & HRPWM0_CSG_DSV2_DSV2_Msk;
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param value Value to be written to blanking register.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Updates the BLV register<br>\n
 *
 * Updates the blanking register.<br>
 * Note BLV register does not have shadow register.
 *
 */
__STATIC_INLINE void XMC_HRPWM_CSG_UpdateBlankingValue(XMC_HRPWM_CSG_t *const csg, uint8_t value)
{
  XMC_ASSERT("XMC_HRPWM_CSG_UpdateBlankingValue:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->BLV = (uint32_t) value;
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param window Size of filter window.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Updates the filter window size<br>\n
 *
 * Updates the filter window size used for pulse swallowing, in slope generation.<br>
 * This value is used in slope generation when filter window is enabled.
 * A certain no of clock pulses in the filter window are swallowed and applied to slope generation.
 * The pulse swallowed are determined by "Pulse swallow value"
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_CSG_UpdatePulseClk()<br>
 */

__STATIC_INLINE void XMC_HRPWM_CSG_UpdateFilterWindow(XMC_HRPWM_CSG_t *const csg, XMC_HRPWM_CSG_CMP_FILTER_WINDOW_t window)
{
  XMC_ASSERT("XMC_HRPWM_CSG_UpdateFilterWindow:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->CC &= ~(HRPWM0_CSG_CC_COFM_Msk);
  csg->CC |= (uint32_t) window << HRPWM0_CSG_CC_COFM_Pos;
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param value No of clock pulses to be swallowed in the filter window.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Updates the no of clock pulses to be swallowed in the filter window<br>\n
 *
 * Update the pulse swallow value.<br>
 * This value is used in slope generation when filter window is enabled for slope generation.
 * No of clock pulse swallow is determined by this value.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_CSG_UpdateFilterWindow()<br>
 */

__STATIC_INLINE void XMC_HRPWM_CSG_UpdatePulseClk(XMC_HRPWM_CSG_t *const csg, uint32_t value) 
{
  XMC_ASSERT("XMC_HRPWM_CSG_UpdatePulseClk:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->SPC = value & HRPWM0_CSG_SPC_SPSWV_Msk;
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param value Value to be written to DSV1 shadow register.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Updates the DSV1 shadow register<br>\n
 *
 * Update the DSV1 shadow register.<br>
 * Call the shadow transfer update API.
 * A shadow transfer request in corresponding CCU8 slice may also be required.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_EnableHighResolutionShadowTransfer()<br>
 * XMC_HRPWM_CSG_UpdateDACRefDSV2()<br>
 *
 */
__STATIC_INLINE void XMC_HRPWM_CSG_UpdateDACRefDSV1(XMC_HRPWM_CSG_t *const csg, uint32_t value)
{
  XMC_ASSERT("XMC_HRPWM_CSG_UpdateDACRefDSV1:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->SDSV1 = value & HRPWM0_CSG_SDSV1_SDSV1_Msk;
}                             

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param gain Gain value.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Updates the gain value of slope generation<br>\n
 *
 * Updates the gain value of slope generation by setting SC.GCFG bits.
 * The value by which DAC increments/decrements is determined by the step gain.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_CSG_UpdateDACPrescaler()<br>
 *
 */
__STATIC_INLINE void XMC_HRPWM_CSG_UpdateDACStepGain(XMC_HRPWM_CSG_t *const csg, XMC_HRPWM_CSG_SLOPE_STEP_GAIN_t gain)
{
  XMC_ASSERT("XMC_HRPWM_CSG_UpdateDACStepGain:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->SC &= ~(HRPWM0_CSG_SC_GCFG_Msk);
  csg->SC |= (uint32_t) gain << HRPWM0_CSG_SC_GCFG_Pos;
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param div_value Prescaler value.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Updates the prescaler value of slope generation<br>\n
 *
 * Updates the prescaler value of slope generation by setting SC.PSV
 * The rate of DAC value update is determined by prescaler.
 *
 * \par<b>Related APIs: </b><br>
 * XMC_HRPWM_CSG_UpdateDACStepGain()<br>
 *
 */

__STATIC_INLINE void XMC_HRPWM_CSG_UpdateDACPrescaler(XMC_HRPWM_CSG_t *const csg, XMC_HRPWM_CSG_PRESCALER_DIVISION_t div_value)
{
  XMC_ASSERT("XMC_HRPWM_CSG_UpdateDACPrescaler:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->SC &= ~(HRPWM0_CSG_SC_PSV_Msk);
  csg->SC |= (uint32_t) div_value << HRPWM0_CSG_SC_PSV_Pos;
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param event Event selected for interrupt.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Enables the interrupt<br>\n
 *
 * Enables the selected interrupt request which may be forwarded to service node.
 * The enabled event may be connected to any of the four service nodes.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_CSG_DisableEvent()<br>
 *  XMC_HRPWM_CSG_GetEventStatus()<br>
 *  XMC_HRPWM_CSG_SetSRNode()<br>
 *
 */

__STATIC_INLINE void XMC_HRPWM_CSG_EnableEvent(XMC_HRPWM_CSG_t *const csg, XMC_HRPWM_CSG_IRQ_ID_t event)
{
  XMC_ASSERT("XMC_HRPWM_CSG_EnableEvent:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->SRE |= event;
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param event Event selected for interrupt.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Disables the interrupt<br>\n
 *
 * Disables the selected interrupt request which may be forwarded to service node.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_CSG_EnableEvent()<br>
 *  XMC_HRPWM_CSG_GetEventStatus()<br>
 *  XMC_HRPWM_CSG_SetSRNode()<br>
 *
 */

__STATIC_INLINE void XMC_HRPWM_CSG_DisableEvent(XMC_HRPWM_CSG_t *const csg, XMC_HRPWM_CSG_IRQ_ID_t event)
{
  XMC_ASSERT("XMC_HRPWM_CSG_DisableEvent:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->SRE &= ~event;
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param event Event selected for software trigger.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Software request for selected event<br>\n
 *
 * Perform a software request for selected event.This overrides any hardware trigger.
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_CSG_EnableEvent()<br>
 *  XMC_HRPWM_CSG_GetEventStatus()<br>
 *  XMC_HRPWM_CSG_SetSRNode()<br>
 *  XMC_HRPWM_CSG_ClrEventSW()<br>
 *
 */

__STATIC_INLINE void XMC_HRPWM_CSG_SetEventSW(XMC_HRPWM_CSG_t *const csg, XMC_HRPWM_CSG_IRQ_ID_t event)
{
  XMC_ASSERT("XMC_HRPWM_CSG_SetEventSW:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->SWS = event;
}

/**
 * @param csg Constant pointer to XMC_HRPWM_CSG_t, pointing to the CSG channel base address
 * @param event Event selected for software trigger.
 * @return None
 *
 * \par<b>Description </b>
 * <br>Cancel software request for selected event<br>\n
 *
 * Cancel the Event trigger request performed via software.<br>
 *
 * \par<b>Related APIs: </b><br>
 *  XMC_HRPWM_CSG_EnableEvent()<br>
 *  XMC_HRPWM_CSG_GetEventStatus()<br>
 *  XMC_HRPWM_CSG_SetSRNode()<br>
 *  XMC_HRPWM_CSG_SetEventSW()<br>
 *
 */
__STATIC_INLINE void XMC_HRPWM_CSG_ClrEventSW(XMC_HRPWM_CSG_t *const csg, XMC_HRPWM_CSG_IRQ_ID_t event)
{
  XMC_ASSERT("XMC_HRPWM_CSG_ClrEventSW:Invalid CSG pointer", XMC_HRPWM_CHECK_CSG_PTR(csg));
  csg->SWC = event;
}

/**
 * @}
 */
 
/**
 * @}
 */

#endif  /* #if defined(HRPWM0) */

#ifdef __cplusplus
}
#endif

#endif /* HRPWM_H */
