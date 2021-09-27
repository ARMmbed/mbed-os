/**
 * @file    i2s.h
 * @brief   I2S (Inter-Integrated Sound) driver function prototypes and data types.
 */

/* ****************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 *************************************************************************** */

#ifndef _I2S_H_
#define _I2S_H_

/* **** Includes **** */
#include "mxc_sys.h"
#include "dma.h"
#include "spimss_regs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup i2s Inter-Integrated Sound (I2S)
 * @ingroup periphlibs
 * @{
 */

/* **** Definitions **** */

typedef enum {
    I2S_MAP_A = 0,
    I2S_MAP_B = 1,
} mxc_i2s_sys_map_t;

typedef enum {
    LEFT_JUSTIFIED= 0,
    RIGHT_JUSTIFIED = 1,
} mxc_i2s_justify_t;

typedef enum {
    STEREO_MODE = 0,
    MONO_MODE = 1,
} mxc_i2s_audio_mode_t;

/** @brief I2S audio directions */
typedef enum {
    AUDIO_OUT = 1,
    AUDIO_IN = 2,
} mxc_i2s_direction_t;


/** @brief I2S Configuration Struct */
typedef struct {
    mxc_i2s_sys_map_t       map;
    mxc_i2s_justify_t       justify;
    mxc_i2s_audio_mode_t    audio_mode;
    mxc_i2s_direction_t     audio_direction;
    uint16_t                sample_rate; 
    unsigned int            start_immediately;
    unsigned int            dma_reload_en;
    void                    *src_addr;
    void                    *dst_addr;
    uint32_t                length;
} mxc_i2s_config_t;
    
/* **** Function Prototypes **** */

/**
 * @brief   Initialize I2S resources 
 * 
 * @param   config        see \ref mxc_i2s_config_t I2S Config Struct
 * @param   dma_ctz_cb    Function pointer to Count-to-Zero callback function.
 * @return  Success/Fail, see \ref MXC_Error_Codes for a list of return codes.
 */
int MXC_I2S_Init(const mxc_i2s_config_t *config, void(*dma_ctz_cb)(int, int));

/**
 * @brief      Release I2S
 * @details    De-configures the I2S protocol and stops DMA request
 * @return   \c #E_BAD_PARAM if DMA cannot be stopped, #E_NO_ERROR otherwise
 */  
int MXC_I2S_Shutdown(void);

/**
 * @brief      Mute I2S Output
 * @details    Sets I2S data to zero, continues sending clock and accessing DMA
 * @return   \c #E_NO_ERROR
 */  
int MXC_I2S_Mute(void);

/**
 * @brief      Unmute I2S Output
 * @details    Restores I2S data
 * @return   \c #E_NO_ERROR
 */  
int MXC_I2S_Unmute(void);

/**
 * @brief      Pause I2S Output
 * @details    Similar to mute, but stops FIFO and DMA access, clocks continue
 * @return   \c #E_NO_ERROR
 */  
int MXC_I2S_Pause(void);

/**
 * @brief      Unpause I2S Output
 * @details    Similar to mute, but restarts FIFO and DMA access
 * @return   \c #E_NO_ERROR
 */  
int MXC_I2S_Unpause(void);

/**
 * @brief      Stops I2S Output
 * @details    Similar to pause, but also halts clock
 * @return   \c #E_NO_ERROR
 */  
int MXC_I2S_Stop(void);

/**
 * @brief      Starts I2S Output
 * @details    Starts I2S Output, automatically called by configure if requested
 * @return   \c #E_NO_ERROR
 */  
int MXC_I2S_Start(void);

/**
 * @brief      Clears DMA Interrupt Flags
 * @details    Clears the DMA Interrupt flags, should be called at the end of a dma_ctz_cb
 * @return   \c #E_NO_ERROR
 */  
int MXC_I2S_DMA_ClearFlags(void);

/**
 * @brief      Set DMA Addr (Source or Dest) and bytes to transfer
 * @param      src_addr The address to read data from (Audio Out)
 * @param      dst_addr The address to write data to (Audio In)    
 * @param      count    The length of the transfer in bytes
 * @details    Sets the address to read/write data in memory and the length of
 *             the transfer. The unused addr parameter is ignored.
 * @return   \c #E_NO_ERROR
 */  
int MXC_I2S_DMA_SetAddrCnt(void *src_addr, void *dst_addr, unsigned int count);

/**
 * @brief      Sets the DMA reload address and count
 * @param      src_addr The address to read data from (Audio Out)
 * @param      dst_addr The address to write data to (Audio In)    
 * @param      count    The length of the transfer in bytes
 * @details    If DMA reload is enabled, when the DMA has transfered $count bytes
 *             (a CTZ event occurs) the src, dst, and count registers will be 
 *             set to these. The DMA reload flag clears after a reload occurs.
 * @return   \c #E_NO_ERROR
 */  
int MXC_I2S_DMA_SetReload(void *src_addr, void *dst_addr, unsigned int count);
/**@} end of group i2s */


#ifdef __cplusplus
}
#endif

#endif /* _I2S_H_ */
