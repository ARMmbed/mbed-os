
/**
 * @file xmc_math.c
 * @date 2017-04-20
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
 * 2015-09-23: 
 *     - Added SQRT functions
 *
 * 2015-10-08:
 *     - Return values for sin(), cos(), sinh(), cosh(), arctan() are corrected.
 *
 * 2017-04-20:
 *     - Foward declaration of __aeabi_* to fix link time optimization (-flto) compilation errors
 * 
 * @endcond
 *
 */

/**
 *
 * @brief MATH driver - API implementation for XMC13 family MATH libraries. <br>
 * 
 * <b>Detailed description of file</b> <br>
 * APIs provided in this file cover the following functional blocks of MATH: <br>
 *
 */

/*********************************************************************************************************************
 * HEADER FILES
 ********************************************************************************************************************/
#include <xmc_math.h>

#if defined (MATH)
/*********************************************************************************************************************
 * MACROS
 ********************************************************************************************************************/
/* Reciprocal of Circular gain in XMC_MATH_Q0_23_t format ((2^23)/1.646760258121) */
#define XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023       (0x4DBA76U)
/* Reciprocal of Hyperbolic gain in XMC_MATH_Q1_22_t format ((2^22)/0.828159360960) */
#define XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22    (0x4D47A1U)
/* Signed division is selected */
#define XMC_MATH_SIGNED_DIVISION                      ((uint32_t) 0 << MATH_DIVCON_USIGN_Pos)
/* Unsigned division is selected */
#define XMC_MATH_UNSIGNED_DIVISION                    ((uint32_t) 1 << MATH_DIVCON_USIGN_Pos)

/*********************************************************************************************************************
 * ENUMS
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * GLOBAL DATA
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * DATA STRUCTURES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * LOCAL ROUTINES
 ********************************************************************************************************************/

/*********************************************************************************************************************
 * API IMPLEMENTATION - Utility functions
 ********************************************************************************************************************/

/* Utility function to check if the DIV unit is busy */
bool XMC_MATH_DIV_IsBusy(void)
{
  bool status;
  if (MATH->DIVST & MATH_DIVST_BSY_Msk)
  {
    status = true;  /* DIV unit is busy running a division operation */ 
  }
  else
  {
    status = false; /* DIV unit is idle */ 
  }

  return (status);
}

/* Utility function to check if the CORDIC unit is busy */
bool XMC_MATH_CORDIC_IsBusy(void)
{
  bool status;
  if (MATH->STATC & MATH_STATC_BSY_Msk)
  {
    status = true;  /* CORDIC unit is busy running an operation */  
  }
  else
  {
    status = false; /* CORDIC unit is idle */   
  }

  return (status);
}

/* This functions returns the status of a requested event */
bool XMC_MATH_GetEventStatus(const XMC_MATH_EVENT_t event)
{
  bool status;
  if (MATH->EVFR & (uint32_t) event)
  {
    status = true; /* Requested event has been detected */
  }
  else
  {
    status = false; /* Requested event has not been detected */
  }
  return (status);
}

#ifndef XMC_MATH_DISABLE_DIV_ABI

/* Forward prototypes.  */
uint32_t __aeabi_uidiv(uint32_t dividend, uint32_t divisor) __attribute__((externally_visible));
int32_t __aeabi_idiv(int32_t dividend, int32_t divisor) __attribute__((externally_visible));
uint64_t __aeabi_uidivmod(uint32_t dividend, uint32_t divisor) __attribute__((externally_visible));
int64_t __aeabi_idivmod(int32_t dividend, int32_t divisor) __attribute__((externally_visible));

/***********************************************************************************************************************
 * API IMPLEMENTATION - aeabi routines
 **********************************************************************************************************************/
/* This function performs unsigned integer division */
uint32_t __aeabi_uidiv(uint32_t dividend, uint32_t divisor)
{
  MATH->DIVCON  = XMC_MATH_UNSIGNED_DIVISION;
  MATH->DVD     = dividend;
  MATH->DVS     = divisor;

  return ((uint32_t) MATH->QUOT);
}

/* This function performs signed integer division */
int32_t __aeabi_idiv(int32_t dividend, int32_t divisor)
{
  MATH->DIVCON  = XMC_MATH_SIGNED_DIVISION;
  MATH->DVD     = dividend;
  MATH->DVS     = divisor;

  return ((int32_t) MATH->QUOT);
}

/* This function performs unsigned integer division modulo */
uint64_t __aeabi_uidivmod(uint32_t dividend, uint32_t divisor)
{
  uint64_t remainder;

  MATH->DIVCON  = XMC_MATH_UNSIGNED_DIVISION;
  MATH->DVD     = dividend;
  MATH->DVS     = divisor;

  remainder = ((uint64_t) MATH->RMD) << 32U;
  return (remainder | MATH->QUOT);
}

/* This function performs signed integer division modulo */
int64_t __aeabi_idivmod(int32_t dividend, int32_t divisor)
{
  uint64_t remainder;
  uint64_t result;

  MATH->DIVCON  = XMC_MATH_SIGNED_DIVISION;
  MATH->DVD     = dividend;
  MATH->DVS     = divisor;

  remainder = ((uint64_t) MATH->RMD) << 32U;
  result    = (remainder | MATH->QUOT);
  return ((int64_t) result);
}
#endif

/***********************************************************************************************************************
 * API IMPLEMENTATION - Blocking functions
 **********************************************************************************************************************/
/* This function computes the cosine of a given angle in radians */
XMC_MATH_Q0_23_t XMC_MATH_CORDIC_Cos(XMC_MATH_Q0_23_t angle_in_radians)
{
  int32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U;  /* Clear register */
  MATH->CORDX = XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRX) >> MATH_CORRX_RESULT_Pos;
  return ((XMC_MATH_Q0_23_t) result);
}

/* This function computes the sine of a given angle in radians */
XMC_MATH_Q0_23_t XMC_MATH_CORDIC_Sin(XMC_MATH_Q0_23_t angle_in_radians)
{
  int32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRY) >> MATH_CORRY_RESULT_Pos;
  return ((XMC_MATH_Q0_23_t) result);
}

/* This function computes the tangent of a given angle in radians */
XMC_MATH_Q0_11_t XMC_MATH_CORDIC_Tan(XMC_MATH_Q0_23_t angle_in_radians)
{
  uint32_t result;
  MATH->GLBCON = (uint32_t) XMC_MATH_DIV_DVDRC_CORRY_IS_SOURCE + \
                 (uint32_t) XMC_MATH_DIV_DVSRC_CORRX_IS_SOURCE;      /* Chain the results of CORDIC operation to DIV unit */
  MATH->DIVCON = (uint32_t) 11 << MATH_DIVCON_DVSSRC_Pos;            /* Right Shifts Divisor by 11 places prior to division */
  MATH->STATC  = 0U; /* Clear register */
  MATH->CON    = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                 (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ  = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY  = 0U; /* Clear register */
  MATH->CORDX  = XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
  result       = MATH->QUOT;
  return ((XMC_MATH_Q0_11_t) result);
}

/* This function computes the arc tangent of a given angle in radians */
XMC_MATH_Q0_23_t XMC_MATH_CORDIC_ArcTan(XMC_MATH_Q8_15_t x, XMC_MATH_Q8_15_t y)
{
  uint32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_CIRCULAR;
  MATH->CORDZ = 0U; /* Clear register */
  MATH->CORDY = ((uint32_t) y) << MATH_CORDY_DATA_Pos;
  MATH->CORDX = ((uint32_t) x) << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRZ) >> MATH_CORRZ_RESULT_Pos;
  return ((XMC_MATH_Q0_23_t) result);
}

/* This function computes the hyperbolic cosine of a given angle in radians */
XMC_MATH_Q1_22_t XMC_MATH_CORDIC_Cosh(XMC_MATH_Q0_23_t angle_in_radians)
{
  int32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRX) >> MATH_CORRX_RESULT_Pos;
  return ((XMC_MATH_Q1_22_t) result);
}

/* This function computes the hyperbolic sine of a given angle in radians */
XMC_MATH_Q1_22_t XMC_MATH_CORDIC_Sinh(XMC_MATH_Q0_23_t angle_in_radians)
{
  int32_t result;
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
  result      = ((int32_t)MATH->CORRY) >> MATH_CORRY_RESULT_Pos;
  return ((XMC_MATH_Q1_22_t) result);
}

/* This function computes the hyperbolic tangent of a given angle in radians */
XMC_MATH_Q0_11_t XMC_MATH_CORDIC_Tanh(XMC_MATH_Q0_23_t angle_in_radians)
{
  uint32_t result;
  MATH->GLBCON = (uint32_t) XMC_MATH_DIV_DVDRC_CORRY_IS_SOURCE + \
                 (uint32_t) XMC_MATH_DIV_DVSRC_CORRX_IS_SOURCE;      /* Chain the results of CORDIC operation to DIV unit */
  MATH->DIVCON = (uint32_t) 11 << MATH_DIVCON_DVSSRC_Pos;            /* Right Shifts Divisor by 11 places prior to division */
  MATH->STATC  = 0U; /* Clear register */
  MATH->CON    = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                 (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ  = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY  = 0U; /* Clear register */
  MATH->CORDX  = XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
  result       = MATH->QUOT;
  return ((XMC_MATH_Q0_11_t) result);
}

/***********************************************************************************************************************
 * API IMPLEMENTATION - Non blocking functions
 **********************************************************************************************************************/
/* This function computes the cosine of a given angle in radians */
void XMC_MATH_CORDIC_CosNB(XMC_MATH_Q0_23_t angle_in_radians)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U;  /* Clear register */
  MATH->CORDX = XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
}

/* This function computes the sine of a given angle in radians */
void XMC_MATH_CORDIC_SinNB(XMC_MATH_Q0_23_t angle_in_radians)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
}

/* This function computes the tangent of a given angle in radians */
void XMC_MATH_CORDIC_TanNB(XMC_MATH_Q0_23_t angle_in_radians)
{
  MATH->GLBCON = (uint32_t) XMC_MATH_DIV_DVDRC_CORRY_IS_SOURCE + \
                 (uint32_t) XMC_MATH_DIV_DVSRC_CORRX_IS_SOURCE;      /* Chain the results of CORDIC operation to DIV unit */
  MATH->DIVCON = (uint32_t) 11 << MATH_DIVCON_DVSSRC_Pos;            /* Right Shifts Divisor by 11 places prior to division */

  MATH->STATC  = 0U; /* Clear register */
  MATH->CON    = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_CIRCULAR + \
                 (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ  = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY  = 0U; /* Clear register */
  MATH->CORDX  = XMC_MATH_RECIPROC_CIRCULAR_GAIN_IN_Q023 << MATH_CORDX_DATA_Pos;
}

/* This function computes the arc tangent of a given value */
void XMC_MATH_CORDIC_ArcTanNB(XMC_MATH_Q8_15_t x, XMC_MATH_Q8_15_t y)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_CIRCULAR;
  MATH->CORDZ = 0U; /* Clear register */
  MATH->CORDY = ((uint32_t) y) << MATH_CORDY_DATA_Pos;
  MATH->CORDX = ((uint32_t) x) << MATH_CORDX_DATA_Pos;
}

/* This function computes the hyperbolic cosine of a given angle in radians */
void XMC_MATH_CORDIC_CoshNB(XMC_MATH_Q0_23_t angle_in_radians)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
}

/* This function computes the hyperbolic sine of a given angle in radians */
void XMC_MATH_CORDIC_SinhNB(XMC_MATH_Q0_23_t angle_in_radians)
{
  MATH->STATC = 0U; /* Clear register */
  MATH->CON   = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ = ((uint32_t)angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY = 0U; /* Clear register */
  MATH->CORDX = XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
}

/* This function computes the hyperbolic tangent of a given angle in radians */
void XMC_MATH_CORDIC_TanhNB(XMC_MATH_Q0_23_t angle_in_radians)
{
  MATH->GLBCON = (uint32_t) XMC_MATH_DIV_DVDRC_CORRY_IS_SOURCE + \
                 (uint32_t) XMC_MATH_DIV_DVSRC_CORRX_IS_SOURCE;      /* Chain the results of CORDIC operation to DIV unit */
  MATH->DIVCON = (uint32_t) 11 << MATH_DIVCON_DVSSRC_Pos;            /* Right Shifts Divisor by 11 places prior to division */
  MATH->STATC  = 0U; /* Clear register */
  MATH->CON    = (uint32_t) XMC_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC + \
                 (uint32_t) XMC_MATH_CORDIC_ROTVEC_MODE_ROTATION;
  MATH->CORDZ  = ((uint32_t) angle_in_radians) << MATH_CORDZ_DATA_Pos;
  MATH->CORDY  = 0U; /* Clear register */
  MATH->CORDX  = XMC_MATH_RECIPROC_HYPERBOLIC_GAIN_IN_Q1_22 << MATH_CORDX_DATA_Pos;
}

/* This function performs division for given two unsigned arguments */
void XMC_MATH_DIV_UnsignedDivNB(uint32_t dividend, uint32_t divisor)
{
  MATH->DIVCON = XMC_MATH_UNSIGNED_DIVISION;
  MATH->DVD    = dividend;
  MATH->DVS    = divisor;
}

/* This function performs division for given two signed arguments */
void XMC_MATH_DIV_SignedDivNB(int32_t dividend, int32_t divisor)
{
  MATH->DIVCON = XMC_MATH_SIGNED_DIVISION;
  MATH->DVD    = dividend;
  MATH->DVS    = divisor;
}

/* This function performs modulo operation for given two unsigned arguments */
void XMC_MATH_DIV_UnsignedModNB(uint32_t dividend, uint32_t divisor)
{
  MATH->DIVCON = XMC_MATH_UNSIGNED_DIVISION;
  MATH->DVD    = dividend;
  MATH->DVS    = divisor;
}

/* This function performs modulo operation for given two signed arguments */
void XMC_MATH_DIV_SignedModNB(int32_t dividend, int32_t divisor)
{
  MATH->DIVCON = XMC_MATH_SIGNED_DIVISION;
  MATH->DVD    = dividend;
  MATH->DVS    = divisor;
}

int16_t XMC_MATH_CORDIC_Q15_Sqrt(int16_t x)
{
  int32_t temp;
  MATH->STATC = 0U; /* Clear register */

  MATH->CON   = (uint32_t)XMC_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC |
                (uint32_t)XMC_MATH_CORDIC_ROTVEC_MODE_VECTORING;

  temp = (int32_t)x << 15; /* Q30 to handle numbers > 1.0 */

  MATH->CORDY = (temp - 0x10000000U); /* x - 0.25 */
  MATH->CORDX = (temp + 0x10000000U); /* x + 0.25 */

  return (int16_t)(((MATH->CORRX >> 14) * 39568) >> 16); /* Q16 * Q15 */
}

int32_t XMC_MATH_CORDIC_Q31_Sqrt(int32_t x)
{
  MATH->STATC = 0U; /* Clear register */

  MATH->CON   = (uint32_t)XMC_MATH_CORDIC_OPERATING_MODE_HYPERBOLIC |
                (uint32_t)XMC_MATH_CORDIC_ROTVEC_MODE_VECTORING;

  x >>= 1;  /* Q30 to handle numbers > 1.0 */

  MATH->CORDY = (x - 0x10000000U); /* x - 0.25 */
  MATH->CORDX = (x + 0x10000000U); /* x + 0.25 */

  return ((MATH->CORRX >> 14) * 39568); /* Q16 * Q15 */
}

#endif /* end of #if defined (MATH) */
