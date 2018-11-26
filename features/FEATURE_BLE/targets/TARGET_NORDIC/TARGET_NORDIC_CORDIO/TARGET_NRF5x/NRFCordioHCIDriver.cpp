/* mbed Microcontroller Library
 * Copyright (c) 2017-2017 ARM Limited
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

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// mbed Includes
#include "mbed_assert.h"
#include "rtos/rtos_idle.h"
#include "platform/mbed_power_mgmt.h"
#include "mbed_critical.h"

// Cordio Includes
#include "ll_init_api.h"
#include "ll_defs.h"
#include "chci_drv.h"
#include "lhci_api.h"
#include "platform_api.h"
#include "platform_ble_api.h"
#include "wsf_assert.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"

// Nordic Includes
#include "nrf.h"

#include "NRFCordioHCIDriver.h"
#include "NRFCordioHCITransportDriver.h"

#ifdef DEBUG
#include <stdio.h>
#include <stdarg.h>
#define DBG_WARN(...) printf(__VA_ARGS__)
#else
#define DBG_WARN(...)
#endif

using namespace ble::vendor::nordic;
using namespace ble::vendor::cordio;

/*! \brief      Memory that should be reserved for the stack. */
#define CORDIO_LL_MEMORY_FOOTPRINT 3776UL

/*! \brief      Typical implementation revision number (LlRtCfg_t::implRev). */
#define LL_IMPL_REV             0x2303

// Note to implementer: this should be amended if the Cordio stack is updated

// The Nordic-specific baseband configuration
// The BB_ config macros are set in the bb_api.h header file
const BbRtCfg_t NRFCordioHCIDriver::_bb_cfg = {
    /*clkPpm*/ 20,
    /*rfSetupDelayUsec*/ BB_RF_SETUP_DELAY_US,
    /*maxScanPeriodMsec*/ BB_MAX_SCAN_PERIOD_MS,
    /*schSetupDelayUsec*/ BB_SCH_SETUP_DELAY_US
};

static const uint16_t maxAdvReports = 16;
static const uint16_t numRxBufs      = 8;
#if !defined(NRF52840_XXAA)
static const uint16_t advDataLen     = 128;
static const uint16_t connDataLen    = 256;
static const uint16_t numTxBufs      = 8;
#else
static const uint16_t advDataLen     = LL_MAX_ADV_DATA_LEN;
static const uint16_t connDataLen    = 512;
static const uint16_t numTxBufs      = 16;
#endif


/* +12 for message headroom, + 2 event header, +255 maximum parameter length. */
static const uint16_t maxRptBufSize = 12 + 2 + 255;

/* +12 for message headroom, +4 for header. */
static const uint16_t aclBufSize = 12 + connDataLen + 4 + BB_DATA_PDU_TAILROOM;

const LlRtCfg_t NRFCordioHCIDriver::_ll_cfg = {
    /* Device */
    /*compId*/         			LL_COMP_ID_ARM,
    /*implRev*/					LL_IMPL_REV,
    /*btVer*/         			LL_VER_BT_CORE_SPEC_4_2,
    0, // padding 
    /* Advertiser */
    /*maxAdvSets*/         		0,   /* Disable extended advertising. */
    /*maxAdvReports*/         	4,
    /*maxExtAdvDataLen*/         advDataLen,
    /*defExtAdvDataFrag*/        64,
    0, // Aux delay
    /* Scanner */
    /*maxScanReqRcvdEvt*/         4,
    /*maxExtScanDataLen*/         advDataLen,
    /* Connection */
    /*maxConn*/          4,
    /*numTxBufs*/          numTxBufs,
    /*numRxBufs*/          numRxBufs,
    /*maxAclLen*/          connDataLen,
    /*defTxPwrLvl*/          0,
    /*ceJitterUsec*/          0,
    /* DTM */
    /*dtmRxSyncMs*/          10000,
    /* PHY */
    /*phy2mSup*/          TRUE,
    /*phyCodedSup*/          TRUE,
    /*stableModIdxTxSup*/         TRUE,
    /*stableModIdxRxSup*/          TRUE
};

extern "C" void TIMER0_IRQHandler(void);

static void idle_hook(void)
{
	wsfTimerTicks_t nextExpiration;
	bool_t          timerRunning;

	nextExpiration = WsfTimerNextExpiration(&timerRunning);
	if(timerRunning && nextExpiration > 0)
	{
		// Make sure we hae enough time to go to sleep
		if( nextExpiration < 1 /* 10 ms per tick which is long enough to got to sleep */ )
		{
			// Bail
			return;
		}
	}

    // critical section to complete sleep with locked deepsleep
    core_util_critical_section_enter();
    sleep_manager_lock_deep_sleep();
    sleep();
    sleep_manager_unlock_deep_sleep();
    core_util_critical_section_exit();
}

NRFCordioHCIDriver::NRFCordioHCIDriver(CordioHCITransportDriver& transport_driver) : cordio::CordioHCIDriver(transport_driver), _is_init(false), _stack_buffer(NULL)
{
    _stack_buffer = (uint8_t*)malloc(CORDIO_LL_MEMORY_FOOTPRINT);
    MBED_ASSERT(_stack_buffer != NULL);
}

NRFCordioHCIDriver::~NRFCordioHCIDriver()
{
	// Deativate all interrupts
	NVIC_DisableIRQ(RADIO_IRQn);
	NVIC_DisableIRQ(TIMER0_IRQn);

    // Switch off Radio peripheral
    // TODO interop with 802.15.4
    NRF_RADIO->POWER = 0;

    // Stop timer
    NRF_TIMER0->TASKS_STOP  = 1;
    NRF_TIMER0->TASKS_CLEAR = 1;

	NVIC_ClearPendingIRQ(RADIO_IRQn);
	NVIC_ClearPendingIRQ(TIMER0_IRQn);

    MBED_ASSERT(_stack_buffer != NULL);
    free(_stack_buffer);
    _stack_buffer = NULL;

    // Restore RTOS idle thread
	rtos_attach_idle_hook(NULL);

    MBED_ASSERT(_stack_buffer == NULL);
}

ble::vendor::cordio::buf_pool_desc_t NRFCordioHCIDriver::get_buffer_pool_description()
{
    static union {
        uint8_t buffer[ 17304 ];
        uint64_t align;
    };
    static const wsfBufPoolDesc_t pool_desc[] = {
            {  16, 16  + 8},
            {  32, 16 + 4 },
            {  64, 8 },
            { 128, 4 + maxAdvReports },
		    { aclBufSize, numTxBufs + numRxBufs },
            { 272, 1 }
    };

    return buf_pool_desc_t(buffer, pool_desc);
}

void NRFCordioHCIDriver::do_initialize()
{
	if(_is_init) {
		return;
	}

	_is_init = true;

    // Setup BB & LL config
    LlInitRtCfg_t ll_init_cfg =
    {
        .pBbRtCfg     = &_bb_cfg,
        .wlSizeCfg    = 4,
        .rlSizeCfg    = 4,
        .plSizeCfg    = 4,
        .pLlRtCfg     = &_ll_cfg,
        .pFreeMem     = _stack_buffer,
        .freeMemAvail = CORDIO_LL_MEMORY_FOOTPRINT
    };

    // Override RTOS idle thread
    rtos_attach_idle_hook(idle_hook);

    /* switch to more accurate 16 MHz crystal oscillator (system starts up using 16MHz RC oscillator) */
    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART    = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0)
    {
    }

    /* configure low-frequency clock */
    NRF_CLOCK->LFCLKSRC             = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED  = 0;
    NRF_CLOCK->TASKS_LFCLKSTART     = 1;
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
    }
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;

    // Start RTC0
    NRF_RTC0->TASKS_STOP  = 1;
    NRF_RTC0->TASKS_CLEAR = 1;
    NRF_RTC0->PRESCALER   = 0;            /* clear prescaler */
    NRF_RTC0->TASKS_START = 1;

    // Cycle radio peripheral power to guarantee known radio state
    NRF_RADIO->POWER = 0;
    NRF_RADIO->POWER = 1;

    // For some reason, the mbed target uses this (TIMER0_IRQHandler_v) vector name instead of the "standard" TIMER0_IRQHandler one
    NVIC_SetVector(TIMER0_IRQn, (uint32_t)TIMER0_IRQHandler);

    // Extremely ugly
    for(uint32_t irqn = 0; irqn < 32; irqn++)
    {
    		uint8_t prio = NVIC_GetPriority((IRQn_Type)irqn);
    		if( prio < 2 ) {
    			NVIC_SetPriority((IRQn_Type)irqn, 2);
    		}
    }

    // WARNING
    // If a submodule does not have enough space to allocate its memory from buffer, it will still allocate its memory (and do a buffer overflow) and return 0 (as in 0 byte used)
    // however that method will still continue which will lead to undefined behaviour
    // So whenever a change of configuration is done, it's a good idea to set CORDIO_LL_MEMORY_FOOTPRINT to a high value and then reduce accordingly
    uint32_t mem_used = LlInitControllerInit(&ll_init_cfg);
    if( mem_used < CORDIO_LL_MEMORY_FOOTPRINT )
    {
        // Sub-optimal, give warning
        DBG_WARN("NRFCordioHCIDriver: CORDIO_LL_MEMORY_FOOTPRINT can be reduced to %lu instead of %lu", mem_used, CORDIO_LL_MEMORY_FOOTPRINT);
    }

    // BD Addr
    bdAddr_t bd_addr;
    PlatformLoadBdAddress(bd_addr);
    LlSetBdAddr((uint8_t *)&bd_addr);
    LlMathSetSeed((uint32_t *)&bd_addr);

//#ifdef DEBUG
//    WsfTraceRegister(wsf_trace_handler);
//#endif

    // We're sharing the host stack's event queue
}

void NRFCordioHCIDriver::do_terminate()
{

}

ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver() { 
    static NRFCordioHCITransportDriver transport_driver;

    static NRFCordioHCIDriver hci_driver(
        transport_driver
    );

    return hci_driver;
}

// Do not handle any vendor specific command
extern "C" bool_t lhciCommonVsStdDecodeCmdPkt(LhciHdr_t *pHdr, uint8_t *pBuf)
{
    return false;
}

// Nordic implementation
void PlatformLoadBdAddress(uint8_t *pDevAddr)
{
  unsigned int devAddrLen = 6;

  /* Load address from nRF configuration. */
  uint64_t devAddr = (((uint64_t)NRF_FICR->DEVICEID[0]) <<  0) |
                     (((uint64_t)NRF_FICR->DEVICEID[1]) << 32);

  unsigned int i = 0;
  while (i < devAddrLen)
  {
    pDevAddr[i] = devAddr >> (i * 8);
    i++;
  }

  pDevAddr[5] |= 0xC0;     /* cf. "Static Address" (Vol C, Part 3, section 10.8.1) */
}
