#ifndef _SAMD21_PM_INSTANCE_
#define _SAMD21_PM_INSTANCE_

/* ========== Register definition for PM peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PM_CTRL                (0x40000400U) /**< \brief (PM) Control */
#define REG_PM_SLEEP               (0x40000401U) /**< \brief (PM) Sleep Mode */
#define REG_PM_CPUSEL              (0x40000408U) /**< \brief (PM) CPU Clock Select */
#define REG_PM_APBASEL             (0x40000409U) /**< \brief (PM) APBA Clock Select */
#define REG_PM_APBBSEL             (0x4000040AU) /**< \brief (PM) APBB Clock Select */
#define REG_PM_APBCSEL             (0x4000040BU) /**< \brief (PM) APBC Clock Select */
#define REG_PM_AHBMASK             (0x40000414U) /**< \brief (PM) AHB Mask */
#define REG_PM_APBAMASK            (0x40000418U) /**< \brief (PM) APBA Mask */
#define REG_PM_APBBMASK            (0x4000041CU) /**< \brief (PM) APBB Mask */
#define REG_PM_APBCMASK            (0x40000420U) /**< \brief (PM) APBC Mask */
#define REG_PM_INTENCLR            (0x40000434U) /**< \brief (PM) Interrupt Enable Clear */
#define REG_PM_INTENSET            (0x40000435U) /**< \brief (PM) Interrupt Enable Set */
#define REG_PM_INTFLAG             (0x40000436U) /**< \brief (PM) Interrupt Flag Status and Clear */
#define REG_PM_RCAUSE              (0x40000438U) /**< \brief (PM) Reset Cause */
#else
#define REG_PM_CTRL                (*(RwReg8 *)0x40000400U) /**< \brief (PM) Control */
#define REG_PM_SLEEP               (*(RwReg8 *)0x40000401U) /**< \brief (PM) Sleep Mode */
#define REG_PM_CPUSEL              (*(RwReg8 *)0x40000408U) /**< \brief (PM) CPU Clock Select */
#define REG_PM_APBASEL             (*(RwReg8 *)0x40000409U) /**< \brief (PM) APBA Clock Select */
#define REG_PM_APBBSEL             (*(RwReg8 *)0x4000040AU) /**< \brief (PM) APBB Clock Select */
#define REG_PM_APBCSEL             (*(RwReg8 *)0x4000040BU) /**< \brief (PM) APBC Clock Select */
#define REG_PM_AHBMASK             (*(RwReg  *)0x40000414U) /**< \brief (PM) AHB Mask */
#define REG_PM_APBAMASK            (*(RwReg  *)0x40000418U) /**< \brief (PM) APBA Mask */
#define REG_PM_APBBMASK            (*(RwReg  *)0x4000041CU) /**< \brief (PM) APBB Mask */
#define REG_PM_APBCMASK            (*(RwReg  *)0x40000420U) /**< \brief (PM) APBC Mask */
#define REG_PM_INTENCLR            (*(RwReg8 *)0x40000434U) /**< \brief (PM) Interrupt Enable Clear */
#define REG_PM_INTENSET            (*(RwReg8 *)0x40000435U) /**< \brief (PM) Interrupt Enable Set */
#define REG_PM_INTFLAG             (*(RwReg8 *)0x40000436U) /**< \brief (PM) Interrupt Flag Status and Clear */
#define REG_PM_RCAUSE              (*(RoReg8 *)0x40000438U) /**< \brief (PM) Reset Cause */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PM peripheral ========== */
#define PM_CTRL_MCSEL_DFLL48M       3
#define PM_CTRL_MCSEL_GCLK          0
#define PM_CTRL_MCSEL_OSC8M         1
#define PM_CTRL_MCSEL_XOSC          2
#define PM_PM_CLK_APB_NUM           2

#endif /* _SAMD21_PM_INSTANCE_ */
