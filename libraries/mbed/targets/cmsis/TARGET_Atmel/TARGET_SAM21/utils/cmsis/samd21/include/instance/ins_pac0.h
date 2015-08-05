#ifndef _SAMD21_PAC0_INSTANCE_
#define _SAMD21_PAC0_INSTANCE_

/* ========== Register definition for PAC0 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PAC0_WPCLR             (0x40000000U) /**< \brief (PAC0) Write Protection Clear */
#define REG_PAC0_WPSET             (0x40000004U) /**< \brief (PAC0) Write Protection Set */
#else
#define REG_PAC0_WPCLR             (*(RwReg  *)0x40000000U) /**< \brief (PAC0) Write Protection Clear */
#define REG_PAC0_WPSET             (*(RwReg  *)0x40000004U) /**< \brief (PAC0) Write Protection Set */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PAC0 peripheral ========== */
#define PAC0_WPROT_DEFAULT_VAL      0x00000000 // PAC protection mask at reset

#endif /* _SAMD21_PAC0_INSTANCE_ */
