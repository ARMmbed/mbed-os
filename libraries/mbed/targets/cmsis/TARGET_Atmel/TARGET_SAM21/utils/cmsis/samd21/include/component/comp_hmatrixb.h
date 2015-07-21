#ifndef _SAMD21_HMATRIXB_COMPONENT_
#define _SAMD21_HMATRIXB_COMPONENT_

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR HMATRIXB */
/* ========================================================================== */
/** \addtogroup SAMD21_HMATRIXB HSB Matrix */
/*@{*/

#define HMATRIXB_I7638
#define REV_HMATRIXB                0x212

/* -------- HMATRIXB_PRAS : (HMATRIXB Offset: 0x080) (R/W 32) PRS Priority A for Slave -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
    uint32_t reg;                /*!< Type      used for register access              */
} HMATRIXB_PRAS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define HMATRIXB_PRAS_OFFSET        0x080        /**< \brief (HMATRIXB_PRAS offset) Priority A for Slave */
#define HMATRIXB_PRAS_RESETVALUE    0x00000000ul /**< \brief (HMATRIXB_PRAS reset_value) Priority A for Slave */

#define HMATRIXB_PRAS_MASK          0x00000000ul /**< \brief (HMATRIXB_PRAS) MASK Register */

/* -------- HMATRIXB_PRBS : (HMATRIXB Offset: 0x084) (R/W 32) PRS Priority B for Slave -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
    uint32_t reg;                /*!< Type      used for register access              */
} HMATRIXB_PRBS_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define HMATRIXB_PRBS_OFFSET        0x084        /**< \brief (HMATRIXB_PRBS offset) Priority B for Slave */
#define HMATRIXB_PRBS_RESETVALUE    0x00000000ul /**< \brief (HMATRIXB_PRBS reset_value) Priority B for Slave */

#define HMATRIXB_PRBS_MASK          0x00000000ul /**< \brief (HMATRIXB_PRBS) MASK Register */

/* -------- HMATRIXB_SFR : (HMATRIXB Offset: 0x110) (R/W 32) Special Function -------- */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
    struct {
        uint32_t SFR:32;           /*!< bit:  0..31  Special Function Register          */
    } bit;                       /*!< Structure used for bit  access                  */
    uint32_t reg;                /*!< Type      used for register access              */
} HMATRIXB_SFR_Type;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

#define HMATRIXB_SFR_OFFSET         0x110        /**< \brief (HMATRIXB_SFR offset) Special Function */
#define HMATRIXB_SFR_RESETVALUE     0x00000000ul /**< \brief (HMATRIXB_SFR reset_value) Special Function */

#define HMATRIXB_SFR_SFR_Pos        0            /**< \brief (HMATRIXB_SFR) Special Function Register */
#define HMATRIXB_SFR_SFR_Msk        (0xFFFFFFFFul << HMATRIXB_SFR_SFR_Pos)
#define HMATRIXB_SFR_SFR(value)     ((HMATRIXB_SFR_SFR_Msk & ((value) << HMATRIXB_SFR_SFR_Pos)))
#define HMATRIXB_SFR_MASK           0xFFFFFFFFul /**< \brief (HMATRIXB_SFR) MASK Register */

/** \brief HmatrixbPrs hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
    __IO HMATRIXB_PRAS_Type        PRAS;        /**< \brief Offset: 0x000 (R/W 32) Priority A for Slave */
    __IO HMATRIXB_PRBS_Type        PRBS;        /**< \brief Offset: 0x004 (R/W 32) Priority B for Slave */
} HmatrixbPrs;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/** \brief HMATRIXB hardware registers */
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct {
    RoReg8                    Reserved1[0x80];
    HmatrixbPrs               Prs[16];     /**< \brief Offset: 0x080 HmatrixbPrs groups */
    RoReg8                    Reserved2[0x10];
    __IO HMATRIXB_SFR_Type         SFR[16];     /**< \brief Offset: 0x110 (R/W 32) Special Function */
} Hmatrixb;
#endif /* !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) */

/*@}*/

#endif /* _SAMD21_HMATRIXB_COMPONENT_ */
