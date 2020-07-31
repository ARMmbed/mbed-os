/**************************************************************************//**
 * @file     mmu_RZ_A2M.c
 * @brief    MMU Configuration for RZ_A2M Device Series
 * @version  V1.00
 * @date     6 Sep 2018
 *
 * @note
 *
 ******************************************************************************/
/*
 * Copyright (c) 2009-2020 ARM Limited. All rights reserved.
 * Copyright (c) 2018-2020 Renesas Electronics Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Memory map description

== User's Manual:Hardware ==
                                                     Memory Type
0xffffffff |--------------------------|             ------------
           |       Peripherals        |                Device
0xffff0000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0xfd000000 |--------------------------|             ------------
           |       Peripherals        |                Device
0xfcfe0000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0xfc040000 |--------------------------|             ------------
           |       Peripherals        |                Device
0xfc000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0xf0002000 |--------------------------|             ------------
           |  Cortex-A9 private area  |                Device
0xf0000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0xeb000000 |--------------------------|             ------------
           |       Peripherals        |                Device
0xea000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0xe8240000 |--------------------------|             ------------
           |       Peripherals        |                Device
0xe8200000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0xe8050000 |--------------------------|             ------------
           |       Peripherals        |                Device
0xe8000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x80400000 |--------------------------|             ------------
           |     On Chip RAM 4MB      |                  RW
0x80000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x70000000 |--------------------------|             ------------
           |      OctaRAM 256MB       |                  RW
0x60000000 |--------------------------|             ------------
           |     OctaFlash 256MB      |                  RO
0x50000000 |--------------------------|             ------------
           |      HyperRAM 256MB      |                  RW
0x40000000 |--------------------------|             ------------
           |     HyperFlash 256MB     |                  RO
0x30000000 |--------------------------|             ------------
           |   SPI multi I/O 256MB    |                  RO
0x20000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x1f809000 |--------------------------|             ------------
           |       Peripherals        |                Device
0x1f808000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x1f801000 |--------------------------|             ------------
           |       Peripherals        |                Device
0x1f800000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x1f402000 |--------------------------|             ------------
           |       Peripherals        |                Device
0x1f400000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x1f004000 |--------------------------|             ------------
           |       Peripherals        |                Device
0x1f000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x1c000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x18000000 |--------------------------|             ------------
           |         CS5 64MB         |                  RW
0x14000000 |--------------------------|             ------------
           |         CS4 64MB         |                  RW
0x10000000 |--------------------------|             ------------
           |         CS3 64MB         |                  RW
0x0c000000 |--------------------------|             ------------
           |         CS2 64MB         |                  RW
0x08000000 |--------------------------|             ------------
           |         CS1 64MB         |                  RW
0x04000000 |--------------------------|             ------------
           |         CS0 64MB         |                  RW
0x00000000 |--------------------------|             ------------


== Actual setting of MMU ==
                                                     Memory Type
0xffffffff |--------------------------|             ------------
           |       Peripherals        |                Device
0xe8000000 |--------------------------|             ------------
           |            -             |                   -
0x80400000 |--------------------------|             ------------
           |     On Chip RAM 4MB      |                  RW
0x80000000 |--------------------------|             ------------
           |            -             |                   -
0x70000000 |--------------------------|             ------------
           |      OctaRAM 256MB       |                  RW
0x60000000 |--------------------------|             ------------
           |     OctaFlash 256MB      |                  RO
0x50000000 |--------------------------|             ------------
           |      HyperRAM 256MB      |                  RW
0x40000000 |--------------------------|             ------------
           |     HyperFlash 256MB     |                  RO
0x30000000 |--------------------------|             ------------
           |   SPI multi I/O 256MB    |                  RO
0x20000000 |--------------------------|             ------------
           |       Peripherals        |                Device
0x18000000 |--------------------------|             ------------
           |            -             |                   -
0x10000000 |--------------------------|             ------------
           |      CS3 SDRAM 64MB      |                  RW
0x0c000000 |--------------------------|             ------------
           |            -             |                   -
0x00000000 |--------------------------|             ------------
*/

// L1 Cache info and restrictions about architecture of the caches (CCSIR register):
// Write-Through support *not* available
// Write-Back support available.
// Read allocation support available.
// Write allocation support available.

//Note: You should use the Shareable attribute carefully.
//For cores without coherency logic (such as SCU) marking a region as shareable forces the processor to not cache that region regardless of the inner cache settings.
//Cortex-A versions of RTX use LDREX/STREX instructions relying on Local monitors. Local monitors will be used only when the region gets cached, regions that are not cached will use the Global Monitor.
//Some Cortex-A implementations do not include Global Monitors, so wrongly setting the attribute Shareable may cause STREX to fail.

//Recall: When the Shareable attribute is applied to a memory region that is not Write-Back, Normal memory, data held in this region is treated as Non-cacheable.
//When SMP bit = 0, Inner WB/WA Cacheable Shareable attributes are treated as Non-cacheable.
//When SMP bit = 1, Inner WB/WA Cacheable Shareable attributes are treated as Cacheable.


//Following MMU configuration is expected
//SCTLR.AFE == 1 (Simplified access permissions model - AP[2:1] define access permissions, AP[0] is an access flag)
//SCTLR.TRE == 0 (TEX remap disabled, so memory type and attributes are described directly by bits in the descriptor)
//Domain 0 is always the Client domain
//Descriptors should place all memory in domain 0
//There are no restrictions by privilege level (PL0 can access all memory)


#include "RZ_A2M.h"

//Import symbols from linker
#if defined ( __ICCARM__ )
__no_init uint32_t Image$$TTB$$ZI$$Base @ "TTB";
__no_init uint32_t Image$$TTB_L2$$ZI$$Base @ "TTB_L2";
#else
extern uint32_t Image$$TTB$$ZI$$Base;
extern uint32_t Image$$TTB_L2$$ZI$$Base;
extern uint32_t Image$$RW_DATA_NC$$Base;
extern uint32_t Image$$ZI_DATA_NC$$Limit;
#endif

#define page4k_normal(descriptor_l1, descriptor_l2, region) region.rg_t = PAGE_4k; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = WB_WA; \
                                   region.outer_norm_t = WB_WA; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetPageDescriptor(&descriptor_l1, &descriptor_l2, region);

#define page4k_normal_nc(descriptor_l1, descriptor_l2, region) region.rg_t = PAGE_4k; \
                                   region.domain = 0x0; \
                                   region.e_t = ECC_DISABLED; \
                                   region.g_t = GLOBAL; \
                                   region.inner_norm_t = NON_CACHEABLE; \
                                   region.outer_norm_t = NON_CACHEABLE; \
                                   region.mem_t = NORMAL; \
                                   region.sec_t = SECURE; \
                                   region.xn_t = EXECUTE; \
                                   region.priv_t = RW; \
                                   region.user_t = RW; \
                                   region.sh_t = NON_SHARED; \
                                   MMU_GetPageDescriptor(&descriptor_l1, &descriptor_l2, region);

__STATIC_INLINE void MMU_TTSection_Va(uint32_t *ttb, uint32_t vaddress, uint32_t paddress, uint32_t count, uint32_t descriptor_l1)
{
    uint32_t offset;
    uint32_t entry;
    uint32_t i;

    offset = vaddress >> 20;
    entry  = (paddress & 0xFFF00000) | descriptor_l1;

    //4 bytes aligned
    ttb = ttb + offset;

    for (i = 0; i < count; i++) {
        //4 bytes aligned
        *ttb++ = entry;
        entry += OFFSET_1M;
    }
}

static void MMU_TTPage4k_local(uint32_t *ttb, uint32_t start_address, uint32_t size, uint32_t descriptor_l1, uint32_t *ttb_l2, uint32_t descriptor_l2)
{
    uint32_t base_address  = start_address & 0xFF000000;
    uint32_t section_count = (start_address & 0x00F00000) >> 20;
    uint32_t rest_count    = (size + OFFSET_4K - 1) / OFFSET_4K;
    uint32_t count;
    uint32_t count_adjust;

    if (rest_count == 0) {
        return;
    }

    if ((start_address & 0x000FFFFF) != 0) {
        /* When "start_address" is not the beginning of the section. */
        count_adjust = 0x100 - ((start_address & 0x000FFFFF) / OFFSET_4K);

        count = rest_count;
        if (count > count_adjust) {
            count = count_adjust;
        }
        MMU_TTPage4k(ttb, start_address, count, descriptor_l1,
                     (uint32_t *)((uint32_t)ttb_l2 + (0x400 * section_count)), descriptor_l2);
        rest_count -= count;
        section_count++;
    }

    while (rest_count > 0) {
        count = rest_count;
        if (count > 0x100) {
            count = 0x100;
        }
        MMU_TTPage4k(ttb, base_address + (OFFSET_1M * section_count), count, descriptor_l1,
                     (uint32_t *)((uint32_t)ttb_l2 + (0x400 * section_count)), descriptor_l2);
        rest_count -= count;
        section_count++;
    }
}

void MMU_CreateTranslationTable(void)
{
    mmu_region_attributes_Type region;

    uint32_t Sect_Normal_Cod; //outer & inner wb/wa, non-shareable, executable, ro, domain 0, base addr 0
    uint32_t Sect_Normal_RW;  //as Sect_Normal_Cod, but writeable and not executable
    uint32_t Sect_Device_RW;  //as Sect_Device_RO, but writeable
    uint32_t Sect_Normal_NC;  //non-shareable, non-executable, rw, domain 0, base addr 0

    uint32_t Page_L1_4k  = 0x0;  //generic
    uint32_t Page_4k_Normal_RW;
    uint32_t Page_4k_Normal_NC;

    uint32_t sram_non_cache_base;
    uint32_t sram_non_cache_size;

#if defined ( __ICCARM__ )
#pragma section="NC_RAM"
    sram_non_cache_base = (uint32_t)__section_begin("NC_RAM");
    sram_non_cache_size = (uint32_t)__section_end("NC_RAM") - (uint32_t)__section_begin("NC_RAM");
#else
    sram_non_cache_base = (uint32_t)&Image$$RW_DATA_NC$$Base;
    sram_non_cache_size = (uint32_t)&Image$$ZI_DATA_NC$$Limit - (uint32_t)&Image$$RW_DATA_NC$$Base;
#endif

    /*
     * Generate descriptors. Refer to core_ca.h to get information about attributes
     *
     */
    //Create descriptors for Vectors, RO, RW, ZI sections
    section_normal_cod(Sect_Normal_Cod, region);
    section_normal(Sect_Normal_RW, region);
    section_normal_nc(Sect_Normal_NC, region);

    //Create descriptors for peripherals
    section_device_rw(Sect_Device_RW, region);
    //Create descriptors for 4k pages
    page4k_normal(Page_L1_4k, Page_4k_Normal_RW, region);
    page4k_normal_nc(Page_L1_4k, Page_4k_Normal_NC, region);

    /*
     *  Define MMU flat-map regions and attributes
     *
     */
    //Create 4GB of faulting entries
    MMU_TTSection(&Image$$TTB$$ZI$$Base, 0, 4096, DESCRIPTOR_FAULT);

    // memory map.
    MMU_TTSection(&Image$$TTB$$ZI$$Base, RZ_A2_SDRAM, 64, Sect_Normal_RW);
    MMU_TTSection(&Image$$TTB$$ZI$$Base, RZ_A2_SPI_IO, 256, Sect_Normal_Cod);
    MMU_TTSection(&Image$$TTB$$ZI$$Base, RZ_A2_HYPER_FLASH, 256, Sect_Normal_Cod);
    MMU_TTSection(&Image$$TTB$$ZI$$Base, RZ_A2_HYPER_RAM, 256, Sect_Normal_RW);
    MMU_TTSection(&Image$$TTB$$ZI$$Base, RZ_A2_OCTA_FLASH, 256, Sect_Normal_Cod);
    MMU_TTSection(&Image$$TTB$$ZI$$Base, RZ_A2_OCTA_RAM, 256, Sect_Normal_RW);
    MMU_TTSection(&Image$$TTB$$ZI$$Base, RZ_A2_PERIPH_BASE0, 384, Sect_Device_RW);
    MMU_TTSection(&Image$$TTB$$ZI$$Base, RZ_A2_PERIPH_BASE1, 128, Sect_Device_RW);

    // SRAM
    // Register all memory as cache.
    MMU_TTPage4k_local(&Image$$TTB$$ZI$$Base, RZ_A2_ONCHIP_SRAM_BASE, 0x400000, Page_L1_4k,
                       (uint32_t *)&Image$$TTB_L2$$ZI$$Base, Page_4k_Normal_RW);

    // Overwrite non-cache if necessary.
    MMU_TTPage4k_local(&Image$$TTB$$ZI$$Base, sram_non_cache_base, sram_non_cache_size, Page_L1_4k,
                       (uint32_t *)&Image$$TTB_L2$$ZI$$Base, Page_4k_Normal_NC);

    // Virtual address
    MMU_TTSection_Va(&Image$$TTB$$ZI$$Base, RZ_A2_HYPER_FLASH_IO, RZ_A2_HYPER_FLASH, 256, Sect_Device_RW);
    MMU_TTSection_Va(&Image$$TTB$$ZI$$Base, RZ_A2_OCTA_FLASH_NC, RZ_A2_OCTA_FLASH, 256, Sect_Normal_NC);

    /* Set location of level 1 page table
    ; 31:14 - Translation table base addr (31:14-TTBCR.N, TTBCR.N is 0 out of reset)
    ; 13:7  - 0x0
    ; 6     - IRGN[0] 0x0 (Inner WB WA)
    ; 5     - NOS     0x0 (Non-shared)
    ; 4:3   - RGN     0x1 (Outer WB WA)
    ; 2     - IMP     0x0 (Implementation Defined)
    ; 1     - S       0x0 (Non-shared)
    ; 0     - IRGN[1] 0x1 (Inner WB WA) */
    __set_TTBR0(((uint32_t)&Image$$TTB$$ZI$$Base) | 0x48);
    __ISB();

    /* Set up domain access control register
    ; We set domain 0 to Client and all other domains to No Access.
    ; All translation table entries specify domain 0 */
    __set_DACR(1);
    __ISB();
}
