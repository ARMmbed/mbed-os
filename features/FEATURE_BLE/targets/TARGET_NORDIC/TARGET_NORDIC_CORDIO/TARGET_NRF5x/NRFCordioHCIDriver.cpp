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
#include "rtos/source/rtos_idle.h"
#include "platform/mbed_power_mgmt.h"
#include "mbed_critical.h"

// Cordio Includes
#include "ll_init_api.h"
#include "ll_defs.h"
#include "fake_lhci_drv.h"
#include "pal_bb.h"
#include "pal_cfg.h"
#include "lhci_api.h"
#include "wsf_assert.h"
#include "wsf_buf.h"
#include "wsf_timer.h"
#include "wsf_trace.h"
#include "dm_api.h"

// Nordic Includes
#include "nrf.h"
#include "nrf5x_lf_clk_helper.h"

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
#if defined(NRF52840_XXAA)

#undef MBED_CONF_CORDIO_LL_EXTENDED_ADVERTISING_SIZE
#undef MBED_CONF_CORDIO_LL_MAX_ACL_SIZE
#undef MBED_CONF_CORDIO_LL_TX_BUFFERS
#undef MBED_CONF_CORDIO_LL_PHY_CODED_SUPPORT
#define MBED_CONF_CORDIO_LL_EXTENDED_ADVERTISING_SIZE MBED_CONF_CORDIO_LL_NRF52840_EXTENDED_ADVERTISING_SIZE
#define MBED_CONF_CORDIO_LL_MAX_ACL_SIZE              MBED_CONF_CORDIO_LL_NRF52840_MAX_ACL_SIZE
#define MBED_CONF_CORDIO_LL_TX_BUFFERS                MBED_CONF_CORDIO_LL_NRF52840_TX_BUFFERS
#define MBED_CONF_CORDIO_LL_PHY_CODED_SUPPORT         MBED_CONF_CORDIO_LL_NRF52840_PHY_CODED_SUPPORT

#define CORDIO_LL_MEMORY_FOOTPRINT  15400UL

#else

#define CORDIO_LL_MEMORY_FOOTPRINT  12500UL

#endif

/*! \brief      Typical implementation revision number (LlRtCfg_t::implRev). */
#define LL_IMPL_REV             0x2303

#if MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC == NRF_LF_SRC_SYNTH
#define NRF_LF_CLK_SRC CLOCK_LFCLKSRC_SRC_Synth
#elif MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC == NRF_LF_SRC_XTAL
#define NRF_LF_CLK_SRC CLOCK_LFCLKSRC_SRC_Xtal
#elif MBED_CONF_NORDIC_NRF_LF_CLOCK_SRC == NRF_LF_SRC_RC
#define NRF_LF_CLK_SRC CLOCK_LFCLKSRC_SRC_RC
#endif

// Note to implementer: this should be amended if the Cordio stack is updated

// The Nordic-specific baseband configuration
// The BB_ config macros are set in the bb_api.h header file
const BbRtCfg_t NRFCordioHCIDriver::_bb_cfg = {
    /*clkPpm*/ 20,
    /*rfSetupDelayUsec*/ BB_RF_SETUP_DELAY_US,
    /*maxScanPeriodMsec*/ BB_MAX_SCAN_PERIOD_MS,
    /*schSetupDelayUsec*/ BB_SCH_SETUP_DELAY_US
};

/* +12 for message headroom, + 2 event header, +255 maximum parameter length. */
static const uint16_t maxRptBufSize = 12 + 2 + 255;

/* +12 for message headroom, +4 for header. */
static const uint16_t aclBufSize = 12 + MBED_CONF_CORDIO_LL_MAX_ACL_SIZE + 4 + BB_DATA_PDU_TAILROOM;

const LlRtCfg_t NRFCordioHCIDriver::_ll_cfg = {
/* Device */
/** Company ID (default to ARM Ltd. ID). */
/*uint16_t*/  .compId = LL_COMP_ID_ARM,
/** Implementation revision number. */
/*uint16_t*/  .implRev = LL_IMPL_REV,
/** Core specification implementation level (LL_VER_BT_CORE_SPEC_4_2). */
/*uint8_t*/   .btVer = LL_VER_BT_CORE_SPEC_5_0,
/** Unused. Align next field to word boundary. */
/*uint32_t*/  ._align32 = 0,

/* Advertiser */
/** Maximum number of advertising sets. */
/*uint8_t*/   .maxAdvSets = MBED_CONF_CORDIO_LL_MAX_ADVERTISING_SETS,
/** Maximum number of pending legacy or extended advertising reports. */
/*uint8_t*/   .maxAdvReports = MBED_CONF_CORDIO_LL_MAX_ADVERTISING_REPORTS,
/** Maximum extended advertising data size. */
/*uint16_t*/  .maxExtAdvDataLen = MBED_CONF_CORDIO_LL_EXTENDED_ADVERTISING_SIZE,
/** Default extended advertising data fragmentation size. */
/*uint8_t*/   .defExtAdvDataFrag = MBED_CONF_CORDIO_LL_DEFAULT_EXTENDED_ADVERTISING_FRAGMENTATION_SIZE,
/** Auxiliary Offset delay above T_MAFS in microseconds. */
/*uint16_t*/  .auxDelayUsec = 0,
/** Delay of auxiliary packet in microseconds from the time specified by auxPtr. */
/*uint16_t*/  .auxPtrOffsetUsec = 0,

/* Scanner */
/** Maximum scan request received events. */
/*uint8_t*/   .maxScanReqRcvdEvt = MBED_CONF_CORDIO_LL_MAX_SCAN_REQUEST_EVENTS,
/** Maximum extended scan data size. */
/*uint16_t*/  .maxExtScanDataLen = MBED_CONF_CORDIO_LL_EXTENDED_ADVERTISING_SIZE,

/* Connection */
/** Maximum number of connections. */
/*uint8_t*/   .maxConn = DM_CONN_MAX,
/** Default number of transmit buffers. */
/*uint8_t*/   .numTxBufs = MBED_CONF_CORDIO_LL_TX_BUFFERS,
/** Default number of receive buffers. */
/*uint8_t*/   .numRxBufs = MBED_CONF_CORDIO_LL_RX_BUFFERS,
/** Maximum ACL buffer size. */
/*uint16_t*/  .maxAclLen = MBED_CONF_CORDIO_LL_MAX_ACL_SIZE,
/** Default Tx power level for connections. */
/*int8_t*/    .defTxPwrLvl = 0,
/** Allowable CE jitter on a slave (account for master's sleep clock resolution). */
/*uint8_t*/   .ceJitterUsec = 0,

/* ISO */
/** Default number of ISO transmit buffers. */
/*uint8_t*/   .numIsoTxBuf = 0,
/** Default number of ISO receive buffers. */
/*uint8_t*/   .numIsoRxBuf = 0,
/** Maximum ISO buffer size between host and controller. */
/*uint16_t*/  .maxIsoBufLen = 0,
/** Maximum ISO PDU buffer size. */
/*uint16_t*/  .maxIsoPduLen = 0,

/* CIS */
/** Maximum number of CIG. */
/*uint8_t*/   .maxCig = 0,
/** Maximum number of CIS. */
/*uint8_t*/   .maxCis = 0,
/** Subevent spacing above T_MSS. */
/*uint16_t*/  .subEvtSpaceDelay = 0,
/* DTM */
/** DTM Rx synchronization window in milliseconds. */
/*uint16_t*/  .dtmRxSyncMs = 10000,

/* PHY */
/** 2M PHY supported. */
/*bool_t*/    .phy2mSup = MBED_CONF_CORDIO_LL_PHY_2M_SUPPORT,
/** Coded PHY supported. */
/*bool_t*/    .phyCodedSup = MBED_CONF_CORDIO_LL_PHY_CODED_SUPPORT,
/** Tx stable modulation index supported. */
/*bool_t*/    .stableModIdxTxSup = TRUE,
/** Rx stable modulation index supported. */
/*bool_t*/    .stableModIdxRxSup = TRUE,
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
        #if defined(NRF52840_XXAA)
        uint8_t buffer[ 4900 ];
        #else
        uint8_t buffer[ 4900 ];
        #endif
        uint64_t align;
    };
    static const wsfBufPoolDesc_t pool_desc[] = {
            {  16, 16  + 8},
            {  32, 16 + 4 },
            {  64, 8 },
            { 128, 4 + MBED_CONF_CORDIO_LL_MAX_ADVERTISING_REPORTS },
		    { aclBufSize, MBED_CONF_CORDIO_LL_TX_BUFFERS + MBED_CONF_CORDIO_LL_RX_BUFFERS }
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
    NRF_CLOCK->LFCLKSRC             = (NRF_LF_CLK_SRC << CLOCK_LFCLKSRC_SRC_Pos);
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
    PalCfgLoadData(PAL_CFG_ID_BD_ADDR, bd_addr, sizeof(bdAddr_t));
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

void NRFCordioHCIDriver::start_reset_sequence()
{
    // Make sure extended adv is init
    DmExtAdvInit();

    CordioHCIDriver::start_reset_sequence();
}

bool NRFCordioHCIDriver::get_random_static_address(ble::address_t& address)
{
    PalCfgLoadData(PAL_CFG_ID_BD_ADDR, address.data(), sizeof(bdAddr_t));

    MBED_ASSERT((address[5] & 0xC0) == 0xC0);

    return true;
}

ble::vendor::cordio::CordioHCIDriver& ble_cordio_get_hci_driver() {
    static NRFCordioHCITransportDriver transport_driver;

    static NRFCordioHCIDriver hci_driver(
        transport_driver
    );

    return hci_driver;
}
