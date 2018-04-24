/**
 * @file xmc_fce.h
 * @date 2015-06-20
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
 * 2015-02-20:
 *     - Initial
 *      
 * 2015-05-20:
 *     - Description updated <br>
 *   
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API
 * @endcond 
 *
 */

#ifndef XMC_FCE_H
#define XMC_FCE_H

/**********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/
 
#include <xmc_common.h>

#if defined (FCE)

/**
 * @addtogroup XMClib
 * @{
 */

/**
 * @addtogroup FCE
 * @brief Flexible CRC Engine(FCE) driver for the XMC microcontroller family.
 *
 * The FCE provides a parallel implementation of Cyclic Redundancy Code (CRC)
 * algorithms. The current FCE version for the XMC4000 microcontroller family implements the
 * IEEE 802.3 ethernet CRC32, the CCITT CRC16 and the SAE J1850 CRC8 polynomials.
 * The primary target of FCE is to be used as an hardware acceleration engine for software
 * applications or operating systems services using CRC signatures.
 *
 * @image html fce_overview.png	 
 * @image latex ../images/fce_overview.png	 
 * FCE Features: <br>
 * @image html fce_polynomials.png
 * @image latex ../images/fce_polynomials.png
 *    * CRC kernel 0 and 1: IEEE 802.3 CRC32 ethernet polynomial: 0x04C11DB71 <br>
 *    * CRC kernel 2: CCITT CRC16 polynomial: 0x1021 <br>
 *    * CRC kernel 3: SAE J1850 CRC8 polynomial: 0x1D <br>
 *    * Configuration Registers enable to control the CRC operation and perform automatic checksum checks at 
 *      the end of a message. <br>
 *    * Extended register interface to control reliability of FCE execution in safety applications. <br>
 *    * Error notification scheme via dedicated interrupt node for: <br>
        a)Transient error detection: Error interrupt generation (maskable) with local status register 
		  (cleared by software) <br>
        b)Checksum failure: Error interrupt generation (maskable) with local status register (cleared by software) <br>
		
     FCE provides one interrupt line to the interrupt system. Each CRC engine has its own set of flag registers. <br>

 * @{
 */
 
/**********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
 
#define XMC_FCE_CRC32_0        FCE_KE0 /**< Kernel 0 <br> */
#define XMC_FCE_CRC32_1        FCE_KE1 /**< Kernel 1 <br> */
#define XMC_FCE_CRC16          FCE_KE2 /**< Kernel 2 <br> */
#define XMC_FCE_CRC8           FCE_KE3 /**< Kernel 3 <br> */

#define XMC_FCE_REFIN_SET      (1U) /**< Enables input reflection */
#define XMC_FCE_REFIN_RESET    (0U) /**< Disables input reflection */
#define XMC_FCE_REFOUT_SET     (1U) /**< Enables output reflection */
#define XMC_FCE_REFOUT_RESET   (0U) /**< Disables output reflection */
#define XMC_FCE_INVSEL_SET     (1U) /**< Enables output inversion */
#define XMC_FCE_INVSEL_RESET   (0U) /**< Disables output inversion */

/**********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/
 
/**
 * FCE interrupt configuration
 */
typedef enum XMC_FCE_CONFIG_INTERRUPT
{
  XMC_FCE_CFG_CONFIG_CMI = FCE_KE_CFG_CMI_Msk, /**< Enables CRC Mismatch interrupt \n*/
  XMC_FCE_CFG_CONFIG_CEI = FCE_KE_CFG_CEI_Msk, /**< Enables Configuration error interrupt \n*/
  XMC_FCE_CFG_CONFIG_LEI = FCE_KE_CFG_LEI_Msk, /**< Enables Length error interrupt \n*/
  XMC_FCE_CFG_CONFIG_BEI = FCE_KE_CFG_BEI_Msk  /**< Enables Bus error interrupt \n*/
} XMC_FCE_CONFIG_INTERRUPT_t;

/**
 * FCE operation configuration
 */
typedef enum XMC_FCE_CONFIG_OPERATION
{
  XMC_FCE_CFG_CONFIG_CCE = FCE_KE_CFG_CCE_Msk, /**< Enables CRC check */
  XMC_FCE_CFG_CONFIG_ALR = FCE_KE_CFG_ALR_Msk  /**< Enables Automatic length reload */
} XMC_FCE_CONFIG_OPERATION_t;

/**
 * FCE algorithm configuration
 */
typedef enum XMC_FCE_CONFIG_ALGO
{
  XMC_FCE_CFG_CONFIG_REFIN  = FCE_KE_CFG_REFIN_Msk,  /**< Enables input byte reflection */
  XMC_FCE_CFG_CONFIG_REFOUT = FCE_KE_CFG_REFOUT_Msk, /**< Enables Final CRC reflection */
  XMC_FCE_CFG_CONFIG_XSEL   = FCE_KE_CFG_XSEL_Msk    /**< Enables output inversion */
} XMC_FCE_CONFIG_ALGO_t;

/**
 * FCE status flag configuration
 */
typedef enum XMC_FCE_STS_FLAG
{
  XMC_FCE_STS_MISMATCH_CRC = FCE_KE_STS_CMF_Msk, /**< CRC Mismatch flag */
  XMC_FCE_STS_CONFIG_ERROR = FCE_KE_STS_CEF_Msk, /**< Configuration Error flag */
  XMC_FCE_STS_LENGTH_ERROR = FCE_KE_STS_LEF_Msk, /**< Length Error flag */
  XMC_FCE_STS_BUS_ERROR    = FCE_KE_STS_BEF_Msk	 /**< Bus Error flag */
} XMC_FCE_STS_FLAG_t;

/**
 * FCE control configuration
 */
typedef enum XMC_FCE_CTR_TEST
{
  XMC_FCE_CTR_MISMATCH_CRC   = FCE_KE_CTR_FCM_Msk,      /**< Forces CRC mismatch */
  XMC_FCE_CTR_MISMATCH_CFG   = FCE_KE_CTR_FRM_CFG_Msk,	/**< Forces CFG Register mismatch */
  XMC_FCE_CTR_MISMATCH_CHECK = FCE_KE_CTR_FRM_CHECK_Msk /**< Forces CRC Check Register mismatch */
} XMC_FCE_CTR_TEST_t;

/**
 * FCE status enumeration
 */
typedef enum XMC_FCE_STATUS
{
  XMC_FCE_STATUS_OK = 0, /**< Returns OK on success */
  XMC_FCE_STATUS_BUSY,   /**< Returns BUSY when API is busy with a previous request */
  XMC_FCE_STATUS_ERROR   /**< Returns ERROR when API cannot fulfil request */
} XMC_FCE_STATUS_t;

/**********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/

/**
 * FCE kernel
 */ 
typedef FCE_KE_TypeDef XMC_FCE_Kernel_t;

/* Anonymous structure/union guard start */
#if defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__TASKING__)
  #pragma warning 586
#endif

/**
 * @brief XMC_FCE configuration structure
 */
typedef struct XMC_FCE_CONFIG
{
  union
  {
    uint32_t regval;
    struct
    {
	  uint32_t               : 8;
      uint32_t config_refin  : 1;  /**< Enables byte-wise reflection */
      uint32_t config_refout : 1;  /**< Enables bit-wise reflection */
      uint32_t config_xsel   : 1;  /**< Enables output inversion */
      uint32_t               : 21; /**< Reserved bits */	  
    };
  };
} XMC_FCE_CONFIG_t;

/* Anonymous structure/union guard end */
#if defined (__CC_ARM)
  #pragma pop
#elif defined (__TASKING__)
  #pragma warning restore
#endif

/**
 * FCE handler
 */
typedef struct XMC_FCE
{
  XMC_FCE_Kernel_t *kernel_ptr;    /**< FCE Kernel Pointer */
  XMC_FCE_CONFIG_t fce_cfg_update; /**< FCE CFG register update */
  uint32_t seedvalue;              /**< CRC seed value to be used */
} XMC_FCE_t;

/**********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/
 
#ifdef __cplusplus
extern "C" {
#endif
 
/**
 * @param None
 * @return uint32_t Module revision number
 *
 * \par<b>Description: </b><br>
 * Read FCE module revision number <br>
 * 
 * \par
 * The value of a module revision starts with 0x01 (first revision). The current revision
 * number is 0x01.
 */
__STATIC_INLINE uint32_t XMC_FCE_ReadModuleRev(void)
{
  return (uint32_t)(FCE->ID & FCE_ID_MOD_REV_Msk);
}

/**
 * @param None
 * @return uint32_t Module type
 *
 * \par<b>Description: </b><br>
 * Read the FCE module type <br>
 * 
 * \par
 * The return value is currently 0xC0. It defines the module as a 32-bit module.
 */
__STATIC_INLINE uint32_t XMC_FCE_ReadModuleType(void)
{
  return (uint32_t)((FCE->ID & FCE_ID_MOD_TYPE_Msk) >> FCE_ID_MOD_TYPE_Pos);
}

/**
 * @param None
 * @return uint32_t Module number
 *
 * \par<b>Description: </b><br>
 * Read FCE module number <br>
 * 
 * \par
 * The return value for FCE module is currently 0x00CA.
 */
__STATIC_INLINE uint32_t XMC_FCE_ReadModuleNumber(void)
{
  return ((uint32_t)((FCE->ID & FCE_ID_MOD_NUMBER_Msk) >> FCE_ID_MOD_NUMBER_Pos));
}

/**
 * @param None
 * @return bool Disable status
 *
 *
 * \par<b>Description: </b><br>
 * Return the disable status <br>
 *
 * \par
 * The function reads the FCE module disable status (DISS) bit. It returns "true" if
 * set, "false" otherwise.
 */
__STATIC_INLINE bool XMC_FCE_Get_DisableStatus(void)
{
  return (bool)(FCE->CLC &= (uint32_t)~FCE_CLC_DISS_Msk);
}

/**
 * @param None
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable the FCE module <br>
 * 
 * \par
 * The function asserts the FCE peripheral reset and sets the DISR bit in the CLC
 * register.
 *
 * \par<b>Note: </b><br>
 * All pending transactions running on the bus slave interface must be completed before
 * entering the disabled state.
 */
void XMC_FCE_Disable(void);

/**
 * @param None
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable the FCE module <br>
 *
 * \par
 * The function de-asserts the peripheral reset and clears the DISR bit CLC register.
 */
void XMC_FCE_Enable(void);

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @return ::XMC_FCE_STATUS_t
 *
 * \par<b>Description: </b><br>
 * Initialize the FCE engine <br>
 * 
 * \par
 * The function sets to the CFG and CRC registers with the FCE configuration and
 * seeds values. The function always returns XMC_FCE_STATUS_SUCCESS.
 *
 * \par<b>Note: </b><br>
 * The software must first ensure that the CRC kernel is properly configured with the
 * initial CRC value (seed value).
 */
XMC_FCE_STATUS_t XMC_FCE_Init(const XMC_FCE_t *const engine);

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param seedvalue Initial CRC value
 * @return None
 *
 * \par<b>Description: </b><br>
 * Initialize FCE seed value
 *
 * \par
 * The function sets the initial CRC (seed) value in the CRC register.
 */
__STATIC_INLINE void XMC_FCE_InitializeSeedValue(const XMC_FCE_t *const engine, uint32_t seedvalue)
{
  engine->kernel_ptr->CRC = seedvalue;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param event OR'd values of ::XMC_FCE_CONFIG_INTERRUPT_t enumeration values
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable FCE event(s) <br>
 *
 * \par
 * The function sets the CFG register to enable FCE event(s).
 */
__STATIC_INLINE void XMC_FCE_EnableEvent(const XMC_FCE_t *const engine, uint32_t event)
{
  engine->kernel_ptr->CFG |= (uint32_t)event;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param event OR'd values of ::XMC_FCE_CONFIG_INTERRUPT_t enumeration values
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable FCE event(s) <br>
 *
 * \par
 * The function clears the CFG register to disable FCE event(s).
 */
__STATIC_INLINE void XMC_FCE_DisableEvent(const XMC_FCE_t *const engine, uint32_t event)
{
  engine->kernel_ptr->CFG &= ~(uint32_t)event;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param event Event of type ::XMC_FCE_STS_FLAG_t
 * @return bool
 *
 * \par<b>Description: </b><br>
 * Return the event status of FCE event <br>
 *
 * \par
 * The function returns the status of a single requested FCE event by reading the
 * appropriate bit-fields of the STS register.
 */
__STATIC_INLINE bool XMC_FCE_GetEventStatus(const XMC_FCE_t *const engine, XMC_FCE_STS_FLAG_t event)
{
  return (bool) (engine->kernel_ptr->STS & (uint32_t)event);
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param event Event of type ::XMC_FCE_STS_FLAG_t
 * @return None
 *
 * \par<b>Description: </b><br>
 * Clear an FCE event <br>
 *
 * \par
 * The function clears requested FCE events by setting the bit-fields of the STS
 * register.
 */
__STATIC_INLINE void XMC_FCE_ClearEvent(const XMC_FCE_t *const engine, XMC_FCE_STS_FLAG_t event)
{
  engine->kernel_ptr->STS |= (uint32_t)event;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param operation FCE operation of type ::XMC_FCE_CONFIG_OPERATION_t
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enable CRC operations <br>
 *
 * \par
 * The function enables FRC operations by writing to the CFG register. 
 *
 * \par<b>Note: </b><br>
 * CRC comparison check (at the end of message) can be enabled using the CCE bit-field.
 * Automatic reload of LENGTH field (at the end of message) can be enabled using the
 * ALR bit field.
 */
__STATIC_INLINE void XMC_FCE_EnableOperation(const XMC_FCE_t *const engine, uint32_t operation)
{
  engine->kernel_ptr->CFG |= operation;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param operation FCE operation of type ::XMC_FCE_CONFIG_OPERATION_t
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable CRC operations <br>
 *
 * \par
 * The function disables FRC operations by writing to the CFG register.
 *
 * \par<b>Note: </b><br>
 * CRC comparison check (at the end of message) can be disabled using the CCE bit-field.
 * Automatic reload of LENGTH field (at the end of message) can be disabled using the
 * ALR bit field.
 */
__STATIC_INLINE void XMC_FCE_DisableOperation(const XMC_FCE_t *const engine, uint32_t operation)
{
  engine->kernel_ptr->CFG &= ~(uint32_t)operation;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param algo A valid algorithm of type ::XMC_FCE_CONFIG_ALGO_t or a valid combination
 *             of logically OR'd algorithms
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enables CRC algorithm(s) <br>
 *
 * \par<b>Note: </b><br>
 * Options for enabling CRC algorithm:    <br>
 * REFIN: Input byte wise reflection      <br>
 * REFOUT: Output bit wise reflection     <br>
 * XSEL: Value to be XORed with final CRC <br>
 */
__STATIC_INLINE void XMC_FCE_EnableCRCAlgorithm(const XMC_FCE_t *const engine, uint32_t algo)
{
  engine->kernel_ptr->CFG |= (uint32_t)algo;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param algo A valid algorithm of type ::XMC_FCE_CONFIG_ALGO_t or a valid combination
 *             of logically OR'd algorithms
 * @return None
 *
 * \par<b>Description: </b><br>
 * Disable CRC algorithm(s) <br>
 *
 * \par<b>Note: </b><br>
 * Options for disabling CRC algorithm:   <br>
 * REFIN: Input byte wise reflection      <br>
 * REFOUT: Output bit wise reflection     <br>
 * XSEL: Value to be XORed with final CRC <br>
 */
__STATIC_INLINE void XMC_FCE_DisableCRCAlgorithm(const XMC_FCE_t *const engine, uint32_t algo)
{
  engine->kernel_ptr->CFG &= ~(uint32_t)algo;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param checkvalue Checksum value
 * @return None
 *
 * \par<b>Description: </b><br>
 * Updates CRC check value <br>
 *
 * \par
 * When the CFG.CCE bit field is set, every time the IR register is written, the
 * LENGTH register is decremented by one until it reaches zero. The hardware monitors
 * the transition of the LENGTH register from 1 to 0 to detect the end of the
 * message and proceed with the comparison of the result register (RES) value with
 * the CHECK register value.
 */
__STATIC_INLINE void XMC_FCE_UpdateCRCCheck(const XMC_FCE_t *const engine, const uint32_t checkvalue)
{
  engine->kernel_ptr->CHECK = 0xFACECAFEU;
  engine->kernel_ptr->CHECK = checkvalue;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param checklength Checksum length
 * @return None <br>
 *
 * \par<b>Description: </b><br>
 * Updates CRC length specified in the input parameter <br>
 *
 * \par
 * When the ALR bit field is set to 1, every write to the IR register decrements
 * the value of the LENGTH bit field. The LENGTH field shall be reloaded with its
 * configuration value at the end of the cycle where LENGTH reaches 0.
 */
__STATIC_INLINE void XMC_FCE_UpdateLength(const XMC_FCE_t *const engine, const uint32_t checklength)
{
  engine->kernel_ptr->LENGTH = 0xFACECAFEU;
  engine->kernel_ptr->LENGTH = checklength;
}

/**
 * @param engine Constant pointer to @ref XMC_FCE_t, pointing to the FCE base address
 * @param data Pointer to the data buffer
 * @param length Total number of bytes of data buffer
 * @param result Pointer to computed CRC result
 * @return XMC_FCE_STATUS_ERROR on error
 * @return XMC_FCE_STATUS_SUCCESS otherwise.
 *
 * \par<b>Description: </b><br>
 * Calculate and updates the CRC8 checksum in the result pointer <br>
 *
 * \par<b>Note: </b><br>
 * A write to IRm (m = 3) triggers the CRC kernel to update the message checksum
 * according to the IR and current CRC register contents. Any write transaction
 * is allowed to this IRm register. Only the lower 8-bit of the write transactions
 * will be used. ::XMC_FCE_GetCRCResult() should be called after invoking
 * ::XMC_FCE_CalculateCRC8() to get final CRC value.
 */
XMC_FCE_STATUS_t XMC_FCE_CalculateCRC8(const XMC_FCE_t *const engine,
                                       const uint8_t *data,
									   uint32_t length,
									   uint8_t *result);

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param data Pointer to the data buffer
 * @param length Length of data buffer
 * @param result Pointer to computed CRC result
 * @return XMC_FCE_STATUS_ERROR on error
 * @return XMC_FCE_STATUS_SUCCESS otherwise.
 *
 * \par<b>Description: </b><br>
 * Calculate and update the RC16 checksum in the result pointer <br>
 *
 * \par<b>Note: </b><br>
 * A write to Internal Register (IRm m = 2) triggers the CRC kernel to update the
 * message checksum according to the IR and current CRC register contents. Only 32-bit
 * or 16-bit write transactions are permitted. Any other bus write transaction will
 * lead to a bus error. Only the lower 16-bit of the write transactions will be used.
 * ::XMC_FCE_GetCRCResult() should be called after ::XMC_FCE_CalculateCRC16() to get
 * final CRC value.
 */
XMC_FCE_STATUS_t XMC_FCE_CalculateCRC16(const XMC_FCE_t *const engine,
                                        const uint16_t *data,
										uint32_t length,
										uint16_t *result);

/**
 * @param engine Constant pointer to @ref XMC_FCE_t, pointing to the FCE base address
 * @param data Pointer to the data buffer
 * @param length Total number of bytes of data buffer
 * @param result Pointer to computed CRC result
 * @return XMC_FCE_STATUS_ERROR on error
 * @return XMC_FCE_STATUS_SUCCESS otherwise.
 *
 * \par<b>Description</b><br>
 * Calculate and update the calculated CRC32 checksum in the result pointer <br>
 *
 * \par<b>Note:</b><br>
 * A write to Internal Register (IRm, m = 0-1) triggers the CRC kernel to update
 * the message checksum according to the IR and current CRC register contents. Only
 * 32-bit write transactions are permitted. Any other bus write transaction will
 * lead to a bus error. ::XMC_FCE_GetCRCResult() should be called after
 * ::XMC_FCE_CalculateCRC32() to get final CRC value.
 */
XMC_FCE_STATUS_t XMC_FCE_CalculateCRC32(const XMC_FCE_t *const engine,
                                        const uint32_t *data,
										uint32_t length,
										uint32_t *result);

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param result Pointer to CRC result
 * @return None
 *
 * \par<b>Description: </b><br>
 * Read the final CRC value from RES register <br>
 */
__STATIC_INLINE void XMC_FCE_GetCRCResult(const XMC_FCE_t *const engine, uint32_t *result)
{
  *result= engine->kernel_ptr->RES;
}

/**
 * @param engine Constant pointer to ::XMC_FCE_t, pointing to the FCE base address
 * @param test values of type ::XMC_FCE_CTR_TEST_t
 * @return None
 *
 * \par<b>Description: </b><br>
 * Trigger the CTR register to generate a CRC mismatch/register mismatch/check register
 * mismatch interrupt <br>
 */
void XMC_FCE_TriggerMismatch(const XMC_FCE_t *const engine, XMC_FCE_CTR_TEST_t test);

/**
 * @param inbuffer Pointer to input data buffer
 * @param outbuffer Pointer to the output data buffer
 * @param length Length of the input buffer
 * @return None
 *
 * \par<b>Description: </b><br>
 * Convert input data buffer's endianness from big endian to little endian <br>
 *
 * \par
 * The function stores the converted data in output data buffer.
 *
 * \par<b>Note: </b><br>
 * This function should be invoked before using ::XMC_FCE_CalculateCRC16() to compute
 * the CRC value.
 */
void XMC_FCE_LittleEndian16bit(uint8_t* inbuffer, uint16_t* outbuffer, uint16_t length);

/**
 * @param inbuffer Pointer to input data buffer
 * @param outbuffer Pointer to the output data buffer
 * @param length Length of the input buffer
 * @return None
 *
 * \par<b>Description: </b><br>
 * Convert input data buffer's endianness from big endian to little endian <br>
 *
 * \par
 * The function stores the converted data in output data buffer.
 *
 * \par<b>Note: </b><br>
 * This function should be invoked before using ::XMC_FCE_CalculateCRC32() to compute
 * the CRC value.
 */
void XMC_FCE_LittleEndian32bit(uint8_t* inbuffer, uint32_t* outbuffer, uint16_t length);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */
 
#endif /* defined (FCE) */

#endif /* XMC_FCE_H */
