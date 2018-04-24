
/**
 * @file xmc_prng.h
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
 *     - Initial <br>
 *     - Documentation updates <br>
 *
 * 2015-06-20:
 *     - Removed version macros and declaration of GetDriverVersion API <br>
 *
 * @endcond
 */
 
#ifndef XMC_PRNG_H
#define XMC_PRNG_H

/*********************************************************************************************************************
 * HEADER FILES
 *********************************************************************************************************************/

#include "xmc_common.h"

#if defined (PRNG) 

/**
 * @addtogroup XMClib XMC Peripheral Library
 * @{
 */

/**
 * @addtogroup PRNG
 * @brief Pseudo Random Number Generator (PRNG) driver for XMC1000 microcontroller family
 *
 * The pseudo random bit generator (PRNG) provides random data with fast generation times.
 * PRNG has to be initialized by the user software before use. The initialization consists
 * of two basic phases: key-loading and warm-up.
 *  
 * The PRNG low level driver provides functions to configure and initialize the PRNG hardware
 * peripheral.
 * @{
 */  

/*********************************************************************************************************************
 * MACROS
 *********************************************************************************************************************/
 
/**
 * Byte mask value for random data block size
 */ 
#define XMC_PRNG_RDBS_BYTE_READ_MASK (0x00FFU)

/*********************************************************************************************************************
 * ENUMS
 *********************************************************************************************************************/

/**
 * PRNG key load operation modes
 */
typedef enum XMC_PRNG_KEY_LOAD_OP_MODE {
  XMC_PRNG_STRM_MODE = 0U, /**< Streaming mode (default) */
  XMC_PRNG_KLD_MODE  = 1U  /**< Loading mode */
} XMC_PRNG_KEY_LOAD_OP_MODE_t;

/**
 * PRNG data block size
 */
typedef enum XMC_PRNG_DATA_BLOCK_SIZE {
  XMC_PRNG_RDBS_RESET = 0U, /**< Reset state (no random data block size defined) */
  XMC_PRNG_RDBS_BYTE  = 1U, /**< BYTE (8-bit) */
  XMC_PRNG_RDBS_WORD  = 2U  /**< WORD (16-bit) */
} XMC_PRNG_DATA_BLOCK_SIZE_t;

/**
 * PRNG driver initialization status
 */
typedef enum XMC_PRNG_INIT_STATUS {
  XMC_PRNG_NOT_INITIALIZED = 0U, /**< Reset state or Non-initialized state (Same as XMC_PRNG_RDBS_RESET) */
  XMC_PRNG_INITIALIZED     = 1U  /**< Initialized state */
} XMC_PRNG_INIT_STATUS_t;

/*********************************************************************************************************************
 * DATA STRUCTURES
 *********************************************************************************************************************/

/**
 * Key words and data block size configuration values of PRNG <br>
 *
 * The structure presents a convenient way to set/obtain the key word and data block configuration
 * values of PRNG.
 * The XMC_PRNG_Init() can be used to populate the structure with the key word and data block
 * configuration values of the PRNG module.
 */ 
typedef struct XMC_PRNG_INIT
{
  uint16_t key_words[5];                 /**< Keywords */
  XMC_PRNG_DATA_BLOCK_SIZE_t block_size; /**< Block size */
} XMC_PRNG_INIT_t;

/*********************************************************************************************************************
 * API PROTOTYPES
 *********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @param prng Pointer to a constant instance of ::XMC_PRNG_INIT_t, pointing to
 *             the initialization configuration.
 * @return XMC_PRNG_INIT_STATUS_t XMC_PRNG_INITIALIZED if initialized,
 *                                XMC_PRNG_NOT_INITIALIZED otherwise.
 *
 * \par<b>Description: </b><br>
 * Initialize the PRNG peripheral with the configured key words and block size <br>
 *
 * \par
 * The function configures block size for key loading mode, enables key loading mode,
 * loads key words (80 bits) and wait till RDV is set, enables the streaming mode and
 * waits for warmup phase. This function programmes the CTRL and WORD registers. 
 */
XMC_PRNG_INIT_STATUS_t XMC_PRNG_Init(const XMC_PRNG_INIT_t *prng);



/**
 * @param block_size Block size of type ::XMC_PRNG_DATA_BLOCK_SIZE_t for read access
 * @return None
 *
 * \par<b>Description: </b><br>
 * Programming Random Block Size <br>
 *
 * \par
 * The function sets the random data block size as byte or word by programming CTRL.RDBS bitfield.
 * block_size = 0 for Reset state, block_size = 1 for 'byte' and block_size = 2 for 'word'.
 */
__STATIC_INLINE void XMC_PRNG_SetRandomDataBlockSize(XMC_PRNG_DATA_BLOCK_SIZE_t block_size)
{
  PRNG->CTRL = (uint16_t)((PRNG->CTRL & (uint32_t)~PRNG_CTRL_RDBS_Msk) |
                          ((uint32_t)block_size << (uint32_t)PRNG_CTRL_RDBS_Pos));
}

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Checks the validity (CHK.RDV bit) of the generated random data <br>
 *
 * \par
 * The function checks the validity (CHK.RDV bit) of the generated random data.
 * In key loading mode, this value indicates if the next partial key word can be written
 * to PRNG_WORD or not.
 */
__STATIC_INLINE uint16_t XMC_PRNG_CheckValidStatus(void)
{
  return (PRNG->CHK & PRNG_CHK_RDV_Msk);
}

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enables the PRNG key loading mode <br>
 *
 * \par
 * The function initializes the key loading by setting the bit CTRL.KLD. In this mode, Register WORD
 * acts as always as a 16 bit destination register. After the complete key has been loaded, the CTRL.KLD
 * must be set to '0' to prepare the following warmup phase.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_PRNG_EnableStreamingMode()
 */
__STATIC_INLINE void XMC_PRNG_EnableKeyLoadingMode(void)
{
  PRNG->CTRL |= (uint16_t)PRNG_CTRL_KLD_Msk;
}

/**
 * @return None
 *
 * \par<b>Description: </b><br>
 * Enables the Streaming mode <br>
 *
 * \par
 * The function enables the streaming mode and disables the PRNG key loading mode by resetting the
 * CTRL.KLD bit. 
 *
 * \par<b>Related APIs:</b><br>
 * XMC_PRNG_EnableKeyLoadingMode()
 */
__STATIC_INLINE void XMC_PRNG_EnableStreamingMode(void)
{
  PRNG->CTRL &= (uint16_t)~PRNG_CTRL_KLD_Msk;
}

/**
 * @param key Key word to load into PRNG WORD register
 * @return None
 *
 * \par<b>Description: </b><br>
 * Loads a partial key word to the PRNG WORD register <br>
 *
 * \par
 * The function loads partial key word to WORD registr. These partial
 * words are sequentially written and loading a key word will take 16 clock
 * cycles. The CHK.RDV bit is set to '0' while loading is in progress. '1' indicates
 * that the next partial key word can be written to WORD register. 
 */
__STATIC_INLINE void XMC_PRNG_LoadKeyWords(uint16_t key)
{
  PRNG->WORD = key;
}

/**
 * @param None
 * @return uint16_t Generated random number
 *
 * \par<b>Description: </b><br>
 * Gets the generated random number <br>
 *
 * \par
 * The function gives the generated random number by returning the content of WORD
 * register. Before reading the WORD register to get the generated random number it is
 * required to check the bit CHK.RDV is set which indicates that the next random data block
 * can be read from WORD register. After a word has been read the bit CHK.RDV is reset
 * by the hardware and generation of new random bits starts.
 *
 * \par<b>Related APIs:</b><br>
 * XMC_PRNG_CheckValidStatus()
 */
__STATIC_INLINE uint16_t XMC_PRNG_GetPseudoRandomNumber(void)
{
  return PRNG->WORD;
}

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

/**
 * @}
 */

#endif /* #if defined (PRNG) */

#endif /* XMC_PRNG_H */
