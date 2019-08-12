/**************************************************************************//**
 * @file     hdiv.h
 * @version  V3.0
 * $Revision: 1 $
 * $Date: 16/07/07 7:50p $
 * @brief    M261 series Hardware Divider(HDIV) driver header file
 *
 * @note
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of Nuvoton Technology Corp. nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/
#ifndef __HDIV_H__
#define __HDIV_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup HDIV_Driver HDIV Driver
  @{
*/

/** @addtogroup HDIV_EXPORTED_FUNCTIONS HDIV Exported Functions
  @{
*/

/**
 * @brief      Division function to calculate (x/y)
 *
 * @param[in]  x the dividend of the division
 * @param[in]  y the divisor of the division
 *
 * @return     The result of (x/y)
 *
 * @details    This is a division function to calculate x/y
 *
 */
static __INLINE int32_t HDIV_Div(int32_t x, int16_t y)
{
    uint32_t *p32;

    p32 = (uint32_t *)HDIV_BASE;
    *p32++ = (uint32_t)x;
    *p32++ = (uint32_t)y;
    return (int32_t) * p32;
}


/**
 * @brief      To calculate the remainder of x/y, i.e., the result of x mod y.
 *
 * @param[in]  x the dividend of the division
 * @param[in]  y the divisor of the division
 *
 * @return     The remainder of (x/y)
 *
 * @details    This function is used to calculate the remainder of x/y.
 */
static __INLINE int16_t HDIV_Mod(int32_t x, int16_t y)
{
    uint32_t *p32;

    p32 = (uint32_t *)HDIV_BASE;
    *p32++ = (uint32_t)x;
    *p32++ = (uint32_t)y;
    return (int16_t)p32[1];
}

/*@}*/ /* end of group HDIV_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group HDIV_Driver */

/*@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif //__HDIV_H__

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/


