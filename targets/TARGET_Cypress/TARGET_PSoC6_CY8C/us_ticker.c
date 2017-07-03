
#include <stddef.h>
#include "PeripheralNames.h"
#include "us_ticker_api.h"


#define CY_Counter_TCPWM_CNT_HW             TCPWM0_CNT0
#define CY_Counter_TCPWM_CNT_IDX            0u
#define CY_Counter_TCPWM_HW                 TCPWM0
#define CY_Counter_TCPWM_IDX                0u

#define CY_Counter_INIT_COMPARE_OR_CAPTURE  (0uL)

/** This is a ptr to the base address of the TCPWM instance */
#define CY_Counter_HW                       (CY_Counter_TCPWM_HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define CY_Counter_CNT_HW                   (CY_Counter_TCPWM_CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define CY_Counter_CNT_NUM                  (CY_Counter_TCPWM_CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define CY_Counter_CNT_MASK                 (1uL << CY_Counter_CNT_NUM)

#define CY_Counter_INPUT_MODE_MASK          (0x3u)


#define CY_SysInt_INTC_CORTEXM0P_ASSIGNED   1
#define CY_SysInt_INTC_CORTEXM0P_MUX        0u
#define CY_SysInt_INTC_CORTEXM0P_PRIORITY   3u
#define CY_SysInt_INTC_NUMBER               82u

#define CY_SysInt_INTR_CORTEXM0P_SRC        (CY_SysInt_INTC_NUMBER)
#define CY_SysInt_INTR_CORTEXM0P_PRIORITY   (CY_SysInt_INTC_CORTEXM0P_PRIORITY)
#define CY_SysInt_INTR_CORTEXM0P_MUX        (CY_SysInt_INTC_CORTEXM0P_MUX)


cy_stc_sysint_t cy_SysInt_cfg_cm0p = {
    .intrSrc = (IRQn_Type)CY_SysInt_INTR_CORTEXM0P_SRC,
    .intrCm0p = (IRQn_CM0P_Type)CY_SysInt_INTR_CORTEXM0P_MUX,
    .intrPriority = CY_SysInt_INTR_CORTEXM0P_PRIORITY
};

int us_ticker_inited = 0;


void irq_handler(void)
{
    us_ticker_clear_interrupt();
    us_ticker_irq_handler();
}

void us_ticker_init(void)
{
    cy_stc_tcpwm_counter_config_t cy_counter_config =
    {
        .period = 4294967295uL,
        .clockPrescaler = 0uL,
        .runMode = 0uL,
        .countDirection = 0uL,
        .compareOrCapture = CY_Counter_INIT_COMPARE_OR_CAPTURE,
        .compare0 = 0uL,
        .compare1 = 0uL,
        .enableCompareSwap = false,
        .interruptSources = CY_TCPWM_INT_ON_CC,
        .captureInputMode = 7uL & CY_Counter_INPUT_MODE_MASK,
        .captureInput = CY_TCPWM_INPUT_CREATOR,
        .reloadInputMode = 7uL & CY_Counter_INPUT_MODE_MASK,
        .reloadInput = CY_TCPWM_INPUT_CREATOR,
        .startInputMode = 7uL & CY_Counter_INPUT_MODE_MASK,
        .startInput = CY_TCPWM_INPUT_CREATOR,
        .stopInputMode = 7uL & CY_Counter_INPUT_MODE_MASK,
        .stopInput = CY_TCPWM_INPUT_CREATOR,
        .countInputMode = 7uL & CY_Counter_INPUT_MODE_MASK,
        .countInput = CY_TCPWM_INPUT_CREATOR,
    };

    if (us_ticker_inited)
        return;
    us_ticker_inited = 1;

    /*
        Configure the clock
    */

    // us_ticker 1 MHz from PCLK 50 MHz
    Cy_SysClk_PeriphAssignDivider(PCLK_TCPWM0_CLOCKS0, CY_SYSCLK_DIV_8_BIT, 0u);
    Cy_SysClk_PeriphSetDivider(CY_SYSCLK_DIV_8_BIT, 0u, 49u);
    Cy_SysClk_PeriphPhaseAlignDivider(CY_SYSCLK_DIV_8_BIT, 0u, CY_SYSCLK_DIV_24_5_BIT, 0xFF);

    /*
        Configure the counter
    */

    Cy_TCPWM_Counter_Init(CY_Counter_HW, CY_Counter_CNT_NUM, &cy_counter_config);
    Cy_TCPWM_Enable_Multiple(CY_Counter_HW, CY_Counter_CNT_MASK);
    Cy_TCPWM_TriggerStart(CY_Counter_HW, CY_Counter_CNT_MASK);

    Cy_SysInt_Init(&cy_SysInt_cfg_cm0p, irq_handler);
}

uint32_t us_ticker_read(void)
{
    if (!us_ticker_inited)
        us_ticker_init();
    return Cy_TCPWM_Counter_GetCounter(CY_Counter_HW, CY_Counter_CNT_NUM);
}

void us_ticker_set_interrupt(timestamp_t timestamp)
{
    if (!us_ticker_inited)
        us_ticker_init();

    // Set new output compare value
    Cy_TCPWM_Counter_SetCompare0(CY_Counter_HW, CY_Counter_CNT_NUM, timestamp);
    // Enable int
    NVIC_EnableIRQ((IRQn_Type)cy_SysInt_cfg_cm0p.intrCm0p);
}

void us_ticker_disable_interrupt(void)
{
    NVIC_DisableIRQ((IRQn_Type)cy_SysInt_cfg_cm0p.intrCm0p);
}

void us_ticker_clear_interrupt(void)
{
    Cy_TCPWM_ClearInterrupt(CY_Counter_HW, CY_Counter_CNT_NUM, CY_TCPWM_INT_ON_CC);
}
