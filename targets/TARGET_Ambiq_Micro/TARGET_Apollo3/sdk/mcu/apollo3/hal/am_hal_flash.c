//*****************************************************************************
//
//  am_hal_flash.c
//! @file
//!
//! @brief Functions for performing Flash operations.
//!
//! IMPORTANT: Interrupts are active during execution of all HAL flash
//! functions. If an interrupt occurs during execution of a flash function
//! that programs or erases flash or INFO space, errors will occur if the
//! interrupt service routine (ISR) is located in on-chip flash.
//! If interrupts are expected during execution of a flash function that
//! programs or erases either flash or INFO space:
//! - Interrupts must be disabled via a critical section handler prior to
//!   calling the flash function.
//! - Alternatively, applicable ISRs must be located in non-flash address space
//!   (i.e. SRAM, off-chip ROM, etc.).
//!
//! @addtogroup flash3 Flash
//! @ingroup apollo3hal
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"

//
// Look-up table
//
const g_am_hal_flash_t g_am_hal_flash =
{
         ((int  (*)(uint32_t, uint32_t))                                0x0800004d),    // flash_mass_erase
         ((int  (*)(uint32_t, uint32_t, uint32_t))                      0x08000051),    // flash_page_erase
         ((int  (*)(uint32_t, uint32_t *, uint32_t *, uint32_t))        0x08000055),    // flash_program_main
         ((int  (*)(uint32_t, uint32_t, uint32_t *, uint32_t, uint32_t))0x08000059),    // flash_program_info_area
         ((int  (*)(uint32_t, uint32_t))                                0x0800006d),    // flash_mass_erase_nb
         ((int  (*)(uint32_t, uint32_t, uint32_t))                      0x08000071),    // flash_page_erase_nb
         ((int  (*)( uint32_t, uint32_t))                               0x08000095),    // flash_page_erase2_nb
         ((bool (*)(void))                                              0x0800007d),    // flash_nb_operation_complete
         ((uint32_t (*)(uint32_t *))                                    0x08000075),    // flash_util_read_word
         ((void (*)( uint32_t *, uint32_t))                             0x08000079),    // flash_util_write_word
         ((void (*)(uint32_t ))                                         0x0800009D),    // bootrom_delay_cycles
         ((int  (*)( uint32_t, uint32_t))                               0x08000081),    // flash_info_erase
         ((int  (*)( uint32_t, uint32_t))                               0x08000089),    // flash_info_plus_main_erase
         ((int  (*)(uint32_t))                                          0x08000091),    // flash_info_plus_main_erase_both
         ((int  (*)( uint32_t ))                                        0x08000099),    // flash_recovery
         ((void (*)(void))                                              0x0800005d),    // flash_program_main_from_sram
         ((void (*)(void))                                              0x08000061),    // flash_program_info_area_from_sram
         ((void (*)(void))                                              0x08000065),    // flash_erase_main_pages_from_sram
         ((void (*)(void))                                              0x08000069),    // flash_mass_erase_from_sram
         ((void (*)(void))                                              0x08000085),    // flash_info_erase_from_sram
         ((void (*)(void))                                              0x0800008D),    // flash_info_plus_main_erase_from_sram
         ((void (*)(void))                                              0x080000A1),    // flash_nb_operation_complete_from_sram
         ((void (*)(void))                                              0x080000A5),    // flash_page_erase2_nb_from_sram
         ((void (*)(void))                                              0x080000A9)     // flash_recovery_from_sram
};

const uint32_t ui32SramMaxAddr = (AM_HAL_FLASH_SRAM_LARGEST_VALID_ADDR + 1);
//*****************************************************************************
//
//! @brief This function performs a mass erase on a flash instance.
//!
//! @param ui32ProgramKey - The flash program key.
//! @param ui32FlashInst - The flash instance to erase.
//!
//! This function will erase the desired instance of flash.
//!
//! @note For Apollo3, each flash instance contains a maximum of 512KB.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return 0 for success, non-zero for failure.
//!     Failing return code indicates:
//!     1   ui32ProgramKey is invalid.
//!     2   ui32FlashInst is invalid.
//!     3   Flash controller hardware timeout.
//
//*****************************************************************************
int
am_hal_flash_mass_erase(uint32_t ui32ProgramKey, uint32_t ui32FlashInst)
{
    return g_am_hal_flash.flash_mass_erase(ui32ProgramKey, ui32FlashInst);
} // am_hal_flash_mass_erase()

//*****************************************************************************
//
//! @brief This function performs a page erase on a flash instance.
//!
//! @param ui32ProgramKey - The flash program key.
//! @param ui32FlashInst - The flash instance to reference the page number with.
//! @param ui32PageNum - The flash page relative to the specified instance.
//!
//! This function will erase the desired flash page in the desired instance of
//! flash.
//!
//! @note For Apollo3, each flash page is 8KB (or AM_HAL_FLASH_PAGE_SIZE).
//! Each flash instance contains a maximum of 64 pages (or
//! AM_HAL_FLASH_INSTANCE_PAGES).
//!
//! @note When given an absolute flash address, a couple of helpful macros can
//! be utilized when calling this function.
//! For example:
//!     am_hal_flash_page_erase(AM_HAL_FLASH_PROGRAM_KEY,
//!                             AM_HAL_FLASH_ADDR2INST(ui32Addr),
//!                             AM_HAL_FLASH_ADDR2PAGE(ui32Addr) );
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return 0 for success, non-zero for failure.
//!     Failing return code indicates:
//!     1   ui32ProgramKey is invalid.
//!     2   ui32FlashInst is invalid.
//!     3   ui32PageNum is invalid.
//!     4   Flash controller hardware timeout.
//
//*****************************************************************************
int
am_hal_flash_page_erase(uint32_t ui32ProgramKey, uint32_t ui32FlashInst,
                        uint32_t ui32PageNum)
{
    return g_am_hal_flash.flash_page_erase(ui32ProgramKey,
                                           ui32FlashInst,
                                           ui32PageNum);
} // am_hal_flash_page_erase()

//*****************************************************************************
//
//! @brief This programs up to N words of the Main array on one flash instance.
//!
//! @param ui32ProgramKey - The programming key, AM_HAL_FLASH_PROGRAM_KEY.
//! @param pui32Src - Pointer to word aligned array of data to program into
//! the flash instance.
//! @param pui32Dst - Pointer to the word aligned flash location where
//! programming of the flash instance is to begin.
//! @param ui32NumWords - The number of words to be programmed.
//!
//! This function will program multiple words in main flash.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return 0 for success, non-zero for failure.
//!     Failing return code indicates:
//!     1   ui32ProgramKey is invalid.
//!     2   pui32Dst is invalid.
//!     3   Flash addressing range would be exceeded.  That is, (pui32Dst +
//!         (ui32NumWords * 4)) is greater than the last valid address.
//!     4   pui32Src is invalid.
//!     5   Unused - will never be returned.
//!     6   Flash controller hardware timeout.
//
//*****************************************************************************
int
am_hal_flash_program_main(uint32_t ui32ProgramKey, uint32_t *pui32Src,
                          uint32_t *pui32Dst, uint32_t ui32NumWords)
{
    uint32_t ui32MaxSrcAddr = (uint32_t)pui32Src + (ui32NumWords << 2);

    //
    // Workaround, the last word of SRAM cannot be the source
    // of programming by BootRom, check to see if it is the last
    //
    if ( ui32MaxSrcAddr == ui32SramMaxAddr )
    {
        uint32_t ui32Temp;
        int iRetVal;

        //
        // program the other words using the boot-rom function
        //
        if ( ui32NumWords > 1 )
        {
            iRetVal = g_am_hal_flash.flash_program_main(
                        ui32ProgramKey,
                        pui32Src,
                        pui32Dst,
                        ui32NumWords - 1);

            //
            // return if anything wrong
            //
            if ( iRetVal != 0 )
            {
                return iRetVal;
            }
        }

        //
        // program the last word of the pSrc from a local
        // variable if it is the last word of SRAM
        //
        ui32Temp = *(uint32_t *)(ui32MaxSrcAddr - 4);

        return g_am_hal_flash.flash_program_main(
                        ui32ProgramKey,
                        &ui32Temp,
                        pui32Dst + ui32NumWords - 1,
                        1);
    }

    return g_am_hal_flash.flash_program_main(ui32ProgramKey, pui32Src,
                                             pui32Dst, ui32NumWords);
} // am_hal_flash_program_main()


//*****************************************************************************
//
//! @brief This clears the specified bits in the addressed flash word
//!
//! @param ui32ProgramKey - The programming key, AM_HAL_FLASH_PROGRAM_KEY.
//! @param pui32Addr - Pointer to word aligned flash word to program into
//! @param ui32BitMask - The bits to be cleared
//!
//! This function will clear one of more bits in a word in main flash.
//! This function is mainly used when the same word is to be written multiple times
//! few bits at a time, between erase cycle
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @note We can reprogram a bit in flash to 0 only once. This function takes
//! care of not re-clearing bits if they are already programmed as 0
//!
//! @return 0 for success, non-zero for failure.
//!
//! Note: See am_hal_flash_program_main() for further details on return codes.
//
//*****************************************************************************
int
am_hal_flash_clear_bits(uint32_t ui32ProgramKey, uint32_t *pui32Addr,
                        uint32_t ui32BitMask)
{
    uint32_t ui32Val = ~ui32BitMask;

    //
    // CAUTION: We can reprogram a bit in flash to 0 only once...so make sure
    //          that we do not re-clear bits
    //
    ui32Val |= ~(*pui32Addr);

    return g_am_hal_flash.flash_program_main(ui32ProgramKey, &ui32Val,
                                             pui32Addr, 1);
} // am_hal_flash_clear_bits()

//*****************************************************************************
//
//! @brief This function programs multiple words in the customer INFO space.
//!
//! @param ui32InfoKey - The customer INFO space key.
//! @param ui32InfoInst - The INFO space instance, 0 or 1.
//! @param *pui32Src - Pointer to word aligned array of data to program into
//! the customer INFO space.
//! @param ui32Offset - Word offset into customer INFO space (offset of 0 is
//! the first word, 1 is second word, etc.).
//! @param ui32NumWords - The number of words to be programmed, must not
//! exceed AM_HAL_FLASH_INFO_SIZE/4.
//!
//! This function will program multiple words in the customer INFO space.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return 0 for success, non-zero for failure.
//!     Failing return code indicates:
//!     1   ui32InfoKey is invalid.
//!     2   ui32InfoInst is invalid.
//!     3   ui32Offset is invalid.
//!     4   INFO addressing range would be exceeded.  That is, (ui32Offset +
//!         ui32NumWords) is greater than the last valid address.
//!     5   pui32Src is invalid.
//!     6   pui32Src is invalid.
//!     7   Hardware error.
//!     8   Flash controller hardware timeout.
//
//*****************************************************************************
int
am_hal_flash_program_info(uint32_t ui32InfoKey, uint32_t ui32InfoInst,
                          uint32_t *pui32Src, uint32_t ui32Offset,
                          uint32_t ui32NumWords)
{
    uint32_t ui32MaxSrcAddr = (uint32_t)pui32Src + (ui32NumWords << 2);

    //
    // workround, the last word of SRAM cannot be the source
    // of programming by BootRom, check to see if it is the last
    //
    if ( ui32MaxSrcAddr == ui32SramMaxAddr )
    {
        uint32_t ui32Temp;
        int iRetVal;

        //
        // program the other words using the boot-rom function
        //
        if ( ui32NumWords > 1 )
        {
            iRetVal = g_am_hal_flash.flash_program_info_area(
                        ui32InfoKey,
                        ui32InfoInst,
                        pui32Src,
                        ui32Offset,
                        ui32NumWords - 1);

            //
            // return if anything wrong
            //
            if ( iRetVal != 0 )
            {
                return iRetVal;
            }
        }

        //
        // program the last word of the pSrc from a local
        // variable if it is the last word of SRAM
        //
        ui32Temp = *(uint32_t *)(ui32MaxSrcAddr - 4);
        return g_am_hal_flash.flash_program_info_area(
                        ui32InfoKey,
                        ui32InfoInst,
                        &ui32Temp,
                        ui32Offset + ui32NumWords - 1,
                        1);
    }

    return g_am_hal_flash.flash_program_info_area(ui32InfoKey, ui32InfoInst, pui32Src,
                                                  ui32Offset, ui32NumWords);

} // am_hal_flash_program_info()

//*****************************************************************************
//
//! @brief This function erases an instance of the customer INFO space.
//!
//! @param ui32InfoKey - The customer INFO space programming key
//! (AM_HAL_FLASH_INFO_KEY).
//! @param ui32Inst - The flash instance, either 0 or 1.
//!
//! This function will erase the the customer INFO space of the specified
//! instance.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return 0 for success, non-zero for failure.
//!     Failing return code indicates:
//!     1   ui32InfoKey is invalid.
//!     2   ui32Inst is invalid.
//!     3   Hardware error.
//!     4   Flash controller hardware timeout.
//
//*****************************************************************************
int
am_hal_flash_erase_info(uint32_t ui32InfoKey,
                        uint32_t ui32Inst)
{
    return g_am_hal_flash.flash_info_erase(ui32InfoKey, ui32Inst);
} // am_hal_flash_erase_info()

//*****************************************************************************
//
//! @brief This function erases the main instance + the customer INFO space.
//!
//! @param ui32InfoKey - The customer INFO space key.
//! @param ui32Inst      - The flash instance, either 0 or 1.
//!
//! This function will erase the main flash + the customer INFO space of the
//! specified instance.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return 0 for success, non-zero for failure.
//!     Failing return code indicates:
//!     1   ui32InfoKey is invalid.
//!     2   ui32Inst is invalid.
//!     3   Hardware error.
//!     4   Flash controller hardware timeout.
//!     11  Internal error.
//!     12  Internal error.
//!     13  Flash controller hardware timeout.
//
//*****************************************************************************
int
am_hal_flash_erase_main_plus_info(uint32_t ui32InfoKey,
                                  uint32_t ui32Inst)
{
    return g_am_hal_flash.flash_info_plus_main_erase(ui32InfoKey,
                                                     ui32Inst);
} // am_hal_flash_erase_main_plus_info()

//*****************************************************************************
//
//! @brief This function erases the main flash + the customer INFO space.
//!
//! @param ui32InfoKey - The customer INFO space key.
//!
//! This function will erase both instances the main flash + the
//! customer INFO space.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return 0 for success, non-zero for failure.
//!     Failing return code indicates:
//!     1   ui32InfoKey is invalid, instance 0.
//!     2   Internal error, instance 0.
//!     3   Hardware error, instance 0.
//!     4   Flash controller hardware timeout, instance 0.
//!    11   Internal error.
//!    12   Internal error.
//!    13   Flash controller hardware timeout.
//!    21   ui32InfoKey is invalid, instance 1.
//!    22   Internal error, instance 1.
//!    23   Hardware error, instance 1.
//!    24   Flash controller hardware timeout, instance 1.
//!    31   Internal error, instance 1.
//!    32   Internal error, instance 1.
//!    33   Flash controller hardware timeout, instance 1.
//
//*****************************************************************************
int
am_hal_flash_erase_main_plus_info_both_instances(uint32_t ui32InfoKey)
{
    return g_am_hal_flash.flash_info_plus_main_erase_both(ui32InfoKey);
} // am_hal_flash_erase_main_plus_info_both_instances()

//*****************************************************************************
//
//! @brief This function erases both main flash instances + both customer INFO
//! space instances.
//!
//! @param ui32RecoveryKey - The recovery key.
//!
//! This function erases both main instances and both customer INFOinstances
//! even if the customer INFO space is programmed to not be erasable. This
//! function completely erases the flash main and info instances and wipes the
//! SRAM. Upon completion of the erasure operations, it does a POI (power on
//! initialization) reset.
//!
//! @note The customer key lock is enforced by this function.  Therefore, the
//! customer key must be written prior to calling otherwise, the function will
//! fail.  Therefore, always check for a return code.  If the function returns,
//! a failure has occured.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return Does not return if successful.  Returns failure code otherwise.
//!     Failing return code indicates:
//!     0x00000001  ui32RecoveryKey is invalid.
//!     0x00000002  Customer key lock not set.
//!     0x00001001  Internal error.
//!     0x00001002  Internal error.
//!     0x00001003  Info erase, instance 0 - hardware error.
//!     0x00001004  Info erase, instance 0 - flash controller hardware timeout.
//!     0xi000ppee  Error erasing page in instance, pp=page number, ee=error code.
//!                 i=2|3, instance 0.
//!                 i=4|5, instance 1.
//!                 ee=1|2|3    Internal or hardware error.
//!                 ee=4        Flash controller hardware timeout.
//
//*****************************************************************************
void
am_hal_flash_recovery(uint32_t ui32RecoveryKey)
{
    g_am_hal_flash.flash_recovery(ui32RecoveryKey);
} // am_hal_flash_recovery()

//*****************************************************************************
//
//! @brief Use the bootrom to implement a spin loop.
//!
//! @param ui32Iterations - Number of iterations to delay.
//!
//! Use this function to implement a CPU busy waiting spin loop without cache
//! or delay uncertainties.
//!
//! Notes for Apollo3:
//! - The ROM-based function executes at 3 cycles per iteration plus the normal
//!   function call, entry, and exit overhead and latencies.
//! - Cache settings affect call overhead.  However, the cache does not affect
//!   the time while inside the BOOTROM function.
//! - The function accounts for burst vs normal mode, along with some of the
//!   overhead encountered with executing the function itself (such as the
//!   check for burst mode).
//! - Use of the FLASH_CYCLES_US() or FLASH_CYCLES_US_NOCACHE() macros for the
//!   ui32Iterations parameter will result in approximate microsecond timing.
//! - The parameter ui32Iterations==0 is allowed but is still incurs a delay.
//!
//! Example:
//! - MCU operating at 48MHz -> 20.83 ns / cycle
//! - Therefore each iteration (once inside the bootrom function) will consume
//!   62.5ns (non-burst-mode).
//!
//! @note Interrupts are not disabled during execution of this function.
//!       Therefore, any interrupt taken will affect the delay timing.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_flash_delay(uint32_t ui32Iterations)
{
    //
    // The read of the FREQCTRL register in order to check for burst mode
    // could take up to 13 cycles, and almost double if in burst mode.
    // There are also overhead delays encountered in this function, such
    // as computing the cycle count adjustment itself.
    // Let's account for these delays as much as possible.
    //
    register uint32_t ui32CycleCntAdj;

    if ( am_hal_burst_mode_status() == AM_HAL_BURST_MODE )
    {
        ui32Iterations <<= 1;

        //
        // There's an additional shift to account for.
        //
        ui32CycleCntAdj = ((13 * 2) + 16) / 3;
    }
    else
    {
        ui32CycleCntAdj = ((13 * 1) + 20) / 3;
    }

    //
    // Allow for the overhead of the burst-mode check and these comparisons
    // by eliminating an appropriate number of iterations.
    //
    if ( ui32Iterations > ui32CycleCntAdj )
    {
        ui32Iterations -= ui32CycleCntAdj;

        g_am_hal_flash.bootrom_delay_cycles(ui32Iterations);
    }

} // am_hal_flash_delay()

//*****************************************************************************
//
//! @brief Delays for a desired amount of cycles while also waiting for a
//! status to change a value.
//!
//! @param ui32usMaxDelay - Maximum number of ~1uS delay loops.
//! @param ui32Address    - Address of the register for the status change.
//! @param ui32Mask       - Mask for the status change.
//! @param ui32Value      - Target value for the status change.
//!
//! This function will delay for approximately the given number of microseconds
//! while checking for a status change, exiting when either the given time has
//! expired or the status change is detected.
//!
//! @returns 0 = timeout.
//!          1 = status change detected.
//
//*****************************************************************************
uint32_t
am_hal_flash_delay_status_change(uint32_t ui32usMaxDelay, uint32_t ui32Address,
                                 uint32_t ui32Mask, uint32_t ui32Value)
{
    while ( 1 )
    {
        //
        // Check the status
        //
        if ( ( AM_REGVAL(ui32Address) & ui32Mask ) == ui32Value )
        {
            return AM_HAL_STATUS_SUCCESS;
        }

        if ( ui32usMaxDelay-- )
        {
            //
            // Call the BOOTROM cycle function to delay for about 1 microsecond.
            //
            am_hal_flash_delay( FLASH_CYCLES_US(1) );
        }
        else
        {
            break;
        }
    }

    return AM_HAL_STATUS_TIMEOUT;

} // am_hal_flash_delay_status_change()

//*****************************************************************************
//
//! @brief Delays for a desired amount of cycles while also waiting for a
//! status to equal OR not-equal to a value.
//!
//! @param ui32usMaxDelay - Maximum number of ~1uS delay loops.
//! @param ui32Address    - Address of the register for the status change.
//! @param ui32Mask       - Mask for the status change.
//! @param ui32Value      - Target value for the status change.
//! @param bIsEqual       - Check for equal if true; not-equal if false.
//!
//! This function will delay for approximately the given number of microseconds
//! while checking for a status change, exiting when either the given time has
//! expired or the status change is detected.
//!
//! @returns 0 = timeout.
//!          1 = status change detected.
//
//*****************************************************************************
uint32_t
am_hal_flash_delay_status_check(uint32_t ui32usMaxDelay, uint32_t ui32Address,
                                uint32_t ui32Mask, uint32_t ui32Value,
                                bool bIsEqual)
{
    while ( 1 )
    {
        //
        // Check the status
        //
        if ( bIsEqual )
        {
            if ( ( AM_REGVAL(ui32Address) & ui32Mask ) == ui32Value )
            {
                return AM_HAL_STATUS_SUCCESS;
            }
        }
        else
        {
            if ( ( AM_REGVAL(ui32Address) & ui32Mask ) != ui32Value )
            {
                return AM_HAL_STATUS_SUCCESS;
            }
        }

        if ( ui32usMaxDelay-- )
        {
            //
            // Call the BOOTROM cycle function to delay for about 1 microsecond.
            //
            am_hal_flash_delay( FLASH_CYCLES_US(1) );
        }
        else
        {
            break;
        }
    }

    return AM_HAL_STATUS_TIMEOUT;

} // am_hal_flash_delay_status_check()

//*****************************************************************************
//
//! @brief Static Helper Function to check customer info valid bits erasure.
//!
//! Use this function to test the state of the 128 valid bits at the beginning
//! of customer info space. If these are all erased then return true.
//!
//! @return true if the customer info bits are currently erased.
//
//*****************************************************************************
static bool
customer_info_signature_erased(void)
{
    uint32_t *pui32Signature = (uint32_t *) AM_HAL_FLASH_INFO_ADDR;

    return ( (pui32Signature[3] == 0xFFFFFFFF)  &&
             (pui32Signature[2] == 0xFFFFFFFF)  &&
             (pui32Signature[1] == 0xFFFFFFFF)  &&
             (pui32Signature[0] == 0xFFFFFFFF) ) ? true : false;

} // customer_info_signature_erased()

//*****************************************************************************
//
//! @brief Static Helper Function to set customer info valid bits
//!
//! Use this function to set the state of the 128 valid bits at the beginning
//! of customer info space. If these bits are not set correctly then the
//! customer protection bits in the INFO space will not be honored by the
//! hardware.
//!
//! @return Zero for success. Non-Zero for errors.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
static int
customer_info_signature_set(uint32_t ui32InfoKey)
{
    uint32_t ui32Valid[4];
    int iRC;

    //
    // If they are already set then we are done.
    //
    if ( am_hal_flash_customer_info_signature_check() )
    {
        return 0;
    }

    //
    // If they are not erased at this point we have an error.
    //
    if ( !customer_info_signature_erased() )
    {
        return (2 << 16);
    }

    //
    // OK they need to be set so do it.
    //
    ui32Valid[3] = AM_HAL_FLASH_INFO_SIGNATURE3;
    ui32Valid[2] = AM_HAL_FLASH_INFO_SIGNATURE2;
    ui32Valid[1] = AM_HAL_FLASH_INFO_SIGNATURE1;
    ui32Valid[0] = AM_HAL_FLASH_INFO_SIGNATURE0;

    iRC = g_am_hal_flash.flash_program_info_area(ui32InfoKey,
                                            0,         // instance
                                            ui32Valid, // source data
                                            0,         // offset
                                            4);        // number of words

    //
    // See am_hal_flash_program_info() for further details on return codes.
    //
    return iRC | ((iRC) ? (1 << 16) : 0);

} // customer_info_signature_set()

//*****************************************************************************
//
//! @brief Check that the customer info bits are valid.
//!
//! Use this function to test the state of the 128 valid bits at the beginning
//! of customer info space. If these are not set correctly then the customer
//! protection bits in the INFO space will not be honored by the hardware.
//!
//! @return true if valid.
//
//*****************************************************************************
bool
am_hal_flash_customer_info_signature_check(void)
{
    uint32_t *pui32Signature = (uint32_t *)AM_HAL_FLASH_INFO_ADDR;

    return ( (pui32Signature[3] == AM_HAL_FLASH_INFO_SIGNATURE3)    &&
             (pui32Signature[2] == AM_HAL_FLASH_INFO_SIGNATURE2)    &&
             (pui32Signature[1] == AM_HAL_FLASH_INFO_SIGNATURE1)    &&
             (pui32Signature[0] == AM_HAL_FLASH_INFO_SIGNATURE0) );

} // am_hal_flash_customer_info_signature_check()

//*****************************************************************************
//
//! @brief INFO signature set.
//!
//! @param ui32InfoKey - The customer INFO space programming key
//!
//! Use this function to set the state of the 128 valid bits at the beginning
//! of customer info space, if needed.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return Zero for success. Non-Zero for errors.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
bool
am_hal_flash_info_signature_set(uint32_t ui32InfoKey)
{
    //
    // Check and set signature.
    //
    return customer_info_signature_set(ui32InfoKey) ? false : true;

} // am_hal_flash_info_signature_set()

//*****************************************************************************
//
//! @brief Disable FLASH INFO space.
//!
//! @param ui32InfoKey - The customer INFO space programming key
//!
//! Use this function to set the state of the 128 valid bits at the beginning
//! of customer info space, if needed. Then disable FLASH erasure.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return Zero for success. Non-Zero for errors.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
int32_t
am_hal_flash_info_erase_disable(uint32_t ui32InfoKey)
{
    int iRC;
    uint32_t ui32SecurityValue;

    //
    // Security protection only works if the signature data is correct.
    //
    iRC = customer_info_signature_set(ui32InfoKey);
    if ( iRC )
    {
        return iRC;
    }

    //
    // Clear bit in INFO space to disable erasure.
    //
    ui32SecurityValue = AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR)  &
                        ~AM_HAL_FLASH_INFO_SECURITY_ENINFOERASE_M;

    //
    // Now write the word to the flash INFO space.
    //
    return g_am_hal_flash.flash_program_info_area(
            ui32InfoKey,
            0,                                  // instance
            &ui32SecurityValue,                 // source data
            AM_HAL_FLASH_INFO_SECURITY_O / 4,   // word offset
            1 );                                // number of words

} // am_hal_flash_info_erase_disable()

//*****************************************************************************
//
//! @brief Check for Disabled FLASH INFO space.
//!
//! Use this function to determine whether FLASH INFO erasure is disabled.
//!
//! @return true if FLASH INFO erase is disabled, otherwise false.
//
//*****************************************************************************
bool
am_hal_flash_info_erase_disable_check(void)
{
    //
    // If they are erased at this point then SRAM wipe can't be enabled.
    //
    if ( customer_info_signature_erased() )
    {
        return false;
    }

    //
    // If they are not valid at this point then SRAM wipe can't be enabled.
    //
    if ( !am_hal_flash_customer_info_signature_check() )
    {
        return false;
    }

    //
    // Looking good so far, now check the SRAM WIPE bit.
    //
    return AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR)   &
                     AM_HAL_FLASH_INFO_SECURITY_ENINFOERASE_M  ? false : true;

} // am_hal_flash_info_erase_disable_check()

//*****************************************************************************
//
//! @brief Mask off 1 to 4 quadrants of FLASH INFO space for programming.
//!
//! Use this function to set the state of the 128 valid bits at the beginning
//! of customer info space, if needed. Then and the mask bits with the INFO
//! space programming disable bits.
//!
//! @param ui32InfoKey - The customer INFO space programming key
//!
//! @param ui32Mask - A mask of the 4 quadrants of info space where
//!                   bit0 = First quadrant (first 2KB).
//!                   bit1 = Second quadrant (second 2KB).
//!                   bit2 = Third quadrant (third 2KB).
//!                   bit3 = Fourth quadrant (fourth 2KB).
//!
//! @note This function disables only, any quadrant already disabled is not
//! reenabled.  That is, any ui32Mask bits specified as 0 are essentially nops.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return Zero for success. Non-Zero for errors.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
int32_t
am_hal_flash_info_program_disable(uint32_t ui32InfoKey, uint32_t ui32Mask)
{
    int iRC;
    uint32_t ui32SecurityValue;

    //
    // Security protection only works if the signature data is correct.
    //
    iRC = customer_info_signature_set(ui32InfoKey);
    if ( iRC )
    {
        return iRC;
    }

    //
    // Make sure we have a valid mask and get the mask into the correct position.
    //
    ui32Mask <<= AM_HAL_FLASH_INFO_SECURITY_ENINFOPRGM_S;
    ui32Mask &= AM_HAL_FLASH_INFO_SECURITY_ENINFOPRGM_M;

    //
    // The security bit set to 1 enables programming, 0 disables programming.
    //
    ui32SecurityValue = AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) & ~ui32Mask;

    //
    // Now write the word to the flash INFO space.
    //
    return g_am_hal_flash.flash_program_info_area(
            ui32InfoKey,
            0,                                  // instance
            &ui32SecurityValue,                 // source data
            AM_HAL_FLASH_INFO_SECURITY_O / 4,   // word offset
            1 );                                // number of words

} // am_hal_flash_info_program_disable()

//*****************************************************************************
//
//! @brief Return a mask specifying which quadrants of customer INFO space have
//! been disabled for programming.
//!
//! Use this function to determine whether programming of customer INFO space
//! has been disabled.
//!
//! @return A 4-bit mask of the disabled quadrants.
//! 0xFFFFFFFF indicates an error.
//! 0x0  indicates all customer INFO space programming is enabled.
//! 0xF  indicates all customer INFO space programming is disabled.
//! bit0 indicates the first customer INFO space is disabled for programming.
//! bit1 indicates the second customer INFO space is disabled for programming.
//! bit2 indicates the third customer INFO space is disabled for programming.
//! bit3 indicates the fourth customer INFO space is disabled for programming.
//
//*****************************************************************************
uint32_t
am_hal_flash_info_program_disable_get(void)
{
    //
    // If they are erased at this point then SRAM wipe can't be enabled.
    //
    if ( customer_info_signature_erased() )
    {
        return 0xFFFFFFFF;
    }

    //
    // If not valid at this point, then INFO programming can't be enabled.
    //
    if ( !am_hal_flash_customer_info_signature_check() )
    {
        return 0xFFFFFFFF;
    }

    //
    // Looking good so far, now return a mask of the disabled bits.
    //
    return  ((AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) &
                AM_HAL_FLASH_INFO_SECURITY_ENINFOPRGM_M) ^
                AM_HAL_FLASH_INFO_SECURITY_ENINFOPRGM_M) >>
                AM_HAL_FLASH_INFO_SECURITY_ENINFOPRGM_S;

} // am_hal_flash_info_program_disable_get()

//*****************************************************************************
//
//! @brief Enable FLASH debugger protection (FLASH gets wiped if a debugger is
//! connected).
//!
//! @param ui32InfoKey - The customer INFO space programming key
//!
//! Use this function to set the state of the 128 valid bits at the beginning
//! of customer info space, if needed. Then set the FLASH wipe bit to zero.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return Zero for success. Non-Zero for errors.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
int32_t
am_hal_flash_wipe_flash_enable(uint32_t ui32InfoKey)
{
    int iRC;
    uint32_t ui32SecurityValue;

    //
    // Security protection only works if the signature data is correct.
    //
    iRC = customer_info_signature_set(ui32InfoKey);
    if ( iRC )
    {
        return iRC;
    }

    //
    // Clear the FLASH Wipe bit.
    //
    ui32SecurityValue = AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) &
                        ~AM_HAL_FLASH_INFO_SECURITY_FLASHWIPE_M;

    //
    // Now write the word to the flash INFO space.
    //
    return g_am_hal_flash.flash_program_info_area(
            ui32InfoKey,
            0,                                  // instance
            &ui32SecurityValue,                 // source data
            AM_HAL_FLASH_INFO_SECURITY_O / 4,   // word offset
            1 );                                // number of words

} // am_hal_flash_wipe_flash_enable()

//*****************************************************************************
//
//! @brief check for FLASH wipe protection enabled.
//!
//! Use this function to determine if FLASH wipe protection is enabled.
//!
//! @return true if FLASH wipe protection is enabled, otherwise false.
//
//*****************************************************************************
bool
am_hal_flash_wipe_flash_enable_check(void)
{
    //
    // If they are erased at this point then flash wipe can't be enabled.
    //
    if ( customer_info_signature_erased() )
    {
        return false;
    }

    //
    // If they are not valid at this point then flash wipe can't be enabled.
    //
    if ( !am_hal_flash_customer_info_signature_check() )
    {
        return false;
    }

    //
    // Looking good so far, now check the Flash WIPE bit.
    //
    return AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) &
                     AM_HAL_FLASH_INFO_SECURITY_FLASHWIPE_M ? false : true;

} // am_hal_flash_wipe_flash_enable_check()

//*****************************************************************************
//
//! @brief Enable SRAM protection so SRAM gets wiped if a debgger is connected.
//!
//! @param ui32InfoKey - The customer INFO space programming key
//!
//! Use this function to set the state of the 128 valid bits at the beginning
//! of customer info space, if needed. Then set the SRAM wipe bit to zero.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return Zero for success. Non-Zero for errors.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
int32_t
am_hal_flash_wipe_sram_enable(uint32_t ui32InfoKey)
{
    int iRC;
    uint32_t ui32SecurityValue;

    //
    // Security protection only works if the signature data is correct.
    //
    iRC = customer_info_signature_set(ui32InfoKey);
    if ( iRC )
    {
        return iRC;
    }

    //
    // Clear the SRAM Wipe bit.
    //
    ui32SecurityValue = AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) &
                        ~AM_HAL_FLASH_INFO_SECURITY_SRAMWIPE_M;

    //
    // Now write the word to the flash INFO space.
    //
    return g_am_hal_flash.flash_program_info_area(
            ui32InfoKey,
            0,                                  // instance
            &ui32SecurityValue,                 // source data
            AM_HAL_FLASH_INFO_SECURITY_O / 4,   // word offset
            1 );                                // number of words

} // am_hal_flash_wipe_sram_enable()

//*****************************************************************************
//
//! @brief check for SRAM protection enabled.
//!
//! Use this function to determine if SRAM protection is enabled.
//!
//! @return true if SRAM wipe protection is enabled, otherwise false.
//
//*****************************************************************************
bool
am_hal_flash_wipe_sram_enable_check(void)
{
    //
    // If they are erased at this point then SRAM wipe can't be enabled.
    //
    if ( customer_info_signature_erased() )
    {
        return false;
    }

    //
    // If they are not vale at this point then SRAM wipe can't be enabled.
    //
    if ( !am_hal_flash_customer_info_signature_check() )
    {
        return false;
    }

    //
    // Looking good so far, now check the SRAM WIPE bit.
    //
    return AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) &
                     AM_HAL_FLASH_INFO_SECURITY_SRAMWIPE_M ? false : true;

} // am_hal_flash_wipe_sram_enable_check()

//*****************************************************************************
//
//! @brief Disable Output from ITM/SWO.
//!
//! @param ui32InfoKey - The customer INFO space programming key
//!
//! Use this function to set the state of the 128 valid bits at the beginning
//! of customer info space, if needed. Set the SWO disable bit to zero.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return Zero for success. Non-Zero for errors.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
int32_t
am_hal_flash_swo_disable(uint32_t ui32InfoKey)
{
    int iRC;
    uint32_t ui32SecurityValue;

    //
    // Security protection only works if the signature data is correct.
    //
    iRC = customer_info_signature_set(ui32InfoKey);
    if ( iRC )
    {
        return iRC;
    }

    //
    // Clear the SWO bit.
    //
    ui32SecurityValue = AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) &
                        ~AM_HAL_FLASH_INFO_SECURITY_SWOCTRL_M;

    //
    // Now write the word to the flash INFO space.
    //
    return g_am_hal_flash.flash_program_info_area(
            ui32InfoKey,
            0,                                  // instance
            &ui32SecurityValue,                 // source data
            AM_HAL_FLASH_INFO_SECURITY_O / 4,   // word offset
            1 );                                // number of words

} // am_hal_flash_swo_disable()

//*****************************************************************************
//
//! @brief check for SWO disabled.
//!
//! Use this function to determine if the SWO is disabled.
//!
//! @return true if the ITM/SWO is disabled, otherwise false.
//
//*****************************************************************************
bool
am_hal_flash_swo_disable_check(void)
{
    //
    // If they are erased at this point then SRAM wipe can't be enabled.
    //
    if ( customer_info_signature_erased() )
    {
        return false;
    }

    //
    // If they are not vale at this point then SRAM wipe can't be enabled.
    //
    if ( !am_hal_flash_customer_info_signature_check() )
    {
        return false;
    }

    //
    // Looking good so far, now check the SWO bit.
    //
    return AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) &
            AM_HAL_FLASH_INFO_SECURITY_SWOCTRL_M ? false : true;

} // am_hal_flash_swo_disable_check()

//*****************************************************************************
//
//! @brief Disable Connections from a debugger on the SWD interface.
//!
//! @param ui32InfoKey - The customer INFO space programming key
//!
//! Use this function to set the state of the 128 valid bits at the beginning
//! of customer info space, if needed. Set the debugger disable bit to zero.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return Zero for success. Non-Zero for errors.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
int32_t
am_hal_flash_debugger_disable(uint32_t ui32InfoKey)
{
    int iRC;
    uint32_t ui32SecurityValue;

    //
    // Security protection only works if the signature data is correct.
    //
    iRC = customer_info_signature_set(ui32InfoKey);
    if ( iRC )
    {
        return iRC;
    }

    //
    // Clear the DEBUGGER bit.
    //
    ui32SecurityValue = AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) &
                        ~AM_HAL_FLASH_INFO_SECURITY_DEBUGGERPROT_M;

    //
    // Now write the word to the flash INFO space.
    //
    return g_am_hal_flash.flash_program_info_area(
            ui32InfoKey,
            0,                                  // instance
            &ui32SecurityValue,                 // source data
            AM_HAL_FLASH_INFO_SECURITY_O / 4,   // word offset
            1 );                                // number of words

} // am_hal_flash_debugger_disable()

//*****************************************************************************
//
//! @brief check for debugger disabled.
//!
//! Use this function to determine if the debugger is disabled.
//!
//! @return true if the debugger is disabled, otherwise false.
//
//*****************************************************************************
bool
am_hal_flash_debugger_disable_check(void)
{
    //
    // If they are erased at this point then SRAM wipe can't be enabled.
    //
    if ( customer_info_signature_erased() )
    {
        return false;
    }

    //
    // If they are not vale at this point then SRAM wipe can't be enabled.
    //
    if ( !am_hal_flash_customer_info_signature_check() )
    {
        return false;
    }

    //
    // Looking good so far, now check the debugger disable bit.
    //
    return AM_REGVAL(AM_HAL_FLASH_INFO_SECURITY_ADDR) &
                     AM_HAL_FLASH_INFO_SECURITY_DEBUGGERPROT_M ? false : true;

} // am_hal_flash_debugger_disable_check()

//*****************************************************************************
//
//! @brief This static helper function generates a 64-bit protection mask.
//!
//! @param pui32StartAddress - Starting address in flash to begin protection.
//! @param pui32StopAddress  - Ending address in flash to stop protection.
//!
//! This function computes a chunk map for the protection range.
//!
//! @return Inverse of the actual chunk mask.  That is, chunks to be protected
//! are represented as 0 in the returned mask, while chunks to be left alone
//! are represented as 1.  This value can therefore be directly ANDed with the
//! existing bits in INFO space.
//! Note that -1 is returned if input parameters are invalid - this return
//! value would indicate that no chunks are to be protected.
//!
//
//*****************************************************************************
static uint64_t
generate_chunk_mask(uint32_t *pui32StartAddress, uint32_t *pui32StopAddress)
{
    uint32_t ui32ChunkStart, ui32ChunkStop;
    uint32_t ui32Width;
    uint64_t ui64Mask;

    //
    // Validate the address input parameters
    //
    if ( (pui32StartAddress > pui32StopAddress)  ||
         (pui32StopAddress > (uint32_t*)AM_HAL_FLASH_LARGEST_VALID_ADDR) )
    {
        //
        // Argument error, return value to leave all chunks unprotected.
        //
        return 0xFFFFFFFFFFFFFFFF;
    }

    //
    // Extract chunk related information
    //
    ui32ChunkStart = AM_HAL_FLASH_INFO_ADDR2CHUNK((uint32_t)pui32StartAddress);
    ui32ChunkStop  = AM_HAL_FLASH_INFO_ADDR2CHUNK((uint32_t)pui32StopAddress);
    ui32Width = ui32ChunkStop - ui32ChunkStart + 1;

    if ( ui32Width == 64 )
    {
        ui64Mask = (uint64_t)0xFFFFFFFFFFFFFFFFLLU;
    }
    else
    {
        ui64Mask = ( ((uint64_t)0x0000000000000001) << ui32Width) - 1;
        ui64Mask <<= ui32ChunkStart;
    }

    //
    // OK now return the chunk mask (inverted).
    //
    return ~ui64Mask;

} // generate_chunk_mask()

//*****************************************************************************
//
//! @brief This function sets copy protection for a range of flash chunks.
//!
//! @param ui32InfoKey - The customer INFO space programming key
//! @param pui32StartAddress - Starting address in flash to begin protection.
//! @param pui32StopAddress - Ending address in flash to stop protection.
//!
//! This function will set copy protection bits for a range of flash chunks
//!
//! @note Each flash chunk contains 16KBytes and corresponds to one bit in
//! the protection register. Set the bit to zero to enable protection.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return
//!     0 for success.
//!     0x400000 if the protection bits were already programmed (mask the return
//!              value with 0x3FFFFF to ignore this case and treat as success).
//!     Otherwise, non-zero for failure.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
int32_t
am_hal_flash_copy_protect_set(uint32_t ui32InfoKey,
                              uint32_t *pui32StartAddress,
                              uint32_t *pui32StopAddress)
{
    int iRC;
    bool bModified = false;
    uint64_t ui64Mask;
    uint32_t ui32Work;
    uint32_t ui32Protection[2];
    uint32_t *pui32Protection = (uint32_t *)AM_HAL_FLASH_INFO_COPYPROT_ADDR;

    //
    // Extract chunk mask from parameters.
    // Also checks parameter validity (returns -1 if bad parameters).
    //
    ui64Mask = generate_chunk_mask(pui32StartAddress, pui32StopAddress);
    if ( ~ui64Mask == 0x0 )
    {
        return 0x100000;
    }

    //
    // Go get the current settings for copy protection.
    //
    ui32Protection[0] = pui32Protection[0];
    ui32Protection[1] = pui32Protection[1];

    //
    // AND mask off the necessary protection bits in the lower word.
    //
    ui32Work = (uint32_t)ui64Mask;
    if ( ( ~ui32Work )  &&  ( ui32Work != ui32Protection[0] ) )
    {
        bModified = true;
        // Need to change only the bits changing - bits already set to 0 should not be rewritten to 0
        // Flash has limits on number of times a bit can be set to 0
        ui32Protection[0] = ui32Work | ~ui32Protection[0];
        iRC = g_am_hal_flash.flash_program_info_area(
                ui32InfoKey,
                0,                                      // instance
                &ui32Protection[0],                     // source data
                (AM_HAL_FLASH_INFO_COPYPROT_O / 4) + 0, // word offset
                1 );                                    // number of words

        if ( iRC )
        {
            return iRC | 0x10000;
        }
    }

    //
    // AND mask off the necessary protection bits in the upper word.
    //
    ui32Work = (uint32_t)(ui64Mask >> 32);
    if ( ( ~ui32Work )  &&  ( ui32Work != ui32Protection[1] ) )
    {
        bModified = true;
        // Need to change only the bits changing - bits already set to 0 should not be rewritten to 0
        // Flash has limits on number of times a bit can be set to 0
        ui32Protection[1] = ui32Work | ~ui32Protection[1];
        iRC = g_am_hal_flash.flash_program_info_area(
                ui32InfoKey,
                0,                                      // instance
                &ui32Protection[1],                     // source data
                (AM_HAL_FLASH_INFO_COPYPROT_O / 4) + 1, // word offset
                1 );                                    // number of words

        if ( iRC )
        {
            return iRC | 0x20000;
        }
    }

    if ( bModified )
    {
        return 0;
    }
    else
    {
        return 0x400000;
    }

} // am_hal_flash_copy_protect_set()

//*****************************************************************************
//
//! @brief This function checks copy protection for a range of flash chunks.
//!
//! @param pui32StartAddress - Starting address in flash.
//! @param pui32StopAddress - Ending address in flash.
//!
//! This function will check copy protection bits for a range of flash chunks
//! it expects all chunks in the range to be protected.
//!
//! @note Each flash chunk contains 16KBytes and corresponds to one bit in
//! the protection register. Set the bit to zero to enable protection.
//!
//! @return false for at least one chunk in the covered range is not protected,
//!         and true if all chunks in the covered range are protected.
//!
//
//*****************************************************************************
bool
am_hal_flash_copy_protect_check(uint32_t *pui32StartAddress,
                                uint32_t *pui32StopAddress)
{
    uint64_t ui64Mask;
    uint32_t ui32Work;
    uint32_t *pui32Protection = (uint32_t *)AM_HAL_FLASH_INFO_COPYPROT_ADDR;

    //
    // Extract chunk mask from parameters.
    // Also checks parameter validity (returns -1 if bad parameters).
    //
    ui64Mask = generate_chunk_mask(pui32StartAddress, pui32StopAddress);
    if ( ~ui64Mask == 0x0 )
    {
        return false;
    }

    //
    // Now check the lower word of protection bits.
    //
    ui32Work = (uint32_t)ui64Mask;
    if ( ~ui32Work  &  pui32Protection[0] )
    {
        return false;
    }

    //
    // Now check the lower word of protection bits.
    //
    ui32Work = (uint32_t)(ui64Mask >> 32);
    if ( ~ui32Work & pui32Protection[1] )
    {
        return false;
    }

    //
    // If we get here, there are no unprotected chunks within specified range.
    //
    return true;

} // am_hal_flash_copy_protect_check()

//*****************************************************************************
//
//! @brief This function sets write protection for a range of flash chunks.
//!
//! @param ui32InfoKey - The customer INFO space programming key
//! @param pui32StartAddress - Starting address in flash to begin protection.
//! @param pui32StopAddress - Ending address in flash to stop protection.
//!
//! This function will set write protection bits for a range of flash chunks
//!
//! @note Each flash chunk contains 16KBytes and corresponds to one bit in
//! the protection register. Set the bit to zero to enable protection.
//!
//! @note Interrupts are active during execution of this function. Any interrupt
//! taken could cause execution errors. Please see the IMPORTANT note under
//! Detailed Description above for more details.
//!
//! @return
//!     0 for success.
//!     0x400000 if the protection bits were already programmed (mask the return
//!              value with 0x3FFFFF to ignore this case and treat as success).
//!     Otherwise, non-zero for failure.
//!
//! Note: See am_hal_flash_program_info() for further details on return codes.
//
//*****************************************************************************
int32_t
am_hal_flash_write_protect_set(uint32_t ui32InfoKey,
                               uint32_t *pui32StartAddress,
                               uint32_t *pui32StopAddress)
{
    int iRC;
    bool bModified = false;
    uint64_t ui64Mask;
    uint32_t ui32Work;
    uint32_t ui32Protection[2];
    uint32_t *pui32Protection = (uint32_t *)AM_HAL_FLASH_INFO_WRITPROT_ADDR;

    //
    // Extract chunk mask from parameters.
    // Also checks parameter validity (returns -1 if bad parameters).
    //
    ui64Mask = generate_chunk_mask(pui32StartAddress, pui32StopAddress);
    if ( ~ui64Mask == 0x0 )
    {
        return 0x100000;
    }

    //
    // Go get the current settings for copy protection.
    //
    ui32Protection[0] = pui32Protection[0];
    ui32Protection[1] = pui32Protection[1];

    //
    // AND mask off the necessary protection bits in the lower word.
    //
    ui32Work = (uint32_t)ui64Mask;
    if ( ( ~ui32Work )  &&  ( ui32Work != ui32Protection[0] ) )
    {
        bModified = true;
        // Need to change only the bits changing - bits already set to 0 should not be rewritten to 0
        // Flash has limits on number of times a bit can be set to 0
        ui32Protection[0] = ui32Work | ~ui32Protection[0];
        iRC = g_am_hal_flash.flash_program_info_area(
                ui32InfoKey,
                0,                                      // instance
                &ui32Protection[0],                     // source data
                (AM_HAL_FLASH_INFO_WRITPROT_O / 4) + 0, // word offset
                1 );                                    // number of words

        if ( iRC )
        {
            return iRC | 0x10000;
        }
    }

    //
    // AND mask off the necessary protection bits in the upper word.
    //
    ui32Work = (uint32_t)(ui64Mask >> 32);
    if ( ( ~ui32Work )  &&  ( ui32Work != ui32Protection[1] ) )
    {
        bModified = true;
        // Need to change only the bits changing - bits already set to 0 should not be rewritten to 0
        // Flash has limits on number of times a bit can be set to 0
        ui32Protection[1] = ui32Work | ~ui32Protection[1];
        iRC = g_am_hal_flash.flash_program_info_area(
                ui32InfoKey,
                0,                                      // instance
                &ui32Protection[1],                     // source data
                (AM_HAL_FLASH_INFO_WRITPROT_O / 4) + 1, // word offset
                1 );                                    // number of words

        if ( iRC )
        {
            return iRC | 0x20000;
        }
    }

    if ( bModified )
    {
        return 0;
    }
    else
    {
        return 0x400000;
    }

} // am_hal_flash_write_protect_set()

//*****************************************************************************
//
//! @brief This function checks write protection for a range of flash chunks.
//!
//! @param pui32StartAddress - Starting address in flash.
//! @param pui32StopAddress - Ending address in flash.
//!
//! This function will check write protection bits for a range of flash chunks
//! it expects all chunks in the range to be protected.
//!
//! @note Each flash chunk contains 16KBytes and corresponds to one bit in
//! the protection register. Set the bit to zero to enable protection.
//!
//! @return false for at least one chunk in the covered range is not protected,
//!         and true if all chunks in the covered range are protected.
//!
//
//*****************************************************************************
bool
am_hal_flash_write_protect_check(uint32_t *pui32StartAddress,
                                 uint32_t *pui32StopAddress)
{
    uint64_t ui64Mask;
    uint32_t ui32Work;
    uint32_t *pui32Protection = (uint32_t *)AM_HAL_FLASH_INFO_WRITPROT_ADDR;

    //
    // Extract chunk mask from parameters.
    // Also checks parameter validity (returns -1 if bad parameters).
    //
    ui64Mask = generate_chunk_mask(pui32StartAddress, pui32StopAddress);
    if ( ~ui64Mask == 0x0 )
    {
        return false;
    }

    //
    // Now check the lower word of protection bits.
    //
    ui32Work = (uint32_t)ui64Mask;
    if ( ~ui32Work & pui32Protection[0] )
    {
        return false;
    }

    //
    // Now check the lower word of protection bits.
    //
    ui32Work = (uint32_t)(ui64Mask >> 32);
    if ( ~ui32Work & pui32Protection[1] )
    {
        return false;
    }

    //
    // If we get here, there are no unprotected chunks within specified range.
    //
    return true;

}// am_hal_flash_write_protect_check()

//*****************************************************************************
//
//! @brief Read a uint32 value from a valid memory or peripheral location.
//!
//! @param ui32Address - The location to be read.
//!
//! Use this function to safely read a value from peripheral or memory locations.
//!
//! This function calls a function that resides BOOTROM or SRAM to do the actual
//! read, thus completely avoiding any conflict with flash or INFO space.
//!
//! @return The value read from the given address.
//
//*****************************************************************************
uint32_t
am_hal_flash_load_ui32(uint32_t *pui32Address)
{
    return g_am_hal_flash.flash_util_read_word(pui32Address);
} // am_hal_flash_load_ui32()

//*****************************************************************************
//
//! @brief Write a given uint32 value to a valid memory or peripheral location.
//!
//! @param pui32Address - The location to be written.
//!
//! Use this function to safely store a value to peripheral or memory locations.
//!
//! This function calls a function that resides in BOOTROM or SRAM to do the
//! actual write, thus completely avoiding any conflict with flash or INFO.
//!
//! @return The value read from the given address.
//
//*****************************************************************************
#if defined(__GNUC_STDC_INLINE__)
uint32_t SRAM_write_ui32[12 / 4] =
    {
        //
        // A very simple, word-aligned function residing in SRAM (stack).  This
        // function writes a given memory location while executing outside of
        // flash. It then does a read back to ensure that the write completed.
        // Prototype:   uint32_t SRAM_write_ui32(ui32Addr, ui32Value);
        //
        0xBF006001,         // 6001   str    r1,[r0,#0]
                            // BF00   nop
        0xBF006800,         // 6800   ldr    r0,[r0,#0]
                            // BF00   nop
        0xBF004770          // 4770   bx lr
                            // BF00   nop
    };
#elif (defined (__ARMCC_VERSION) || defined(__IAR_SYSTEMS_ICC__))
#else
#error Compiler is unknown, please contact Ambiq support team
#endif

void
am_hal_flash_store_ui32(uint32_t *pui32Address, uint32_t ui32Value)
{
#if (defined (__ARMCC_VERSION) || defined(__IAR_SYSTEMS_ICC__))
    uint32_t SRAM_write_ui32[12 / 4] =
    {
        //
        // A very simple, word-aligned function residing in SRAM (stack).  This
        // function writes a given memory location while executing outside of
        // flash. It then does a read back to ensure that the write completed.
        // Prototype:   uint32_t SRAM_write_ui32(ui32Addr, ui32Value);
        //
        0xBF006001,         // 6001   str    r1,[r0,#0]
                            // BF00   nop
        0xBF006800,         // 6800   ldr    r0,[r0,#0]
                            // BF00   nop
        0xBF004770          // 4770   bx lr
                            // BF00   nop
    };
#elif defined(__GNUC_STDC_INLINE__)
#else
#error Compiler is unknown, please contact Ambiq support team
#endif

    //
    // Call the simple routine that has been coded in SRAM.
    // First set up a function pointer to the array, being sure to set the
    //  .T bit (Thumb bit, bit0) in the branch address, then use that
    //  function ptr to call the SRAM function.
    //
    uint32_t SRAMCode = (uint32_t)SRAM_write_ui32 | 0x1;
    uint32_t (*pFunc)(uint32_t*, uint32_t) = (uint32_t (*)(uint32_t*, uint32_t))SRAMCode;
    (*pFunc)(pui32Address, ui32Value);

} // am_hal_flash_store_ui32()

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
