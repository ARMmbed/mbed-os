
#include <stddef.h>
#include <limits.h>
#include "device.h"
#include "PeripheralNames.h"
#include "us_ticker_api.h"

#include "drivers/peripheral/sysint/cy_sysint.h"
#include "drivers/peripheral/sysclk/cy_sysclk.h"
#include "drivers/peripheral/tcpwm/cy_tcpwm_counter.h"

/** Each CPU core nn PSoC6 each CPU needs its own usec timer.
 ** Although each of TCPWM timers have two compare registers,
 ** it has only one interrupt line, so we need to allocate 
 ** two TCPWM counters for purpose of us_ticker */

 
#if defined(TARGET_MCU_PSoC6_M0)

#define TICKER_COUNTER_UNIT					TCPWM0 
#define TICKER_COUNTER_NUM					0
#define TICKER_COUNTER_INTERRUPT_SOURCE		tcpwm_0_interrupts_0_IRQn
#define TICKER_COUNTER_NVIC_IRQN        	NvicMux0_IRQn
#define TICKER_COUNTER_INTERRUPT_PRIORITY	3

#elif defined (TARGET_MCU_PSoC6_M4)

#define TICKER_COUNTER_UNIT					TCPWM0 
#define TICKER_COUNTER_NUM					1
#define TICKER_COUNTER_INTERRUPT_SOURCE		tcpwm_0_interrupts_1_IRQn
#define TICKER_COUNTER_NVIC_IRQN        	TICKER_COUNTER_INTERRUPT_SOURCE
#define TICKER_COUNTER_INTERRUPT_PRIORITY	3

#else
	#error "Unknown MCU type."
#endif



const cy_stc_sysint_t cy_sysint_cfg = {
    .intrSrc = TICKER_COUNTER_INTERRUPT_SOURCE,
#if defined(MCU_PSoC6_M0)
    .intrCm0p = TICKER_COUNTER_NVIC_IRQN,
#endif
    .intrPriority = TICKER_COUNTER_INTERRUPT_PRIORITY
};

int us_ticker_inited = 0;

const cy_stc_tcpwm_counter_config_t cy_counter_config =
{
	.period = ULONG_MAX,
	.clockPrescaler = CY_TCPWM_COUNTER_PRESCALER_DIVBY_1,
	.runMode = CY_TCPWM_COUNTER_CONTINUOUS,
	.countDirection = CY_TCPWM_COUNTER_COUNT_UP,
	.compareOrCapture = CY_TCPWM_COUNTER_MODE_COMPARE,
	.enableCompareSwap = false,
	.interruptSources = CY_TCPWM_INT_ON_CC,
	.countInputMode = CY_TCPWM_INPUT_LEVEL,
	.countInput = CY_TCPWM_INPUT_1
};

static void local_irq_handler(void)
{
    us_ticker_clear_interrupt();
    us_ticker_irq_handler();
}

void us_ticker_init(void)
{

    if (us_ticker_inited)
        return;
    us_ticker_inited = 1;

    /*
        Configure the clock
    */

    // us_ticker 1 MHz from PCLK 50 MHz
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM0_CLOCKS0, CY_SYSCLK_DIV_8_BIT, 0u);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 0u, 49u);
    Cy_SysClk_PeriphEnablePhaseAlignDivider(CY_SYSCLK_DIV_8_BIT, 0u, CY_SYSCLK_DIV_24_5_BIT, 0xFF);

    /*
        Configure the counter
    */

    Cy_TCPWM_Counter_Init(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM, &cy_counter_config);
    Cy_TCPWM_Counter_Enable(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM);
    Cy_TCPWM_TriggerStart(TICKER_COUNTER_UNIT, 1UL << TICKER_COUNTER_NUM);
	
    Cy_SysInt_Init(&cy_sysint_cfg, local_irq_handler);
}

uint32_t us_ticker_read(void)
{
    if (!us_ticker_inited)
        us_ticker_init();
    return Cy_TCPWM_Counter_GetCounter(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    if (!us_ticker_inited)
        us_ticker_init();

    // Set new output compare value
    Cy_TCPWM_Counter_SetCompare0(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM, timestamp);
    // Enable int
    NVIC_EnableIRQ(TICKER_COUNTER_NVIC_IRQN);
}

void us_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ(TICKER_COUNTER_NVIC_IRQN);
}

void us_ticker_clear_interrupt(void)
{
    Cy_TCPWM_ClearInterrupt(TICKER_COUNTER_UNIT, TICKER_COUNTER_NUM, CY_TCPWM_INT_ON_CC);
}
