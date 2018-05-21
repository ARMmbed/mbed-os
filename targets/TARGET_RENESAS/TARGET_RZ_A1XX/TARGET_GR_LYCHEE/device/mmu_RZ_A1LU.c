/**************************************************************************//**
 * @file     mmu_RZ_A1LU.c
 * @brief    MMU Configuration for RZ_A1LU Device Series
 * @version  V1.00
 * @date     10 Mar 2017
 *
 * @note
 *
 ******************************************************************************/
/*
 * Copyright (c) 2009-2017 ARM Limited. All rights reserved.
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

/* Memory map description from: Renesas RZ_A1H_05E_121130.pdf

                                                     Memory Type
0xffffffff |--------------------------|             ------------
           |        Peripherals       |                Device
0xfcf00000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0xe8300000 |--------------------------|             ------------
           |        Peripherals       |                Device
0xe8000000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x60A00000 |--------------------------|             ------------
           | On Chip RAM (10M) Mirror |                Fault
0x60000000 |--------------------------|             ------------
           |  SPI multi I/O 64MB      |                Fault
0x5c000000 |--------------------------|             ------------
           |  SPI multi I/O 64MB      |                Fault
0x58000000 |--------------------------|             ------------
           |        CS5 Mirror        |                Fault
0x54000000 |--------------------------|             ------------
           |        CS4 Mirror        |                Fault
0x50000000 |--------------------------|             ------------
           |        CS3 Mirror        |                Fault
0x4c000000 |--------------------------|             ------------
           |        CS2 Mirror        |                Fault
0x48000000 |--------------------------|             ------------
           |        CS1 Mirror        |                Fault
0x44000000 |--------------------------|             ------------
           |        CS0 Mirror        |                Fault
0x40000000 |--------------------------|             ------------
           |          BSC             |                 RW
0x3ff00000 |--------------------------|             ------------
           |      SPI_MIO_BASE        |                 RW
0x3fe00000 |--------------------------|             ------------
           |        Page Fault        |                Fault
0x20A00000 |--------------------------|             ------------
           |    On Chip RAM (10M)     |                 RW
0x20000000 |--------------------------|             ------------
           |  SPI multi I/O 64MB      |                 RO
0x1c000000 |--------------------------|             ------------
           |  SPI multi I/O 64MB      |                 RO
0x18000000 |--------------------------|             ------------
           |  CS5 User Area 64MB      |                 RW
0x14000000 |--------------------------|             ------------
           |  CS4 User Area 64MB      |                 RW
0x10000000 |--------------------------|             ------------
           |      CS3 SDRAM 64MB      |                 RW
0x0c000000 |--------------------------|             ------------
           |      CS2 SDRAM 64MB      |                 RW
0x08000000 |--------------------------|             ------------
           |  CS1 NOR Flash 64MB      |                 RO
0x04000000 |--------------------------|             ------------
           |  CS0 NOR Flash 64MB      |                 RO
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


#include "RZ_A1LU.h"

//Import symbols from linker
extern uint32_t Image$$VECTORS$$Base;
extern uint32_t Image$$RO_DATA$$Base;
extern uint32_t Image$$RW_DATA$$Base;
extern uint32_t Image$$RW_IRAM1$$Base;
#if !defined ( __ICCARM__ )
extern uint32_t Image$$TTB$$ZI$$Base;
#endif 

#if defined( __CC_ARM )
#elif defined( __ICCARM__ )
#else
extern uint32_t Image$$RW_DATA_NC$$Base;
extern uint32_t Image$$ZI_DATA_NC$$Base;
#endif

extern uint32_t Image$$VECTORS$$Limit;
extern uint32_t Image$$RO_DATA$$Limit;
extern uint32_t Image$$RW_DATA$$Limit;
extern uint32_t Image$$RW_IRAM1$$Limit;
#if defined( __CC_ARM )
#else
extern uint32_t Image$$RW_DATA_NC$$Limit;
extern uint32_t Image$$ZI_DATA_NC$$Limit;
#endif

#if defined( __ICCARM__ )
#define VECTORS_SIZE    (((uint32_t)Image$$VECTORS$$Limit >> 20) - ((uint32_t)Image$$VECTORS$$Base >> 20) + 1)
#define RO_DATA_SIZE    (((uint32_t)Image$$RO_DATA$$Limit >> 20) - ((uint32_t)Image$$RO_DATA$$Base >> 20) + 1)
#define RW_DATA_SIZE    (((uint32_t)Image$$RW_DATA$$Limit >> 20) - ((uint32_t)Image$$RW_DATA$$Base >> 20) + 1)
#define RW_IRAM1_SIZE   (((uint32_t)Image$$RW_IRAM1$$Limit >> 20) - ((uint32_t)Image$$RW_IRAM1$$Base >> 20) + 1)
#else
#define VECTORS_SIZE    (((uint32_t)&Image$$VECTORS$$Limit >> 20) - ((uint32_t)&Image$$VECTORS$$Base >> 20) + 1)
#define RO_DATA_SIZE    (((uint32_t)&Image$$RO_DATA$$Limit >> 20) - ((uint32_t)&Image$$RO_DATA$$Base >> 20) + 1)
#define RW_DATA_SIZE    (((uint32_t)&Image$$RW_DATA$$Limit >> 20) - ((uint32_t)&Image$$RW_DATA$$Base >> 20) + 1)
#define RW_IRAM1_SIZE   (((uint32_t)&Image$$RW_IRAM1$$Limit >> 20) - ((uint32_t)&Image$$RW_IRAM1$$Base >> 20) + 1)
#endif

#if defined( __CC_ARM )
#else
#define RW_DATA_NC_SIZE (((uint32_t)&Image$$RW_DATA_NC$$Limit >> 20) - ((uint32_t)&Image$$RW_DATA_NC$$Base >> 20) + 1)
#define ZI_DATA_NC_SIZE (((uint32_t)&Image$$ZI_DATA_NC$$Limit >> 20) - ((uint32_t)&Image$$ZI_DATA_NC$$Base >> 20) + 1)
#endif

static uint32_t Sect_Normal;     //outer & inner wb/wa, non-shareable, executable, rw, domain 0, base addr 0
static uint32_t Sect_Normal_NC;  //non-shareable, non-executable, rw, domain 0, base addr 0
static uint32_t Sect_Normal_Cod; //outer & inner wb/wa, non-shareable, executable, ro, domain 0, base addr 0
static uint32_t Sect_Normal_RO;  //as Sect_Normal_Cod, but not executable
static uint32_t Sect_Normal_RW;  //as Sect_Normal_Cod, but writeable and not executable
static uint32_t Sect_Device_RO;  //device, non-shareable, non-executable, ro, domain 0, base addr 0
static uint32_t Sect_Device_RW;  //as Sect_Device_RO, but writeable

/* Define global descriptors */
static uint32_t Page_L1_4k  = 0x0;  //generic
static uint32_t Page_L1_64k = 0x0;  //generic
static uint32_t Page_4k_Device_RW;  //Shared device, not executable, rw, domain 0
static uint32_t Page_64k_Device_RW; //Shared device, not executable, rw, domain 0

#if defined ( __ICCARM__ )
__no_init uint32_t Image$$TTB$$ZI$$Base @ ".retram";
uint32_t Image$$VECTORS$$Base;
uint32_t Image$$RO_DATA$$Base;
uint32_t Image$$RW_DATA$$Base;
uint32_t Image$$RW_IRAM1$$Base;

uint32_t Image$$VECTORS$$Limit;
uint32_t Image$$RO_DATA$$Limit;
uint32_t Image$$RW_DATA$$Limit;
uint32_t Image$$RW_IRAM1$$Limit;
#endif

void MMU_CreateTranslationTable(void)
{
    mmu_region_attributes_Type region;
#if defined ( __ICCARM__ )
#pragma section=".intvec"
#pragma section=".rodata"
#pragma section=".rwdata"
#pragma section=".bss"

    Image$$VECTORS$$Base = (uint32_t) __section_begin(".intvec");
    Image$$VECTORS$$Limit= ((uint32_t)__section_begin(".intvec")+(uint32_t)__section_size(".intvec"));
    Image$$RO_DATA$$Base = (uint32_t) __section_begin(".rodata");
    Image$$RO_DATA$$Limit= ((uint32_t)__section_begin(".rodata")+(uint32_t)__section_size(".rodata"));
    Image$$RW_DATA$$Base = (uint32_t) __section_begin(".rwdata"); 
    Image$$RW_DATA$$Limit= ((uint32_t)__section_begin(".rwdata")+(uint32_t)__section_size(".rwdata"));
    Image$$RW_IRAM1$$Base = (uint32_t) __section_begin(".bss");  
    Image$$RW_IRAM1$$Limit= ((uint32_t)__section_begin(".bss")+(uint32_t)__section_size(".bss"));
#endif
    /*
     * Generate descriptors. Refer to core_ca.h to get information about attributes
     *
     */
    //Create descriptors for Vectors, RO, RW, ZI sections
    section_normal(Sect_Normal, region);
    section_normal_cod(Sect_Normal_Cod, region);
    section_normal_ro(Sect_Normal_RO, region);
    section_normal(Sect_Normal_RW, region);
    //Create descriptors for peripherals
    section_device_ro(Sect_Device_RO, region);
    section_device_rw(Sect_Device_RW, region);
    section_normal_nc(Sect_Normal_NC, region);
    //Create descriptors for 64k pages
    page64k_device_rw(Page_L1_64k, Page_64k_Device_RW, region);
    //Create descriptors for 4k pages
    page4k_device_rw(Page_L1_4k, Page_4k_Device_RW, region);

    /*
     *  Define MMU flat-map regions and attributes
     *
     */

    //Create 4GB of faulting entries
    MMU_TTSection (&Image$$TTB$$ZI$$Base, 0, 4096, DESCRIPTOR_FAULT);

    // R7S721030 memory map.
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_NORFLASH_BASE0    , 64, Sect_Normal_RO);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_NORFLASH_BASE1    , 64, Sect_Normal_RO);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_SDRAM_BASE0       , 64, Sect_Normal_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_SDRAM_BASE1       , 64, Sect_Normal_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_USER_AREA0        , 64, Sect_Normal_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_USER_AREA1        , 64, Sect_Normal_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_SPI_IO0           , 64, Sect_Normal_RO);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_ONCHIP_SRAM_BASE  ,  3, Sect_Normal_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_SPI_MIO_BASE      ,  1, Sect_Device_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_BSC_BASE          ,  1, Sect_Device_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_PERIPH_BASE0      ,  3, Sect_Device_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_PERIPH_BASE1      , 49, Sect_Device_RW);

#if defined( __ICCARM__ )
    //Define Image
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)Image$$RO_DATA$$Base , RO_DATA_SIZE , Sect_Normal_Cod);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)Image$$VECTORS$$Base , VECTORS_SIZE , Sect_Normal_Cod);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)Image$$RW_DATA$$Base , RW_DATA_SIZE , Sect_Normal_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)Image$$RW_IRAM1$$Base, RW_IRAM1_SIZE, Sect_Normal_RW);
#else
    //Define Image
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)&Image$$RO_DATA$$Base , RO_DATA_SIZE , Sect_Normal_Cod);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)&Image$$VECTORS$$Base , VECTORS_SIZE , Sect_Normal_Cod);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)&Image$$RW_DATA$$Base , RW_DATA_SIZE , Sect_Normal_RW);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)&Image$$RW_IRAM1$$Base, RW_IRAM1_SIZE, Sect_Normal_RW);
#endif

#if defined( __CC_ARM )
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_ONCHIP_SRAM_NC_BASE         ,              10, Sect_Normal_NC);
#elif defined ( __ICCARM__ ) 
    MMU_TTSection (&Image$$TTB$$ZI$$Base, RZ_A1_ONCHIP_SRAM_NC_BASE         ,              10, Sect_Normal_NC);

#else
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)&Image$$RW_DATA_NC$$Base, RW_DATA_NC_SIZE, Sect_Normal_NC);
    MMU_TTSection (&Image$$TTB$$ZI$$Base, (uint32_t)&Image$$ZI_DATA_NC$$Base, ZI_DATA_NC_SIZE, Sect_Normal_NC);
#endif

    /* Set location of level 1 page table
    ; 31:14 - Translation table base addr (31:14-TTBCR.N, TTBCR.N is 0 out of reset)
    ; 13:7  - 0x0
    ; 6     - IRGN[0] 0x0 (Inner WB WA)
    ; 5     - NOS     0x0 (Non-shared)
    ; 4:3   - RGN     0x1 (Outer WB WA)
    ; 2     - IMP     0x0 (Implementation Defined)
    ; 1     - S       0x0 (Non-shared)
    ; 0     - IRGN[1] 0x1 (Inner WB WA) */
    __set_TTBR0(((uint32_t)&Image$$TTB$$ZI$$Base) | 9);
    __ISB();

    /* Set up domain access control register
    ; We set domain 0 to Client and all other domains to No Access.
    ; All translation table entries specify domain 0 */
    __set_DACR(1);
    __ISB();
}
