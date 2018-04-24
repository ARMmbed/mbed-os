/** @file startup_XMC4500.c
 *  @brief mbed start-up code for the XMC4500-1024 written in C.
 *
 *  Following things are done in this file.
 *  	1. Build and load vector table
 *  	2. call System_init (clock setup..)
 *  	3. l
 *  	4. Clear the bss section (zero) => none initialized data
 *		5. call OS hook functions
 *		6. initialize C library
 *		7. jump into main()
 *
 *  @author Fränz Ney (es16m013@technikum-wien.at)
 *  @bug No known bugs.
 */

/***************************************************************** Includes **/
#include <XMC4500.h>

/****************************************************** Function Prototypes **/
void Reset_Handler               ( void );
void NMI_Handler                 ( void );
void HardFault_Handler           ( void );
void SVC_Handler                 ( void );
void PendSV_Handler              ( void );
void SysTick_Handler             ( void );

int main(void);                                 ///> main function
void __libc_init_array(void);                   ///> Initialize C library (syscall)
void Dummy_Handler(void);                       ///> Dummy Handler for unused interrupts
extern void software_init_hook(void) __attribute__((weak)); 	///> reset entry point for mbed-os

/****************************************************** Vector Table Layout **/
typedef struct _DeviceVectors {
    /* Stack pointer */
    void* pvStack;

    /* Cortex-M handlers */
    void* pfnReset_Handler;
    void* pfnNMI_Handler;
    void* pfnHardFault_Handler;
    void* pfnReservedM12;
    void* pfnReservedM11;
    void* pfnReservedM10;
    void* pfnReservedM9;
    void* pfnReservedM8;
    void* pfnReservedM7;
    void* pfnReservedM6;
    void* pfnSVC_Handler;
    void* pfnReservedM4;
    void* pfnReservedM3;
    void* pfnPendSV_Handler;
    void* pfnSysTick_Handler;

    /* Peripheral handlers */
    void* pfnSCU_0_IRQHandler;
    void* pfnERU0_0_IRQHandler;
    void* pfnERU0_1_IRQHandler;
    void* pfnERU0_2_IRQHandler;
    void* pfnERU0_3_IRQHandler;
    void* pfnERU1_0_IRQHandler;
    void* pfnERU1_1_IRQHandler;
    void* pfnERU1_2_IRQHandler;
    void* pfnERU1_3_IRQHandler;
    void* pfnNotAvailable1;
    void* pfnNotAvailable2;
    void* pfnNotAvailable3;
    void* pfnPMU0_0_IRQHandler;
    void* pfnNotAvailable4;
    void* pfnVADC0_C0_0_IRQHandler;
    void* pfnVADC0_C0_1_IRQHandler;
    void* pfnVADC0_C0_2_IRQHandler;
    void* pfnVADC0_C0_3_IRQHandler;
    void* pfnVADC0_G0_0_IRQHandler;
    void* pfnVADC0_G0_1_IRQHandler;
    void* pfnVADC0_G0_2_IRQHandler;
    void* pfnVADC0_G0_3_IRQHandler;
    void* pfnVADC0_G1_0_IRQHandler;
    void* pfnVADC0_G1_1_IRQHandler;
    void* pfnVADC0_G1_2_IRQHandler;
    void* pfnVADC0_G1_3_IRQHandler;
    void* pfnVADC0_G2_0_IRQHandler;
    void* pfnVADC0_G2_1_IRQHandler;
    void* pfnVADC0_G2_2_IRQHandler;
    void* pfnVADC0_G2_3_IRQHandler;
    void* pfnVADC0_G3_0_IRQHandler;
    void* pfnVADC0_G3_1_IRQHandler;
    void* pfnVADC0_G3_2_IRQHandler;
    void* pfnVADC0_G3_3_IRQHandler;
    void* pfnDSD0_0_IRQHandler;
    void* pfnDSD0_1_IRQHandler;
    void* pfnDSD0_2_IRQHandler;
    void* pfnDSD0_3_IRQHandler;
    void* pfnDSD0_4_IRQHandler;
    void* pfnDSD0_5_IRQHandler;
    void* pfnDSD0_6_IRQHandler;
    void* pfnDSD0_7_IRQHandler;
    void* pfnDAC0_0_IRQHandler;
    void* pfnDAC0_1_IRQHandler;
    void* pfnCCU40_0_IRQHandler;
    void* pfnCCU40_1_IRQHandler;
    void* pfnCCU40_2_IRQHandler;
    void* pfnCCU40_3_IRQHandler;
    void* pfnCCU41_0_IRQHandler;
    void* pfnCCU41_1_IRQHandler;
    void* pfnCCU41_2_IRQHandler;
    void* pfnCCU41_3_IRQHandler;
    void* pfnCCU42_0_IRQHandler;
    void* pfnCCU42_1_IRQHandler;
    void* pfnCCU42_2_IRQHandler;
    void* pfnCCU42_3_IRQHandler;
    void* pfnCCU43_0_IRQHandler;
    void* pfnCCU43_1_IRQHandler;
    void* pfnCCU43_2_IRQHandler;
    void* pfnCCU43_3_IRQHandler;
    void* pfnCCU80_0_IRQHandler;
    void* pfnCCU80_1_IRQHandler;
    void* pfnCCU80_2_IRQHandler;
    void* pfnCCU80_3_IRQHandler;
    void* pfnCCU81_0_IRQHandler;
    void* pfnCCU81_1_IRQHandler;
    void* pfnCCU81_2_IRQHandler;
    void* pfnCCU81_3_IRQHandler;
    void* pfnPOSIF0_0_IRQHandler;
    void* pfnPOSIF0_1_IRQHandler;
    void* pfnPOSIF1_0_IRQHandler;
    void* pfnPOSIF1_1_IRQHandler;
    void* pfnNotAvailable5;
    void* pfnNotAvailable6;
    void* pfnNotAvailable7;
    void* pfnNotAvailable8;
    void* pfnCAN0_0_IRQHandler;
    void* pfnCAN0_1_IRQHandler;
    void* pfnCAN0_2_IRQHandler;
    void* pfnCAN0_3_IRQHandler;
    void* pfnCAN0_4_IRQHandler;
    void* pfnCAN0_5_IRQHandler;
    void* pfnCAN0_6_IRQHandler;
    void* pfnCAN0_7_IRQHandler;
    void* pfnUSIC0_0_IRQHandler;
    void* pfnUSIC0_1_IRQHandler;
    void* pfnUSIC0_2_IRQHandler;
    void* pfnUSIC0_3_IRQHandler;
    void* pfnUSIC0_4_IRQHandler;
    void* pfnUSIC0_5_IRQHandler;
    void* pfnUSIC1_0_IRQHandler;
    void* pfnUSIC1_1_IRQHandler;
    void* pfnUSIC1_2_IRQHandler;
    void* pfnUSIC1_3_IRQHandler;
    void* pfnUSIC1_4_IRQHandler;
    void* pfnUSIC1_5_IRQHandler;
    void* pfnUSIC2_0_IRQHandler;
    void* pfnUSIC2_1_IRQHandler;
    void* pfnUSIC2_2_IRQHandler;
    void* pfnUSIC2_3_IRQHandler;
    void* pfnUSIC2_4_IRQHandler;
    void* pfnUSIC2_5_IRQHandler;
    void* pfnLEDTS0_0_IRQHandler;
    void* pfnNotAvailable9;
    void* pfnFCE0_0_IRQHandler;
    void* pfnGPDMA0_0_IRQHandler;
    void* pfnSDMMC0_0_IRQHandler;
    void* pfnUSB0_0_IRQHandler;
    void* pfnETH0_0_IRQHandler;
    void* pfnNotAvailable10;
    void* pfnGPDMA1_0_IRQHandler;
    void* pfnNotAvailable11;
}
DeviceVectors;

/********************************* Initialized segments (see linker script) **/
extern uint32_t __exidx_end;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __StackTop;

/********************* ISR Handler (If not declared point to Dummy Handler) **/

/* Cortex-M4 core handlers */
void NMI_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void HardFault_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void MemManage_Handler  ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void BusFault_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void UsageFault_Handler ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SVC_Handler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DebugMon_Handler   ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PendSV_Handler     ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SysTick_Handler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/* Peripherals handlers */
void SCU_0_IRQHandler         ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ERU0_0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ERU0_1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ERU0_2_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ERU0_3_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ERU1_0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ERU1_1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ERU1_2_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ERU1_3_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void PMU0_0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_C0_0_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_C0_1_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_C0_2_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_C0_3_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G0_0_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G0_1_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G0_2_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G0_3_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G1_0_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G1_1_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G1_2_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G1_3_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G2_0_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G2_1_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G2_2_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G2_3_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G3_0_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G3_1_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G3_2_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void VADC0_G3_3_IRQHandler    ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DSD0_0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DSD0_1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DSD0_2_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DSD0_3_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DSD0_4_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DSD0_5_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DSD0_6_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DSD0_7_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC0_0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void DAC0_1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU40_0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU40_1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU40_2_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU40_3_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU41_0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU41_1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU41_2_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU41_3_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU42_0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU42_1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU42_2_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU42_3_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU43_0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU43_1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU43_2_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU43_3_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU80_0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU80_1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU80_2_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU80_3_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU81_0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU81_1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU81_2_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CCU81_3_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void POSIF0_0_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void POSIF0_1_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void POSIF1_0_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void POSIF1_1_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_1_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_2_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_3_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_4_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_5_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_6_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void CAN0_7_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC0_0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC0_1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC0_2_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC0_3_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC0_4_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC0_5_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC1_0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC1_1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC1_2_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC1_3_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC1_4_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC1_5_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC2_0_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC2_1_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC2_2_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC2_3_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC2_4_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USIC2_5_IRQHandler       ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void LEDTS0_0_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void FCE0_0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void GPDMA0_0_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void SDMMC0_0_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void USB0_0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void ETH0_0_IRQHandler        ( void ) __attribute__ ((weak, alias("Dummy_Handler")));
void GPDMA1_0_IRQHandler      ( void ) __attribute__ ((weak, alias("Dummy_Handler")));

/******************************************** Load vector table into memory **/
__attribute__ ((section(".vectors")))
const DeviceVectors exception_table = {

	  /* Configure Initial Stack Pointer, using linker-generated symbols */
	  (void*) (&__StackTop),
	  (void*) Reset_Handler,
	  (void*) NMI_Handler,
	  (void*) HardFault_Handler,
	  (void*) MemManage_Handler,
	  (void*) BusFault_Handler,
	  (void*) UsageFault_Handler,
	  (void*) (0UL),           /* Reserved */
	  (void*) (0UL),           /* Reserved */
	  (void*) (0UL),           /* Reserved */
	  (void*) (0UL),           /* Reserved */
	  (void*) SVC_Handler,
	  (void*) DebugMon_Handler,
	  (void*) (0UL),           /* Reserved */
	  (void*) PendSV_Handler,
	  (void*) SysTick_Handler,

	  /* Configurable interrupts */
	  (void*) SCU_0_IRQHandler,
	  (void*) ERU0_0_IRQHandler,
	  (void*) ERU0_1_IRQHandler,
	  (void*) ERU0_2_IRQHandler,
	  (void*) ERU0_3_IRQHandler,
	  (void*) ERU1_0_IRQHandler,
	  (void*) ERU1_1_IRQHandler,
	  (void*) ERU1_2_IRQHandler,
	  (void*) ERU1_3_IRQHandler,
	  (void*) (0UL),           /* Not available */
	  (void*) (0UL),           /* Not available */
	  (void*) (0UL),           /* Not available */
	  (void*) PMU0_0_IRQHandler,
	  (void*) (0UL),           /* Not available */
	  (void*) VADC0_C0_0_IRQHandler,
	  (void*) VADC0_C0_1_IRQHandler,
	  (void*) VADC0_C0_2_IRQHandler,
	  (void*) VADC0_C0_3_IRQHandler,
	  (void*) VADC0_G0_0_IRQHandler,
	  (void*) VADC0_G0_1_IRQHandler,
	  (void*) VADC0_G0_2_IRQHandler,
	  (void*) VADC0_G0_3_IRQHandler,
	  (void*) VADC0_G1_0_IRQHandler,
	  (void*) VADC0_G1_1_IRQHandler,
	  (void*) VADC0_G1_2_IRQHandler,
	  (void*) VADC0_G1_3_IRQHandler,
	  (void*) VADC0_G2_0_IRQHandler,
	  (void*) VADC0_G2_1_IRQHandler,
	  (void*) VADC0_G2_2_IRQHandler,
	  (void*) VADC0_G2_3_IRQHandler,
	  (void*) VADC0_G3_0_IRQHandler,
	  (void*) VADC0_G3_1_IRQHandler,
	  (void*) VADC0_G3_2_IRQHandler,
	  (void*) VADC0_G3_3_IRQHandler,
	  (void*) DSD0_0_IRQHandler,
	  (void*) DSD0_1_IRQHandler,
	  (void*) DSD0_2_IRQHandler,
	  (void*) DSD0_3_IRQHandler,
	  (void*) DSD0_4_IRQHandler,
	  (void*) DSD0_5_IRQHandler,
	  (void*) DSD0_6_IRQHandler,
	  (void*) DSD0_7_IRQHandler,
	  (void*) DAC0_0_IRQHandler,
	  (void*) DAC0_1_IRQHandler,
	  (void*) CCU40_0_IRQHandler,
	  (void*) CCU40_1_IRQHandler,
	  (void*) CCU40_2_IRQHandler,
	  (void*) CCU40_3_IRQHandler,
	  (void*) CCU41_0_IRQHandler,
	  (void*) CCU41_1_IRQHandler,
	  (void*) CCU41_2_IRQHandler,
	  (void*) CCU41_3_IRQHandler,
	  (void*) CCU42_0_IRQHandler,
	  (void*) CCU42_1_IRQHandler,
	  (void*) CCU42_2_IRQHandler,
	  (void*) CCU42_3_IRQHandler,
	  (void*) CCU43_0_IRQHandler,
	  (void*) CCU43_1_IRQHandler,
	  (void*) CCU43_2_IRQHandler,
	  (void*) CCU43_3_IRQHandler,
	  (void*) CCU80_0_IRQHandler,
	  (void*) CCU80_1_IRQHandler,
	  (void*) CCU80_2_IRQHandler,
	  (void*) CCU80_3_IRQHandler,
	  (void*) CCU81_0_IRQHandler,
	  (void*) CCU81_1_IRQHandler,
	  (void*) CCU81_2_IRQHandler,
	  (void*) CCU81_3_IRQHandler,
	  (void*) POSIF0_0_IRQHandler,
	  (void*) POSIF0_1_IRQHandler,
	  (void*) POSIF1_0_IRQHandler,
	  (void*) POSIF1_1_IRQHandler,
	  (void*) (0UL),           /* Not available */
	  (void*) (0UL),           /* Not available */
	  (void*) (0UL),           /* Not available */
	  (void*) (0UL),           /* Not available */
	  (void*) CAN0_0_IRQHandler,
	  (void*) CAN0_1_IRQHandler,
	  (void*) CAN0_2_IRQHandler,
	  (void*) CAN0_3_IRQHandler,
	  (void*) CAN0_4_IRQHandler,
	  (void*) CAN0_5_IRQHandler,
	  (void*) CAN0_6_IRQHandler,
	  (void*) CAN0_7_IRQHandler,
	  (void*) USIC0_0_IRQHandler,
	  (void*) USIC0_1_IRQHandler,
	  (void*) USIC0_2_IRQHandler,
	  (void*) USIC0_3_IRQHandler,
	  (void*) USIC0_4_IRQHandler,
	  (void*) USIC0_5_IRQHandler,
	  (void*) USIC1_0_IRQHandler,
	  (void*) USIC1_1_IRQHandler,
	  (void*) USIC1_2_IRQHandler,
	  (void*) USIC1_3_IRQHandler,
	  (void*) USIC1_4_IRQHandler,
	  (void*) USIC1_5_IRQHandler,
	  (void*) USIC2_0_IRQHandler,
	  (void*) USIC2_1_IRQHandler,
	  (void*) USIC2_2_IRQHandler,
	  (void*) USIC2_3_IRQHandler,
	  (void*) USIC2_4_IRQHandler,
	  (void*) USIC2_5_IRQHandler,
	  (void*) LEDTS0_0_IRQHandler,
	  (void*) (0UL),           /* Not available */
	  (void*) FCE0_0_IRQHandler,
	  (void*) GPDMA0_0_IRQHandler,
	  (void*) SDMMC0_0_IRQHandler,
	  (void*) USB0_0_IRQHandler,
	  (void*) ETH0_0_IRQHandler,
	  (void*) (0UL),           /* Not available */
	  (void*) GPDMA1_0_IRQHandler,
	  (void*) (0UL)            /* Not available */
};

/** @brief This is the code that gets called on processor reset.
 *
 */
void Reset_Handler(void) {
    uint32_t *pSrc, *pDest;

    /* Initialize the relocate segment */
    pSrc = &__exidx_end;
    pDest = &__data_start__;

    /* Initialize System */
    SystemInit();

    /* copy data from LMA to VMA */
    if (pSrc != pDest) {
        for (; pDest < &__data_end__;) {
            *pDest++ = *pSrc++;
        }
    }

    /* Clear the zero segment */
    for (pDest = &__bss_start__; pDest < &__bss_end__;) {
        *pDest++ = 0;
    }

    if (software_init_hook)
        software_init_hook();
    else {
        __libc_init_array();
        main();
    }

    /* Infinite loop */
    while (1) {}
}

/** @brief Default interrupt handler for unused IRQs.
 *
 */
void Dummy_Handler(void) {

#ifdef DMBED_DEBUG
    /* break into debugger */
    __asm("BKPT #0\n") ;
#else
    /* Reset System */
    //NVIC_SystemReset();
#endif
}

/*EOF*/
