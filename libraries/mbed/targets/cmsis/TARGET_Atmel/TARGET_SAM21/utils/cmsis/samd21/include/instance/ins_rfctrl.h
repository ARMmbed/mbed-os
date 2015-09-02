#ifndef _SAMR21_RFCTRL_INSTANCE_
#define _SAMR21_RFCTRL_INSTANCE_

/* ========== Register definition for RFCTRL peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_RFCTRL_FECFG           (0x42005400U) /**< \brief (RFCTRL) Front-end control bus configuration */
#else
#define REG_RFCTRL_FECFG           (*(RwReg16*)0x42005400U) /**< \brief (RFCTRL) Front-end control bus configuration */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for RFCTRL peripheral ========== */
#define RFCTRL_FBUSMSB              5

#endif /* _SAMR21_RFCTRL_INSTANCE_ */
