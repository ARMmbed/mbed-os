/*
 * mbed Microcontroller Library
 * Copyright (c) 2017-2018 Future Electronics
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

#include "psoc6_utils.h"

#if defined(__MBED__)

#include "mbed_critical.h"
#include "mbed_error.h"

#else

/** Adaptation layer to native Cypress environment */
/* Notice, that since we use critical section here only for operations
 * that do not involve function calls, we can get away with using
 * a global static variable for interrupt status saving.
 */

#include "cy_syslib.h"

#define error(arg)  CY_ASSERT(0)
#define MBED_ASSERT CY_ASSERT

#define core_util_critical_section_enter() \
    uint32_t _last_irq_status_ = Cy_SysLib_EnterCriticalSection()

#define core_util_critical_section_exit() \
     Cy_SysLib_ExitCriticalSection(_last_irq_status_)

#endif /* defined(__MBED__) */


#define CY_NUM_PSOC6_PORTS      14
#define CY_NUM_DIVIDER_TYPES    4
#define NUM_SCB                 8
#define NUM_TCPWM               32


#if defined(TARGET_MCU_PSOC6_M0) || PSOC6_DYNSRM_DISABLE || !defined(__MBED__)

/****************************************************************************/
/*                   Dynamic Shared Resource Manager                        */
/****************************************************************************/
/*
 * This part of the code is responsible for management of the hardware
 * resource shared between both CPUs of the PSoC 6.
 * It supports allocation, freeing and conflict detection, so that never
 * both CPUs try to use a single resource.
 * It also detects conflicts arising from allocation of hardware devices
 * for different modes of operation and when user tries to assign multiple
 * functions to the same chip pin.
 * It supports two modes of operation:
 * 1. DYNAMIC (default mode)
 *    Resource manager is run on M0 core and M4 core asks it to allocate
 *    or free resources using RPC over IPC mechanism.
 *    M0 core communicates with manager via local function calls.
 * 2. STATIC (enabled with PSOC6_DYNSRM_DISABLE compilation flag)
 *    In this mode resources are split statically between both cores.
 *    Targets using this mode should add psoc6_static_srm.h file to
 *    each core folder with declarations of resources assigned to it.
 *    See example file for details.
 */


#if PSOC6_DYNSRM_DISABLE

#define SRM_INIT_RESOURCE(_type_, _res_, _field_, ...)                   \
do {                                                            \
    struct _init_s_ {                                           \
        uint8_t idx;                                            \
        _type_ val;                                             \
    } init[] = {{0, 0}, __VA_ARGS__};                           \
    uint32_t i;                                                 \
    for (i = 1; i < sizeof(init)/sizeof(struct _init_s_); ++i)  \
        _res_[init[i].idx]_field_ = init[i].val;                \
} while(0)

#if defined(TARGET_MCU_PSOC6_M0)
/*
 * On M0 we start with all resources assigned to M4 and then clear reservations
 * for those assigned to it (M0).
 */
#define SRM_PORT(port, pins)               {(port), (uint8_t)~(pins)}
#define SRM_DIVIDER(type, dividers)        {(type), (uint16_t)~(dividers)}
#define SRM_SCB(num)                       {(num), (0)}
#define SRM_TCPWM(num)                     {(num), (0)}

#define DEFAULT_PORT_RES        0xff
#define DEFAULT_DIVIDER_RES     0xffff
#define DEFAULT_DIVIDER8_RES    0xffff
#define DEFAULT_SCM_RES         1
#define DEFAULT_TCPWM_RES       1

#else // defined(TARGET_MCU_PSOC6_M0)

#define SRM_PORT(port, pins)               {(port), (pins)}
#define SRM_DIVIDER(type, dividers)        {(type), (dividers)}
#define SRM_SCB(num)                       {(num), (1)}
#define SRM_TCPWM(num)                     {(num), (1)}

#define DEFAULT_PORT_RES        0
#define DEFAULT_DIVIDER_RES     0
#define DEFAULT_DIVIDER8_RES    0
#define DEFAULT_SCM_RES         0
#define DEFAULT_TCPWM_RES       0
#endif // defined(TARGET_MCU_PSOC6_M0)

#include "psoc6_static_srm.h"

#else // PSOC6_DYNSRM_DISABLE

#define DEFAULT_PORT_RES        0
#define DEFAULT_DIVIDER_RES     0
#define DEFAULT_DIVIDER8_RES    0x3         // dividers 0 & 1 are reserved for us_ticker
#define DEFAULT_SCM_RES         0
#define DEFAULT_TCPWM_RES       0x3         // 32b counters 0 & 1 are reserved for us_ticker

#endif // PSOC6_DYNSRM_DISABLE

static uint8_t port_reservations[CY_NUM_PSOC6_PORTS] = {DEFAULT_PORT_RES};

typedef struct {
    const uint32_t  max_index;
    uint32_t        current_index;
    uint32_t        reservations;
} divider_alloc_t;

static divider_alloc_t divider_allocations[CY_NUM_DIVIDER_TYPES] = {
    { PERI_DIV_8_NR - 1,    2,  DEFAULT_DIVIDER8_RES },   // CY_SYSCLK_DIV_8_BIT
    { PERI_DIV_16_NR - 1,   0,  DEFAULT_DIVIDER_RES },    // CY_SYSCLK_DIV_16_BIT
    { PERI_DIV_16_5_NR - 1, 0,  DEFAULT_DIVIDER_RES },    // CY_SYSCLK_DIV_16_5_BIT
    { PERI_DIV_24_5_NR - 1, 0,  DEFAULT_DIVIDER_RES }     // CY_SYSCLK_DIV_24_5_BIT
};

static uint8_t scb_reservations[NUM_SCB] = {DEFAULT_SCM_RES};

static uint8_t tcpwm_reservations[NUM_TCPWM] = {DEFAULT_TCPWM_RES};


int cy_reserve_io_pin(PinName pin_name)
{
    uint32_t port = CY_PORT(pin_name);
    uint32_t pin = CY_PIN(pin_name);
    int result = (-1);

    if ((port < CY_NUM_PSOC6_PORTS) && (pin <= 7)) {
        core_util_critical_section_enter();
        if (!(port_reservations[port] & (1 << pin))) {
            port_reservations[port] |= (1 << pin);
            result = 0;
        }
        core_util_critical_section_exit();
    } else {
        error("Trying to reserve non existing port/pin!");
    }
    return result;
}


void cy_free_io_pin(PinName pin_name)
{
    uint32_t port = CY_PORT(pin_name);
    uint32_t pin = CY_PIN(pin_name);
    int result = (-1);

    if ((port < CY_NUM_PSOC6_PORTS) && (pin <= 7)) {
        core_util_critical_section_enter();
        if (port_reservations[port] & (1 << pin)) {
            port_reservations[port] &= ~(1 << pin);
            result = 0;
        }
        core_util_critical_section_exit();
    }

    if (result) {
        error("Trying to free wrong port/pin.");
    }
}


uint32_t cy_clk_reserve_divider(cy_en_divider_types_t div_type, uint32_t div_num)
{
    uint32_t        divider = CY_INVALID_DIVIDER;
    divider_alloc_t *p_alloc = &divider_allocations[div_type];

    MBED_ASSERT(div_type < CY_NUM_DIVIDER_TYPES);
    MBED_ASSERT(div_num <= p_alloc->max_index);

    core_util_critical_section_enter();

    if ((p_alloc->reservations & (1 << div_num)) == 0) {
        p_alloc->reservations |= (1 << div_num);
        divider = div_num;
        p_alloc->current_index = ++div_num;
        if (p_alloc->current_index > p_alloc->max_index) {
            p_alloc->current_index = 0;
        }
    }

    core_util_critical_section_exit();

    return divider;
}


void cy_clk_free_divider(cy_en_divider_types_t div_type, uint32_t div_num)
{
    int result = (-1);
    divider_alloc_t *p_alloc = &divider_allocations[div_type];

    MBED_ASSERT(div_type < CY_NUM_DIVIDER_TYPES);
    MBED_ASSERT(div_num <= p_alloc->max_index);

    core_util_critical_section_enter();

    if ((p_alloc->reservations & (1 << div_num)) != 0) {
        p_alloc->reservations &= ~(1 << div_num);
        result = 0;
    }

    core_util_critical_section_exit();

    if (result) {
        error("Trying to release wrong clock divider.");
    }
}


uint32_t cy_clk_allocate_divider(cy_en_divider_types_t div_type)
{
    uint32_t        divider = CY_INVALID_DIVIDER;
    divider_alloc_t *p_alloc = &divider_allocations[div_type];

    MBED_ASSERT(div_type < CY_NUM_DIVIDER_TYPES);

    core_util_critical_section_enter();

    MBED_ASSERT(p_alloc->current_index < p_alloc->max_index);


    for ( uint32_t first_index = p_alloc->current_index;
            CY_INVALID_DIVIDER == (divider = cy_clk_reserve_divider(div_type, p_alloc->current_index));
            ++p_alloc->current_index) {
        if (p_alloc->current_index > p_alloc->max_index) {
            p_alloc->current_index = 0;
        }
        if (p_alloc->current_index == first_index) {
            break;
        }
    }

    core_util_critical_section_exit();

    return divider;
}


int cy_reserve_scb(uint32_t scb_num)
{
    int result = (-1);

    if (scb_num < NUM_SCB) {
        core_util_critical_section_enter();
        if (scb_reservations[scb_num] == 0) {
            scb_reservations[scb_num] = 1;
        }
        core_util_critical_section_exit();
    }
    return result;
}


void cy_free_scb(uint32_t scb_num)
{
    int result = (-1);

    if (scb_num < NUM_SCB) {
        core_util_critical_section_enter();
        if (scb_reservations[scb_num] == 1) {
            scb_reservations[scb_num] = 0;
        }
        core_util_critical_section_exit();
    }
    if (result) {
        error("Trying to release wrong SCB.");
    }
}


int cy_reserve_tcpwm(uint32_t tcpwm_num)
{
    int result = (-1);

    if (tcpwm_num < NUM_TCPWM) {
        core_util_critical_section_enter();
        if (tcpwm_reservations[tcpwm_num] == 0) {
            tcpwm_reservations[tcpwm_num] = 1;
            result = 0;
        }
        core_util_critical_section_exit();
    }
    return result;
}


void cy_free_tcpwm(uint32_t tcpwm_num)
{
    int result = (-1);

    if (tcpwm_num < NUM_TCPWM) {
        core_util_critical_section_enter();
        if (tcpwm_reservations[tcpwm_num] == 1) {
            tcpwm_reservations[tcpwm_num] = 0;
            result = 0;
        }
        core_util_critical_section_exit();
    }
    if (result) {
        error("Trying to release wrong TCPWM.");
    }
}


/*
 * NVIC channel dynamic allocation (multiplexing) is used only on M0.
 * On M4 IRQs are statically pre-assigned to NVIC channels.
 */

#if defined(TARGET_MCU_PSOC6_M0)

#define NUM_NVIC_CHANNELS       ((uint32_t)(NvicMux31_IRQn - NvicMux0_IRQn) + 1)
/*
 * First 8 NVIC channels are wakeup-capable, we reserve them for manual allocation.
 */
#define FIRST_ALLOC_CHANNEL     ((uint32_t)(NvicMux8_IRQn - NvicMux0_IRQn))

static uint32_t irq_channels[NUM_NVIC_CHANNELS] = {0};


IRQn_Type cy_m0_nvic_allocate_channel(uint32_t channel_id)
{
    IRQn_Type alloc = (IRQn_Type)(-1);
    uint32_t chn;
    MBED_ASSERT(channel_id);

    core_util_critical_section_enter();
    for (chn = FIRST_ALLOC_CHANNEL; chn < NUM_NVIC_CHANNELS; ++chn) {
        if (irq_channels[chn] == 0) {
            irq_channels[chn] = channel_id;
            alloc = NvicMux0_IRQn + chn;
            break;
            irq_channels[chn] = channel_id;

        }
    }
    core_util_critical_section_exit();
    return alloc;
}

IRQn_Type cy_m0_nvic_reserve_channel(IRQn_Type channel, uint32_t channel_id)
{
    uint32_t chn = channel - NvicMux0_IRQn;

    MBED_ASSERT(chn < NUM_NVIC_CHANNELS);
    MBED_ASSERT(channel_id);

    core_util_critical_section_enter();
    if (irq_channels[chn]) {
        channel =  (IRQn_Type)(-1);
    } else {
        irq_channels[chn] = channel_id;
    }
    core_util_critical_section_exit();
    return channel;
}

void cy_m0_nvic_release_channel(IRQn_Type channel, uint32_t channel_id)
{
    uint32_t chn = channel - NvicMux0_IRQn;

    MBED_ASSERT(chn < NUM_NVIC_CHANNELS);
    MBED_ASSERT(channel_id);

    core_util_critical_section_enter();
    if (irq_channels[chn] == channel_id) {
        irq_channels[chn] = 0;
    } else {
        error("NVIC channel cross-check failed on release.");
    }
    core_util_critical_section_exit();
}

#define CY_BLE_SFLASH_DIE_X_MASK                   (0x3Fu)
#define CY_BLE_SFLASH_DIE_X_BITS                   (6u)
#define CY_BLE_SFLASH_DIE_Y_MASK                   (0x3Fu)
#define CY_BLE_SFLASH_DIE_Y_BITS                   (6u)
#define CY_BLE_SFLASH_DIE_XY_BITS                  (CY_BLE_SFLASH_DIE_X_BITS + CY_BLE_SFLASH_DIE_Y_BITS)
#define CY_BLE_SFLASH_DIE_WAFER_MASK               (0x1Fu)
#define CY_BLE_SFLASH_DIE_WAFER_BITS               (5u)
#define CY_BLE_SFLASH_DIE_XYWAFER_BITS             (CY_BLE_SFLASH_DIE_XY_BITS + CY_BLE_SFLASH_DIE_WAFER_BITS)
#define CY_BLE_SFLASH_DIE_LOT_MASK                 (0x7Fu)
#define CY_BLE_SFLASH_DIE_LOT_BITS                 (7u)

static uint8_t cy_ble_deviceAddress[6] = {0x19u, 0x00u, 0x00u, 0x50u, 0xA0u, 0x00u};

void cy_get_bd_mac_address(uint8_t *buffer)
{
    uint32_t bdAddrLoc;
    bdAddrLoc = ((uint32_t)SFLASH->DIE_X & (uint32_t)CY_BLE_SFLASH_DIE_X_MASK) |
                ((uint32_t)(((uint32_t)SFLASH->DIE_Y) & ((uint32_t)CY_BLE_SFLASH_DIE_Y_MASK)) <<
                 CY_BLE_SFLASH_DIE_X_BITS) |
                ((uint32_t)(((uint32_t)SFLASH->DIE_WAFER) & ((uint32_t)CY_BLE_SFLASH_DIE_WAFER_MASK)) <<
                 CY_BLE_SFLASH_DIE_XY_BITS) |
                ((uint32_t)(((uint32_t)SFLASH->DIE_LOT[0]) & ((uint32_t)CY_BLE_SFLASH_DIE_LOT_MASK)) <<
                 CY_BLE_SFLASH_DIE_XYWAFER_BITS);

    cy_ble_deviceAddress[0] = (uint8_t)bdAddrLoc;
    cy_ble_deviceAddress[1] = (uint8_t)(bdAddrLoc >> 8u);
    cy_ble_deviceAddress[2] = (uint8_t)(bdAddrLoc >> 16u);

    for (int i = 0; i < 6; ++i) {
        buffer[i] = cy_ble_deviceAddress[i];
    }
}

#endif // defined(TARGET_MCU_PSOC6_M0)

#endif // defined(TARGET_MCU_PSOC6_M0) || PSOC6_DSRM_DISABLE || !defined(__MBED__)

void cy_srm_initialize(void)
{
#if defined(TARGET_MCU_PSOC6_M0) || PSOC6_DYNSRM_DISABLE || !defined(__MBED__)
    uint32_t    i;

    for (i = 0; i < CY_NUM_PSOC6_PORTS; ++i) {
        port_reservations[i] = DEFAULT_PORT_RES;
    }

    for (i = 0; i < NUM_SCB; ++i) {
        scb_reservations[i] = DEFAULT_SCM_RES;
    }

    for (i = 0; i < NUM_TCPWM; ++i) {
        tcpwm_reservations[i] = DEFAULT_TCPWM_RES;
    }

#if PSOC6_DYNSRM_DISABLE
#ifdef M0_ASSIGNED_PORTS
    SRM_INIT_RESOURCE(uint8_t, port_reservations,, M0_ASSIGNED_PORTS);
#endif
#ifdef M0_ASSIGNED_DIVIDERS
    SRM_INIT_RESOURCE(uint32_t, divider_allocations, .reservations, M0_ASSIGNED_DIVIDERS);
#endif
#ifdef M0_ASSIGNED_SCBS
    SRM_INIT_RESOURCE(uint8_t, scb_reservations,, M0_ASSIGNED_SCBS);
#endif
#ifdef M0_ASSIGNED_TCPWMS
    SRM_INIT_RESOURCE(uint8_t, tcpwm_reservations,,  M0_ASSIGNED_TCPWMS);
#endif
#endif // PSOC6_DYNSRM_DISABLE
#endif // defined(TARGET_MCU_PSOC6_M0) || PSOC6_DSRM_DISABLE || !defined(__MBED__)
}

