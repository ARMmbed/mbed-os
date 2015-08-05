#ifndef _SAMD21_AC_INSTANCE_
#define _SAMD21_AC_INSTANCE_

/* ========== Register definition for AC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_AC_CTRLA               (0x42004400U) /**< \brief (AC) Control A */
#define REG_AC_CTRLB               (0x42004401U) /**< \brief (AC) Control B */
#define REG_AC_EVCTRL              (0x42004402U) /**< \brief (AC) Event Control */
#define REG_AC_INTENCLR            (0x42004404U) /**< \brief (AC) Interrupt Enable Clear */
#define REG_AC_INTENSET            (0x42004405U) /**< \brief (AC) Interrupt Enable Set */
#define REG_AC_INTFLAG             (0x42004406U) /**< \brief (AC) Interrupt Flag Status and Clear */
#define REG_AC_STATUSA             (0x42004408U) /**< \brief (AC) Status A */
#define REG_AC_STATUSB             (0x42004409U) /**< \brief (AC) Status B */
#define REG_AC_STATUSC             (0x4200440AU) /**< \brief (AC) Status C */
#define REG_AC_WINCTRL             (0x4200440CU) /**< \brief (AC) Window Control */
#define REG_AC_COMPCTRL0           (0x42004410U) /**< \brief (AC) Comparator Control 0 */
#define REG_AC_COMPCTRL1           (0x42004414U) /**< \brief (AC) Comparator Control 1 */
#define REG_AC_SCALER0             (0x42004420U) /**< \brief (AC) Scaler 0 */
#define REG_AC_SCALER1             (0x42004421U) /**< \brief (AC) Scaler 1 */
#else
#define REG_AC_CTRLA               (*(RwReg8 *)0x42004400U) /**< \brief (AC) Control A */
#define REG_AC_CTRLB               (*(WoReg8 *)0x42004401U) /**< \brief (AC) Control B */
#define REG_AC_EVCTRL              (*(RwReg16*)0x42004402U) /**< \brief (AC) Event Control */
#define REG_AC_INTENCLR            (*(RwReg8 *)0x42004404U) /**< \brief (AC) Interrupt Enable Clear */
#define REG_AC_INTENSET            (*(RwReg8 *)0x42004405U) /**< \brief (AC) Interrupt Enable Set */
#define REG_AC_INTFLAG             (*(RwReg8 *)0x42004406U) /**< \brief (AC) Interrupt Flag Status and Clear */
#define REG_AC_STATUSA             (*(RoReg8 *)0x42004408U) /**< \brief (AC) Status A */
#define REG_AC_STATUSB             (*(RoReg8 *)0x42004409U) /**< \brief (AC) Status B */
#define REG_AC_STATUSC             (*(RoReg8 *)0x4200440AU) /**< \brief (AC) Status C */
#define REG_AC_WINCTRL             (*(RwReg8 *)0x4200440CU) /**< \brief (AC) Window Control */
#define REG_AC_COMPCTRL0           (*(RwReg  *)0x42004410U) /**< \brief (AC) Comparator Control 0 */
#define REG_AC_COMPCTRL1           (*(RwReg  *)0x42004414U) /**< \brief (AC) Comparator Control 1 */
#define REG_AC_SCALER0             (*(RwReg8 *)0x42004420U) /**< \brief (AC) Scaler 0 */
#define REG_AC_SCALER1             (*(RwReg8 *)0x42004421U) /**< \brief (AC) Scaler 1 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for AC peripheral ========== */
#define AC_CMP_NUM                  2        // Number of comparators
#define AC_GCLK_ID_ANA              32       // Index of Generic Clock for analog
#define AC_GCLK_ID_DIG              31       // Index of Generic Clock for digital
#define AC_NUM_CMP                  2
#define AC_PAIRS                    1        // Number of pairs of comparators

#endif /* _SAMD21_AC_INSTANCE_ */
