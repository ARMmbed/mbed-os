/******************************************************************************
 * @file     system_RZ_A2M_H.c
 * @brief    CMSIS Device System Source File for ARM Cortex-A9 Device Series
 * @version  V1.00
 * @date     10 Mar 2017
 *
 * @note
 *
 ******************************************************************************/
/*
 * Copyright (c) 2013-2020 Renesas Electronics Corporation. All rights reserved.
 * Copyright (c) 2009-2020 ARM Limited. All rights reserved.
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

#include "RZ_A2M.h"
#include "RZ_A2_Init.h"
#include "irq_ctrl.h"
#include "mbed_drv_cfg.h"
#include "r_cache_lld_rza2m.h"

extern void HyperRAM_Init(void);
extern void R_OCTABUS_Setup(void);
extern void r_octabus_memclk_setup(void);
/*
 Port 0 (P0) MD pin assignment
 P0_0: MD_BOOT0
 P0_1: MD_BOOT1
 P0_2: MD_CLK
 P0_3: MD_CLKS
 */

#include "rza_io_regrw.h"
#include "iobitmask.h"
#include "rtc_iobitmask.h"
#include "usb_iobitmask.h"

#define STARTUP_CFG_USB_PHY_PLL_DELAY_COUNT (500)

#define IOREG_NONMASK_ACCESS    (0xFFFFFFFFuL)
#define IOREG_NONSHIFT_ACCESS   (0)

#define R_PRV_RTC_COUNT (2)
#define R_PRV_USB_COUNT (2)

static void disable_rtc(uint32_t ch)
{
    uint8_t dummy8;
    uint32_t mask;
    uint8_t shift;
    static volatile struct st_rtc *const rtc[R_PRV_RTC_COUNT] = {
        &RTC0, &RTC1
    };
    static const uint8_t mstp_mask[R_PRV_RTC_COUNT] = {
        CPG_STBCR5_MSTP53, CPG_STBCR5_MSTP52
    };
    static const uint8_t mstp_shift[R_PRV_RTC_COUNT] = {
        CPG_STBCR5_MSTP53_SHIFT, CPG_STBCR5_MSTP52_SHIFT
    };
    static const uint16_t rtcxtalsel_mask[R_PRV_RTC_COUNT] = {
        PMG_RTCXTALSEL_RTC0XT, PMG_RTCXTALSEL_RTC1XT
    };
    static const uint8_t rtcxtalsel_shift[R_PRV_RTC_COUNT] = {
        PMG_RTCXTALSEL_RTC0XT_SHIFT, PMG_RTCXTALSEL_RTC1XT_SHIFT
    };

    /* channel check */
    if (ch >= R_PRV_RTC_COUNT) {
        return;
    }

    /* 1: select RTCXTAL for RTC (RCR4.RCKSEL = 0) */
    RZA_IO_RegWrite_8(&rtc[ch]->RCR4.BYTE, 0, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
    RZA_IO_RegRead_8(&rtc[ch]->RCR4.BYTE, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /* 2: disable RTC clock (RCR3.RTCEN = 0) */
    RZA_IO_RegWrite_8(&rtc[ch]->RCR3.BYTE, 0, RTC_RCR3_RTCEN_SHIFT, RTC_RCR3_RTCEN);

    /* Wait for successfully disabled */
    dummy8 = 1;
    while (0 != dummy8) {
        dummy8 = RZA_IO_RegRead_8(&rtc[ch]->RCR3.BYTE, RTC_RCR3_RTCEN_SHIFT, RTC_RCR3_RTCEN);
    }

    /* 3: disable RTC clock while standby mode */
    mask  = rtcxtalsel_mask[ch];
    shift = rtcxtalsel_shift[ch];
    RZA_IO_RegWrite_16(&PMG.RTCXTALSEL.WORD, 0, shift, mask);
    RZA_IO_RegRead_16(&PMG.RTCXTALSEL.WORD, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /* 4: Stop RTC module */
    mask  = mstp_mask[ch];
    shift = mstp_shift[ch];
    RZA_IO_RegWrite_8(&CPG.STBCR5.BYTE, 1, shift, mask);
    RZA_IO_RegRead_8(&CPG.STBCR5.BYTE, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
}

static void disable_usb(uint32_t ch)
{
    uint8_t dummy8;
    uint32_t mask;
    uint8_t shift;

    /* USB Host IO reg Top Address(ch0, ch1) */
    static volatile struct st_usb00 *const usb00_host[R_PRV_USB_COUNT] = {
        &USB00, &USB10
    };

    /* USB Function IO reg Top Address(ch0, ch1) */
    static volatile struct st_usb01 *const usb01_func[R_PRV_USB_COUNT] = {
        &USB01, &USB11
    };

    /* MSTP */
    static const uint8_t mstp_mask[R_PRV_USB_COUNT] = {
        CPG_STBCR6_MSTP61, CPG_STBCR6_MSTP60
    };
    static const uint8_t mstp_shift[R_PRV_USB_COUNT] = {
        CPG_STBCR6_MSTP61_SHIFT, CPG_STBCR6_MSTP60_SHIFT
    };

    /* STBREQ */
    static const uint8_t stbreq_mask[R_PRV_USB_COUNT] = {
        (CPG_STBREQ3_STBRQ31 | CPG_STBREQ3_STBRQ30), (CPG_STBREQ3_STBRQ33 | CPG_STBREQ3_STBRQ32)
    };
    static const uint8_t stbreq_shift[R_PRV_USB_COUNT] = {
        CPG_STBREQ3_STBRQ30_SHIFT, CPG_STBREQ3_STBRQ32_SHIFT
    };

    /* STBACK */
    static const uint8_t stback_mask[R_PRV_USB_COUNT] = {
        (CPG_STBACK3_STBAK31 | CPG_STBACK3_STBAK30), (CPG_STBACK3_STBAK33 | CPG_STBACK3_STBAK32)
    };
    static const uint8_t stback_shift[R_PRV_USB_COUNT] = {
        CPG_STBACK3_STBAK30_SHIFT, CPG_STBACK3_STBAK32_SHIFT
    };

    /* channel check */
    if (ch >= R_PRV_USB_COUNT) {
        return;
    }

    /* 1: Start USB module */

    /* MSTP = 0 */
    mask  = mstp_mask[ch];
    shift = mstp_shift[ch];
    RZA_IO_RegWrite_8(&CPG.STBCR6.BYTE, 0, shift, mask);
    RZA_IO_RegRead_8(&CPG.STBCR6.BYTE, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /* STBREQ = 0 */
    mask  = stbreq_mask[ch];
    shift = stbreq_shift[ch];
    RZA_IO_RegWrite_8(&CPG.STBREQ3.BYTE, 0x0, shift, mask);
    RZA_IO_RegRead_8(&CPG.STBREQ3.BYTE, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /* check STBACK = 0 */
    mask  = stback_mask[ch];
    shift = stback_shift[ch];
    dummy8 = 0x3;
    while (0x0 != dummy8) {
        dummy8 = RZA_IO_RegRead_8(&CPG.STBACK3.BYTE, shift, mask);
    }
    (void)dummy8;

    /* 2: Set the clock supplied to USBPHY to EXTAL clock (PHYCLK_CTRL.UCLKSEL = 0) */
    RZA_IO_RegWrite_32(&usb00_host[ch]->PHYCLK_CTRL.LONG, 0, USB_PHYCLK_CTRL_UCLKSEL_SHIFT, USB_PHYCLK_CTRL_UCLKSEL);

    /* 3: It can recover from deep standby by DP, DM change (PHYIF_CTRL.FIXPHY = 1) */
    RZA_IO_RegWrite_32(&usb00_host[ch]->PHYIF_CTRL.LONG, 1, USB_PHYIF_CTRL_FIXPHY_SHIFT, USB_PHYIF_CTRL_FIXPHY);

    /* 4: UTMI+PHY Normal Mode (LPSTS.SUSPM = 1) */
    RZA_IO_RegWrite_16(&usb01_func[ch]->LPSTS.WORD, 1, USB_LPSTS_SUSPM_SHIFT, USB_LPSTS_SUSPM);

    /* 5: UTMI + reset release (USBCTR.PLL_RST = 0) */
    RZA_IO_RegWrite_32(&usb00_host[ch]->USBCTR.LONG, 0, USB_USBCTR_PLL_RST_SHIFT, USB_USBCTR_PLL_RST);

    /* 6: wait 200us delay(Waiting for oscillation stabilization of USBPHY built-in PLL) */
    for (volatile int i = 0; i < STARTUP_CFG_USB_PHY_PLL_DELAY_COUNT; i++) {
        ;
    }

    /* 7: Pulldown resistance control is effective (LINECTRL1 = 0x000A0000) */
    RZA_IO_RegWrite_32(
        &usb00_host[ch]->LINECTRL1.LONG,
        (USB_LINECTRL1_DPRPD_EN | USB_LINECTRL1_DMRPD_EN),
        IOREG_NONSHIFT_ACCESS,
        IOREG_NONMASK_ACCESS);

    /* 8: USBPHY standby mode (USBCTR.DIRPD = 1) */
    RZA_IO_RegWrite_32(&usb00_host[ch]->USBCTR.LONG, 1, USB_USBCTR_DIRPD_SHIFT, USB_USBCTR_DIRPD);

    /* 9: Stop USB module */

    /* STBREQ = 1 */
    mask  = stbreq_mask[ch];
    shift = stbreq_shift[ch];
    RZA_IO_RegWrite_8(&CPG.STBREQ3.BYTE, 0x3, shift, mask);
    RZA_IO_RegRead_8(&CPG.STBREQ3.BYTE, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);

    /* check STBACK = 1 */
    mask  = stback_mask[ch];
    shift = stback_shift[ch];
    dummy8 = 0x0;
    while (0x3 != dummy8) {
        dummy8 = RZA_IO_RegRead_8(&CPG.STBACK3.BYTE, shift, mask);
    }
    (void)dummy8;

    /* MSTP = 1 */
    mask  = mstp_mask[ch];
    shift = mstp_shift[ch];
    RZA_IO_RegWrite_8(&CPG.STBCR6.BYTE, 1, shift, mask);
    RZA_IO_RegRead_8(&CPG.STBCR6.BYTE, IOREG_NONSHIFT_ACCESS, IOREG_NONMASK_ACCESS);
}


/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = CM0_RENESAS_RZ_A2_I_CLK;

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate(void)
{
}

/*----------------------------------------------------------------------------
  IRQ Handler Register/Unregister
 *----------------------------------------------------------------------------*/
uint32_t InterruptHandlerRegister(IRQn_Type irq, IRQHandler handler)
{
    return IRQ_SetHandler(irq, handler);
}

uint32_t InterruptHandlerUnregister(IRQn_Type irq)
{
    return IRQ_SetHandler(irq, (IRQHandler_t)NULL);
}

/*----------------------------------------------------------------------------
  System Initialization
 *----------------------------------------------------------------------------*/
void SystemInit(void)
{
    /*       do not use global variables because this function is called before
             reaching pre-main. RW section may be overwritten afterwards.          */
#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    // Enable FPU
    __FPU_Enable();
#endif

    volatile uint8_t  dummy_buf_8b;

    // Enable SRAM write access
    CPG.SYSCR3.BYTE = 0x0F;
    dummy_buf_8b = CPG.SYSCR3.BYTE;
    (void)dummy_buf_8b;

    RZ_A2_InitClock();
    RZ_A2_InitBus();
    r_octabus_memclk_setup();

    R_OCTABUS_Setup();
#if defined(USE_HYPERRAM)
    HyperRAM_Init();
#endif

    // Invalidate entire Unified TLB
    __set_TLBIALL(0);

    // Invalidate entire branch predictor array
    __set_BPIALL(0);
    __DSB();
    __ISB();

    //  Invalidate instruction cache and flush branch target cache
    __set_ICIALLU(0);
    __DSB();
    __ISB();

    //  Invalidate data cache
    L1C_InvalidateDCacheAll();

    // Create Translation Table
    MMU_CreateTranslationTable();

    // Enable MMU
    MMU_Enable();

#if (__L2C_PRESENT == 1)
    /* Initial setting of the level 2 cache */
    R_CACHE_L2Init();

    /* DRP L2 Cache ON */
    PRR.AXIBUSCTL4.BIT.DRPARCACHE = 0xF;
    PRR.AXIBUSCTL4.BIT.DRPAWCACHE = 0xF;
#endif

    // IRQ Initialize
    IRQ_Initialize();

    disable_rtc(0);
    if (RTC_BCNT1.RCR2.BIT.START == 0) {
        disable_rtc(1);
    }
    disable_usb(0);
    disable_usb(1);

    volatile uint16_t  dummy_buf_16b;

    // Clear the IOKEEP bit in DSFR
    dummy_buf_16b = PMG.DSFR.WORD;
    PMG.DSFR.BIT.IOKEEP = 0;
    dummy_buf_16b = PMG.DSFR.WORD;
    (void)dummy_buf_16b;
}

void mbed_sdk_init(void)
{
    /* Initial setting of the level 1 cache */
    R_CACHE_L1Init();
}

void soft_reset(void)
{
    volatile uint16_t data;
    WDT.WTCNT.WORD = 0x5A00;
    data = WDT.WRCSR.WORD;
    (void)data;
    WDT.WTCNT.WORD = 0x5A00;
    WDT.WRCSR.WORD = 0xA500;
    WDT.WTCSR.WORD = 0xA578;
    WDT.WRCSR.WORD = 0x5A40;

    while (1) {}
}

