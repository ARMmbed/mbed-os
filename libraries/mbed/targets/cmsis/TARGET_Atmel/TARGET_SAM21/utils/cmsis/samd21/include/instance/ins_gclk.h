#ifndef _SAMD21_GCLK_INSTANCE_
#define _SAMD21_GCLK_INSTANCE_

/* ========== Register definition for GCLK peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_GCLK_CTRL              (0x40000C00U) /**< \brief (GCLK) Control */
#define REG_GCLK_STATUS            (0x40000C01U) /**< \brief (GCLK) Status */
#define REG_GCLK_CLKCTRL           (0x40000C02U) /**< \brief (GCLK) Generic Clock Control */
#define REG_GCLK_GENCTRL           (0x40000C04U) /**< \brief (GCLK) Generic Clock Generator Control */
#define REG_GCLK_GENDIV            (0x40000C08U) /**< \brief (GCLK) Generic Clock Generator Division */
#else
#define REG_GCLK_CTRL              (*(RwReg8 *)0x40000C00U) /**< \brief (GCLK) Control */
#define REG_GCLK_STATUS            (*(RoReg8 *)0x40000C01U) /**< \brief (GCLK) Status */
#define REG_GCLK_CLKCTRL           (*(RwReg16*)0x40000C02U) /**< \brief (GCLK) Generic Clock Control */
#define REG_GCLK_GENCTRL           (*(RwReg  *)0x40000C04U) /**< \brief (GCLK) Generic Clock Generator Control */
#define REG_GCLK_GENDIV            (*(RwReg  *)0x40000C08U) /**< \brief (GCLK) Generic Clock Generator Division */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for GCLK peripheral ========== */
#define GCLK_GENDIV_BITS            16
#define GCLK_GEN_NUM                9        // Number of Generic Clock Generators
#define GCLK_GEN_NUM_MSB            8        // Number of Generic Clock Generators - 1
#define GCLK_GEN_SOURCE_NUM_MSB     8        // Number of Generic Clock Sources - 1
#define GCLK_NUM                    37       // Number of Generic Clock Users
#define GCLK_SOURCE_DFLL48M         7
#define GCLK_SOURCE_FDPLL           8
#define GCLK_SOURCE_GCLKGEN1        2
#define GCLK_SOURCE_GCLKIN          1
#define GCLK_SOURCE_NUM             9        // Number of Generic Clock Sources
#define GCLK_SOURCE_OSCULP32K       3
#define GCLK_SOURCE_OSC8M           6
#define GCLK_SOURCE_OSC32K          4
#define GCLK_SOURCE_XOSC            0
#define GCLK_SOURCE_XOSC32K         5

#endif /* _SAMD21_GCLK_INSTANCE_ */
