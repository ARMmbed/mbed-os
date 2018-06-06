/*!
 *****************************************************************************
 * @file:    startup_ADuCM4050.c
 * @brief:   Interrupt table and default handlers for ADuCM4x50
 * @version: $Revision: $
 * @date:    $Date: $
 *-----------------------------------------------------------------------------
 *
Copyright (c) 2010-2018 Analog Devices, Inc.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
  - Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  - Modified versions of the software must be conspicuously marked as such.
  - This software is licensed solely and exclusively for use with processors
    manufactured by or for Analog Devices, Inc.
  - This software may not be combined or merged with other code in any manner
    that would cause the software to become subject to terms and conditions
    which differ from those listed here.
  - Neither the name of Analog Devices, Inc. nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.
  - The use of this software may or may not infringe the patent rights of one
    or more patent holders.  This license does not release you from the
    requirement that you obtain separate licenses from these patent holders
    to use this software.

THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
TITLE, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
NO EVENT SHALL ANALOG DEVICES, INC. OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, PUNITIVE OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, DAMAGES ARISING OUT OF CLAIMS OF INTELLECTUAL
PROPERTY RIGHTS INFRINGEMENT; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
#ifdef __ARMCC_VERSION
#include <rt_misc.h>
#endif
#include <cmsis.h>
#include <startup_ADuCM4050.h>
#include <mbed_rtx.h>

/*----------------------------------------------------------------------------
  External function Declaration
 *----------------------------------------------------------------------------*/
extern void SramInit(void);

/*----------------------------------------------------------------------------
  Checksum options
 *----------------------------------------------------------------------------*/

#if defined(__ICCARM__)
__root
#endif
const uint32_t SECTION_PLACE(blank_checksum[],".checksum") =
{
    BLANKX60,BLANKX600
};

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
WEAK_FUNCTION( NMI_Handler                )
WEAK_FUNCTION( HardFault_Handler          )
WEAK_FUNCTION( MemManage_Handler          )
WEAK_FUNCTION( BusFault_Handler           )
WEAK_FUNCTION( UsageFault_Handler         )
WEAK_FUNCTION( SVC_Handler                )
WEAK_FUNCTION( DebugMon_Handler           )
WEAK_FUNCTION( PendSV_Handler             )
WEAK_FUNCTION( SysTick_Handler            )
WEAK_FUNCTION( RTC1_Int_Handler           )
WEAK_FUNCTION( Ext_Int0_Handler           )
WEAK_FUNCTION( Ext_Int1_Handler           )
WEAK_FUNCTION( Ext_Int2_Handler           )
WEAK_FUNCTION( Ext_Int3_Handler           )
WEAK_FUNCTION( WDog_Tmr_Int_Handler       )
WEAK_FUNCTION( Vreg_over_Int_Handler      )
WEAK_FUNCTION( Battery_Voltage_Int_Handler)
WEAK_FUNCTION( RTC0_Int_Handler           )
WEAK_FUNCTION( GPIO_A_Int_Handler         )
WEAK_FUNCTION( GPIO_B_Int_Handler         )
WEAK_FUNCTION( GP_Tmr0_Int_Handler        )
WEAK_FUNCTION( GP_Tmr1_Int_Handler        )
WEAK_FUNCTION( Flash0_Int_Handler         )
WEAK_FUNCTION( UART0_Int_Handler          )
WEAK_FUNCTION( SPI0_Int_Handler           )
WEAK_FUNCTION( SPI2_Int_Handler           )
WEAK_FUNCTION( I2C0_Slave_Int_Handler     )
WEAK_FUNCTION( I2C0_Master_Int_Handler    )
WEAK_FUNCTION( DMA_Err_Int_Handler        )
WEAK_FUNCTION( DMA_SPIH_TX_Int_Handler    )
WEAK_FUNCTION( DMA_SPIH_RX_Int_Handler    )
WEAK_FUNCTION( DMA_SPORT0A_Int_Handler    )
WEAK_FUNCTION( DMA_SPORT0B_Int_Handler    )
WEAK_FUNCTION( DMA_SPI0_TX_Int_Handler    )
WEAK_FUNCTION( DMA_SPI0_RX_Int_Handler    )
WEAK_FUNCTION( DMA_SPI1_TX_Int_Handler    )
WEAK_FUNCTION( DMA_SPI1_RX_Int_Handler    )
WEAK_FUNCTION( DMA_UART0_TX_Int_Handler   )
WEAK_FUNCTION( DMA_UART0_RX_Int_Handler   )
WEAK_FUNCTION( DMA_I2C0_STX_Int_Handler   )
WEAK_FUNCTION( DMA_I2C0_SRX_Int_Handler   )
WEAK_FUNCTION( DMA_I2C0_MX_Int_Handler    )
WEAK_FUNCTION( DMA_AES0_IN_Int_Handler    )
WEAK_FUNCTION( DMA_AES0_OUT_Int_Handler   )
WEAK_FUNCTION( DMA_FLASH0_Int_Handler     )
WEAK_FUNCTION( SPORT0A_Int_Handler        )
WEAK_FUNCTION( SPORT0B_Int_Handler        )
WEAK_FUNCTION( Crypto_Int_Handler         )
WEAK_FUNCTION( DMA_ADC0_Int_Handler       )
WEAK_FUNCTION( GP_Tmr2_Int_Handler        )
WEAK_FUNCTION( Crystal_osc_Int_Handler    )
WEAK_FUNCTION( SPI1_Int_Handler           )
WEAK_FUNCTION( PLL_Int_Handler            )
WEAK_FUNCTION( RNG_Int_Handler            )
WEAK_FUNCTION( Beep_Int_Handler           )
WEAK_FUNCTION( ADC0_Int_Handler           )
WEAK_FUNCTION( DMA_SIP0_Int_Handler       )
WEAK_FUNCTION( DMA_SIP1_Int_Handler       )
WEAK_FUNCTION( DMA_SIP2_Int_Handler       )
WEAK_FUNCTION( DMA_SIP3_Int_Handler       )
WEAK_FUNCTION( DMA_SIP4_Int_Handler       )
WEAK_FUNCTION( DMA_SIP5_Int_Handler       )
WEAK_FUNCTION( DMA_SIP6_Int_Handler       )
WEAK_FUNCTION( DMA_SIP7_Int_Handler       )
WEAK_FUNCTION( UART1_Int_Handler          )
WEAK_FUNCTION( DMA_UART1_TX_Int_Handler   )
WEAK_FUNCTION( DMA_UART1_RX_Int_Handler   )
WEAK_FUNCTION( RGB_Tmr_Int_Handler        )
WEAK_FUNCTION( Root_Clk_Err_Handler       )

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
const pFunc SECTION_PLACE(IVT_NAME[104],VECTOR_SECTION) = {
    (pFunc) INITIAL_SP,    /* Initial Stack Pointer */
    ADUCM4050_VECTORS
};

/*----------------------------------------------------------------------------
* Initialize .bss and .data for GNU
*----------------------------------------------------------------------------*/
#if defined( __GNUC__) && !defined (__ARMCC_VERSION)
void zero_bss(void)
{
    uint32_t *pSrc, *pDest;
    uint32_t *pTable __attribute__((unused));
#ifdef __STARTUP_COPY_MULTIPLE
    /*  Multiple sections scheme.
     *
     *  Between symbol address __copy_table_start__ and __copy_table_end__,
     *  there are array of triplets, each of which specify:
     *    offset 0: LMA of start of a section to copy from
     *    offset 4: VMA of start of a section to copy to
     *    offset 8: size of the section to copy. Must be multiply of 4
     *
     *  All addresses must be aligned to 4 bytes boundary.
     */
    pTable = &__copy_table_start__;

    for (; pTable < &__copy_table_end__; pTable = pTable + 3) {
        pSrc  = (uint32_t*)*(pTable + 0);
        pDest = (uint32_t*)*(pTable + 1);
        for (; pDest < (uint32_t*)(*(pTable + 1) + *(pTable + 2)) ; ) {
            *pDest++ = *pSrc++;
        }
    }
#else
    /*  Single section scheme.
     *
     *  The ranges of copy from/to are specified by following symbols
     *    __etext: LMA of start of the section to copy from. Usually end of text
     *    __data_start__: VMA of start of the section to copy to
     *    __data_end__: VMA of end of the section to copy to
     *
     *  All addresses must be aligned to 4 bytes boundary.
     */
    pSrc  = &__etext;
    pDest = &__data_start__;

    for ( ; pDest < &__data_end__ ; ) {
        *pDest++ = *pSrc++;
    }
#endif /*__STARTUP_COPY_MULTIPLE */

    /*  This part of work usually is done in C library startup code. Otherwise,
     *  define this macro to enable it in this startup.
     *
     *  There are two schemes too. One can clear multiple BSS sections. Another
     *  can only clear one section. The former is more size expensive than the
     *  latter.
     *
     *  Define macro __STARTUP_CLEAR_BSS_MULTIPLE to choose the former.
     *  Otherwise efine macro __STARTUP_CLEAR_BSS to choose the later.
     */
#ifdef __STARTUP_CLEAR_BSS_MULTIPLE
    /*  Multiple sections scheme.
     *
     *  Between symbol address __copy_table_start__ and __copy_table_end__,
     *  there are array of tuples specifying:
     *    offset 0: Start of a BSS section
     *    offset 4: Size of this BSS section. Must be multiply of 4
     */
    pTable = &__zero_table_start__;

    for (; pTable < &__zero_table_end__; pTable = pTable + 2) {
        pDest = (uint32_t*)*(pTable + 0);
        for (; pDest < (uint32_t*)(*(pTable + 0) + *(pTable + 1)) ; ) {
            *pDest++ = 0;
        }
    }
#elif defined (__STARTUP_CLEAR_BSS)
    /*  Single BSS section scheme.
     *
     *  The BSS section is specified by following symbols
     *    __bss_start__: start of the BSS section.
     *    __bss_end__: end of the BSS section.
     *
     *  Both addresses must be aligned to 4 bytes boundary.
     */
    pDest = &__bss_start__;

    for ( ; pDest < &__bss_end__ ; ) {
        *pDest++ = 0ul;
    }
#endif /* __STARTUP_CLEAR_BSS_MULTIPLE || __STARTUP_CLEAR_BSS */
}
#endif

/*----------------------------------------------------------------------------
* Function    : Reset_Handler (-15)
* Description : Reset event handler
*----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
    /* Initialize SRAM configuration. */
    SramInit();

#if defined(__GNUC__) && !defined (__ARMCC_VERSION)
    zero_bss();
#endif

    /* Setup system. */
    SystemInit();

    /* Call remaining startup code and then main. */
    RESET_EXCPT_HNDLR();
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
#if defined(__ARMCC_VERSION) || defined (__GNUC__)
void Default_Handler(void)
{
    while(1);
}
#endif

