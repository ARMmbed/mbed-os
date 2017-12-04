/*
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "rtl8195a.h"
<<<<<<< HEAD
#include "system_8195a.h"
#if   defined ( __CC_ARM )                                            /* ARM Compiler 4/5 */
extern uint8_t Image$$RW_IRAM1$$ZI$$Base[];
#define __bss_start__ Image$$RW_IRAM1$$ZI$$Base
extern uint8_t Image$$RW_IRAM1$$ZI$$Limit[];
#define __bss_end__ Image$$RW_IRAM1$$ZI$$Limit
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)       /* ARM Compiler 6 */
extern uint8_t Image$$RW_IRAM1$$ZI$$Base[];
#define __bss_start__ Image$$RW_IRAM1$$ZI$$Base
extern uint8_t Image$$RW_IRAM1$$ZI$$Limit[];
#define __bss_end__ Image$$RW_IRAM1$$ZI$$Limit

#elif defined ( __ICCARM__ )
#pragma section=".ram.bss"
#pragma section=".rom.bss"
#pragma section=".ram.start.table"
#pragma section=".ram_image1.bss"
#pragma section=".image2.start.table1"
#pragma section=".image2.start.table2"

uint8_t *__bss_start__;
uint8_t *__bss_end__;

void __iar_data_init_app(void)
{
    __bss_start__ = (uint8_t *)__section_begin(".ram.bss");
    __bss_end__   = (uint8_t *)__section_end(".ram.bss");
}
#else
extern uint8_t __bss_start__[];
extern uint8_t __bss_end__[];
extern uint8_t __image1_bss_start__[];
extern uint8_t __image1_bss_end__[];
extern uint8_t __image2_entry_func__[];
extern uint8_t __image2_validate_code__[];
=======

#if defined(__CC_ARM) || \
    (defined (__ARMCC_VERSION) && __ARMCC_VERSION >= 6010050)

extern uint8_t Image$$RW_IRAM2$$ZI$$Base[];
extern uint8_t Image$$RW_IRAM2$$ZI$$Limit[];
extern uint8_t Image$$TCM_OVERLAY$$ZI$$Base[];
extern uint8_t Image$$TCM_OVERLAY$$ZI$$Limit[];
extern uint8_t Image$$RW_DRAM2$$ZI$$Base[];
extern uint8_t Image$$RW_DRAM2$$ZI$$Limit[];
#define __bss_sram_start__ Image$$RW_IRAM2$$ZI$$Base
#define __bss_sram_end__   Image$$RW_IRAM2$$ZI$$Limit
#define __bss_dtcm_start__ Image$$TCM_OVERLAY$$ZI$$Base
#define __bss_dtcm_end__   Image$$TCM_OVERLAY$$ZI$$Limit
#define __bss_dram_start__ Image$$RW_DRAM2$$ZI$$Base
#define __bss_dram_end__   Image$$RW_DRAM2$$ZI$$Limit

#elif defined (__ICCARM__)

#pragma section=".bss.sram"
#pragma section=".bss.dtcm"
#pragma section=".bss.dram"

uint8_t *__bss_sram_start__;
uint8_t *__bss_sram_end__;
uint8_t *__bss_dtcm_start__;
uint8_t *__bss_dtcm_end__;
uint8_t *__bss_dram_start__;
uint8_t *__bss_dram_end__;

void __iar_data_init_app(void)
{
    __bss_sram_start__ = (uint8_t *)__section_begin(".bss.sram");
    __bss_sram_end__   = (uint8_t *)__section_end(".bss.sram");
    __bss_dtcm_start__ = (uint8_t *)__section_begin(".bss.dtcm");
    __bss_dtcm_end__   = (uint8_t *)__section_end(".bss.dtcm");
    __bss_dram_start__ = (uint8_t *)__section_begin(".bss.dram");
    __bss_dram_end__   = (uint8_t *)__section_end(".bss.dram");
}

#else

extern uint8_t __bss_sram_start__[];
extern uint8_t __bss_sram_end__[];
extern uint8_t __bss_dtcm_start__[];
extern uint8_t __bss_dtcm_end__[];
extern uint8_t __bss_dram_start__[];
extern uint8_t __bss_dram_end__[];

>>>>>>> upstream/master
#endif

extern VECTOR_Func NewVectorTable[];
extern void SystemCoreClockUpdate(void);
extern void PLAT_Start(void);
extern void PLAT_Main(void);
<<<<<<< HEAD
extern HAL_TIMER_OP HalTimerOp;
 
IMAGE2_START_RAM_FUN_SECTION const RAM_START_FUNCTION gImage2EntryFun0 = {
    PLAT_Start
};

IMAGE1_VALID_PATTEN_SECTION const uint8_t RAM_IMG1_VALID_PATTEN[] = {
    0x23, 0x79, 0x16, 0x88, 0xff, 0xff, 0xff, 0xff
};

IMAGE2_VALID_PATTEN_SECTION const uint8_t RAM_IMG2_VALID_PATTEN[20] = {
=======

IMAGE2_START_RAM_FUN_SECTION
const RAM_START_FUNCTION gImage2EntryFun0 = {
    PLAT_Start
};

IMAGE2_VALID_PATTEN_SECTION
const uint8_t IMAGE2_SIGNATURE[20] = {
>>>>>>> upstream/master
    'R', 'T', 'K', 'W', 'i', 'n', 0x0, 0xff,
    (FW_VERSION&0xff), ((FW_VERSION >> 8)&0xff),
    (FW_SUBVERSION&0xff), ((FW_SUBVERSION >> 8)&0xff),
    (FW_CHIP_ID&0xff), ((FW_CHIP_ID >> 8)&0xff),
    (FW_CHIP_VER),
    (FW_BUS_TYPE),
    (FW_INFO_RSV1),
    (FW_INFO_RSV2),
    (FW_INFO_RSV3),
    (FW_INFO_RSV4)
};

void TRAP_NMIHandler(void)
{
#ifdef CONFIG_WDG_NORMAL
    uint32_t val;
    WDG_REG *ctl;

    // Check if this NMI is triggered by Watchdog Timer
    val = __RTK_READ32(VENDOR_REG_BASE, 0);
    ctl = (WDG_REG*) &val;
    if (ctl->WdgToISR) {
        INTR_WatchdogHandler();
    }
#endif
}

<<<<<<< HEAD
#if defined ( __ICCARM__ )
=======
#if defined (__ICCARM__)
>>>>>>> upstream/master
void __TRAP_HardFaultHandler_Patch(uint32_t addr)
{
    uint32_t cfsr;
    uint32_t bfar;

    uint32_t stackpc;
    uint16_t asmcode;

    cfsr = HAL_READ32(0xE000ED28, 0x0);

    // Violation to memory access protection
    if (cfsr & 0x82) {

        bfar = HAL_READ32(0xE000ED38, 0x0);

        // invalid access to wifi register, usually happened in LPS 32K or IPS
        if (bfar >= WIFI_REG_BASE && bfar < WIFI_REG_BASE + 0x40000) {

            //__BKPT(0);

            /* Get the MemManage fault PC, and step to next command.
             * Otherwise it will keep hitting MemMange Fault on the same assembly code.
             *
             * To step to next command, we need parse the assembly code to check if
<<<<<<< HEAD
	     * it is 16-bit or 32-bit command.
             * Ref: ARM Architecture Reference Manual (ARMv7-A and ARMv7-R edition),
	     * Chapter A6 - Thumb Instruction Set Encoding
=======
         * it is 16-bit or 32-bit command.
             * Ref: ARM Architecture Reference Manual (ARMv7-A and ARMv7-R edition),
         * Chapter A6 - Thumb Instruction Set Encoding
>>>>>>> upstream/master
             *
             * However, the fault assembly code (Ex. LDR or ADR) is not actually executed,
             * So the register value is un-predictable.
             **/
            stackpc = HAL_READ32(addr, 0x18);
            asmcode = HAL_READ16(stackpc, 0);
            if ((asmcode & 0xF800) > 0xE000) {
                // 32-bit instruction, (opcode[15:11] = 0b11111, 0b11110, 0b11101)
                HAL_WRITE32(addr, 0x18, stackpc + 4);
            } else {
                // 16-bit instruction
                HAL_WRITE32(addr, 0x18, stackpc + 2);
            }

            // clear Hard Fault Status Register
            HAL_WRITE32(0xE000ED2C, 0x0, HAL_READ32(0xE000ED2C, 0x0));
            return;
        }
    }

    __TRAP_HardFaultHandler(addr);
}

void TRAP_HardFaultHandler_Patch(void)
{
    __asm("TST LR, #4                      \n"
          "ITE EQ                          \n"
          "MRSEQ R0, MSP                   \n"
          "MRSNE R0, PSP                   \n"
          "B __TRAP_HardFaultHandler_Patch   ");
}
#endif

<<<<<<< HEAD
// Override original Interrupt Vector Table
INFRA_START_SECTION void TRAP_OverrideTable(uint32_t stackp)
{
    // Override NMI Handler
    NewVectorTable[2] = (VECTOR_Func) TRAP_NMIHandler;

    #if defined ( __ICCARM__ )
    NewVectorTable[3] = (VECTOR_Func) TRAP_HardFaultHandler_Patch;
    #endif
}

INFRA_START_SECTION void PLAT_Init(void)
{
    uint32_t val;

    //Set SPS lower voltage
=======
extern _LONG_CALL_ void * __rtl_memset_v1_00(void * m , int c , size_t n);
// Image2 Entry Function
void PLAT_Init(void)
{
    uint32_t val;

    // Overwrite vector table
    NewVectorTable[2] = (VECTOR_Func) TRAP_NMIHandler;
#if defined ( __ICCARM__ )
    NewVectorTable[3] = (VECTOR_Func) TRAP_HardFaultHandler_Patch;
#endif

    // Clear RAM BSS
#if defined (__ICCARM__)
    __iar_data_init_app();
#endif
    __rtl_memset_v1_00((void *)__bss_sram_start__, 0, __bss_sram_end__ - __bss_sram_start__);
    __rtl_memset_v1_00((void *)__bss_dtcm_start__, 0, __bss_dtcm_end__ - __bss_dtcm_start__);
    __rtl_memset_v1_00((void *)__bss_dram_start__, 0, __bss_dram_end__ - __bss_dram_start__);

    extern HAL_TIMER_OP_EXT HalTimerOpExt;
    __rtl_memset_v1_00((void *)&HalTimerOpExt, 0, sizeof(HalTimerOpExt));
    __rtl_memset_v1_00((void *)&HalTimerOp, 0, sizeof(HalTimerOp));

    HalTimerOpInit_Patch(&HalTimerOp);
    SystemCoreClockUpdate();

    // Set SPS lower voltage
>>>>>>> upstream/master
    val = __RTK_CTRL_READ32(REG_SYS_EFUSE_SYSCFG0);
    val &= 0xf0ffffff;
    val |= 0x6000000;
    __RTK_CTRL_WRITE32(REG_SYS_EFUSE_SYSCFG0, val);
<<<<<<< HEAD

    //xtal buffer driving current
=======
    
    // xtal buffer driving current
>>>>>>> upstream/master
    val = __RTK_CTRL_READ32(REG_SYS_XTAL_CTRL1);
    val &= ~(BIT_MASK_SYS_XTAL_DRV_RF1 << BIT_SHIFT_SYS_XTAL_DRV_RF1);
    val |= BIT_SYS_XTAL_DRV_RF1(1);
    __RTK_CTRL_WRITE32(REG_SYS_XTAL_CTRL1, val);
<<<<<<< HEAD
}

//3 Image 2
extern _LONG_CALL_ void * __rtl_memset_v1_00(void * m , int c , size_t n);

//extern uint32_t mbed_stack_isr_start;
//extern uint32_t mbed_stack_isr_size;
INFRA_START_SECTION void PLAT_Start(void)
{
	u8 isFlashEn;
#if defined ( __ICCARM__ )
    __iar_data_init_app();
#endif
    // Clear RAM BSS
    __rtl_memset_v1_00((void *)__bss_start__, 0, __bss_end__ - __bss_start__);

    TRAP_OverrideTable(0x1FFFFFFC);
/* add by Ian --for mbed isr stack address setting */
		__set_MSP(0x1fffffbc);


#ifdef CONFIG_SPIC_MODULE 
	if ((HAL_PERI_ON_READ32(REG_SOC_FUNC_EN) & BIT_SOC_FLASH_EN) != 0) {
		isFlashEn = 1;
	} else {
		isFlashEn = 0;
	}
#endif

#ifdef CONFIG_TIMER_MODULE
    HalTimerOpInit_Patch(&HalTimerOp);
#endif

	//DBG_8195A("===== Enter Image 2 ====\n");


    SystemCoreClockUpdate();

	if (isFlashEn) {
#if CONFIG_SPIC_EN && SPIC_CALIBRATION_IN_NVM
		SpicNVMCalLoadAll();
#endif
		SpicReadIDRtl8195A();
		// turn off SPIC for power saving
		SpicDisableRtl8195A();	
	}


    PLAT_Init();
#ifdef CONFIG_TIMER_MODULE
	Calibration32k();
		
#ifdef CONFIG_WDG
#ifdef CONFIG_WDG_TEST
		WDGInit();
#endif  //CONFIG_WDG_TEST
#endif  //CONFIG_WDG
#endif  //CONFIG_TIMER_MODULE

#ifdef CONFIG_SOC_PS_MODULE
    //InitSoCPM();
#endif    
    /* GPIOA_7 does not pull high at power on. It causes SDIO Device
     * hardware to enable automatically and occupy GPIOA[7:0] */
=======

    // Initialize SPIC, then disable it for power saving.
    if ((HAL_PERI_ON_READ32(REG_SOC_FUNC_EN) & BIT_SOC_FLASH_EN) != 0) {
        SpicNVMCalLoadAll();
        SpicReadIDRtl8195A();
        SpicDisableRtl8195A();
    }

#ifdef CONFIG_TIMER_MODULE
    Calibration32k();
#endif

>>>>>>> upstream/master
#ifndef CONFIG_SDIO_DEVICE_EN
    SDIO_DEV_Disable();
#endif

    // Enter App start function
    PLAT_Main();
}

extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);

<<<<<<< HEAD
#if defined (__CC_ARM)
__asm void ARM_PLAT_Main(void)
{
	IMPORT  SystemInit
	IMPORT  __main
   	BL	SystemInit
	BL  __main
}
#endif

extern void __iar_program_start( void );
// The Main App entry point
=======
// The Main App entry point
#if defined (__CC_ARM)
__asm void ARM_PLAT_Main(void)
{
    IMPORT  SystemInit
    IMPORT  __main
    BL      SystemInit
    BL  __main
}
#elif defined (__ICCARM__)
extern void __iar_program_start(void);

void IAR_PLAT_Main(void)
{
    SystemInit();
    __iar_program_start();
}
#endif

>>>>>>> upstream/master
void PLAT_Main(void)
{
    TRAP_Init((void *)SVC_Handler, (void *)PendSV_Handler, (void *)SysTick_Handler);

<<<<<<< HEAD
#if defined (__ICCARM__)
	//IAR_PLAT_Main();
	SystemInit();
    __iar_program_start();
#elif defined (__CC_ARM)
	ARM_PLAT_Main();

#elif defined (__GNUC__)
    __asm (
    		"ldr	r0,	=SystemInit	\n"
           "blx  r0                \n"
           "ldr  r0, =_start       \n"
           "bx   r0                \n"
    );
#endif
    // Never reached
    for(;;);
=======
#if defined (__CC_ARM)
    ARM_PLAT_Main();
#elif defined (__ICCARM__)
    IAR_PLAT_Main();
#else
    __asm ("ldr  r0, =SystemInit   \n"
           "blx  r0                \n"
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
           "ldr  r0, =__main \n"
#else
           "ldr  r0, =_start       \n"
#endif
           "bx   r0                \n"
    );
#endif

    // Never reached
    for (;;);
>>>>>>> upstream/master
}
