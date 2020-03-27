//*****************************************************************************
//
//! @file system_apollo3.c
//!
//! @brief Ambiq Micro Apollo3 MCU specific functions.
//
//*****************************************************************************

//*****************************************************************************
//
// ${copyright}
//
//*****************************************************************************

#include <stdint.h>
#include "system_apollo3.h"
#include "apollo3.h"

//*****************************************************************************
//
// Defines
//
//*****************************************************************************

//
// Clocks
//
#define __HSI             (6000000UL)
#define __XTAL            (32768UL)         // Crystal Oscillator frequency
#define __SYS_OSC_CLK     (48000000)        // Main oscillator frequency
#define __SYSTEM_CLOCK    (1*__SYS_OSC_CLK)

//
// Initialize SystemCoreClock with the system core clock frequency value
// achieved after system intitialization.
// This means system core clock frequency after call to SystemInit()
//
uint32_t SystemCoreClock = __SYSTEM_CLOCK;  // System Clock Frequency (Core Clock)

//*****************************************************************************
//
//! @brief Set the global clock frequncy.
//!
//! This function sets the global clock frequency.
//!
//! @return None.
//
//*****************************************************************************
void
SystemCoreClockUpdate(void)
{
    //
    // Calculate the system frequency based upon the current register settings.
    // This function can be used to retrieve the system core clock frequeny
    // after user changed register sittings.
    //
    SystemCoreClock = __SYS_OSC_CLK / (CLKGEN->CCTRL_b.CORESEL + 1);
}

//*****************************************************************************
//
//! @brief Initialize the system.
//!
//! This function sets up the microcontroller system.
//!
//! @return None.
//
//*****************************************************************************
void
SystemInit(void)
{
    //
    // Initialize the system
    // Do not use global variables because this function is called before
    // reaching pre-main. RW section maybe overwritten afterwards.
    //
    SystemCoreClock = __SYSTEM_CLOCK;

    CLKGEN->CLKKEY = 0x47;              // Enable write to CCTRL
    CLKGEN->CCTRL_b.CORESEL = 0;        // Div by 1 for 48MHz
    CLKGEN->CLKKEY = 0;                 // Disable write to CCTRL
}

