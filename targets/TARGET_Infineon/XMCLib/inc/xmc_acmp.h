/**
 * @file xmc_acmp.h
 * @date 2015-09-02
 *
 * @cond
 *********************************************************************************************************************
 * XMClib v2.1.16 - XMC Peripheral Driver Library 
 *
 * Copyright (c) 2015-2017, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2014-12-10:
 *     - Initial version
 * 2015-02-20:
 *     - Removed unused declarations<br> 
 * 2015-05-08:
 *     - Fixed sequence problem of low power mode in XMC_ACMP_Init() API<br>
 *     - Fixed wrong register setting in XMC_ACMP_SetInput() API<br> 
 *     - Removed return type variable and by default comparator enable from XMC_ACMP_Init() API. <br>
 *       Additional call to XMC_ACMP_EnableComparator() API needed to start Comparator after Init.<br>
 * 2015-06-04:
 *     - Removed return type variable and by default comparator enable from XMC_ACMP_Init() API. <br>
 *     - Divided XMC_ACMP_SetInput into two 3 APIs to reduce the code size and complexity as stated below<br> 
 *       (a)XMC_ACMP_EnableReferenceDivider <br>
 *       (b)XMC_ACMP_DisableReferenceDivider <br> 
 *       (c)XMC_ACMP_SetInput <br> 
 *     - Optimized enable and disable API's and moved to header file as static inline APIs.
 *     - XMC_ACMP_t typedef changed to structure which overrides the standard header file structure.
 * 2015-06-20: 
 *     - Removed version macros and declaration of GetDriverVersion API 
 * 2015-06-26: 
 *     - API help documentation modified.
 * 2015-09-02: 
 *     - API help documentation modified for XMC1400 device support.   
 * @endcond 
 *
 */
 
#ifndef XMC_ACMP_H
#define XMC_ACMP_H
 

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup ACMP
 * @brief Analog Comparator(ACMP) low level driver for XMC1 family of microcontrollers. <br>
 *
 * The ACMP module consists of minimum of 3 analog comparators. Each analog comparator has two inputs, INP and INN. 
 * Input INP is compared with input INN in the pad voltage domain.
 * It generates a digital comparator output signal. The digital comparator output signal is shifted down from VDDP 
 * power supply voltage level to VDDC core voltage level. The ACMP module provides the following functionalities.\n
 * -# Monitor external voltage level
 * -# Operates in low power mode
 * -# Provides Inverted ouput option\n

 * \par The ACMP low level driver funtionalities
 * <OL>
 * <LI>Initializes an instance of analog comparator module with the @ref XMC_ACMP_CONFIG_t configuration structure 
 * using the API XMC_ACMP_Init().</LI>
 * <LI>Programs the source of input(INP) specified by @ref XMC_ACMP_INP_SOURCE_t parameter using the API 
 * XMC_ACMP_SetInput(). </LI>
 * <LI>Sets the low power mode of operation using XMC_ACMP_SetLowPowerMode() API.</LI>
 * </OL>
 * @{
 */
 
/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_common.h>

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* If ACMP is available*/
#if defined (COMPARATOR)

#define XMC_ACMP0 (XMC_ACMP_t*)COMPARATOR /**< Comparator module base address defined*/

#if UC_SERIES == XMC14
#define XMC_ACMP_MAX_INSTANCES     (4U) /* Maximum number of Analog Comparators available*/
#else
#define XMC_ACMP_MAX_INSTANCES     (3U) /* Maximum number of Analog Comparators available*/
#endif

/* Checks if the pointer being passed is valid*/
#define XMC_ACMP_CHECK_MODULE_PTR(PTR)  (((PTR)== (XMC_ACMP_t*)COMPARATOR))

/* Checks if the instance being addressed is valid*/
#define XMC_ACMP_CHECK_INSTANCE(INST)   (((INST)< XMC_ACMP_MAX_INSTANCES))

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/**
 * Defines the return value of an API.
 */ 
typedef enum XMC_ACMP_STATUS
{
  XMC_ACMP_STATUS_SUCCESS = 0U, /**< API completes the execution successfully */
  XMC_ACMP_STATUS_ERROR       , /**< API cannot fulfill the request */
} XMC_ACMP_STATUS_t;

/**
 * Defines the hysteresis voltage levels to reduce noise sensitivity.
 */ 
typedef enum XMC_ACMP_HYSTERESIS
{
  XMC_ACMP_HYSTERESIS_OFF = 0U, /**< No hysteresis */
  XMC_ACMP_HYSTERESIS_10     ,  /**< Hysteresis = 10mv */
  XMC_ACMP_HYSTERESIS_15     ,  /**< Hysteresis = 15mv */
  XMC_ACMP_HYSTERESIS_20        /**< Hysteresis = 20mv */
} XMC_ACMP_HYSTERESIS_t;

/**
 *  Defines the comparator output status options.
 */ 
typedef enum XMC_ACMP_COMP_OUT
{
  XMC_ACMP_COMP_OUT_NO_INVERSION = 0U, /**< ACMP output is HIGH when, Input Positive(INP) greater than Input
                                            Negative(INN). Vplus > Vminus */
  XMC_ACMP_COMP_OUT_INVERSION          /**< ACMP output is HIGH when, Input Negative(INN) greater than Input
                                            Positive(INP). Vminus > Vplus*/
} XMC_ACMP_COMP_OUT_t;

/**
 *  Defines the analog comparator input connection method.
 */ 
typedef enum XMC_ACMP_INP_SOURCE
{
  XMC_ACMP_INP_SOURCE_STANDARD_PORT  = 0U,                                          /**< Input is connected to port */
  XMC_ACMP_INP_SOURCE_ACMP1_INP_PORT = (uint16_t)(COMPARATOR_ANACMP0_ACMP0_SEL_Msk) /**< Input is connected to port
                                                                                     and ACMP1 INP */
} XMC_ACMP_INP_SOURCE_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/


/*Anonymous structure/union guard start*/
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__TASKING__)
  #pragma warning 586
#endif

/**
 * ACMP module
 */
typedef struct {
  __IO uint32_t  ORCCTRL;
  __I  uint32_t  RESERVED[726];
  __IO uint32_t  ANACMP[XMC_ACMP_MAX_INSTANCES];
} XMC_ACMP_t;

/**
 *  Structure for initializing the ACMP module. It configures the ANACMP register of the respective input.
 */
typedef struct XMC_ACMP_CONFIG
{
  union
  {
    struct
    {
      uint32_t                  : 1;
      uint32_t filter_disable   : 1; /**< Comparator filter option for removing glitches. By default this option
                                          is selected in ANACMP register. Setting this option disables the filter */
      uint32_t                  : 1;
      uint32_t output_invert    : 1; /**< Option to invert the comparator output. Use XMC_@ref XMC_ACMP_COMP_OUT_t type*/
      uint32_t hysteresis       : 2; /**< Hysteresis voltage to reduce noise sensitivity. Select the voltage levels
                                          from the values defined in @ref XMC_ACMP_HYSTERESIS_t. */
      uint32_t                  : 26;
    };
    uint32_t anacmp;
  };
} XMC_ACMP_CONFIG_t;

/*Anonymous structure/union guard end*/
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__TASKING__)
  #pragma warning restore
#endif

#ifdef __cplusplus
   extern "C" {
#endif

/*********************************************************************************************************************
 * API Prototypes
 ********************************************************************************************************************/

/**
 * @param peripheral Constant pointer to analog comparator module, of @ref XMC_ACMP_t type. Use @ref XMC_ACMP0 macro.
 * @param instance ACMP instance number. <BR>
 *                 Range:<BR> 0 - ACMP0<BR>
 *                            1 - ACMP1<BR>
 *                            2 - ACMP2<BR>
 *                            3 - ACMP3 - Only applicable for XMC1400 devices <BR> 
 *
 * @param config Pointer to configuration data. Refer data structure @ref XMC_ACMP_CONFIG_t for settings.
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 *  Initializes an instance of analog comparator module.<BR>\n
 *  Configures the ANACMP resister with hysteresis, comparator filter and inverted comparator output.
 *
 * \par<b>Related APIs:</b><br>
 *  None.
 */
void XMC_ACMP_Init(XMC_ACMP_t *const peripheral, uint32_t instance, const XMC_ACMP_CONFIG_t *const config);

/**
 * @param peripheral Constant pointer to analog comparator module, of @ref XMC_ACMP_t type. Use @ref XMC_ACMP0 macro.
 * @param instance ACMP instance number. <BR>
 *                 Range:<BR> 0 - ACMP0<BR>
 *                            1 - ACMP1<BR>
 *                            2 - ACMP2<BR>
 *                            3 - ACMP3 - Only applicable for XMC1400 devices <BR>
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables an instance of ACMP module.<BR>\n
 * Starts the comparator by setting CMP_EN bit of respective ANACMP \a instance register. The \a instance number 
 * determines which analog comparator to be switched on. Call this API after the successful completion of the comparator 
 * initilization and input selection.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_ACMP_DisableComparator().<BR>
 */
__STATIC_INLINE void XMC_ACMP_EnableComparator(XMC_ACMP_t *const peripheral, uint32_t instance)
{
  XMC_ASSERT("XMC_ACMP_EnableComparator:Wrong module pointer", XMC_ACMP_CHECK_MODULE_PTR(peripheral))
  XMC_ASSERT("XMC_ACMP_EnableComparator:Wrong instance number", XMC_ACMP_CHECK_INSTANCE(instance) )

  peripheral->ANACMP[instance] |= (uint16_t)COMPARATOR_ANACMP0_CMP_EN_Msk;

}


/**
 * @param peripheral Constant pointer to analog comparator module, of @ref XMC_ACMP_t type. Use @ref XMC_ACMP0 macro.
 * @param instance ACMP instance number. <BR>
 *                 Range:<BR> 0 - ACMP0<BR>
 *                            1 - ACMP1<BR>
 *                            2 - ACMP2<BR>
 *                            3 - ACMP3 - Only applicable for XMC1400 devices <BR> 
 * @return
 *    None<BR>
 * \par<b>Description:</b><br>
 * Disables an instance of ACMP module.<BR>\n
 * Stops the comparator by resetting CMP_EN bit of respective ANACMP \a instance register. The \a instance number 
 * determines which analog comparator to be switched off.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_ACMP_EnableComparator().
 */
__STATIC_INLINE void XMC_ACMP_DisableComparator(XMC_ACMP_t *const peripheral, uint32_t instance)
{
  XMC_ASSERT("XMC_ACMP_DisableComparator:Wrong module pointer", XMC_ACMP_CHECK_MODULE_PTR(peripheral))
  XMC_ASSERT("XMC_ACMP_DisableComparator:Wrong instance number", XMC_ACMP_CHECK_INSTANCE(instance) )

  peripheral->ANACMP[instance] &= (uint16_t)(~((uint32_t)COMPARATOR_ANACMP0_CMP_EN_Msk));
}

/**
 * @param None 
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Enables the reference divider for analog comparator instance 1.<BR>\n
 * ACMP1 input INP is driven by an internal reference voltage by setting DIV_EN bit of ANACMP1 register.
 * Other comparator instances can also share this reference divider option by calling the XMC_ACMP_SetInput() API.
 * 
 * \par<b>Related APIs:</b><br>
 * XMC_ACMP_SetInput().
 */
__STATIC_INLINE void XMC_ACMP_EnableReferenceDivider(void)
{
  /* Enable the divider switch and connect the divided reference to ACMP1.INP */
  COMPARATOR->ANACMP1 |= (uint16_t)(COMPARATOR_ANACMP1_REF_DIV_EN_Msk);
}

/**
 * @param None 
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Disables the reference divider for analog comparator instance 1.<BR>\n
 * ACMP1 input INP is disconnected from the reference divider. This is achieved by reseting DIV_EN bit of ANACMP1 
 * register.
 *
 * \par<b>Related APIs:</b><br>
 * None.
 */
__STATIC_INLINE void XMC_ACMP_DisableReferenceDivider(void)
{
  /* Disable the divider switch and use ACMP1.INP as standard port*/
  COMPARATOR->ANACMP1 &= (uint16_t)(~(COMPARATOR_ANACMP1_REF_DIV_EN_Msk));
}

/**
 * @param peripheral Constant pointer to analog comparator module, of @ref XMC_ACMP_t type. Use @ref XMC_ACMP0 macro.
 * @param instance ACMP instance number. <BR>
 *                 Range:<BR> 0 - ACMP0<BR>
 *                            2 - ACMP2<BR>
 *                            3 - ACMP3 - Only applicable for XMC1400 devices <BR> 
 * @param source ACMP input source selection options.<BR>
 *                 Range:<BR> XMC_ACMP_INP_SOURCE_STANDARD_PORT  - Input is connected to port<BR>
 *                            XMC_ACMP_INP_SOURCE_ACMP1_INP_PORT - Input is connected to port and ACMP1 INP <BR> 
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Sets the analog comparartor input selection for ACMP0, ACMP2 instances.<BR>\n
 * Apart from ACMP1 instance, each ACMP instances can be connected to its own port and ACMP1 INP.
 * Calling @ref XMC_ACMP_EnableReferenceDivider() API, after this API can share the reference divider to one of the 
 * comparartor input as explained in the following options.<br>
 * The hardware options to set input are listed below.<br>
 * <OL>
 * <LI>The comparator inputs aren't connected to other ACMP1 comparator inputs.</LI>
 * <LI>Can program the comparator-0 to connect ACMP0.INP to ACMP1.INP in XMC1200 AA or XMC1300 AA</LI>
 * <LI>Can program the comparator-0 to connect ACMP0.INN to ACMP1.INP in XMC1200 AB or XMC1300 AB or XMC1400 AA</LI>
 * <LI>Can program the comparator-2 to connect ACMP2.INP to ACMP1.INP</LI>
 * </OL><br>
 * Directly accessed registers are ANACMP0, ANACMP2 according to the availability of instance in the devices.
 *
 * \par<b>Related APIs:</b><br>
 * @ref XMC_ACMP_EnableReferenceDivider.<BR>
 * @ref XMC_ACMP_DisableReferenceDivider. 
 */
void XMC_ACMP_SetInput(XMC_ACMP_t *const peripheral, uint32_t instance, const XMC_ACMP_INP_SOURCE_t source);


/**
 * @param None
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Set the comparartors to operate in low power mode, by setting the LPWR bit of ANACMP0 register.<BR>\n
 * The low power mode is controlled by ACMP0 instance. Low power mode is applicable for all instances of the
 * comparator. In low power mode, blanking time will be introduced to ensure the stability of comparartor output. This 
 * will slow down the comparator operation.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_ACMP_ClearLowPowerMode().
 */
__STATIC_INLINE void XMC_ACMP_SetLowPowerMode(void)
{
  COMPARATOR->ANACMP0 |= (uint16_t)COMPARATOR_ANACMP0_CMP_LPWR_Msk;
}

/**
 * @param None 
 * @return
 *    None<BR>
 *
 * \par<b>Description:</b><br>
 * Exits the low power mode by reseting LPWR bit of ANACMP0 register.<BR>\n
 * The low power mode is controlled by ACMP0 module.  Low power mode is applicable for all instances of the
 * comparator. To re-enable the low power mode, call the related API @ref XMC_ACMP_SetLowPowerMode().  
 *
 * \par<b>Related APIs:</b><br>
 * XMC_ACMP_SetLowPowerMode().
 */
__STATIC_INLINE void XMC_ACMP_ClearLowPowerMode(void)
{
  COMPARATOR->ANACMP0 &= (uint16_t)(~(uint16_t)COMPARATOR_ANACMP0_CMP_LPWR_Msk);
}

/**
 * @}
 */

/**
 * @}
 */
 
#ifdef __cplusplus
}
#endif

#endif /* If ACMP is available*/

#endif /* XMC_ACMP_H */
