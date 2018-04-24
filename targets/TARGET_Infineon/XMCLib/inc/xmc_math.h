/**
 * @file xmc_math.h
 * @date 2015-10-08
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
 * 2015-06-20: 
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *     - Updated copyright and change history section.
 *
 * 2015-08-25: 
 *     - XMC_MATH_ClearEvent() API is updated to set the event clear flag bit. <br>
 *
 * 2015-09-23: 
 *     - Added SQRT functions
 *
 * 2015-10-08:
 *     - Return values for sin(), cos(), sinh(), cosh(), arctan() are corrected.
 *
 * @endcond 
 *
 */

#ifndef XMC_MATH_H
#define XMC_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_common.h>

#if defined(MATH)
#include <xmc_scu.h>

/**
 * @addtogroup XMClib
 * @{
 */

/**
 * @addtogroup MATH
 * @{
 * @brief MATH Coprocessor (MATH) driver for the XMC1302 microcontroller family <br>
 * 
 * The MATH Coprocessor (MATH) module comprises of two independent sub-blocks to support the CPU in math-intensive 
 * computations: a Divider Unit (DIV) for signed and unsigned 32-bit division operations and a CORDIC
 * (COrdinate Rotation DIgital Computer) Coprocessor for computation of trigonometric, linear or hyperbolic functions.<br>
 *
 * MATH driver features:
 * -# CORDIC Coprocessor is used for computation of trigonometric and hyperbolic functions
 * -# Supports result chaining between the Divider Unit and CORDIC Coprocessor
 * -# All MATH APIs are available in <B>Blocking</B> and <B>non-blocking</B> modes. Non-blocking APIs are suffixed with <B>NB</B>.
 * -# 32bit signed and unsigned division implementations available for __aeabi_uidiv(), __aeabi_idiv(), __aeabi_uidivmod(), __aeabi_idivmod()
 * -# Divider and CORDIC unit busy status can be checked by XMC_MATH_DIV_IsBusy() and XMC_MATH_CORDIC_IsBusy()
 * -# Individual APIs available to return the result of each non-blocking MATH function
 *
 * <B>Note:</B> <br>
 * All non-blocking MATH APIs are not atomic and hence occurence of interrupts during the normal execution of
 * these APIs may lead to erroneous results. User has to exercise caution while using these APIs.
 *
 * Example: 
 * Execution of divide instruction (/) in an ISR during the normal execution of non-blocking APIs may give erroneous results.
 *
 */


/*********************************************************************************************************************
 * TYPE DEFINITIONS
 ********************************************************************************************************************/
/**
 * @brief This typedef is used for Input and Output Data representation in blocking & non-blocking functions.
 * XMC_MATH_Q0_23_t => 1 Signed bit, 0 Integer bits, 23 fraction bits.
 */
typedef int32_t XMC_MATH_Q0_23_t;

/**
 * @brief This typedef is used for Input Data representation in blocking & non-blocking functions.
 * XMC_MATH_Q8_15_t => 1 Signed bit, 8 Integer bits, 15 fraction bits.
 */
typedef int32_t XMC_MATH_Q8_15_t;

/**
 * @brief This typedef is used for Output Data representation in blocking & non-blocking functions.
 * XMC_MATH_Q1_22_t => 1 Signed bit, 1 Integer bits, 22 fraction bits.
 */
typedef int32_t XMC_MATH_Q1_22_t;

/**
 * @brief This typedef is used for Output Data representation in blocking & non-blocking functions.
 * XMC_MATH_Q0_11_t => 1 Signed bit, 0 Integer bits, 11 fraction bits.
 */
typedef int32_t XMC_MATH_Q0_11_t;

/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* Utility macros */
#define XMC_MATH_Q0_23(x) ((XMC_MATH_Q0_23_t)(((x) >= 0) ? ((x) * (1 << 23) + 0.5) : ((x) * (1 << 23) - 0.5))) /**< Converts the given number to XMC_MATH_Q0_23_t format */
#define XMC_MATH_Q0_11(x) ((XMC_MATH_Q0_11_t)(((x) >= 0) ? ((x) * (1 << 11) + 0.5) : ((x) * (1 << 11) - 0.5))) /**< Converts the given number to XMC_MATH_Q0_11_t format */

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/**
 * @brief Service request events for the DIV and CORDIC modules
 */
typedef enum XMC_MATH_EVENT
{
  XMC_MATH_EVENT_DIV_END_OF_CALC    = 1U, /**< Divider end of calculation event */
  XMC_MATH_EVENT_DIV_ERROR          = 2U, /**< Divider error event */
  XMC_MATH_EVENT_CORDIC_END_OF_CALC = 4U, /**< CORDIC end of calculation event */
  XMC_MATH_EVENT_CORDIC_ERROR       = 8U  /**< CORDIC error event */
} XMC_MATH_EVENT_t;

/**
 * @brief Dividend Register Result Chaining
 */
typedef enum XMC_MATH_DIV_DVDRC
{
  XMC_MATH_DIV_DVDRC_DISABLED        = 0U << MATH_GLBCON_DVDRC_Pos, /**< No result chaining is selected */
  XMC_MATH_DIV_DVDRC_QUOT_IS_SOURCE  = 1U << MATH_GLBCON_DVDRC_Pos, /**< QUOT register is the selected source */
  XMC_MATH_DIV_DVDRC_RMD_IS_SOURCE   = 2U << MATH_GLBCON_DVDRC_Pos, /**< RMD register is the selected source */
  XMC_MATH_DIV_DVDRC_CORRX_IS_SOURCE = 3U << MATH_GLBCON_DVDRC_Pos, /**< CORRX is the selected source */
  XMC_MATH_DIV_DVDRC_CORRY_IS_SOURCE = 4U << MATH_GLBCON_DVDRC_Pos, /**< CORRY is the selected source */
  XMC_MATH_DIV_DVDRC_CORRZ_IS_SOURCE = 5U << MATH_GLBCON_DVDRC_Pos  /**< CORRZ is the selected source */
} XMC_MATH_DIV_DVDRC_t;

/**
 * @brief Divisor Register Result Chaining
 */
typedef enum XMC_MATH_DIV_DVSRC
{
  XMC_MATH_DIV_DVSRC_DISABLED        = 0U << MATH_GLBCON_DVSRC_Pos, /**< No result chaining is selected */
  XMC_MATH_DIV_DVSRC_QUOT_IS_SOURCE  = 1U << MATH_GLBCON_DVSRC_Pos, /**< QUOT register is the selected source */
  XMC_MATH_DIV_DVSRC_RMD_IS_SOURCE   = 2U << MATH_GLBCON_DVSRC_Pos, /**< RMD register is the selected source */
  XMC_MATH_DIV_DVSRC_CORRX_IS_SOURCE = 3U << MATH_GLBCON_DVSRC_Pos, /**< CORRX is the selected source */
  XMC_MATH_DIV_DVSRC_CORRY_IS_SOURCE = 4U << MATH_GLBCON_DVSRC_Pos, /**< CORRY is the selected source */
  XMC_MATH_DIV_DVSRC_CORRZ_IS_SOURCE = 5U << MATH_GLBCON_DVSRC_Pos  /**< CORRZ is the selected source */
} XMC_MATH_DIV_DVSRC_t;

/**
 * @brief CORDX Register Result Chaining
 */
typedef enum XMC_MATH_CORDIC_CORDXRC
{
  XMC_MATH_CORDIC_CORDXRC_DISABLED       = 0U << MATH_GLBCON_CORDXRC_Pos, /**< No result chaining is selected */
  XMC_MATH_CORDIC_CORDXRC_QUOT_IS_SOURCE = 1U << MATH_GLBCON_CORDXRC_Pos, /**< QUOT register is the selected source */
  XMC_MATH_CORDIC_CORDXRC_RMD_IS_SOURCE  = 2U << MATH_GLBCON_CORDXRC_Pos  /**< RMD register is the selected source */
} XMC_MATH_CORDIC_CORDXRC_t;

/**
 * @brief CORDY Register Result Chaining
 */
typedef enum XMC_MATH_CORDIC_CORDYRC
{
  XMC_MATH_CORDIC_CORDYRC_DISABLED       = 0U << MATH_GLBCON_CORDYRC_Pos, /**< No result chaining is selected */
  XMC_MATH_CORDIC_CORDYRC_QUOT_IS_SOURCE = 1U << MATH_GLBCON_CORDYRC_Pos, /**< QUOT register is the selected source */
  XMC_MATH_CORDIC_CORDYRC_RMD_IS_SOURCE  = 2U << MATH_GLBCON_CORDYRC_Pos  /**< RMD register is the selected source */
} XMC_MATH_CORDIC_CORDYRC_t;

/**
 * @brief CORDZ Register Result Chaining
 */
typedef enum XMC_MATH_CORDIC_CORDZRC
{
  XMC_MATH_CORDIC_CORDZRC_DISABLED       = 0U << MATH_GLBCON_CORDZRC_Pos, /**< No result chaining is selected */
  XMC_MATH_CORDIC_CORDZRC_QUOT_IS_SOURCE = 1U << MATH_GLBCON_CORDZRC_Pos, /**< QUOT register is the selected source */
  XMC_MATH_CORDIC_CORDZRC_RMD_IS_SOURCE  = 2U << MATH_GLBCON_CORDZRC_Pos  /**< RMD register is the selected source */
} XMC_MATH_CORDIC_CORDZRC_t;
/**
 * @brief CORDIC operating mode
 */
typedef enum XMC_MATH_CORDIC_OPERATING_MODE
{
  XMC_MATH_CORDIC_OPERATING_MODE_LINEAR     = 0U << MATH_CON_MODE_Pos,    /**< Linear mode */
  XMC_MATH_CORDIC_OPERATING_MODE_CIRCULAR   = 1U << MATH_CON_MODE_Pos,    /**< Circular mode */
  XMC_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC = 3U << MATH_CON_MODE_Pos     /**< Hyperbolic mode */
} XMC_MATH_CORDIC_OPERATING_MODE_t;

/**
 * @brief Rotation vectoring selection
 */
typedef enum XMC_MATH_CORDIC_ROTVEC_MODE
{
  XMC_MATH_CORDIC_ROTVEC_MODE_VECTORING     = 0U << MATH_CON_ROTVEC_Pos,  /**< Vectoring mode */
  XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION      = 1U << MATH_CON_ROTVEC_Pos   /**< Rotation mode */
} XMC_MATH_CORDIC_ROTVEC_MODE_t;

/**
 * @brief Calculated value of CORRX and CORRY are each divided by this factor to yield the result.
 */
typedef enum XMC_MATH_CORDIC_MAGNITUDE
{
  XMC_MATH_CORDIC_MAGNITUDE_DIVBY1 = 0U << MATH_CON_MPS_Pos,              /**< Divide by 1 */
  XMC_MATH_CORDIC_MAGNITUDE_DIVBY2 = 1U << MATH_CON_MPS_Pos,              /**< Divide by 2 */
  XMC_MATH_CORDIC_MAGNITUDE_DIVBY4 = 2U << MATH_CON_MPS_Pos,              /**< Divide by 4 */
} XMC_MATH_CORDIC_MAGNITUDE_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * API Prototypes - General
 ********************************************************************************************************************/

/**
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the Math module by un-gating the clock.
 *
 * \par
 * MATH coprocessor's clock is enabled by setting \a MATH bit of \a CGATCLR0 register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_Disable()\n\n\n
 *
 */
__STATIC_INLINE void XMC_MATH_Enable(void)
{
  /* Un-gates clock to the MATH kernel */
  XMC_SCU_CLOCK_UngatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_MATH);
}

/**
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the Math module by gating the clock.
 *
 * \par
 * MATH coprocessor's clock is disabled by setting \a MATH bit of \a CGATSET0 register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_Disable()\n\n\n
 *
 */
__STATIC_INLINE void XMC_MATH_Disable(void)
{
  /* Gates clock to the MATH kernel */
  XMC_SCU_CLOCK_GatePeripheralClock(XMC_SCU_PERIPHERAL_CLOCK_MATH);
}

/**
 *
 * @return bool \n
 * true  - if DIV unit is busy
 * false - if DIV unit is not busy
 *
 * \par<b>Description:</b><br>
 * Utility function to check if the DIV unit is busy.
 *
 * \par
 * Divider unit status is determined by reading \a BSY bit of \a DIVST register.
 *
 */
bool XMC_MATH_DIV_IsBusy(void);

/**
 *
 * @return bool \n
 * true  - if CORDIC unit is busy\n
 * false - if CORDIC unit is not busy
 *
 * \par<b>Description:</b><br>
 * Utility function to check if the DIV unit is busy.
 *
 * \par
 * CORDIC coprocessor's status is determined by reading \a BSY bit of \a STATC register.
 *
 */
bool XMC_MATH_CORDIC_IsBusy(void);

/**
 * @param event Event of type XMC_MATH_EVENT_t
 *
 * @return bool\n
 * true  - if status is set\n
 * false - if status is not set
 *
 * \par<b>Description:</b><br>
 * Returns the status of the requested event.
 *
 * \par
 * Status of DIV & CORDIC unit's event (end of calculation & error) status is determined by reading \a EVFR register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_EnableEvent(), XMC_MATH_DisableEvent(), XMC_MATH_SetEvent(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
bool XMC_MATH_GetEventStatus(const XMC_MATH_EVENT_t event);

/**
 * @param event Event of type XMC_MATH_EVENT_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Enables the requested event.
 *
 * \par
 * DIV & CORDIC unit's event (end of calculation & error) is enabled by setting bit-fields of \a EVIER register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_GetEventStatus(), XMC_MATH_DisableEvent()\n\n\n
 *
 */
__STATIC_INLINE void XMC_MATH_EnableEvent(const XMC_MATH_EVENT_t event)
{
  MATH->EVIER |= (uint32_t) event;
}

/**
 * @param event Event of type XMC_MATH_EVENT_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Disables the requested event.
 *
 * \par
 * DIV & CORDIC unit's event (end of calculation & error) is disabled by clearing bit-fields of \a EVIER register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_GetEventStatus(), XMC_MATH_EnableEvent()\n\n\n
 *
 */
__STATIC_INLINE void XMC_MATH_DisableEvent(const XMC_MATH_EVENT_t event)
{
  MATH->EVIER &= ~((uint32_t) event);
}

/**
 * @param event Event of type XMC_MATH_EVENT_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Sets the requested event. This is a software setting for the event.
 *
 * \par
 * DIV & CORDIC unit's event (end of calculation & error) is set by setting bit-fields of \a EVFSR register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_ClearEvent()\n\n\n
 *
 */
__STATIC_INLINE void XMC_MATH_SetEvent(const XMC_MATH_EVENT_t event)
{
  MATH->EVFSR |= (uint32_t) event;
}

/**
 * @param event Event of type XMC_MATH_EVENT_t
 *
 * @return None
 *
 * \par<b>Description:</b><br>
 * Clears the requested event.
 *
 * \par
 * DIV & CORDIC unit's event (end of calculation & error) is cleared by setting bit-fields of \a EVFCR register.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_SetEvent()\n\n\n
 *
 */
__STATIC_INLINE void XMC_MATH_ClearEvent(const XMC_MATH_EVENT_t event)
{
  MATH->EVFCR |= (uint32_t) event;
}

/**
 * @return XMC_MATH_Q0_23_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Cosine operation.
 *
 * \par
 * Most significant 24 bits of \a CORRX register returns the result of Cosine operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_CosNB()\n\n\n
 *
 */
__STATIC_INLINE XMC_MATH_Q0_23_t XMC_MATH_CORDIC_GetCosResult(void)
{
  return ((XMC_MATH_Q0_23_t) (((int32_t)MATH->CORRX) >> MATH_CORRX_RESULT_Pos));
}

/**
 * @return XMC_MATH_Q0_23_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Sine operation.
 *
 * \par
 * Most significant 24 bits of \a CORRY register returns the result of Sine operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_SinNB()\n\n\n
 *
 */
__STATIC_INLINE XMC_MATH_Q0_23_t XMC_MATH_CORDIC_GetSinResult(void)
{
  return ((XMC_MATH_Q0_23_t) (((int32_t)MATH->CORRY) >> MATH_CORRY_RESULT_Pos));
}

/**
 * @return XMC_MATH_Q0_11_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Tangent operation.
 *
 * \par
 * \a QUOT register returns the result of Tangent operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_TanNB()\n\n\n
 *
 */
__STATIC_INLINE XMC_MATH_Q0_11_t XMC_MATH_CORDIC_GetTanResult(void)
{
  return ((XMC_MATH_Q0_11_t) MATH->QUOT);
}

/**
 * @return XMC_MATH_Q0_23_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Arc Tangent operation.
 *
 * \par
 * Most significant 24 bits of \a CORRZ register returns the result of Arc Tangent operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_ArcTanNB()\n\n\n
 *
 */
__STATIC_INLINE XMC_MATH_Q0_23_t XMC_MATH_CORDIC_GetArcTanResult(void)
{
  return ((XMC_MATH_Q0_23_t) (((int32_t)MATH->CORRZ) >> MATH_CORRZ_RESULT_Pos));
}

/**
 * @return XMC_MATH_Q1_22_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Hyperbolic Cosine operation.
 *
 * \par
 * Most significant 24 bits of \a CORRX register returns the result of Hyperbolic Cosine operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_CoshNB()\n\n\n
 *
 */
__STATIC_INLINE XMC_MATH_Q1_22_t XMC_MATH_CORDIC_GetCoshResult(void)
{
  return ((XMC_MATH_Q1_22_t) (((int32_t)MATH->CORRX) >> MATH_CORRX_RESULT_Pos));
}

/**
 * @return XMC_MATH_Q1_22_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Hyperbolic Sine operation.
 *
 * \par
 * Most significant 24 bits of \a CORRY register returns the result of Hyperbolic Sine operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_SinhNB()\n\n\n
 *
 */
__STATIC_INLINE XMC_MATH_Q1_22_t XMC_MATH_CORDIC_GetSinhResult(void)
{
  return ((XMC_MATH_Q1_22_t) (((int32_t)MATH->CORRY) >> MATH_CORRY_RESULT_Pos));
}

/**
 * @return XMC_MATH_Q0_11_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Hyperbolic Tangent operation.
 *
 * \par
 * \a QUOT register returns the result of Hyperbolic Tangent operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_TanhNB()\n\n\n
 *
 */
__STATIC_INLINE XMC_MATH_Q0_11_t XMC_MATH_CORDIC_GetTanhResult(void)
{
  return ((XMC_MATH_Q0_11_t) MATH->QUOT);
}

/**
 * @return uint32_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Unsigned Division operation.
 *
 * \par
 * \a QUOT register returns the result of Unsigned Division operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_DIV_UnsignedDivNB()\n\n\n
 *
 */
__STATIC_INLINE uint32_t XMC_MATH_DIV_GetUnsignedDivResult(void)
{
  return ((uint32_t) MATH->QUOT);
}

/**
 * @return uint32_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Unsigned Modulo operation.
 *
 * \par
 * \a RMD register returns the result of Unsigned Modulo operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_DIV_UnsignedModNB()\n\n\n
 *
 */
__STATIC_INLINE uint32_t XMC_MATH_DIV_GetUnsignedModResult(void)
{
  return ((uint32_t) MATH->RMD);
}

/**
 * @return int32_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Signed Division operation.
 *
 * \par
 * \a QUOT register returns the result of Signed Division operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_DIV_SignedDivNB()\n\n\n
 *
 */
__STATIC_INLINE int32_t XMC_MATH_DIV_GetSignedDivResult(void)
{
  return ((int32_t) MATH->QUOT);
}

/**
 * @return int32_t
 *
 * \par<b>Description:</b><br>
 * Returns result of a Signed Modulo operation.
 *
 * \par
 * \a RMD register returns the result of Signed Modulo operation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_DIV_SignedModNB()\n\n\n
 *
 */
__STATIC_INLINE int32_t XMC_MATH_DIV_GetSignedModResult(void)
{
  return ((int32_t) MATH->RMD);
}

/***********************************************************************************************************************
 * API Prototypes - Blocking functions
 **********************************************************************************************************************/
/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return XMC_MATH_Q0_23_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the cosine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 * Most significant 24 bits of \a CORRX register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_Sin(), XMC_MATH_CORDIC_Tan()\n\n\n
 *
 */
XMC_MATH_Q0_23_t XMC_MATH_CORDIC_Cos(XMC_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return XMC_MATH_Q0_23_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the sine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 * Most significant 24 bits of \a CORRY register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_Cos(), XMC_MATH_CORDIC_Tan()\n\n\n
 *
 */
XMC_MATH_Q0_23_t XMC_MATH_CORDIC_Sin(XMC_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return XMC_MATH_Q0_11_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the tangent for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Chains the results of Cosine (\a CORRX) and Sine (\a CORRY) as a dividend and divisor by configuring \a GLBCON register.
 * \a CORDZ register is programmed with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 * Most significant 24 bits of \a CORRY register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_Sin(), XMC_MATH_CORDIC_Cos()\n\n\n
 *
 */
XMC_MATH_Q0_11_t XMC_MATH_CORDIC_Tan(XMC_MATH_Q0_23_t angle_in_radians);

/**
 *
 * @param x Value representing the proportion of the x-coordinate (XMC_MATH_Q8_15_t format)
 * @param y Value representing the proportion of the y-coordinate (XMC_MATH_Q8_15_t format)
 *
 * @return XMC_MATH_Q0_23_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the principal value arc tangent of an angle of y/x expressed in radians.
 * The input radians must be in XMC_MATH_Q8_15_t format.
 *
 * \par
 * This function programs CORDIC as circular mode.
 * \a CORDY register is programmed with input \a y and \a CORDX register is programmed with input \a x.
 * Most significant 24 bits of \a CORRZ register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 */
XMC_MATH_Q0_23_t XMC_MATH_CORDIC_ArcTan(XMC_MATH_Q8_15_t x, XMC_MATH_Q8_15_t y);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return XMC_MATH_Q1_22_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic cosine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 * Most significant 24 bits of \a CORRX register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_Sinh(), XMC_MATH_CORDIC_Tanh()\n\n\n
 *
 */
XMC_MATH_Q1_22_t XMC_MATH_CORDIC_Cosh(XMC_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return XMC_MATH_Q1_22_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic sine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 * Most significant 24 bits of \a CORRY register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_Cosh(), XMC_MATH_CORDIC_Tanh()\n\n\n
 *
 */
XMC_MATH_Q1_22_t XMC_MATH_CORDIC_Sinh(XMC_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return XMC_MATH_Q0_11_t <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic tangent for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Chains the results of Cosine (\a CORRX) and Sine (\a CORRY) as a dividend and divisor by configuring \a GLBCON register.
 * \a CORDZ register is programmed with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 * \a QUOT register returns the result of the operation.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_Sinh(), XMC_MATH_CORDIC_Cosh()()\n\n\n
 *
 */
XMC_MATH_Q0_11_t XMC_MATH_CORDIC_Tanh(XMC_MATH_Q0_23_t angle_in_radians);

/***********************************************************************************************************************
 * API Prototypes - Non blocking functions
 **********************************************************************************************************************/
/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the cosine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 * Call XMC_MATH_CORDIC_GetCosResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_GetCosResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_CORDIC_CosNB(XMC_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the sine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 * Call XMC_MATH_CORDIC_GetSinResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_GetSinResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_CORDIC_SinNB(XMC_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the tangent for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 * Call XMC_MATH_CORDIC_GetTanResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & circular mode.
 * Chains the results of Cosine (\a CORRX) and Sine (\a CORRY) as a dividend and divisor by configuring \a GLBCON register.
 * \a CORDZ register is programmed with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_GetTanResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_CORDIC_TanNB(XMC_MATH_Q0_23_t angle_in_radians);

/**
 *
 * @param x Value representing the proportion of the x-coordinate (XMC_MATH_Q8_15_t format)
 * @param y Value representing the proportion of the y-coordinate (XMC_MATH_Q8_15_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the principal value arc tangent of an angle of y/x expressed in radians.
 * The input radians must be in XMC_MATH_Q8_15_t format.
 * Call XMC_MATH_CORDIC_GetArcTanResult() API to get the result.
 *
 * \par
 * This function programs CORDIC as circular mode.
 * \a CORDY register is programmed with input \a y and \a CORDX register is programmed with input \a x.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_GetArcTanResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_CORDIC_ArcTanNB(XMC_MATH_Q8_15_t x, XMC_MATH_Q8_15_t y);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic cosine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 * Call XMC_MATH_CORDIC_GetCoshResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_GetCoshResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_CORDIC_CoshNB(XMC_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic sine for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 * Call XMC_MATH_CORDIC_GetSinhResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Configures \a CORDZ register with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_GetSinhResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_CORDIC_SinhNB(XMC_MATH_Q0_23_t angle_in_radians);

/**
 * @param angle_in_radians - Normalised Angle in Radians (XMC_MATH_Q0_23_t format)
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Computes the hyperbolic tangent for an angle in radians \e angle_in_radians.
 * The input angle in radians must be in XMC_MATH_Q0_23_t format.
 * Call XMC_MATH_CORDIC_GetTanhResult() API to get the result.
 *
 * \par
 * This function programs CORDIC to rotation & hyperbolic mode.
 * Chains the results of Cosine (\a CORRX) and Sine (\a CORRY) as a dividend and divisor by configuring \a GLBCON register.
 * \a CORDZ register is programmed with input \a angle_in_radians and \a CORDX register with gain \a XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22.
 *
 * \par<b>Note:</b><br>
 * Loading of \a CORDX register triggers the start of computation.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_CORDIC_GetTanhResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_CORDIC_TanhNB(XMC_MATH_Q0_23_t angle_in_radians);

/**
 * @param dividend - Dividend
 * @param divisor  - Divisor
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Performs unsigned integer division and computes quotient of the division.
 * Call XMC_MATH_DIV_GetUnsignedDivResult() API to get the result.
 *
 * \par
 * Divider unit is configured for unsigned division.
 * \a DVD & \a DVS registers are programmed with \a dividend and \a divisor values.
 * The division is started with the write to DVS register.
 *
 * \par<b>Note:</b><br>
 * Ensure \e divisor is smaller than \e dividend.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_DIV_GetUnsignedDivResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_DIV_UnsignedDivNB(uint32_t dividend, uint32_t divisor);

/**
 * @param dividend - Dividend
 * @param divisor  - Divisor
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Performs signed integer division and computes quotient of the division.
 * Call XMC_MATH_DIV_GetSignedDivResult() API to get the result.
 *
 * \par
 * Divider unit is configured for signed division.
 * \a DVD & \a DVS registers are programmed with \a dividend and \a divisor values.
 * The division is started with the write to DVS register.
 *
 * \par<b>Note:</b><br>
 * Ensure \e divisor is smaller than \e dividend.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_DIV_GetSignedDivResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_DIV_SignedDivNB(int32_t dividend, int32_t divisor);

/**
 * @param dividend - Dividend
 * @param divisor  - Divisor
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Performs unsigned modulo operation and computes remainder of the division.
 * Call XMC_MATH_DIV_GetUnsignedModResult() API to get the result.
 *
 * \par
 * Divider unit is configured for unsigned division.
 * \a DVD & \a DVS registers are programmed with \a dividend and \a divisor values.
 * The division is started with the write to DVS register.
 *
 * \par<b>Note:</b><br>
 * Ensure \e divisor is smaller than \e dividend.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_DIV_GetUnsignedModResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_DIV_UnsignedModNB(uint32_t dividend, uint32_t divisor);

/**
 * @param dividend - Dividend
 * @param divisor  - Divisor
 *
 * @return None <BR>
 *
 * \par<b>Description:</b><br>
 * Performs signed modulo operation and computes remainder of the division.
 * Call XMC_MATH_DIV_GetSignedModResult() API to get the result.
 *
 * \par
 * Divider unit is configured for signed division.
 * \a DVD & \a DVS registers are programmed with \a dividend and \a divisor values.
 * The division is started with the write to DVS register.
 *
 * \par<b>Note:</b><br>
 * Ensure \e divisor is smaller than \e dividend.
 *
 * \par<b>Related APIs:</b><BR>
 * XMC_MATH_DIV_GetSignedModResult(), XMC_MATH_EnableEvent(), XMC_MATH_GetEventStatus(), XMC_MATH_ClearEvent()\n\n\n
 *
 */
void XMC_MATH_DIV_SignedModNB(int32_t dividend, int32_t divisor);

/**
 * @param x - Value whose square root is computed
 *
 * @return Square root of x <BR>
 *
 * \par<b>Description:</b><br>
 * Computes square root of Q15 number
 *
 * \par<b>Note:</b><br>
 * x > 0
 *
 */
int16_t XMC_MATH_CORDIC_Q15_Sqrt(int16_t x);

/**
 * @param x - Value whose square root is computed
 *
 * @return Square root of x <BR>
 *
 * \par<b>Description:</b><br>
 * Computes square root of Q31 number
 *
 * \par<b>Note:</b><br>
 * x > 0
 *
 */
int32_t XMC_MATH_CORDIC_Q31_Sqrt(int32_t x);
/**
 * @}
 */

/**
 * @}
 */

#endif /* end of #if defined(MATH) */
 
#ifdef __cplusplus
}
#endif

#endif /* XMC_MATH_H */
