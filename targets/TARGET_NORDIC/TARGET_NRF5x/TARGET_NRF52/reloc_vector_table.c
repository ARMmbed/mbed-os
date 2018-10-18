/* 
 * Copyright (c) 2016 Nordic Semiconductor ASA
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   1. Redistributions of source code must retain the above copyright notice, this list 
 *      of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form, except as embedded into a Nordic Semiconductor ASA 
 *      integrated circuit in a product or a software update for such product, must reproduce 
 *      the above copyright notice, this list of conditions and the following disclaimer in 
 *      the documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of its contributors may be 
 *      used to endorse or promote products derived from this software without specific prior 
 *      written permission.
 *
 *   4. This software, with or without modification, must only be used with a 
 *      Nordic Semiconductor ASA integrated circuit.
 *
 *   5. Any software provided in binary or object form under this license must not be reverse 
 *      engineered, decompiled, modified and/or disassembled. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */
 
#include "nrf.h"
#include "cmsis_nvic.h"
#include "stdint.h"
#include "PinNames.h"
#include "hal/gpio_api.h"

#if defined(SOFTDEVICE_PRESENT)
#include "nrf_sdm.h"
#include "nrf_dfu_mbr.h"
#endif

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    __attribute__ ((section(".bss.nvictable"),zero_init))
    uint32_t nrf_dispatch_vector[NVIC_NUM_VECTORS];
#elif defined(__GNUC__)
    __attribute__ ((section(".nvictable")))
    uint32_t nrf_dispatch_vector[NVIC_NUM_VECTORS];
#elif defined(__ICCARM__)
    uint32_t nrf_dispatch_vector[NVIC_NUM_VECTORS] @ ".nvictable";
#endif

extern uint32_t __Vectors[];

#define VECTORS_FLASH_START __Vectors
#define MBR_VTOR_ADDRESS 0x20000000
#define SOFTDEVICE_VTOR_ADDRESS 0x20000004

/**
 * @brief Function for relocation of the vector to RAM on nRF5x devices.
 * This function is intended to be called during startup.
 */
void nrf_reloc_vector_table(void)
{
    // Copy and switch to dynamic vectors
	uint32_t *old_vectors = VECTORS_FLASH_START;
	uint32_t i;
	for (i = 0; i< NVIC_NUM_VECTORS; i++) {
		nrf_dispatch_vector[i] = old_vectors[i];
	}

#if defined(SOFTDEVICE_PRESENT)

    /**
     * Before setting the new vector table address in the SoftDevice the MBR must be initialized.
     * If no bootloader is present the MBR will be initialized automatically.
     * If a bootloader is present nrf_dfu_mbr_init_sd must be called once and only once.
     * 
     * By resetting the MBR and SoftDevice VTOR address first, it becomes safe to initialize
     * the MBR again regardless of how the application was started. 
     */

    /* Reset MBR VTOR to original state before calling MBR init. */
    uint32_t *mbr_vtor_address = (uint32_t *) MBR_VTOR_ADDRESS;
    *mbr_vtor_address = (uint32_t) VECTORS_FLASH_START;

    /* Reset SoftDevive VTOR. */
    uint32_t *softdevice_vtor_address = (uint32_t *) SOFTDEVICE_VTOR_ADDRESS;
    *softdevice_vtor_address = 0xFFFFFFFF;

    /* Set SCB->VTOR to go through MBR to trap SoftDevice service calls. */
    SCB->VTOR = 0x0;

    /* Initialize MBR so SoftDevice service calls are being trapped correctly. 
     * This call sets MBR_VTOR_ADDRESS to point to the SoftDevice's VTOR at address 0x1000.
     */
    nrf_dfu_mbr_init_sd();

    /* Instruct the SoftDevice to forward interrupts to the application's vector table in RAM. */
    sd_softdevice_vector_table_base_set((uint32_t) nrf_dispatch_vector);

#else

    /* No SoftDevice is present. Set all interrupts to vector table in RAM. */
    SCB->VTOR = (uint32_t) nrf_dispatch_vector;    
#endif
}


void mbed_sdk_init(void)
{
	if (STDIO_UART_RTS != NC) {
		gpio_t rts;
		gpio_init_out(&rts, STDIO_UART_RTS);
		/* Set STDIO_UART_RTS as gpio driven low */
		gpio_write(&rts, 0);
	}
}
