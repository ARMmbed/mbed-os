#ifndef _SAMD21_PAC1_INSTANCE_
#define _SAMD21_PAC1_INSTANCE_

/* ========== Register definition for PAC1 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PAC1_WPCLR             (0x41000000U) /**< \brief (PAC1) Write Protection Clear */
#define REG_PAC1_WPSET             (0x41000004U) /**< \brief (PAC1) Write Protection Set */
#else
#define REG_PAC1_WPCLR             (*(RwReg  *)0x41000000U) /**< \brief (PAC1) Write Protection Clear */
#define REG_PAC1_WPSET             (*(RwReg  *)0x41000004U) /**< \brief (PAC1) Write Protection Set */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PAC1 peripheral ========== */
#define PAC1_WPROT_DEFAULT_VAL      0x00000002 // PAC protection mask at reset

#endif /* _SAMD21_PAC1_INSTANCE_ */
