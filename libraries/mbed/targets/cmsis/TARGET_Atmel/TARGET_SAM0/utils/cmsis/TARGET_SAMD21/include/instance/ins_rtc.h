#ifndef _SAMD21_RTC_INSTANCE_
#define _SAMD21_RTC_INSTANCE_

/* ========== Register definition for RTC peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_RTC_READREQ            (0x40001402U) /**< \brief (RTC) Read Request */
#define REG_RTC_STATUS             (0x4000140AU) /**< \brief (RTC) Status */
#define REG_RTC_DBGCTRL            (0x4000140BU) /**< \brief (RTC) Debug Control */
#define REG_RTC_FREQCORR           (0x4000140CU) /**< \brief (RTC) Frequency Correction */
#define REG_RTC_MODE0_CTRL         (0x40001400U) /**< \brief (RTC) MODE0 Control */
#define REG_RTC_MODE0_EVCTRL       (0x40001404U) /**< \brief (RTC) MODE0 Event Control */
#define REG_RTC_MODE0_INTENCLR     (0x40001406U) /**< \brief (RTC) MODE0 Interrupt Enable Clear */
#define REG_RTC_MODE0_INTENSET     (0x40001407U) /**< \brief (RTC) MODE0 Interrupt Enable Set */
#define REG_RTC_MODE0_INTFLAG      (0x40001408U) /**< \brief (RTC) MODE0 Interrupt Flag Status and Clear */
#define REG_RTC_MODE0_COUNT        (0x40001410U) /**< \brief (RTC) MODE0 Counter Value */
#define REG_RTC_MODE0_COMP0        (0x40001418U) /**< \brief (RTC) MODE0 Compare 0 Value */
#define REG_RTC_MODE1_CTRL         (0x40001400U) /**< \brief (RTC) MODE1 Control */
#define REG_RTC_MODE1_EVCTRL       (0x40001404U) /**< \brief (RTC) MODE1 Event Control */
#define REG_RTC_MODE1_INTENCLR     (0x40001406U) /**< \brief (RTC) MODE1 Interrupt Enable Clear */
#define REG_RTC_MODE1_INTENSET     (0x40001407U) /**< \brief (RTC) MODE1 Interrupt Enable Set */
#define REG_RTC_MODE1_INTFLAG      (0x40001408U) /**< \brief (RTC) MODE1 Interrupt Flag Status and Clear */
#define REG_RTC_MODE1_COUNT        (0x40001410U) /**< \brief (RTC) MODE1 Counter Value */
#define REG_RTC_MODE1_PER          (0x40001414U) /**< \brief (RTC) MODE1 Counter Period */
#define REG_RTC_MODE1_COMP0        (0x40001418U) /**< \brief (RTC) MODE1 Compare 0 Value */
#define REG_RTC_MODE1_COMP1        (0x4000141AU) /**< \brief (RTC) MODE1 Compare 1 Value */
#define REG_RTC_MODE2_CTRL         (0x40001400U) /**< \brief (RTC) MODE2 Control */
#define REG_RTC_MODE2_EVCTRL       (0x40001404U) /**< \brief (RTC) MODE2 Event Control */
#define REG_RTC_MODE2_INTENCLR     (0x40001406U) /**< \brief (RTC) MODE2 Interrupt Enable Clear */
#define REG_RTC_MODE2_INTENSET     (0x40001407U) /**< \brief (RTC) MODE2 Interrupt Enable Set */
#define REG_RTC_MODE2_INTFLAG      (0x40001408U) /**< \brief (RTC) MODE2 Interrupt Flag Status and Clear */
#define REG_RTC_MODE2_CLOCK        (0x40001410U) /**< \brief (RTC) MODE2 Clock Value */
#define REG_RTC_MODE2_ALARM_ALARM0 (0x40001418U) /**< \brief (RTC) MODE2_ALARM Alarm 0 Value */
#define REG_RTC_MODE2_ALARM_MASK0  (0x4000141CU) /**< \brief (RTC) MODE2_ALARM Alarm 0 Mask */
#else
#define REG_RTC_READREQ            (*(RwReg16*)0x40001402U) /**< \brief (RTC) Read Request */
#define REG_RTC_STATUS             (*(RwReg8 *)0x4000140AU) /**< \brief (RTC) Status */
#define REG_RTC_DBGCTRL            (*(RwReg8 *)0x4000140BU) /**< \brief (RTC) Debug Control */
#define REG_RTC_FREQCORR           (*(RwReg8 *)0x4000140CU) /**< \brief (RTC) Frequency Correction */
#define REG_RTC_MODE0_CTRL         (*(RwReg16*)0x40001400U) /**< \brief (RTC) MODE0 Control */
#define REG_RTC_MODE0_EVCTRL       (*(RwReg16*)0x40001404U) /**< \brief (RTC) MODE0 Event Control */
#define REG_RTC_MODE0_INTENCLR     (*(RwReg8 *)0x40001406U) /**< \brief (RTC) MODE0 Interrupt Enable Clear */
#define REG_RTC_MODE0_INTENSET     (*(RwReg8 *)0x40001407U) /**< \brief (RTC) MODE0 Interrupt Enable Set */
#define REG_RTC_MODE0_INTFLAG      (*(RwReg8 *)0x40001408U) /**< \brief (RTC) MODE0 Interrupt Flag Status and Clear */
#define REG_RTC_MODE0_COUNT        (*(RwReg  *)0x40001410U) /**< \brief (RTC) MODE0 Counter Value */
#define REG_RTC_MODE0_COMP0        (*(RwReg  *)0x40001418U) /**< \brief (RTC) MODE0 Compare 0 Value */
#define REG_RTC_MODE1_CTRL         (*(RwReg16*)0x40001400U) /**< \brief (RTC) MODE1 Control */
#define REG_RTC_MODE1_EVCTRL       (*(RwReg16*)0x40001404U) /**< \brief (RTC) MODE1 Event Control */
#define REG_RTC_MODE1_INTENCLR     (*(RwReg8 *)0x40001406U) /**< \brief (RTC) MODE1 Interrupt Enable Clear */
#define REG_RTC_MODE1_INTENSET     (*(RwReg8 *)0x40001407U) /**< \brief (RTC) MODE1 Interrupt Enable Set */
#define REG_RTC_MODE1_INTFLAG      (*(RwReg8 *)0x40001408U) /**< \brief (RTC) MODE1 Interrupt Flag Status and Clear */
#define REG_RTC_MODE1_COUNT        (*(RwReg16*)0x40001410U) /**< \brief (RTC) MODE1 Counter Value */
#define REG_RTC_MODE1_PER          (*(RwReg16*)0x40001414U) /**< \brief (RTC) MODE1 Counter Period */
#define REG_RTC_MODE1_COMP0        (*(RwReg16*)0x40001418U) /**< \brief (RTC) MODE1 Compare 0 Value */
#define REG_RTC_MODE1_COMP1        (*(RwReg16*)0x4000141AU) /**< \brief (RTC) MODE1 Compare 1 Value */
#define REG_RTC_MODE2_CTRL         (*(RwReg16*)0x40001400U) /**< \brief (RTC) MODE2 Control */
#define REG_RTC_MODE2_EVCTRL       (*(RwReg16*)0x40001404U) /**< \brief (RTC) MODE2 Event Control */
#define REG_RTC_MODE2_INTENCLR     (*(RwReg8 *)0x40001406U) /**< \brief (RTC) MODE2 Interrupt Enable Clear */
#define REG_RTC_MODE2_INTENSET     (*(RwReg8 *)0x40001407U) /**< \brief (RTC) MODE2 Interrupt Enable Set */
#define REG_RTC_MODE2_INTFLAG      (*(RwReg8 *)0x40001408U) /**< \brief (RTC) MODE2 Interrupt Flag Status and Clear */
#define REG_RTC_MODE2_CLOCK        (*(RwReg  *)0x40001410U) /**< \brief (RTC) MODE2 Clock Value */
#define REG_RTC_MODE2_ALARM_ALARM0 (*(RwReg  *)0x40001418U) /**< \brief (RTC) MODE2_ALARM Alarm 0 Value */
#define REG_RTC_MODE2_ALARM_MASK0  (*(RwReg  *)0x4000141CU) /**< \brief (RTC) MODE2_ALARM Alarm 0 Mask */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for RTC peripheral ========== */
#define RTC_ALARM_NUM               1        // Number of Alarms
#define RTC_COMP16_NUM              2        // Number of 16-bit Comparators
#define RTC_COMP32_NUM              1        // Number of 32-bit Comparators
#define RTC_GCLK_ID                 4        // Index of Generic Clock
#define RTC_NUM_OF_ALARMS           1        // Number of Alarms (obsolete)
#define RTC_NUM_OF_COMP16           2        // Number of 16-bit Comparators (obsolete)
#define RTC_NUM_OF_COMP32           1        // Number of 32-bit Comparators (obsolete)

#endif /* _SAMD21_RTC_INSTANCE_ */
