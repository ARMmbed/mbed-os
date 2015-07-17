#ifndef _SAMD21_PAC2_INSTANCE_
#define _SAMD21_PAC2_INSTANCE_

/* ========== Register definition for PAC2 peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PAC2_WPCLR             (0x42000000U) /**< \brief (PAC2) Write Protection Clear */
#define REG_PAC2_WPSET             (0x42000004U) /**< \brief (PAC2) Write Protection Set */
#else
#define REG_PAC2_WPCLR             (*(RwReg  *)0x42000000U) /**< \brief (PAC2) Write Protection Clear */
#define REG_PAC2_WPSET             (*(RwReg  *)0x42000004U) /**< \brief (PAC2) Write Protection Set */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PAC2 peripheral ========== */
#define PAC2_WPROT_DEFAULT_VAL      0x00800000 // PAC protection mask at reset

#endif /* _SAMD21_PAC2_INSTANCE_ */
