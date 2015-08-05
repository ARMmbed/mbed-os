#ifndef _SAMD21_PORT_INSTANCE_
#define _SAMD21_PORT_INSTANCE_

/* ========== Register definition for PORT peripheral ========== */
#if (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
#define REG_PORT_DIR0              (0x41004400U) /**< \brief (PORT) Data Direction 0 */
#define REG_PORT_DIRCLR0           (0x41004404U) /**< \brief (PORT) Data Direction Clear 0 */
#define REG_PORT_DIRSET0           (0x41004408U) /**< \brief (PORT) Data Direction Set 0 */
#define REG_PORT_DIRTGL0           (0x4100440CU) /**< \brief (PORT) Data Direction Toggle 0 */
#define REG_PORT_OUT0              (0x41004410U) /**< \brief (PORT) Data Output Value 0 */
#define REG_PORT_OUTCLR0           (0x41004414U) /**< \brief (PORT) Data Output Value Clear 0 */
#define REG_PORT_OUTSET0           (0x41004418U) /**< \brief (PORT) Data Output Value Set 0 */
#define REG_PORT_OUTTGL0           (0x4100441CU) /**< \brief (PORT) Data Output Value Toggle 0 */
#define REG_PORT_IN0               (0x41004420U) /**< \brief (PORT) Data Input Value 0 */
#define REG_PORT_CTRL0             (0x41004424U) /**< \brief (PORT) Control 0 */
#define REG_PORT_WRCONFIG0         (0x41004428U) /**< \brief (PORT) Write Configuration 0 */
#define REG_PORT_PMUX0             (0x41004430U) /**< \brief (PORT) Peripheral Multiplexing 0 */
#define REG_PORT_PINCFG0           (0x41004440U) /**< \brief (PORT) Pin Configuration 0 */
#define REG_PORT_DIR1              (0x41004480U) /**< \brief (PORT) Data Direction 1 */
#define REG_PORT_DIRCLR1           (0x41004484U) /**< \brief (PORT) Data Direction Clear 1 */
#define REG_PORT_DIRSET1           (0x41004488U) /**< \brief (PORT) Data Direction Set 1 */
#define REG_PORT_DIRTGL1           (0x4100448CU) /**< \brief (PORT) Data Direction Toggle 1 */
#define REG_PORT_OUT1              (0x41004490U) /**< \brief (PORT) Data Output Value 1 */
#define REG_PORT_OUTCLR1           (0x41004494U) /**< \brief (PORT) Data Output Value Clear 1 */
#define REG_PORT_OUTSET1           (0x41004498U) /**< \brief (PORT) Data Output Value Set 1 */
#define REG_PORT_OUTTGL1           (0x4100449CU) /**< \brief (PORT) Data Output Value Toggle 1 */
#define REG_PORT_IN1               (0x410044A0U) /**< \brief (PORT) Data Input Value 1 */
#define REG_PORT_CTRL1             (0x410044A4U) /**< \brief (PORT) Control 1 */
#define REG_PORT_WRCONFIG1         (0x410044A8U) /**< \brief (PORT) Write Configuration 1 */
#define REG_PORT_PMUX1             (0x410044B0U) /**< \brief (PORT) Peripheral Multiplexing 1 */
#define REG_PORT_PINCFG1           (0x410044C0U) /**< \brief (PORT) Pin Configuration 1 */
#else
#define REG_PORT_DIR0              (*(RwReg  *)0x41004400U) /**< \brief (PORT) Data Direction 0 */
#define REG_PORT_DIRCLR0           (*(RwReg  *)0x41004404U) /**< \brief (PORT) Data Direction Clear 0 */
#define REG_PORT_DIRSET0           (*(RwReg  *)0x41004408U) /**< \brief (PORT) Data Direction Set 0 */
#define REG_PORT_DIRTGL0           (*(RwReg  *)0x4100440CU) /**< \brief (PORT) Data Direction Toggle 0 */
#define REG_PORT_OUT0              (*(RwReg  *)0x41004410U) /**< \brief (PORT) Data Output Value 0 */
#define REG_PORT_OUTCLR0           (*(RwReg  *)0x41004414U) /**< \brief (PORT) Data Output Value Clear 0 */
#define REG_PORT_OUTSET0           (*(RwReg  *)0x41004418U) /**< \brief (PORT) Data Output Value Set 0 */
#define REG_PORT_OUTTGL0           (*(RwReg  *)0x4100441CU) /**< \brief (PORT) Data Output Value Toggle 0 */
#define REG_PORT_IN0               (*(RoReg  *)0x41004420U) /**< \brief (PORT) Data Input Value 0 */
#define REG_PORT_CTRL0             (*(RwReg  *)0x41004424U) /**< \brief (PORT) Control 0 */
#define REG_PORT_WRCONFIG0         (*(WoReg  *)0x41004428U) /**< \brief (PORT) Write Configuration 0 */
#define REG_PORT_PMUX0             (*(RwReg  *)0x41004430U) /**< \brief (PORT) Peripheral Multiplexing 0 */
#define REG_PORT_PINCFG0           (*(RwReg  *)0x41004440U) /**< \brief (PORT) Pin Configuration 0 */
#define REG_PORT_DIR1              (*(RwReg  *)0x41004480U) /**< \brief (PORT) Data Direction 1 */
#define REG_PORT_DIRCLR1           (*(RwReg  *)0x41004484U) /**< \brief (PORT) Data Direction Clear 1 */
#define REG_PORT_DIRSET1           (*(RwReg  *)0x41004488U) /**< \brief (PORT) Data Direction Set 1 */
#define REG_PORT_DIRTGL1           (*(RwReg  *)0x4100448CU) /**< \brief (PORT) Data Direction Toggle 1 */
#define REG_PORT_OUT1              (*(RwReg  *)0x41004490U) /**< \brief (PORT) Data Output Value 1 */
#define REG_PORT_OUTCLR1           (*(RwReg  *)0x41004494U) /**< \brief (PORT) Data Output Value Clear 1 */
#define REG_PORT_OUTSET1           (*(RwReg  *)0x41004498U) /**< \brief (PORT) Data Output Value Set 1 */
#define REG_PORT_OUTTGL1           (*(RwReg  *)0x4100449CU) /**< \brief (PORT) Data Output Value Toggle 1 */
#define REG_PORT_IN1               (*(RoReg  *)0x410044A0U) /**< \brief (PORT) Data Input Value 1 */
#define REG_PORT_CTRL1             (*(RwReg  *)0x410044A4U) /**< \brief (PORT) Control 1 */
#define REG_PORT_WRCONFIG1         (*(WoReg  *)0x410044A8U) /**< \brief (PORT) Write Configuration 1 */
#define REG_PORT_PMUX1             (*(RwReg  *)0x410044B0U) /**< \brief (PORT) Peripheral Multiplexing 1 */
#define REG_PORT_PINCFG1           (*(RwReg  *)0x410044C0U) /**< \brief (PORT) Pin Configuration 1 */
#endif /* (defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/* ========== Instance parameters for PORT peripheral ========== */
#define PORT_BITS                   84       // Number of PORT pins
#define PORT_DIR_DEFAULT_VAL        { 0x00000000, 0x00000000, 0x00000000 } // Default value for DIR of all pins
#define PORT_DIR_IMPLEMENTED        { 0xDBFFFFFF, 0xC0C3FFFF, 0x000FFFFF } // Implementation mask for DIR of all pins
#define PORT_DRVSTR                 1        // DRVSTR supported
#define PORT_DRVSTR_DEFAULT_VAL     { 0xD8FFFFFF, 0xC0C3FFFF, 0x000FFFFF } // Default value for DRVSTR of all pins
#define PORT_DRVSTR_IMPLEMENTED     { 0xD8FFFFFF, 0xC0C3FFFF, 0x000FFFFF } // Implementation mask for DRVSTR of all pins
#define PORT_EVENT_IMPLEMENTED      { 0x00000000, 0x00000000, 0x00000000 }
#define PORT_INEN_DEFAULT_VAL       { 0x00000000, 0x00000000, 0x00000000 } // Default value for INEN of all pins
#define PORT_INEN_IMPLEMENTED       { 0xD8FFFFFF, 0xC0C3FFFF, 0x000FFFFF } // Implementation mask for INEN of all pins
#define PORT_ODRAIN                 0        // ODRAIN supported
#define PORT_ODRAIN_DEFAULT_VAL     { 0x00000000, 0x00000000, 0x00000000 } // Default value for ODRAIN of all pins
#define PORT_ODRAIN_IMPLEMENTED     { 0x00000000, 0x00000000, 0x00000000 } // Implementation mask for ODRAIN of all pins
#define PORT_OUT_DEFAULT_VAL        { 0x00000000, 0x00000000, 0x00000000 } // Default value for OUT of all pins
#define PORT_OUT_IMPLEMENTED        { 0xDBFFFFFF, 0xC0C3FFFF, 0x000FFFFF } // Implementation mask for OUT of all pins
#define PORT_PIN_IMPLEMENTED        { 0xDBFFFFFF, 0xC0C3FFFF, 0x000FFFFF } // Implementation mask for all PORT pins
#define PORT_PMUXBIT0_DEFAULT_VAL   { 0x00000000, 0x00000000, 0x00000000 } // Default value for PMUX[0] of all pins
#define PORT_PMUXBIT0_IMPLEMENTED   { 0xDBFFFFFF, 0xC0C3FFFF, 0x000D0000 } // Implementation mask for PMUX[0] of all pins
#define PORT_PMUXBIT1_DEFAULT_VAL   { 0x40000000, 0x00000000, 0x00000000 } // Default value for PMUX[1] of all pins
#define PORT_PMUXBIT1_IMPLEMENTED   { 0xDBFFFFF3, 0xC0C3FF0F, 0x00000000 } // Implementation mask for PMUX[1] of all pins
#define PORT_PMUXBIT2_DEFAULT_VAL   { 0x40000000, 0x00000000, 0x00000000 } // Default value for PMUX[2] of all pins
#define PORT_PMUXBIT2_IMPLEMENTED   { 0xDBFFFFF3, 0xC0C3FF0F, 0x000D0000 } // Implementation mask for PMUX[2] of all pins
#define PORT_PMUXBIT3_DEFAULT_VAL   { 0x00000000, 0x00000000, 0x00000000 } // Default value for PMUX[3] of all pins
#define PORT_PMUXBIT3_IMPLEMENTED   { 0x00000000, 0x00000000, 0x00000000 } // Implementation mask for PMUX[3] of all pins
#define PORT_PMUXEN_DEFAULT_VAL     { 0x64000000, 0x3F3C0000, 0x00000000 } // Default value for PMUXEN of all pins
#define PORT_PMUXEN_IMPLEMENTED     { 0xDBFFFFFF, 0xC0C3FFFF, 0x000F7FFE } // Implementation mask for PMUXEN of all pins
#define PORT_PULLEN_DEFAULT_VAL     { 0x00000000, 0x00000000, 0x00000000 } // Default value for PULLEN of all pins
#define PORT_PULLEN_IMPLEMENTED     { 0xDBFFFFFF, 0xC0C3FFFF, 0x000FFFFF } // Implementation mask for PULLEN of all pins
#define PORT_SLEWLIM                0        // SLEWLIM supported
#define PORT_SLEWLIM_DEFAULT_VAL    { 0x00000000, 0x00000000, 0x00000000 } // Default value for SLEWLIM of all pins
#define PORT_SLEWLIM_IMPLEMENTED    { 0x00000000, 0x00000000, 0x00000000 } // Implementation mask for SLEWLIM of all pins

#endif /* _SAMD21_PORT_INSTANCE_ */
