/**************************************************************************//**
 * @file     keystore_reg.h
 * @version  V1.00
 * @brief    Key store register definition header file
 *
 * @copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __KEYSTORE_REG_H__
#define __KEYSTORE_REG_H__

/** @addtogroup REGISTER Control Register

  @{

*/

/*---------------------- Key Store Interface Controller -------------------------*/
/**
    @addtogroup Key Store Interface Controller(KS)
    Memory Mapped Structure for Key Store Controller
@{ */

typedef struct
{


/**
 * @var KS_T::CTL
 * Offset: 0x00  Key Store Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |START     |Key Store Start Control Bit
 * |        |          |0 = No operation
 * |        |          |1 = Start the setted operation
 * |[3:1]   |OPMODE    |Key Store Operation Mode
 * |        |          |000 = Read operation
 * |        |          |001 = Create operation
 * |        |          |010 = Erase one key operation (only for key is in SRAM)
 * |        |          |011 = Erase all keys operation (only for SRAM and Flash)
 * |        |          |100 = Revoke key operation
 * |        |          |101 = Read metadata operation
 * |        |          |Others = reserved
 * |[7]     |CONT      |Read/Write Key Continue bit
 * |        |          |0 = Read/Write key operation is not continuous to previous operation
 * |        |          |1 = Read/Write key operation is continuous to previous operation
 * |[8]     |INIT      |Key Store Initialization
 * |        |          |User should to check BUSY(KS_STS[2]) is 0, and then write 1 to this bit and START(KS_CTL[0[), Key Store will start to initialization.
 * |[9]     |REMNE     |Data Remanence Control bit
 * |        |          |User should to check BUSY(KS_STS[2]) is 0, and then write 1 to this bit and START(KS_CTL[0[), the data remanence prevention function will be operated
 * |[14]    |TCLR      |Tamper Clear Control Bit
 * |        |          |0 = Key Store does not clear all key at SRAM/Flash and revoke all OTP keys when tamper occurs.
 * |        |          |1 = Key Store clear all key at SRAM/Flash and revoke all OTP keys when tamper occurs.
 * |[15]    |IEN       |Key Store Interrupt Enable Bit
 * |        |          |0 = Key Store interrupt is disabled
 * |        |          |1 = Key Store interrupt is enabled
 * |[31:16] |Reversed  |Reversed
 * @var KS_T::METADATA
 * Offset: 0x04  Key Store Metadata Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |SEC       |Secure Key Selection Bit
 * |        |          |0 = Set key as the non-secure key
 * |        |          |1 = Set key as the secure key
 * |[1]     |PRIV      |Privilege Key Selection bit
 * |        |          |0 = Set key as the non-privilege key
 * |        |          |1 = Set key as the privilege key
 * |[2]     |READABLE  |Key Readable Control bit
 * |        |          |0 = key is un-readable
 * |        |          |1 = key is readable
 * |[3]     |RVK       |Key Revoke Control Bit
 * |        |          |0 = Key in Flash current selected will not be changed
 * |        |          |1 = key in Flash current selected will be change to revoked state
 * |[4]     |BS        |Booting Stage Selection bit
 * |        |          |0 = Set key used at normal stage
 * |        |          |1 = Set key used at boot loader stage 1 (BL1)
 * |[11:8]  |SIZE      |Key Size Selection Bits
 * |        |          |0000 = 128 bits.
 * |        |          |0001 = 163 bits
 * |        |          |0010 = 192 bits
 * |        |          |0011 = 224 bits
 * |        |          |0100 = 233 bits
 * |        |          |0101 = 255 bits
 * |        |          |0110 = 256 bits
 * |        |          |0111 = 283 bits
 * |        |          |1000 = 384 bits
 * |        |          |1001 = 409 bits
 * |        |          |1010 = 512 bits
 * |        |          |1011 = 521 bits
 * |        |          |1100 = 571 bits
 * |        |          |1101 = 1024 bits
 * |        |          |1110 = 2048 bits
 * |        |          |1111 = 4096 bits.
 * |[18:16] |TYPE      |Key Type Selection Bits
 * |        |          |000 = Only for AES used
 * |        |          |001 = Only for HMAC engine used
 * |        |          |010 = Only for RSA engine exponential used (private key)
 * |        |          |011 = Only for RSA engine middle data used
 * |        |          |100 = Only for ECC engine used
 * |        |          |101 = Only for CPU engine use
 * |        |          |Others = reserved
 * |[19]    |Reversed  |Reversed
 * |[25:20] |NUMBER    |Key Number
 * |        |          |Before read or erase one key operation start, user should write the key number to be operated
 * |        |          |When create operation is finished, user can read these bits to get its key number.
 * |[31:30] |DST       |Key Location Selection Bits
 * |        |          |00 = key is in SRAM
 * |        |          |01 = key is in Flash
 * |        |          |10 = key is in OTP
 * |        |          |Others = reserved
 * @var KS_T::STS
 * Offset: 0x08  Key Store Status Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |IF        |Key Store Initialization
 * |        |          |This bit is cleared by writing 1 and it has no effect by writing 0.
 * |        |          |0 = Key Store initialization is disabled
 * |        |          |1 = Key Store initialization is enabled
 * |[1]     |EIF       |Data Remanence Control bit
 * |        |          |This bit is cleared by writing 1 and it has no effect by writing 0.
 * |        |          |0 = Data remanence is disabled
 * |        |          |1 = Data remanence is enabled
 * |[2]     |BUSY      |Root Key Selection Bit (RO)
 * |        |          |0 = The key is not root key
 * |        |          |1 = The key is root key
 * |[3]     |SRAMFULL  |Key Storage at SRAM Full Status Bit (RO)
 * |        |          |0 = Key Storage at SRAM is not full
 * |        |          |1 = Key Storage at SRAM is full
 * |[4]     |FLASHFULL |Key Storage at Flash Full Status Bit (RO)
 * |        |          |0 = Key Storage at Flash is not full
 * |        |          |1 = Key Storage at Flash is full
 * |[5]     |SCMBSTS   |Key Store Data Scramble Status (RO)
 * |        |          |0 = Data scramble function is disabled
 * |        |          |1 = Data scramble function is enabled
 * |[6]     |SILACSSTS |Key Store Silent Access Status (RO)
 * |        |          |0 = Silent access function is disabled
 * |        |          |1 = Silent access function is enabled
 * |[7]     |INITDONE  |Key Store Initialization Done Status (RO)
 * |        |          |0 = Key Store is un-initialized
 * |        |          |1 = Key Store is initialized
 * |[31:8]  |Reversed  |Reversed
 * @var KS_T::REMAINING
 * Offset: 0x0C  Key Store Remaining Space Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[12:0]  |RRMNG     |Key Store SRAM Remaining Space
 * |        |          |The RRMNG shows the remaining byte count space of SRAM
 * |[27:16] |FRMNG     |Key Store Flash Remaining Space
 * |        |          |The FRMNG shows the remaining byte count space of Flash 
 * |[31:28] |Reversed  |Reversed
 * @var KS_T::SCMBKEY
 * Offset: 0x10  Key Store Scramble Key Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |SCMBKEY   |Key Store Scramble Key
 * |        |          |When SCMBSTS(KS_STS[5]) is set to 1, user should write the scramble key in this register before new key stores in Key Store
 * |        |          |If user does not write the scramble key in this register, the Key Store will use previous scramble key to execute data scramble function.
 * @var KS_T::KEY0
 * Offset: 0x20  Key Store Entry Key Word 0 Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |KEY       |Key Data
 * |        |          |These register will be clear if Key Store executes the write operation or CPU completes the reading key.
 * @var KS_T::KEY1
 * Offset: 0x24  Key Store Entry Key Word 1 Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |KEY       |Key Data
 * |        |          |These register will be clear if Key Store executes the write operation or CPU completes the reading key.
 * @var KS_T::KEY2
 * Offset: 0x28  Key Store Entry Key Word 2 Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |KEY       |Key Data
 * |        |          |These register will be clear if Key Store executes the write operation or CPU completes the reading key.
 * @var KS_T::KEY3
 * Offset: 0x2C  Key Store Entry Key Word 3 Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |KEY       |Key Data
 * |        |          |These register will be clear if Key Store executes the write operation or CPU completes the reading key.
 * @var KS_T::KEY4
 * Offset: 0x30  Key Store Entry Key Word 4 Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |KEY       |Key Data
 * |        |          |These register will be clear if Key Store executes the write operation or CPU completes the reading key.
 * @var KS_T::KEY5
 * Offset: 0x34  Key Store Entry Key Word 5 Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |KEY       |Key Data
 * |        |          |These register will be clear if Key Store executes the write operation or CPU completes the reading key.
 * @var KS_T::KEY6
 * Offset: 0x38  Key Store Entry Key Word 6 Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |KEY       |Key Data
 * |        |          |These register will be clear if Key Store executes the write operation or CPU completes the reading key.
 * @var KS_T::KEY7
 * Offset: 0x3C  Key Store Entry Key Word 7 Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |KEY       |Key Data
 * |        |          |These register will be clear if Key Store executes the write operation or CPU completes the reading key.
 */
    __IO uint32_t CTL;                   /*!< [0x0000] Key Store Control Register                                       */
    __IO uint32_t METADATA;              /*!< [0x0004] Key Store Metadata Register                                      */
    __IO uint32_t STS;                   /*!< [0x0008] Key Store Status Register                                        */
    __I  uint32_t REMAIN;                /*!< [0x000c] Key Store Remaining Space Register                               */
    __O  uint32_t SCMBKEY;               /*!< [0x0010] Key Store Scramble Key Register                                  */
    __I  uint32_t RESERVE0[3];
    __IO uint32_t KEY[8];                /*!< [0x0020-0x003c] Key Store Entry Key Word 0 Register                              */
    __I  uint32_t OTPSTS;       

} KS_T;

/**
    @addtogroup KS_CONST Key Store Bit Field Definition
    Constant Definitions for Key Store Controller
@{ */


#define KS_CTL_START_Pos                 (0)                                               /*!< KS_T::CTL: START Position              */
#define KS_CTL_START_Msk                 (0x1ul << KS_CTL_START_Pos)                       /*!< KS_T::CTL: START Mask                  */

#define KS_CTL_OPMODE_Pos                (1)                                               /*!< KS_T::CTL: OPMODE Position             */
#define KS_CTL_OPMODE_Msk                (0x7ul << KS_CTL_OPMODE_Pos)                      /*!< KS_T::CTL: OPMODE Mask                 */

#define KS_CTL_CONT_Pos                  (7)                                               /*!< KS_T::CTL: CONT Position               */
#define KS_CTL_CONT_Msk                  (0x1ul << KS_CTL_CONT_Pos)                        /*!< KS_T::CTL: CONT Mask                   */

#define KS_CTL_INIT_Pos                  (8)                                               /*!< KS_T::CTL: INIT Position               */
#define KS_CTL_INIT_Msk                  (0x1ul << KS_CTL_INIT_Pos)                        /*!< KS_T::CTL: INIT Mask                   */

#define KS_CTL_REMNE_Pos                 (9)                                               /*!< KS_T::CTL: REMNE Position              */
#define KS_CTL_REMNE_Msk                 (0x1ul << KS_CTL_REMNE_Pos)                       /*!< KS_T::CTL: REMNE Mask                  */

#define KS_CTL_SILENT_Pos                (10)                                              /*!< KS_T::CTL: SILENT Position              */
#define KS_CTL_SILENT_Msk                (0x1ul << KS_CTL_SILENT_Pos)                      /*!< KS_T::CTL: SILENT Mask                  */

#define KS_CTL_SCMB_Pos                  (11)                                              /*!< KS_T::CTL: SCMB Position              */
#define KS_CTL_SCMB_Msk                  (0x1ul << KS_CTL_SCMB_Pos)                        /*!< KS_T::CTL: SCMB Mask                  */

#define KS_CTL_TCLR_Pos                  (14)                                              /*!< KS_T::CTL: TCLR Position               */
#define KS_CTL_TCLR_Msk                  (0x1ul << KS_CTL_TCLR_Pos)                        /*!< KS_T::CTL: TCLR Mask                   */

#define KS_CTL_IEN_Pos                   (15)                                              /*!< KS_T::CTL: IEN Position                */
#define KS_CTL_IEN_Msk                   (0x1ul << KS_CTL_IEN_Pos)                         /*!< KS_T::CTL: IEN Mask                    */

#define KS_CTL_Reversed_Pos              (16)                                              /*!< KS_T::CTL: Reversed Position           */
#define KS_CTL_Reversed_Msk              (0xfffful << KS_CTL_Reversed_Pos)                 /*!< KS_T::CTL: Reversed Mask               */

#define KS_METADATA_SEC_Pos              (0)                                               /*!< KS_T::METADATA: SEC Position           */
#define KS_METADATA_SEC_Msk              (0x1ul << KS_METADATA_SEC_Pos)                    /*!< KS_T::METADATA: SEC Mask               */

#define KS_METADATA_PRIV_Pos             (1)                                               /*!< KS_T::METADATA: PRIV Position          */
#define KS_METADATA_PRIV_Msk             (0x1ul << KS_METADATA_PRIV_Pos)                   /*!< KS_T::METADATA: PRIV Mask              */

#define KS_METADATA_READABLE_Pos         (2)                                               /*!< KS_T::METADATA: READABLE Position      */
#define KS_METADATA_READABLE_Msk         (0x1ul << KS_METADATA_READABLE_Pos)               /*!< KS_T::METADATA: READABLE Mask          */

#define KS_METADATA_RVK_Pos              (3)                                               /*!< KS_T::METADATA: RVK Position           */
#define KS_METADATA_RVK_Msk              (0x1ul << KS_METADATA_RVK_Pos)                    /*!< KS_T::METADATA: RVK Mask               */

#define KS_METADATA_BS_Pos               (4)                                               /*!< KS_T::METADATA: BS Position            */
#define KS_METADATA_BS_Msk               (0x1ul << KS_METADATA_BS_Pos)                     /*!< KS_T::METADATA: BS Mask                */

#define KS_METADATA_SIZE_Pos             (8)                                               /*!< KS_T::METADATA: SIZE Position          */
#define KS_METADATA_SIZE_Msk             (0x1ful << KS_METADATA_SIZE_Pos)                   /*!< KS_T::METADATA: SIZE Mask              */

#define KS_METADATA_TYPE_Pos             (16)                                              /*!< KS_T::METADATA: TYPE Position          */
#define KS_METADATA_TYPE_Msk             (0x7ul << KS_METADATA_TYPE_Pos)                   /*!< KS_T::METADATA: TYPE Mask              */

#define KS_METADATA_Reversed_Pos         (19)                                              /*!< KS_T::METADATA: Reversed Position      */
#define KS_METADATA_Reversed_Msk         (0x1ul << KS_METADATA_Reversed_Pos)               /*!< KS_T::METADATA: Reversed Mask          */

#define KS_METADATA_NUMBER_Pos           (20)                                              /*!< KS_T::METADATA: NUMBER Position        */
#define KS_METADATA_NUMBER_Msk           (0x3ful << KS_METADATA_NUMBER_Pos)                /*!< KS_T::METADATA: NUMBER Mask            */

#define KS_METADATA_DST_Pos              (30)                                              /*!< KS_T::METADATA: DST Position           */
#define KS_METADATA_DST_Msk              (0x3ul << KS_METADATA_DST_Pos)                    /*!< KS_T::METADATA: DST Mask               */

#define KS_STS_IF_Pos                    (0)                                               /*!< KS_T::STS: IF Position                 */
#define KS_STS_IF_Msk                    (0x1ul << KS_STS_IF_Pos)                          /*!< KS_T::STS: IF Mask                     */

#define KS_STS_EIF_Pos                   (1)                                               /*!< KS_T::STS: EIF Position                */
#define KS_STS_EIF_Msk                   (0x1ul << KS_STS_EIF_Pos)                         /*!< KS_T::STS: EIF Mask                    */

#define KS_STS_BUSY_Pos                  (2)                                               /*!< KS_T::STS: BUSY Position               */
#define KS_STS_BUSY_Msk                  (0x1ul << KS_STS_BUSY_Pos)                        /*!< KS_T::STS: BUSY Mask                   */

#define KS_STS_SRAMFULL_Pos              (3)                                               /*!< KS_T::STS: SRAMFULL Position           */
#define KS_STS_SRAMFULL_Msk              (0x1ul << KS_STS_SRAMFULL_Pos)                    /*!< KS_T::STS: SRAMFULL Mask               */

#define KS_STS_FLASHFULL_Pos             (4)                                               /*!< KS_T::STS: FLASHFULL Position          */
#define KS_STS_FLASHFULL_Msk             (0x1ul << KS_STS_FLASHFULL_Pos)                   /*!< KS_T::STS: FLASHFULL Mask              */

#define KS_STS_SCMBSTS_Pos               (5)                                               /*!< KS_T::STS: SCMBSTS Position            */
#define KS_STS_SCMBSTS_Msk               (0x1ul << KS_STS_SCMBSTS_Pos)                     /*!< KS_T::STS: SCMBSTS Mask                */

#define KS_STS_SILACSSTS_Pos             (6)                                               /*!< KS_T::STS: SILACSSTS Position          */
#define KS_STS_SILACSSTS_Msk             (0x1ul << KS_STS_SILACSSTS_Pos)                   /*!< KS_T::STS: SILACSSTS Mask              */

#define KS_STS_INITDONE_Pos              (7)                                               /*!< KS_T::STS: INITDONE Position           */
#define KS_STS_INITDONE_Msk              (0x1ul << KS_STS_INITDONE_Pos)                    /*!< KS_T::STS: INITDONE Mask               */

#define KS_STS_RAMINV_Pos                (8)                                               /*!< KS_T::STS: RAMINV_Pos Position         */
#define KS_STS_RAMINV_Msk                (0xfffffful << KS_STS_RAMINV_Pos)                 /*!< KS_T::STS: RAMINV_Pos Mask             */

#define KS_REMAIN_RRMNG_Pos              (0)                                               /*!< KS_T::REMAINING: RRMNG Position        */
#define KS_REMAIN_RRMNG_Msk              (0x1ffful << KS_REMAIN_RRMNG_Pos)                 /*!< KS_T::REMAINING: RRMNG Mask            */

#define KS_REMAIN_FRMNG_Pos              (16)                                              /*!< KS_T::REMAINING: FRMNG Position        */
#define KS_REMAIN_FRMNG_Msk              (0xffful << KS_REMAIN_FRMNG_Pos)                  /*!< KS_T::REMAINING: FRMNG Mask            */

#define KS_REMAIN_Reversed_Pos           (28)                                              /*!< KS_T::REMAINING: Reversed Position     */
#define KS_REMAIN_Reversed_Msk           (0xful << KS_REMAIN_Reversed_Pos)                 /*!< KS_T::REMAINING: Reversed Mask         */

#define KS_SCMBKEY_SCMBKEY_Pos           (0)                                               /*!< KS_T::SCMBKEY: SCMBKEY Position        */
#define KS_SCMBKEY_SCMBKEY_Msk           (0xfffffffful << KS_SCMBKEY_SCMBKEY_Pos)          /*!< KS_T::SCMBKEY: SCMBKEY Mask            */

#define KS_KEY0_KEY_Pos                  (0)                                               /*!< KS_T::KEY0: KEY Position               */
#define KS_KEY0_KEY_Msk                  (0xfffffffful << KS_KEY0_KEY_Pos)                 /*!< KS_T::KEY0: KEY Mask                   */

#define KS_KEY1_KEY_Pos                  (0)                                               /*!< KS_T::KEY1: KEY Position               */
#define KS_KEY1_KEY_Msk                  (0xfffffffful << KS_KEY1_KEY_Pos)                 /*!< KS_T::KEY1: KEY Mask                   */

#define KS_KEY2_KEY_Pos                  (0)                                               /*!< KS_T::KEY2: KEY Position               */
#define KS_KEY2_KEY_Msk                  (0xfffffffful << KS_KEY2_KEY_Pos)                 /*!< KS_T::KEY2: KEY Mask                   */

#define KS_KEY3_KEY_Pos                  (0)                                               /*!< KS_T::KEY3: KEY Position               */
#define KS_KEY3_KEY_Msk                  (0xfffffffful << KS_KEY3_KEY_Pos)                 /*!< KS_T::KEY3: KEY Mask                   */

#define KS_KEY4_KEY_Pos                  (0)                                               /*!< KS_T::KEY4: KEY Position               */
#define KS_KEY4_KEY_Msk                  (0xfffffffful << KS_KEY4_KEY_Pos)                 /*!< KS_T::KEY4: KEY Mask                   */

#define KS_KEY5_KEY_Pos                  (0)                                               /*!< KS_T::KEY5: KEY Position               */
#define KS_KEY5_KEY_Msk                  (0xfffffffful << KS_KEY5_KEY_Pos)                 /*!< KS_T::KEY5: KEY Mask                   */

#define KS_KEY6_KEY_Pos                  (0)                                               /*!< KS_T::KEY6: KEY Position               */
#define KS_KEY6_KEY_Msk                  (0xfffffffful << KS_KEY6_KEY_Pos)                 /*!< KS_T::KEY6: KEY Mask                   */

#define KS_KEY7_KEY_Pos                  (0)                                               /*!< KS_T::KEY7: KEY Position               */
#define KS_KEY7_KEY_Msk                  (0xfffffffful << KS_KEY7_KEY_Pos)                 /*!< KS_T::KEY7: KEY Mask                   */





/**@}*/ /* KS_CONST */
/**@}*/ /* end of Key store register group */
/**@}*/ /* end of REGISTER group */

#endif /* __KEYSTORE_REG_H__ */
