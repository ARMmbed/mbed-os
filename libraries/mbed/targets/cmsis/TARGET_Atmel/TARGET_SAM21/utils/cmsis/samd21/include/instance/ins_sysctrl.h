#ifndef _SAMD21_SYSCTRL_INSTANCE_
#define _SAMD21_SYSCTRL_INSTANCE_

/* ========== Register definition for SYSCTRL peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_SYSCTRL_INTENCLR       (0x40000800U) /**< \brief (SYSCTRL) Interrupt Enable Clear */
#define REG_SYSCTRL_INTENSET       (0x40000804U) /**< \brief (SYSCTRL) Interrupt Enable Set */
#define REG_SYSCTRL_INTFLAG        (0x40000808U) /**< \brief (SYSCTRL) Interrupt Flag Status and Clear */
#define REG_SYSCTRL_PCLKSR         (0x4000080CU) /**< \brief (SYSCTRL) Power and Clocks Status */
#define REG_SYSCTRL_XOSC           (0x40000810U) /**< \brief (SYSCTRL) External Multipurpose Crystal Oscillator (XOSC) Control */
#define REG_SYSCTRL_XOSC32K        (0x40000814U) /**< \brief (SYSCTRL) 32kHz External Crystal Oscillator (XOSC32K) Control */
#define REG_SYSCTRL_OSC32K         (0x40000818U) /**< \brief (SYSCTRL) 32kHz Internal Oscillator (OSC32K) Control */
#define REG_SYSCTRL_OSCULP32K      (0x4000081CU) /**< \brief (SYSCTRL) 32kHz Ultra Low Power Internal Oscillator (OSCULP32K) Control */
#define REG_SYSCTRL_OSC8M          (0x40000820U) /**< \brief (SYSCTRL) 8MHz Internal Oscillator (OSC8M) Control */
#define REG_SYSCTRL_DFLLCTRL       (0x40000824U) /**< \brief (SYSCTRL) DFLL48M Control */
#define REG_SYSCTRL_DFLLVAL        (0x40000828U) /**< \brief (SYSCTRL) DFLL48M Value */
#define REG_SYSCTRL_DFLLMUL        (0x4000082CU) /**< \brief (SYSCTRL) DFLL48M Multiplier */
#define REG_SYSCTRL_DFLLSYNC       (0x40000830U) /**< \brief (SYSCTRL) DFLL48M Synchronization */
#define REG_SYSCTRL_BOD33          (0x40000834U) /**< \brief (SYSCTRL) 3.3V Brown-Out Detector (BOD33) Control */
#define REG_SYSCTRL_VREG           (0x4000083CU) /**< \brief (SYSCTRL) Voltage Regulator System (VREG) Control */
#define REG_SYSCTRL_VREF           (0x40000840U) /**< \brief (SYSCTRL) Voltage References System (VREF) Control */
#define REG_SYSCTRL_DPLLCTRLA      (0x40000844U) /**< \brief (SYSCTRL) DPLL Control A */
#define REG_SYSCTRL_DPLLRATIO      (0x40000848U) /**< \brief (SYSCTRL) DPLL Ratio Control */
#define REG_SYSCTRL_DPLLCTRLB      (0x4000084CU) /**< \brief (SYSCTRL) DPLL Control B */
#define REG_SYSCTRL_DPLLSTATUS     (0x40000850U) /**< \brief (SYSCTRL) DPLL Status */
#else
#define REG_SYSCTRL_INTENCLR       (*(RwReg  *)0x40000800U) /**< \brief (SYSCTRL) Interrupt Enable Clear */
#define REG_SYSCTRL_INTENSET       (*(RwReg  *)0x40000804U) /**< \brief (SYSCTRL) Interrupt Enable Set */
#define REG_SYSCTRL_INTFLAG        (*(RwReg  *)0x40000808U) /**< \brief (SYSCTRL) Interrupt Flag Status and Clear */
#define REG_SYSCTRL_PCLKSR         (*(RoReg  *)0x4000080CU) /**< \brief (SYSCTRL) Power and Clocks Status */
#define REG_SYSCTRL_XOSC           (*(RwReg16*)0x40000810U) /**< \brief (SYSCTRL) External Multipurpose Crystal Oscillator (XOSC) Control */
#define REG_SYSCTRL_XOSC32K        (*(RwReg16*)0x40000814U) /**< \brief (SYSCTRL) 32kHz External Crystal Oscillator (XOSC32K) Control */
#define REG_SYSCTRL_OSC32K         (*(RwReg  *)0x40000818U) /**< \brief (SYSCTRL) 32kHz Internal Oscillator (OSC32K) Control */
#define REG_SYSCTRL_OSCULP32K      (*(RwReg8 *)0x4000081CU) /**< \brief (SYSCTRL) 32kHz Ultra Low Power Internal Oscillator (OSCULP32K) Control */
#define REG_SYSCTRL_OSC8M          (*(RwReg  *)0x40000820U) /**< \brief (SYSCTRL) 8MHz Internal Oscillator (OSC8M) Control */
#define REG_SYSCTRL_DFLLCTRL       (*(RwReg16*)0x40000824U) /**< \brief (SYSCTRL) DFLL48M Control */
#define REG_SYSCTRL_DFLLVAL        (*(RwReg  *)0x40000828U) /**< \brief (SYSCTRL) DFLL48M Value */
#define REG_SYSCTRL_DFLLMUL        (*(RwReg  *)0x4000082CU) /**< \brief (SYSCTRL) DFLL48M Multiplier */
#define REG_SYSCTRL_DFLLSYNC       (*(RwReg8 *)0x40000830U) /**< \brief (SYSCTRL) DFLL48M Synchronization */
#define REG_SYSCTRL_BOD33          (*(RwReg  *)0x40000834U) /**< \brief (SYSCTRL) 3.3V Brown-Out Detector (BOD33) Control */
#define REG_SYSCTRL_VREG           (*(RwReg16*)0x4000083CU) /**< \brief (SYSCTRL) Voltage Regulator System (VREG) Control */
#define REG_SYSCTRL_VREF           (*(RwReg  *)0x40000840U) /**< \brief (SYSCTRL) Voltage References System (VREF) Control */
#define REG_SYSCTRL_DPLLCTRLA      (*(RwReg8 *)0x40000844U) /**< \brief (SYSCTRL) DPLL Control A */
#define REG_SYSCTRL_DPLLRATIO      (*(RwReg  *)0x40000848U) /**< \brief (SYSCTRL) DPLL Ratio Control */
#define REG_SYSCTRL_DPLLCTRLB      (*(RwReg  *)0x4000084CU) /**< \brief (SYSCTRL) DPLL Control B */
#define REG_SYSCTRL_DPLLSTATUS     (*(RoReg8 *)0x40000850U) /**< \brief (SYSCTRL) DPLL Status */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for SYSCTRL peripheral ========== */
#define SYSCTRL_BGAP_CALIB_MSB      11
#define SYSCTRL_BOD33_CALIB_MSB     5
#define SYSCTRL_DFLL48M_COARSE_MSB  5
#define SYSCTRL_DFLL48M_FINE_MSB    9
#define SYSCTRL_GCLK_ID_DFLL48      0        // Index of Generic Clock for DFLL48
#define SYSCTRL_GCLK_ID_FDPLL       1        // Index of Generic Clock for DPLL
#define SYSCTRL_GCLK_ID_FDPLL32K    2        // Index of Generic Clock for DPLL 32K
#define SYSCTRL_OSC32K_COARSE_CALIB_MSB 6
#define SYSCTRL_POR33_ENTEST_MSB    1
#define SYSCTRL_ULPVREF_DIVLEV_MSB  3
#define SYSCTRL_ULPVREG_FORCEGAIN_MSB 1
#define SYSCTRL_ULPVREG_RAMREFSEL_MSB 2
#define SYSCTRL_VREF_CONTROL_MSB    48
#define SYSCTRL_VREF_STATUS_MSB     7
#define SYSCTRL_VREG_LEVEL_MSB      2
#define SYSCTRL_BOD12_VERSION       0x111
#define SYSCTRL_BOD33_VERSION       0x111
#define SYSCTRL_DFLL48M_VERSION     0x301
#define SYSCTRL_FDPLL_VERSION       0x111
#define SYSCTRL_OSCULP32K_VERSION   0x111
#define SYSCTRL_OSC8M_VERSION       0x120
#define SYSCTRL_OSC32K_VERSION      0x1101
#define SYSCTRL_VREF_VERSION        0x200
#define SYSCTRL_VREG_VERSION        0x201
#define SYSCTRL_XOSC_VERSION        0x1111
#define SYSCTRL_XOSC32K_VERSION     0x1111

#endif /* _SAMD21_SYSCTRL_INSTANCE_ */
