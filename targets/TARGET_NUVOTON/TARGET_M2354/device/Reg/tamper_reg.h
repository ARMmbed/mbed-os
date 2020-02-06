/**************************************************************************//**
 * @file     tamper_reg.h
 * @version  V1.00
 * @brief    Tamper register definition header file
 *
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __TAMPER_REG_H__
#define __TAMPER_REG_H__

/** @addtogroup REGISTER Control Register

  @{

*/


/*---------------------- Tamper Interface Controller -------------------------*/
/**
    @addtogroup Tamper Interface Controller(TAMPER)
    Memory Mapped Structure for TAMPER Controller
@{ */
 
typedef struct
{


/**
 * @var TAMP_T::INIT
 * Offset: 0x00  TAMPER Function Initiation Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |TCORERST  |TAMPER Core Reset
 * |        |          |0 = Write 0x5500, the TAMPER coreblock will be reset.
 * |        |          |01 = Write 0x55aa00, the TAMPER coreblock reset will be released.
 * |        |          |1 = Write 0x55aa, the TAMPER coreblock will be reset.
 * |[31]    |TLDORDY   |TLDO Power Ready (Read Only)
 * |        |          |0 = TIndcates the power status of TLDO is not ready.
 * |        |          |1 = Indiactes tThe power status of TLDO is ready.
 * @var TAMP_T::FUNEN
 * Offset: 0x04  TAMPER Block Function Enable Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |LXTDETEN  |LXT Clock Detection Enable Bit
 * |        |          |0 = Write 0x40, the LXT clock detection is Disabled.
 * |        |          |1 = Write 0x44, the LXT clock detection is Enabled .
 * |[1]     |HXTDETEN  |HXT Detection Enable Bit
 * |        |          |0 = Write 0x30, the HXT detection is Disabled.
 * |        |          |1 = Write 0x34, the HXT detection is Enabled .
 * |[13:8]  |TMPIOSEL  |Tamper IO Dection Selection Bit
 * |        |          |0 = Write 0x90/0xA0/0xB0/0xC0/0xD0/0xE0 for tamper I/O 0~5, the I/O tamper function is detected through RTC block.
 * |        |          |1 = Write 0x94/0xA4/0xB4/0xC4/0xD4/0xE4 for tamper I/O 0~5, the I/O tamper function is detected through TAMPER block.
 * |[23:16] |HIRC12MEN |HIRC12M Enable Bit
 * |        |          |The HIRC12M is disable when theses bits equal 0x5a, otherwise it will be enable with any other values.
 * @var TAMP_T::TRIEN
 * Offset: 0x08  TAMPER Trigger Enable Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |FMCTRIGEN |FMC Trigger Enable Bit
 * |        |          |0 = Tamper event is detected and to trigger FMC Disabled.
 * |        |          |1 = Tamper event is detected and to trigger FMC Enabled.
 * |[1]     |KSTRIGEN  |Key Store Trigger Enable Bit
 * |        |          |0 = Tamper event is detected and to trigger Key Store Disabled.
 * |        |          |1 = Tamper event is detected and to trigger Key Store Enabled.
 * |[2]     |WakeupEN  |Wakeup Enable Bit
 * |        |          |0 = Tamper wakeup event is Disabled.
 * |        |          |1 = Tamper wakeup event is Enabled.
 * @var TAMP_T::INTEVEN
 * Offset: 0x0C  TAMPER Interrupt Event Enable Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |TAMP0EN   |Tamper 0 Event Enable Bit
 * |        |          |Set TAMP0IEN to 1 can also enable chip wake-up function when tamper 0 event is generated.
 * |        |          |0 = Tamper 0 event Disabled.
 * |        |          |1 = Tamper 0 event Enabled.
 * |[1]     |TAMP1EN   |Tamper 1 or Pair 0 Event Enable Bit
 * |        |          |Set TAMP1IEN to 1 can also enable chip wake-up function when tamper 1 event is generated.
 * |        |          |0 = Tamper 1 or Pair 0 event Disabled.
 * |        |          |1 = Tamper 1 or Pair 0 event Enabled.
 * |[2]     |TAMP2EN   |Tamper 2 Event Enable Bit
 * |        |          |Set TAMP2IEN to 1 can also enable chip wake-up function when tamper 2 event is generated.
 * |        |          |0 = Tamper 2 event Disabled.
 * |        |          |1 = Tamper 2 event Enabled.
 * |[3]     |TAMP3EN   |Tamper 3 or Pair 1 Event Enable Bit
 * |        |          |Set TAMP3IEN to 1 can also enable chip wake-up function when tamper 3 event is generated.
 * |        |          |0 = Tamper 3 or Pair 1 event Disabled.
 * |        |          |1 = Tamper 3 or Pair 1 event Enabled.
 * |[4]     |TAMP4EN   |Tamper 4 Event Enable Bit
 * |        |          |Set TAMP4IEN to 1 can also enable chip wake-up function when tamper 4 event is generated.
 * |        |          |0 = Tamper 4 event Disabled.
 * |        |          |1 = Tamper 4 event Enabled.
 * |[5]     |TAMP5EN   |Tamper 5 or Pair 2 Event Enable Bit
 * |        |          |Set TAMP5IEN to 1 can also enable chip wake-up function when tamper 5 event is generated.
 * |        |          |0 = Tamper 5 or Pair 2 event Disabled.
 * |        |          |1 = Tamper 5 or Pair 2 event Enabled.
 * |[6]     |CLKFEN    |LXT Clock Frequency Monitor Fail Event Enable Bit
 * |        |          |0 = LXT frequency Stop fail event Disabled.
 * |        |          |1 = LXT frequency Stop fail event Enabled.
 * |[7]     |CLKSTOPEN |LXT Clock Frequency Monitor STOP Stop Event Enable Bit
 * |        |          |0 = LXT frequency Fail stop event Disabled.
 * |        |          |1 = LXT frequency Fail stop event Enabled.
 * |[8]     |OVPEN     |CORE_S over over Voltage Protect Detection Enable Bit
 * |        |          |0 = Detect Core_SB over voltage protect detection Disabled.
 * |        |          |1 = Detect Core_SB over voltage protect detection Enabled.
 * |        |          |Note: The function enable of the over voltage detection is defined in GMISC.
 * |[9]     |PWRPEN    |Power DEGLITCH Glitch Postivie Dection Event Enable Bit
 * |        |          |0 = Core Power postive deglitch event Disabled.
 * |        |          |1 = Core Power postive deglitch event Enabled.
 * |[10]    |PWRNEN    |Power DEGLITCH Glitch Negative Dection Event Enable Bit
 * |        |          |0 = Core Power negative deglitch event Disabled.
 * |        |          |1 = Core Power negative deglitch event Enabled.
 * |[11]    |ACTSEN    |Active Shiled Event Enable Bit
 * |        |          |0 = Active shield event Disabled.
 * |        |          |1 = Active shiled event Enabled.
 * |[16]    |RTCLVREN  |RTC Low Voltage Dection Event Enable Bit
 * |        |          |0 = RTC Power low volatage detection event Disabled.
 * |        |          |1 = RTC Power low volatage detection event Enabled.
 * |[17]    |RTCIOEN   |RTC Tamper IO Event Enable Bit
 * |        |          |0 = RTC tamper I/O detection event Disabled.
 * |        |          |1 = RTC tamper I/O detection event Enabled.
 * |[18]    |RTCLKEN   |RTC Clock Monitor Detection Event Enable Bit
 * |        |          |0 = RTC clcok monitor event Disabled.
 * |        |          |1 = RTC clcok monitor event Enabled.
 * |[19]    |HXTERREN  |Core HXT Error Event EVENT Enable Bit
 * |        |          |0 = Core HXT errorvent event Disabled.
 * |        |          |1 = Core HXT errorvent event Enabled.
 * |[20]    |VLOSSEN   |VBAT Power Loss Event Enable Bit
 * |        |          |0 = VBAT power loss event Disabled.
 * |        |          |1 = VBAT power loss event Enabled.
 * |[21]    |WDTEN     |Watch Dog Event Enable Bit
 * |        |          |0 = Watch dog event Disabled.
 * |        |          |1 = Watch dog event Enabled.
 * |        |          |Note: If there is WDT event, it can be used to enable to tamper trigger fucntion when the enable bit is set to 1.
 * |[22]    |BODEN     |BOD Event Enable Bit
 * |        |          |0 = Brown out event Disabled.
 * |        |          |1 = Brown out Enabled.
 * @var TAMP_T::EVSTS
 * Offset: 0x10  TAMPER Event Status Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |TAMP0IF   |Tamper 0 Event Flag
 * |        |          |0 = No Tamper 0 event flag is generated.
 * |        |          |1 = Tamper 0 event flag is generated.
 * |        |          |Note 1: Write 1 to clear this bit.
 * |[1]     |TAMP1IF   |Tamper 1 Event Flag
 * |        |          |0 = No Tamper 1 event flag is generated.
 * |        |          |1 = Tamper 1 event flag is generated.
 * |        |          |Note 1: Write 1 to clear this bit.
 * |[2]     |TAMP2IF   |Tamper 2 Event Flag
 * |        |          |0 = No Tamper 2 event flag is generated.
 * |        |          |1 = Tamper 2 event flag is generated.
 * |        |          |Note 1: Write 1 to clear this bit.
 * |[3]     |TAMP3IF   |Tamper 3 Event Flag
 * |        |          |0 = No Tamper 3 event flag is generated.
 * |        |          |1 = Tamper 3 event flag is generated.
 * |        |          |Note 1: Write 1 to clear this bit.
 * |[4]     |TAMP4IF   |Tamper 4 Event Flag
 * |        |          |0 = No Tamper 4 event flag is generated.
 * |        |          |1 = Tamper 4 event flag is generated.
 * |        |          |Note 1: Write 1 to clear this bit.
 * |[5]     |TAMP5IF   |Tamper 5 Event Flag
 * |        |          |0 = No Tamper 5 event flag is generated.
 * |        |          |1 = Tamper 5 flag is generated.
 * |        |          |Note 1: Write 1 to clear this bit.
 * |[6]     |CLKFAIL   |LXT Clock Frequency Monitor Fail Event Flag
 * |        |          |0 = LXT frequency is normal.
 * |        |          |1 = LXT frequency is abnormal.
 * |        |          |Note 1: Write 1 to clear the bit to 0.
 * |        |          |Note 2: LXT detector will automatic disable when Fail/Stop flag rise , resume after Fail/Stop flag clear.
 * |[7]     |CLKSTOP   |LXT Clock Frequency Monitor STOP Stop Event Flag
 * |        |          |0 = LXT frequency is normal.
 * |        |          |1 = LXT frequency is almost stopped ..
 * |        |          |Note 1: Write 1 to clear the bit to 0.
 * |        |          |Note 2: LXT detector will automatic disable when Fail/Stop flag rise , resume after Fail/Stop flag clear.
 * |[8]     |OVPOUT    |CORE SB over over Voltage Event Flag
 * |        |          |0 = Core_SB no over voltage detected..
 * |        |          |1 = Core_SB over voltage detected.
 * |[9]     |PWRPEV    |Power DEGLITCH Glitch Postivie Dection Flag
 * |        |          |0 = Core Power postive deglitch is not detected.
 * |        |          |1 = Core Power postive deglitch is detected.
 * |[10]    |PWRNEV    |Power DEGLITCH Glitch Negative Dection Flag
 * |        |          |0 = Core Power negative deglitch is not detected.
 * |        |          |1 = Core Power negative deglitch is detected.
 * |[11]    |ACTSEF    |Active Shield Event Dection Flag
 * |        |          |0 = Active Shield shield Event event is not detected.
 * |        |          |1 = Active Shield shield Event event is detected including the VDD and GND attack.
 * |[16]    |RTCLVR    |RTC Low Voltage Dection Event Bit
 * |        |          |0 = RTC Power low volatage detection event is not detected.
 * |        |          |1 = RTC Power low volatage detection event is detected.
 * |[17]    |RIOTRIG   |RTC Tamper IO Event Flag
 * |        |          |0 = There is no RTC tamper I/O detection event.
 * |        |          |1 = There is RTC tamper I/O detection event.
 * |[18]    |RCLKTRIG  |RTC Clock Monitor Detection Event Bit
 * |        |          |0 = There is no RTC clock monitor detection event.
 * |        |          |1 = There is RTC clock monitor detection event.
 * |[19]    |HXTERR    |Core HXT Error Event Flag
 * |        |          |0 = Core HXT no fail event.
 * |        |          |1 = Core HXT fail event.
 * |[20]    |VBATLOSS  |RTC VBAT LOSS Dection Event Bit
 * |        |          |0 = RTC VBAT Power loss detection event is not detected.
 * |        |          |1 = RTC VBAT Power loss detection event is detected.
 * |[21]    |SECWDT    |Security Watch Dog Event Bit
 * |        |          |0 = No security WDT event detected.
 * |        |          |1 = Security WDT event is detected.
 * |        |          |Note: Write 1 to clear this bit.
 * |[22]    |TLDOBOD   |Tamper LDO BOD Event Bit
 * |        |          |0 = Brown out event no detected.
 * |        |          |1 = Brown out is detected.
 * |[23]    |LXTSLOWF  |LXT Slower Than TLIRC32K Flag (Read Only )
 * |        |          |0 = 32.768 kHz external low speed crystal oscillator (LXT) frequency faster than TLIRC32K.
 * |        |          |1 = LXT frequency is slowly.
 * |        |          |Note: LXTSLOWF is vaild during CLKSPIF (TAMP_EVSTS[7]) or CLKFIF (TAMP_EVSTS[6]) rising.
 * |[25]    |ACTST1IF  |Active Shield Tamper 1 Event Flag
 * |        |          |0 = No Active shiled Tamper 1 event flag is generated.
 * |        |          |1 = Active shiled Tamper 1 event flag is generated.
 * |        |          |Note: Write 1 to clear this bit.
 * |[27]    |ACTST3IF  |Active Shield Tamper 3 Event Flag
 * |        |          |0 = No Active shiled Tamper 3 event flag is generated.
 * |        |          |1 = Active shiled Tamper 3 event flag is generated.
 * |        |          |Note: Write 1 to clear this bit.
 * |[29]    |ACTST21IF |2th Active Shield Tamper 1 Event Flag
 * |        |          |0 = No Active shiled Tamper 1 event flag is generated.
 * |        |          |1 = 2th Active shiled Tamper 1 event flag is generated.
 * |        |          |Note: Write 1 to clear this bit.
 * |[31]    |ACTST23IF |2th Active Shield Tamper 3 Event Flag
 * |        |          |0 = No Active shiled Tamper 3 event flag is generated.
 * |        |          |1 = 2th Active shiled Tamper 3 event flag is generated.
 * |        |          |Note: Write 1 to clear this bit.
 * @var TAMP_T::LIRCTL
 * Offset: 0x14  TAMPER LIRC Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[8:0]   |TLRCTRIM  |Tamper TLIRC32K Trim Value
 * |        |          |TLIRC32K trim value setting
 * |[10:9]  |TRIMMOS   |Tamper TLIRC32K Trim MOS Value
 * |        |          |TLIRC32K trim MOS value setting
 * |[16]    |CKSEL12M  |Tamper Clock Selection Bit
 * |        |          |0 = The 32K LIRC is selected for VoltagePower DegGlitch ModuleTamper Controller.
 * |        |          |1 = The 12M HIRC is selected for VoltagePower DegGlitch ModuleTamper Controller.
 * @var TAMP_T::TIOCTL
 * Offset: 0x18  TAMPERu2019s Tamper I/O Function Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |DYN1ISS   |Dynamic Pair 1 Input Source Select
 * |        |          |This bit determines Tamper 3 input is from Tamper 2 or Tamper 0 in dynamic mode.
 * |        |          |0 = Tamper input is from Tamper 2.
 * |        |          |1 = Tamper input is from Tamper 0.
 * |        |          |Note: This bit is effective only when DYNPR1EN (TAMPRTC_TAMPIOCTL[1623]) and DYNPR0EN (TAMPRTC_TAMPIOCTL[15]) are set.
 * |[1]     |DYN2ISS   |Dynamic Pair 2 Input Source Select
 * |        |          |This bit determines Tamper 5 input is from Tamper 4 or Tamper 0 in dynamic mode.
 * |        |          |0 = Tamper input is from Tamper 4.
 * |        |          |1 = Tamper input is from Tamper 0.
 * |        |          |Note: This bit has effect only when DYNPR2EN (RTCTAMP_TAMPIOCTL[2431]) and DYNPR0EN (TAMPRTC_TAMPIOCTL[15]) are set.
 * |[3]     |DYNSRC    |Dynamic Reference Pattern
 * |        |          |This fields determines the new reference pattern when current pattern run out in dynamic pair mode.
 * |        |          |0 = The new reference pattern is generated by random number generator when the reference pattern run out.
 * |        |          |1 = The new reference pattern is repeated from SEED (RTCTAMP_TAMPSEED[31:0]) when the reference pattern run out.
 * |        |          |Note: After this bit is modified, the SEEDRLD (RTCTAMP_TAMIOPCTL[4]) should be set.
 * |[4]     |SEEDRLD   |Reload New Seed for PRNG Engine
 * |        |          |Setting this bit, the tamper configuration will be reload.
 * |        |          |0 = Generating key based on the current seed.
 * |        |          |1 = Reload new seed.
 * |        |          |Note 1: Before this bit is set, the tamper configuration should be set to complete and this bit will be auto clear to 0 after reload new seed completed.
 * |        |          |Note 2: The reference is RTC-clock . Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[7:5]   |DYNRATE   |Dynamic Change Rate
 * |        |          |This item is choice the dynamic tamper output change rate.
 * |        |          |000 = 2160 * RTC_CLK.
 * |        |          |001 = 2171 * RTC_CLK.
 * |        |          |010 = 2182 * RTC_CLK.
 * |        |          |011 = 2193 * RTC_CLK.
 * |        |          |100 = 2104 * RTC_CLK.
 * |        |          |101 = 2115 * RTC_CLK.
 * |        |          |110 = 2126 * RTC_CLK.
 * |        |          |111 = 2137 * RTC_CLK.
 * |        |          |Note: After revising this field, setting SEEDRLD (RTCTAMP_TAMPCTLTIOCTL[4]) can reload change rate immediately.
 * |[8]     |TAMP0EN   |Tamper0 Detect Enable Bit
 * |        |          |0 = Tamper 0 detect Disabled.
 * |        |          |1 = Tamper 0 detect Enabled.
 * |        |          |Note: The reference is RTC-clock . Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[9]     |TAMP0LV   |Tamper 0 Level
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[10]    |TAMP0DBEN |Tamper 0 De-bounce Enable Bit
 * |        |          |0 = Tamper 0 de-bounce Disabled.
 * |        |          |1 = Tamper 0 de-bounce Enabled., Ttamper detection pin will sync 1 RTC clock.
 * |[12]    |TAMP1EN   |Tamper 1 Detect Enable Bit
 * |        |          |0 = Tamper 1 detect Disabled.
 * |        |          |1 = Tamper 1 detect Enabled.
 * |        |          |Note: The reference is RTC-clock . Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[13]    |TAMP1LV   |Tamper 1 Level
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[14]    |TAMP1DBEN |Tamper 1 De-bounce Enable Bit
 * |        |          |0 = Tamper 1 de-bounce Disabled.
 * |        |          |1 = Tamper 1 de-bounce Enabled, tamper detection pin will sync 1 RTC clock.
 * |[15]    |DYNPR0EN  |Dynamic Pair 0 Enable Bit
 * |        |          |0 = Static detect.
 * |        |          |1 = Dynamic detect.
 * |[16]    |TAMP2EN   |Tamper 2 Detect Enable Bit
 * |        |          |0 = Tamper 2 detect Disabled.
 * |        |          |1 = Tamper 2 detect Enabled.
 * |        |          |Note: The reference is RTC-clock . Tamper detector need sync 2 ~ 3 RTC-clock.
 * |[17]    |TAMP2LV   |Tamper 2 Level
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[18]    |TAMP2DBEN |Tamper 2 De-bounce Enable Bit
 * |        |          |0 = Tamper 2 de-bounce Disabled.
 * |        |          |1 = Tamper 2 de-bounce Enabled., Ttamper detection pin will sync 1 RTC clock.
 * |[20]    |TAMP3EN   |Tamper 3 Detect Enable Bit
 * |        |          |0 = Tamper 3 detect Disabled.
 * |        |          |1 = Tamper 3 detect Enabled.
 * |        |          |Note: The reference is RTC-clock . Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[21]    |TAMP3LV   |Tamper 3 Level
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[22]    |TAMP3DBEN |Tamper 3 De-bounce Enable Bit
 * |        |          |0 = Tamper 3 de-bounce Disabled.
 * |        |          |1 = Tamper 3 de-bounce Enabled., Ttamper detection pin will sync 1 RTC clock.
 * |[23]    |DYNPR1EN  |Dynamic Pair 1 Enable Bit
 * |        |          |0 = Static detect.
 * |        |          |1 = Dynamic detect.
 * |[24]    |TAMP4EN   |Tamper4 Detect Enable Bit
 * |        |          |0 = Tamper 4 detect Disabled.
 * |        |          |1 = Tamper 4 detect Enabled.
 * |        |          |Note: The reference is RTC-clock . Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[25]    |TAMP4LV   |Tamper 4 Level
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[26]    |TAMP4DBEN |Tamper 4 De-bounce Enable Bit
 * |        |          |0 = Tamper 4 de-bounce Disabled.
 * |        |          |1 = Tamper 4 de-bounce Enabled., Ttamper detection pin will sync 1 RTC clock.
 * |[28]    |TAMP5EN   |Tamper 5 Detect Enable Bit
 * |        |          |0 = Tamper 5 detect Disabled.
 * |        |          |1 = Tamper 5 detect Enabled.
 * |        |          |Note: The reference is RTC-clock . Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[29]    |TAMP5LV   |Tamper 5 Level
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[30]    |TAMP5DBEN |Tamper 5 De-bounce Enable Bit
 * |        |          |0 = Tamper 5 de-bounce Disabled.
 * |        |          |1 = Tamper 5 de-bounce Enabled., Ttamper detection pin will sync 1 RTC clock.
 * |[31]    |DYNPR2EN  |Dynamic Pair 2 Enable Bit
 * |        |          |0 = Static detect.
 * |        |          |1 = Dynamic detect.
 * @var TAMP_T::SEED
 * Offset: 0x1C  TAMPER Seed Value Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |SEED      |Seed vValue. 
 * @var TAMP_T::SEED2
 * Offset: 0x20  TAMPER 2nd Seed Value Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[31:0]  |SEED2     |Seed value These seed value are used for 2nd active shield I/O.
 * @var TAMP_T::ACTSTIOCTL1
 * Offset: 0x24  TAMPERu2019s Active Shield Tamper I/O Function Control Register 1
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |ADYN1ISS  |Active Shied Dynamic Pair 1 Input Source Select
 * |        |          |This bit determines Tamper 3 input is from Tamper 2 or Tamper 0 in dynamic mode.
 * |        |          |0 = Tamper input is from Tamper 2.
 * |        |          |1 = Tamper input is from Tamper 0.
 * |        |          |Note: This bit is effective only when ADYNPR1EN (TAMP_ACTSTIOCTL1[23]) and ADYNPR0EN (TAMP_ACTSTIOCTL1[15]) are set.
 * |[3]     |ADYNSRC   |Active Shied Dynamic Reference Pattern
 * |        |          |This fields determines the new reference pattern when current pattern run out in dynamic pair mode.
 * |        |          |0 = The new reference pattern is generated by random number generator when the reference pattern run out.
 * |        |          |1 = The new reference pattern is repeated from SEED (TAMP_SEED[31:0]) when the reference pattern run out.
 * |        |          |Note: After this bit is modified, the SEEDRLD (TAMP_TIOCTL[4]) should be set.
 * |[7:5]   |ADYNRATE  |Active Shied Dynamic Change Rate
 * |        |          |This item is choice the dynamic tamper output change rate.
 * |        |          |000 = 210 * LIRC32K.
 * |        |          |001 = 211 * LIRC32K.
 * |        |          |010 = 212 * LIRC32K.
 * |        |          |011 = 213 * LIRC32K.
 * |        |          |100 = 214 * LIRC32K.
 * |        |          |101 = 215 * LIRC32K.
 * |        |          |110 = 216 * LIRC32K.
 * |        |          |111 = 217 * LIRC32K.
 * |        |          |Note: After revising this field, setting SEEDRLD (TAMP_TIOCTL[4]) can reload change rate immediately.
 * |[8]     |ATAMP0EN  |Active Shied Tamper0 Detect Enable Bit
 * |        |          |0 = Tamper 0 detect Disabled.
 * |        |          |1 = Tamper 0 detect Enabled.
 * |        |          |Note: The reference is RTC-clock. Tamper detector need sync 2 ~ 3 RTC-clock.
 * |[9]     |ATAMP0LV  |Active Shied Tamper 0 Level 1.1.1.1 This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[10]    |ATAMP0DBEN|Active Shied Tamper 0 De-bounce Enable Bit
 * |        |          |0 = Tamper 0 de-bounce Disabled.
 * |        |          |1 = Tamper 0 de-bounce Enabled, tamper detection pin will sync 1 RTC clock.
 * |[12]    |ATAMP1EN  |Active Shied Tamper 1 Detect Enable Bit
 * |        |          |0 = Tamper 1 detect Disabled.
 * |        |          |1 = Tamper 1 detect Enabled.
 * |        |          |Note: The reference is RTC-clock. Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[13]    |ATAMP1LV  |Active Shied Tamper 1 Level
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[14]    |ATAMP1DBEN|Active Shied Tamper 1 De-bounce Enable Bit
 * |        |          |0 = Tamper 1 de-bounce Disabled.
 * |        |          |1 = Tamper 1 de-bounce Enabled, tamper detection pin will sync 1 RTC clock.
 * |[15]    |ADYNPR0EN |Active Shied Dynamic Pair 0 Enable Bit
 * |        |          |0 = Static detect (No support).
 * |        |          |1 = Dynamic detect.
 * |[16]    |ATAMP2EN  |Active Shied Tamper 2 Detect Enable Bit
 * |        |          |0 = Tamper 2 detect Disabled.
 * |        |          |1 = Tamper 2 detect Enabled.
 * |        |          |Note: The reference is RTC-clock. Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[17]    |ATAMP2LV  |Active Shied Tamper 2 Level
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[18]    |ATAMP2DBEN|Active Shied Tamper 2 De-bounce Enable Bit
 * |        |          |0 = Tamper 2 de-bounce Disabled.
 * |        |          |1 = Tamper 2 de-bounce Enabled, tamper detection pin will sync 1 RTC clock.
 * |[20]    |ATAMP3EN  |Active Shied Tamper 3 Detect Enable Bit
 * |        |          |0 = Tamper 3 detect Disabled.
 * |        |          |1 = Tamper 3 detect Enabled.
 * |        |          |Note: The reference is RTC-clock. Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[21]    |ATAMP3LV  |Active Shied Tamper 3 Level
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[22]    |ATAMP3DBEN|Active Shied Tamper 3 De-bounce Enable Bit
 * |        |          |0 = Tamper 3 de-bounce Disabled.
 * |        |          |1 = Tamper 3 de-bounce Enabled, tamper detection pin will sync 1 RTC clock.
 * |[23]    |ADYNPR1EN |Active Shied Dynamic Pair 1 Enable Bit
 * |        |          |0 = Static detect (Not Supported).
 * |        |          |1 = Dynamic detect.
 * @var TAMP_T::ACTSTIOCTL2
 * Offset: 0x28  TAMPERu2019s Active Shield Tamper I/O Function Control Register 2
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[0]     |ADYN1ISS2 |Active Shied Dynamic Pair 1 Input Source Select 2
 * |        |          |This bit determines Tamper 3 input is from Tamper 2 or Tamper 0 in dynamic mode.
 * |        |          |0 = Tamper input is from Tamper 2.
 * |        |          |1 = Tamper input is from Tamper 0.
 * |        |          |Note: This bit is effective only when ADYNPR1EN2 (TAMP_ACTSTIOCTL2[23]) and ADYNPR0EN2 (TAMP_ACTSTIOCTL2[15]) are set.
 * |[3]     |ADYNSRC2  |Active Shied Dynamic Reference Pattern 2
 * |        |          |This fields determines the new reference pattern when current pattern run out in dynamic pair mode.
 * |        |          |0 = The new reference pattern is generated by random number generator when the reference pattern run out.
 * |        |          |1 = The new reference pattern is repeated from SEED2 (TAMP_SEED2[31:0]) when the reference pattern run out.
 * |        |          |Note: After this bit is modified, the ASEEDRLD2 (TAMP_ACTSTIOCTL2[4]) should be set.
 * |[4]     |SEEDRLD2  |Reload New Seed for PRNG Engine 2
 * |        |          |Setting this bit, the tamper configuration will be reload.
 * |        |          |0 = Generating key based on the current seed.
 * |        |          |1 = Reload new seed.
 * |        |          |Note 1: Before this bit is set, the tamper configuration should be set to complete and this bit will be auto clear to 0 after reload new seed completed.
 * |        |          |Note 2: The reference is RTC-clock. Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[7:5]   |ADYNRATE2 |Active Shied Dynamic Change Rate 2
 * |        |          |This item is choice the dynamic tamper output change rate.
 * |        |          |000 = 210 * LIRC32K.
 * |        |          |001 = 211 * LIRC32K.
 * |        |          |010 = 212 * LIRC32K.
 * |        |          |011 = 213 * LIRC32K.
 * |        |          |100 = 214 * LIRC32K.
 * |        |          |101 = 215 * LIRC32K.
 * |        |          |110 = 216 * LIRC32K.
 * |        |          |111 = 217 * LIRC32K.
 * |        |          |Note: After revising this field, setting SEEDRLD2 (TAMP_ACTSTIOCTL2[4]) can reload change rate immediately.
 * |[8]     |ATAMP0EN2 |Active Shied Tamper0 Detect Enable Bit 2
 * |        |          |0 = Tamper 0 detect Disabled.
 * |        |          |1 = Tamper 0 detect Enabled.
 * |        |          |Note: The reference is RTC-clock. Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[9]     |ATAMP0LV2 |Active Shied Tamper 0 Level 2
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[10]    |ATAMP0DBEN2|Active Shied Tamper 0 De-bounce Enable Bit 2
 * |        |          |0 = Tamper 0 de-bounce Disabled.
 * |        |          |1 = Tamper 0 de-bounce Enabled, tamper detection pin will sync 1 RTC clock.
 * |[12]    |ATAMP1EN2 |Active Shied Tamper 1 Detect Enable Bit 2
 * |        |          |0 = Tamper 1 detect Disabled.
 * |        |          |1 = Tamper 1 detect Enabled.
 * |        |          |Note: The reference is RTC-clock. Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[13]    |ATAMP1LV2 |Active Shied Tamper 1 Level 2
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[14]    |ATAMP1DBEN2|Active Shied Tamper 1 De-bounce Enable Bit 2
 * |        |          |0 = Tamper 1 de-bounce Disabled.
 * |        |          |1 = Tamper 1 de-bounce Enabled, tamper detection pin will sync 1 RTC clock.
 * |[15]    |ADYNPR0EN2|Active Shied Dynamic Pair 0 Enable Bit 2
 * |        |          |0 = Static detect (No support).
 * |        |          |1 = Dynamic detect.
 * |[16]    |ATAMP2EN2 |Active Shied Tamper 2 Detect Enable Bit 2
 * |        |          |0 = Tamper 2 detect Disabled.
 * |        |          |1 = Tamper 2 detect Enabled.
 * |        |          |Note: The reference is RTC-clock. Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[17]    |ATAMP2LV2 |Active Shied Tamper 2 Level 2
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[18]    |ATAMP2DBEN2|Active Shied Tamper 2 De-bounce Enable Bit 2
 * |        |          |0 = Tamper 2 de-bounce Disabled.
 * |        |          |1 = Tamper 2 de-bounce Enabled, tamper detection pin will sync 1 RTC clock.
 * |[20]    |ATAMP3EN2 |Active Shied Tamper 3 Detect Enable Bit 2
 * |        |          |0 = Tamper 3 detect Disabled.
 * |        |          |1 = Tamper 3 detect Enabled.
 * |        |          |Note: The reference is RTC-clock. Tamper detector needs sync 2 ~ 3 RTC-clock.
 * |[21]    |ATAMP3LV2 |Active Shied Tamper 3 Level 2
 * |        |          |This bit depends on level attribute of tamper pin for static tamper detection.
 * |        |          |0 = Detect voltage level is low.
 * |        |          |1 = Detect voltage level is high.
 * |[22]    |ATAMP3DBEN2|Active Shied Tamper 3 De-bounce Enable Bit 2
 * |        |          |0 = Tamper 3 de-bounce Disabled.
 * |        |          |1 = Tamper 3 de-bounce Enabled, tamper detection pin will sync 1 RTC clock.
 * |[23]    |ADYNPR1EN2|Active Shied Dynamic Pair 1 Enable Bit 2
 * |        |          |0 = Static detect (No Support).
 * |        |          |1 = Dynamic detect.
 * @var TAMP_T::CDBR
 * Offset: 0x2C  TAMPER Clock Frequency Detector Boundary Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |STOPBD    |LXT Clock Frequency Detector Stop Boundary
 * |        |          |The bits define the stop value of frequency monitor window.
 * |        |          |When LXT frequency monitor counter lower than Clock Frequency Detector Stop Boundary, the LXT frequency detect stop interrupt flag will set to 1.
 * |        |          |Note: The boundary is defined as the maximun value of LXT among 256 TAMPER clock time.
 * |[23:16] |FAILBD    |LXT Clock Frequency Detector Fail Boundary
 * |        |          |The bits define the fail value of frequency monitor window.
 * |        |          |When LXT frequency monitor counter lower than Clock Frequency Detector Fail Boundary, the LXT frequency detect fail interrupt flag will set to 1.
 * |        |          |Note: The boundary is defined as the minimun value of LXT among 256 TAMPER clock time.
 * @var TAMP_T::PWRG
 * Offset: 0x30  TAMPER Power Glitch Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[3:0]   |PCLKSEL   |Postive Clock Trim Range
 * |        |          |The setting value of the postive clock tolerance.
 * |        |          |One step is about 2.5% tolerance. The maximun tolerance is 20%.
 * |[7:4]   |NCLKSEL   |Negative Clock Trim Range
 * |        |          |The setting value of the negative clock tolerance.
 * |        |          |One step is about 2.5% tolerance. The maximun tolerance is 20%.
 * |[11:8]  |PDATSEL   |Postive Data Trim Range
 * |        |          |The setting value of the postive data tolerance.
 * |        |          |One step is about 2.5% tolerance. The maximun tolerance is 20%.
 * |[15:12] |NDATSEL   |Negative Data Trim Range
 * |        |          |The setting value of the negative data tolerance.
 * |        |          |One step is about 2.5% tolerance. The maximun tolerance is 20%.
 * @var TAMP_T::PWRGEV
 * Offset: 0x34  TAMPER Power Glitch Event Tolerance Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[7:0]   |PWRECNTP  |Postive Power Glitch Error Tolerance
 * |        |          |The value indicates the tolerance count for postive power glitch event.
 * |[15:8]  |PWRECNTN  |Negative Power Glitch Error Tolerance
 * |        |          |The value indicates the tolerance count for negative power glitch event.
 * @var TAMP_T::LDOTRIM
 * Offset: 0x38  TAMPER LDO Trim Value Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[3:0]   |TLDOTRIM  |TLDO Output Votlage TTrim
 * |        |          |The value indicates the trim value of the TLDO output voltage.
 * |[9:8]   |TLDOIQSEL |TLDO Qu CCurrent Selection
 * |        |          |Indicates the Qu current selection of TLDO.
 * |[31]    |TLDOTM    |TLDO Test Mode Enable Bit
 * |        |          |0 = TLDO volatage output Disabled.
 * |        |          |1 = TLDO votlage output test only in GPB14.
 * @var TAMP_T::LBSTRIM
 * Offset: 0x3C  TAMPER LDO BIAS Trim Value Control Register
 * ---------------------------------------------------------------------------------------------------
 * |Bits    |Field     |Descriptions
 * | :----: | :----:   | :---- |
 * |[2:0]   |TLVDSEL   |Under-shoot Detect Level Trim Bits
 * |        |          |The value indicates the trim value of the under-shoot detection level
 * |[4]     |TOVDSEL   |Over-shoot Detect Level Trim Bits
 * |        |          |The value indicates the trim value of the over-shoot detection level
 * |[9:8]   |BSCMPLV   |Under-shoot Detect Comparator Current Trim Bits
 * |        |          |The value indicates the trim value of the under-shoot detection comparator current trim level
 * |[11:10] |BSCMPOV   |Over-shoot Detect Comparator Current Trim Bits
 * |        |          |The value indicates the trim value of the over-shoot detection comparator current trim level
 * |[13:12] |HYSCMPLV  |Under-shoot Detect Comparator Hysteresis Trim Bits
 * |        |          |The value indicates the trim value of the under-shoot detection comparator of hystersis trim level
 * |[15:14] |HYSCMPOV  |Over-shoot Detect Comparator Hysteresis Trim Bits
 * |        |          |The value indicates the trim value of the over-shoot detection comparator of hystersis trim level
 * |[16]    |TLVDEN    |Enable Over-shoot and Under-shoot Detector
 * |        |          |0 = The over-shoot and under-shoot detector is Disabled.
 * |        |          |1 = The over-shoot and under-shoot detector is Enabled.
 * |[27:20] |HXTFTRIM  |HXT Filter Trim Value
 * |        |          |Test Chip Only.
 * |        |          |Note: They are used to set the trim value of the HXT
 * |        |          |In real chip, the control bits will be located in GMISC.
 * |[30]    |TOVDOUT   |VDD_SB over Shot Detect Output (Read Only)
 * |        |          |0 = VDD_SB not over-shoot.
 * |        |          |1 = VDD_SB over-shoot.
 * |[31]    |TLVDOUT   |VDD_SB Under Shot Detect Output (Read Only)
 * |        |          |0 = VDD_SB not under-shoot.
 * |        |          |1 = VDD_SB under-shoot.
 */
    __IO uint32_t INIT;                  /*!< [0x0000] TAMPER Function Initiation Register                              */
    __IO uint32_t FUNEN;                 /*!< [0x0004] TAMPER Block Function Enable Register                            */
    __IO uint32_t TRIEN;                 /*!< [0x0008] TAMPER Trigger Enable Register                                   */
    __IO uint32_t INTEVEN;               /*!< [0x000c] TAMPER Interrupt Event Enable Register                           */
    __IO uint32_t EVSTS;                 /*!< [0x0010] TAMPER Event Status Register                                     */
    __IO uint32_t LIRCTL;                /*!< [0x0014] TAMPER LIRC Control Register                                     */
    __IO uint32_t TIOCTL;                /*!< [0x0018] TAMPER's Tamper I/O Function Control Register                    */
    __IO uint32_t SEED;                  /*!< [0x001c] TAMPER Seed Value Control Register                               */
    __IO uint32_t SEED2;                 /*!< [0x0020] TAMPER 2nd Seed Value Control Register                           */
    __IO uint32_t ACTSTIOCTL1;           /*!< [0x0024] TAMPER's Active Shield Tamper I/O Function Control Register 1    */
    __IO uint32_t ACTSTIOCTL2;           /*!< [0x0028] TAMPER's Active Shield Tamper I/O Function Control Register 2    */
    __IO uint32_t CDBR;                  /*!< [0x002c] TAMPER Clock Frequency Detector Boundary Register                */
    __IO uint32_t PWRG;                  /*!< [0x0030] TAMPER Power Glitch Control Register                             */
    __IO uint32_t PWRGEV;                /*!< [0x0034] TAMPER Power Glitch Event Tolerance Control Register             */
    __IO uint32_t LDOTRIM;               /*!< [0x0038] TAMPER LDO Trim Value Control Register                           */
    __IO uint32_t LBSTRIM;               /*!< [0x003c] TAMPER LDO BIAS Trim Value Control Register                      */

} TAMP_T;

/**
    @addtogroup TAMPER_CONST TAMPER Bit Field Definition
    Constant Definitions for TAMPER Controller
@{ */

#define TAMP_INIT_TCORERST_Pos         (0)                                               /*!< TAMP_T::INIT: TCORERST Position      */
#define TAMP_INIT_TCORERST_Msk         (0x1ul << TAMP_INIT_TCORERST_Pos)                 /*!< TAMP_T::INIT: TCORERST Mask          */

#define TAMP_INIT_TLDORDY_Pos          (31)                                              /*!< TAMP_T::INIT: TLDORDY Position       */
#define TAMP_INIT_TLDORDY_Msk          (0x1ul << TAMP_INIT_TLDORDY_Pos)                  /*!< TAMP_T::INIT: TLDORDY Mask           */

#define TAMP_FUNEN_LXTDETEN_Pos        (0)                                               /*!< TAMP_T::FUNEN: LXTDETEN Position     */
#define TAMP_FUNEN_LXTDETEN_Msk        (0x1ul << TAMP_FUNEN_LXTDETEN_Pos)                /*!< TAMP_T::FUNEN: LXTDETEN Mask         */

#define TAMP_FUNEN_HXTDETEN_Pos        (1)                                               /*!< TAMP_T::FUNEN: HXTDETEN Position     */
#define TAMP_FUNEN_HXTDETEN_Msk        (0x1ul << TAMP_FUNEN_HXTDETEN_Pos)                /*!< TAMP_T::FUNEN: HXTDETEN Mask         */

#define TAMP_FUNEN_TMPIOSEL_Pos        (8)                                               /*!< TAMP_T::FUNEN: TMPIOSEL Position     */
#define TAMP_FUNEN_TMPIOSEL_Msk        (0x3ful << TAMP_FUNEN_TMPIOSEL_Pos)               /*!< TAMP_T::FUNEN: TMPIOSEL Mask         */

#define TAMP_FUNEN_HIRC12MEN_Pos       (16)                                              /*!< TAMP_T::FUNEN: HIRC12MEN Position    */
#define TAMP_FUNEN_HIRC12MEN_Msk       (0xfful << TAMP_FUNEN_HIRC12MEN_Pos)              /*!< TAMP_T::FUNEN: HIRC12MEN Mask        */

#define TAMP_TRIEN_FMCTRIGEN_Pos       (0)                                               /*!< TAMP_T::TRIEN: FMCTRIGEN Position    */
#define TAMP_TRIEN_FMCTRIGEN_Msk       (0x1ul << TAMP_TRIEN_FMCTRIGEN_Pos)               /*!< TAMP_T::TRIEN: FMCTRIGEN Mask        */

#define TAMP_TRIEN_KSTRIGEN_Pos        (1)                                               /*!< TAMP_T::TRIEN: KSTRIGEN Position     */
#define TAMP_TRIEN_KSTRIGEN_Msk        (0x1ul << TAMP_TRIEN_KSTRIGEN_Pos)                /*!< TAMP_T::TRIEN: KSTRIGEN Mask         */

#define TAMP_TRIEN_WakeupEN_Pos        (2)                                               /*!< TAMP_T::TRIEN: WakeupEN Position     */
#define TAMP_TRIEN_WakeupEN_Msk        (0x1ul << TAMP_TRIEN_WakeupEN_Pos)                /*!< TAMP_T::TRIEN: WakeupEN Mask         */

#define TAMP_INTEVEN_TAMP0EN_Pos       (0)                                               /*!< TAMP_T::INTEVEN: TAMP0EN Position    */
#define TAMP_INTEVEN_TAMP0EN_Msk       (0x1ul << TAMP_INTEVEN_TAMP0EN_Pos)               /*!< TAMP_T::INTEVEN: TAMP0EN Mask        */

#define TAMP_INTEVEN_TAMP1EN_Pos       (1)                                               /*!< TAMP_T::INTEVEN: TAMP1EN Position    */
#define TAMP_INTEVEN_TAMP1EN_Msk       (0x1ul << TAMP_INTEVEN_TAMP1EN_Pos)               /*!< TAMP_T::INTEVEN: TAMP1EN Mask        */

#define TAMP_INTEVEN_TAMP2EN_Pos       (2)                                               /*!< TAMP_T::INTEVEN: TAMP2EN Position    */
#define TAMP_INTEVEN_TAMP2EN_Msk       (0x1ul << TAMP_INTEVEN_TAMP2EN_Pos)               /*!< TAMP_T::INTEVEN: TAMP2EN Mask        */

#define TAMP_INTEVEN_TAMP3EN_Pos       (3)                                               /*!< TAMP_T::INTEVEN: TAMP3EN Position    */
#define TAMP_INTEVEN_TAMP3EN_Msk       (0x1ul << TAMP_INTEVEN_TAMP3EN_Pos)               /*!< TAMP_T::INTEVEN: TAMP3EN Mask        */

#define TAMP_INTEVEN_TAMP4EN_Pos       (4)                                               /*!< TAMP_T::INTEVEN: TAMP4EN Position    */
#define TAMP_INTEVEN_TAMP4EN_Msk       (0x1ul << TAMP_INTEVEN_TAMP4EN_Pos)               /*!< TAMP_T::INTEVEN: TAMP4EN Mask        */

#define TAMP_INTEVEN_TAMP5EN_Pos       (5)                                               /*!< TAMP_T::INTEVEN: TAMP5EN Position    */
#define TAMP_INTEVEN_TAMP5EN_Msk       (0x1ul << TAMP_INTEVEN_TAMP5EN_Pos)               /*!< TAMP_T::INTEVEN: TAMP5EN Mask        */

#define TAMP_INTEVEN_CLKFEN_Pos        (6)                                               /*!< TAMP_T::INTEVEN: CLKFEN Position     */
#define TAMP_INTEVEN_CLKFEN_Msk        (0x1ul << TAMP_INTEVEN_CLKFEN_Pos)                /*!< TAMP_T::INTEVEN: CLKFEN Mask         */

#define TAMP_INTEVEN_CLKSTOPEN_Pos     (7)                                               /*!< TAMP_T::INTEVEN: CLKSTOPEN Position  */
#define TAMP_INTEVEN_CLKSTOPEN_Msk     (0x1ul << TAMP_INTEVEN_CLKSTOPEN_Pos)             /*!< TAMP_T::INTEVEN: CLKSTOPEN Mask      */

#define TAMP_INTEVEN_OVPEN_Pos         (8)                                               /*!< TAMP_T::INTEVEN: OVPEN Position      */
#define TAMP_INTEVEN_OVPEN_Msk         (0x1ul << TAMP_INTEVEN_OVPEN_Pos)                 /*!< TAMP_T::INTEVEN: OVPEN Mask          */

#define TAMP_INTEVEN_PWRPEN_Pos        (9)                                               /*!< TAMP_T::INTEVEN: PWRPEN Position     */
#define TAMP_INTEVEN_PWRPEN_Msk        (0x1ul << TAMP_INTEVEN_PWRPEN_Pos)                /*!< TAMP_T::INTEVEN: PWRPEN Mask         */

#define TAMP_INTEVEN_PWRNEN_Pos        (10)                                              /*!< TAMP_T::INTEVEN: PWRNEN Position     */
#define TAMP_INTEVEN_PWRNEN_Msk        (0x1ul << TAMP_INTEVEN_PWRNEN_Pos)                /*!< TAMP_T::INTEVEN: PWRNEN Mask         */

#define TAMP_INTEVEN_ACTSEN_Pos        (11)                                              /*!< TAMP_T::INTEVEN: ACTSEN Position     */
#define TAMP_INTEVEN_ACTSEN_Msk        (0x1ul << TAMP_INTEVEN_ACTSEN_Pos)                /*!< TAMP_T::INTEVEN: ACTSEN Mask         */

#define TAMP_INTEVEN_RTCLVREN_Pos      (16)                                              /*!< TAMP_T::INTEVEN: RTCLVREN Position   */
#define TAMP_INTEVEN_RTCLVREN_Msk      (0x1ul << TAMP_INTEVEN_RTCLVREN_Pos)              /*!< TAMP_T::INTEVEN: RTCLVREN Mask       */

#define TAMP_INTEVEN_RTCIOEN_Pos       (17)                                              /*!< TAMP_T::INTEVEN: RTCIOEN Position    */
#define TAMP_INTEVEN_RTCIOEN_Msk       (0x1ul << TAMP_INTEVEN_RTCIOEN_Pos)               /*!< TAMP_T::INTEVEN: RTCIOEN Mask        */

#define TAMP_INTEVEN_RTCLKEN_Pos       (18)                                              /*!< TAMP_T::INTEVEN: RTCLKEN Position    */
#define TAMP_INTEVEN_RTCLKEN_Msk       (0x1ul << TAMP_INTEVEN_RTCLKEN_Pos)               /*!< TAMP_T::INTEVEN: RTCLKEN Mask        */

#define TAMP_INTEVEN_HXTERREN_Pos      (19)                                              /*!< TAMP_T::INTEVEN: HXTERREN Position   */
#define TAMP_INTEVEN_HXTERREN_Msk      (0x1ul << TAMP_INTEVEN_HXTERREN_Pos)              /*!< TAMP_T::INTEVEN: HXTERREN Mask       */

#define TAMP_INTEVEN_VLOSSEN_Pos       (20)                                              /*!< TAMP_T::INTEVEN: VLOSSEN Position    */
#define TAMP_INTEVEN_VLOSSEN_Msk       (0x1ul << TAMP_INTEVEN_VLOSSEN_Pos)               /*!< TAMP_T::INTEVEN: VLOSSEN Mask        */

#define TAMP_INTEVEN_WDTEN_Pos         (21)                                              /*!< TAMP_T::INTEVEN: WDTEN Position      */
#define TAMP_INTEVEN_WDTEN_Msk         (0x1ul << TAMP_INTEVEN_WDTEN_Pos)                 /*!< TAMP_T::INTEVEN: WDTEN Mask          */

#define TAMP_INTEVEN_BODEN_Pos         (22)                                              /*!< TAMP_T::INTEVEN: BODEN Position      */
#define TAMP_INTEVEN_BODEN_Msk         (0x1ul << TAMP_INTEVEN_BODEN_Pos)                 /*!< TAMP_T::INTEVEN: BODEN Mask          */

#define TAMP_EVSTS_TAMP0IF_Pos         (0)                                               /*!< TAMP_T::EVSTS: TAMP0IF Position      */
#define TAMP_EVSTS_TAMP0IF_Msk         (0x1ul << TAMP_EVSTS_TAMP0IF_Pos)                 /*!< TAMP_T::EVSTS: TAMP0IF Mask          */

#define TAMP_EVSTS_TAMP1IF_Pos         (1)                                               /*!< TAMP_T::EVSTS: TAMP1IF Position      */
#define TAMP_EVSTS_TAMP1IF_Msk         (0x1ul << TAMP_EVSTS_TAMP1IF_Pos)                 /*!< TAMP_T::EVSTS: TAMP1IF Mask          */

#define TAMP_EVSTS_TAMP2IF_Pos         (2)                                               /*!< TAMP_T::EVSTS: TAMP2IF Position      */
#define TAMP_EVSTS_TAMP2IF_Msk         (0x1ul << TAMP_EVSTS_TAMP2IF_Pos)                 /*!< TAMP_T::EVSTS: TAMP2IF Mask          */

#define TAMP_EVSTS_TAMP3IF_Pos         (3)                                               /*!< TAMP_T::EVSTS: TAMP3IF Position      */
#define TAMP_EVSTS_TAMP3IF_Msk         (0x1ul << TAMP_EVSTS_TAMP3IF_Pos)                 /*!< TAMP_T::EVSTS: TAMP3IF Mask          */

#define TAMP_EVSTS_TAMP4IF_Pos         (4)                                               /*!< TAMP_T::EVSTS: TAMP4IF Position      */
#define TAMP_EVSTS_TAMP4IF_Msk         (0x1ul << TAMP_EVSTS_TAMP4IF_Pos)                 /*!< TAMP_T::EVSTS: TAMP4IF Mask          */

#define TAMP_EVSTS_TAMP5IF_Pos         (5)                                               /*!< TAMP_T::EVSTS: TAMP5IF Position      */
#define TAMP_EVSTS_TAMP5IF_Msk         (0x1ul << TAMP_EVSTS_TAMP5IF_Pos)                 /*!< TAMP_T::EVSTS: TAMP5IF Mask          */

#define TAMP_EVSTS_CLKFAIL_Pos         (6)                                               /*!< TAMP_T::EVSTS: CLKFAIL Position      */
#define TAMP_EVSTS_CLKFAIL_Msk         (0x1ul << TAMP_EVSTS_CLKFAIL_Pos)                 /*!< TAMP_T::EVSTS: CLKFAIL Mask          */

#define TAMP_EVSTS_CLKSTOP_Pos         (7)                                               /*!< TAMP_T::EVSTS: CLKSTOP Position      */
#define TAMP_EVSTS_CLKSTOP_Msk         (0x1ul << TAMP_EVSTS_CLKSTOP_Pos)                 /*!< TAMP_T::EVSTS: CLKSTOP Mask          */

#define TAMP_EVSTS_OVPOUT_Pos          (8)                                               /*!< TAMP_T::EVSTS: OVPOUT Position       */
#define TAMP_EVSTS_OVPOUT_Msk          (0x1ul << TAMP_EVSTS_OVPOUT_Pos)                  /*!< TAMP_T::EVSTS: OVPOUT Mask           */

#define TAMP_EVSTS_PWRPEV_Pos          (9)                                               /*!< TAMP_T::EVSTS: PWRPEV Position       */
#define TAMP_EVSTS_PWRPEV_Msk          (0x1ul << TAMP_EVSTS_PWRPEV_Pos)                  /*!< TAMP_T::EVSTS: PWRPEV Mask           */

#define TAMP_EVSTS_PWRNEV_Pos          (10)                                              /*!< TAMP_T::EVSTS: PWRNEV Position       */
#define TAMP_EVSTS_PWRNEV_Msk          (0x1ul << TAMP_EVSTS_PWRNEV_Pos)                  /*!< TAMP_T::EVSTS: PWRNEV Mask           */

#define TAMP_EVSTS_ACTSEF_Pos          (11)                                              /*!< TAMP_T::EVSTS: ACTSEF Position       */
#define TAMP_EVSTS_ACTSEF_Msk          (0x1ul << TAMP_EVSTS_ACTSEF_Pos)                  /*!< TAMP_T::EVSTS: ACTSEF Mask           */

#define TAMP_EVSTS_RTCLVR_Pos          (16)                                              /*!< TAMP_T::EVSTS: RTCLVR Position       */
#define TAMP_EVSTS_RTCLVR_Msk          (0x1ul << TAMP_EVSTS_RTCLVR_Pos)                  /*!< TAMP_T::EVSTS: RTCLVR Mask           */

#define TAMP_EVSTS_RIOTRIG_Pos         (17)                                              /*!< TAMP_T::EVSTS: RIOTRIG Position      */
#define TAMP_EVSTS_RIOTRIG_Msk         (0x1ul << TAMP_EVSTS_RIOTRIG_Pos)                 /*!< TAMP_T::EVSTS: RIOTRIG Mask          */

#define TAMP_EVSTS_RCLKTRIG_Pos        (18)                                              /*!< TAMP_T::EVSTS: RCLKTRIG Position     */
#define TAMP_EVSTS_RCLKTRIG_Msk        (0x1ul << TAMP_EVSTS_RCLKTRIG_Pos)                /*!< TAMP_T::EVSTS: RCLKTRIG Mask         */

#define TAMP_EVSTS_HXTERR_Pos          (19)                                              /*!< TAMP_T::EVSTS: HXTERR Position       */
#define TAMP_EVSTS_HXTERR_Msk          (0x1ul << TAMP_EVSTS_HXTERR_Pos)                  /*!< TAMP_T::EVSTS: HXTERR Mask           */

#define TAMP_EVSTS_VBATLOSS_Pos        (20)                                              /*!< TAMP_T::EVSTS: VBATLOSS Position     */
#define TAMP_EVSTS_VBATLOSS_Msk        (0x1ul << TAMP_EVSTS_VBATLOSS_Pos)                /*!< TAMP_T::EVSTS: VBATLOSS Mask         */

#define TAMP_EVSTS_SECWDT_Pos          (21)                                              /*!< TAMP_T::EVSTS: SECWDT Position       */
#define TAMP_EVSTS_SECWDT_Msk          (0x1ul << TAMP_EVSTS_SECWDT_Pos)                  /*!< TAMP_T::EVSTS: SECWDT Mask           */

#define TAMP_EVSTS_TLDOBOD_Pos         (22)                                              /*!< TAMP_T::EVSTS: TLDOBOD Position      */
#define TAMP_EVSTS_TLDOBOD_Msk         (0x1ul << TAMP_EVSTS_TLDOBOD_Pos)                 /*!< TAMP_T::EVSTS: TLDOBOD Mask          */

#define TAMP_EVSTS_ACTST1IF_Pos        (25)                                              /*!< TAMP_T::EVSTS: ACTST1IF Position     */
#define TAMP_EVSTS_ACTST1IF_Msk        (0x1ul << TAMP_EVSTS_ACTST1IF_Pos)                /*!< TAMP_T::EVSTS: ACTST1IF Mask         */

#define TAMP_EVSTS_ACTST3IF_Pos        (27)                                              /*!< TAMP_T::EVSTS: ACTST3IF Position     */
#define TAMP_EVSTS_ACTST3IF_Msk        (0x1ul << TAMP_EVSTS_ACTST3IF_Pos)                /*!< TAMP_T::EVSTS: ACTST3IF Mask         */

#define TAMP_EVSTS_ACTST21IF_Pos       (29)                                              /*!< TAMP_T::EVSTS: ACTST21IF Position    */
#define TAMP_EVSTS_ACTST21IF_Msk       (0x1ul << TAMP_EVSTS_ACTST21IF_Pos)               /*!< TAMP_T::EVSTS: ACTST21IF Mask        */

#define TAMP_EVSTS_ACTST23IF_Pos       (31)                                              /*!< TAMP_T::EVSTS: ACTST23IF Position    */
#define TAMP_EVSTS_ACTST23IF_Msk       (0x1ul << TAMP_EVSTS_ACTST23IF_Pos)               /*!< TAMP_T::EVSTS: ACTST23IF Mask        */

#define TAMP_LIRCTL_TLRCTRIM_Pos       (0)                                               /*!< TAMP_T::LIRCTL: TLRCTRIM Position    */
#define TAMP_LIRCTL_TLRCTRIM_Msk       (0x1fful << TAMP_LIRCTL_TLRCTRIM_Pos)             /*!< TAMP_T::LIRCTL: TLRCTRIM Mask        */

#define TAMP_LIRCTL_TRIMMOS_Pos        (9)                                               /*!< TAMP_T::LIRCTL: TRIMMOS Position     */
#define TAMP_LIRCTL_TRIMMOS_Msk        (0x3ul << TAMP_LIRCTL_TRIMMOS_Pos)                /*!< TAMP_T::LIRCTL: TRIMMOS Mask         */

#define TAMP_LIRCTL_CKSEL12M_Pos       (16)                                              /*!< TAMP_T::LIRCTL: CKSEL12M Position    */
#define TAMP_LIRCTL_CKSEL12M_Msk       (0x1ul << TAMP_LIRCTL_CKSEL12M_Pos)               /*!< TAMP_T::LIRCTL: CKSEL12M Mask        */

#define TAMP_TIOCTL_DYN1ISS_Pos        (0)                                               /*!< TAMP_T::TIOCTL: DYN1ISS Position     */
#define TAMP_TIOCTL_DYN1ISS_Msk        (0x1ul << TAMP_TIOCTL_DYN1ISS_Pos)                /*!< TAMP_T::TIOCTL: DYN1ISS Mask         */

#define TAMP_TIOCTL_DYN2ISS_Pos        (1)                                               /*!< TAMP_T::TIOCTL: DYN2ISS Position     */
#define TAMP_TIOCTL_DYN2ISS_Msk        (0x1ul << TAMP_TIOCTL_DYN2ISS_Pos)                /*!< TAMP_T::TIOCTL: DYN2ISS Mask         */

#define TAMP_TIOCTL_DYNSRC_Pos         (3)                                               /*!< TAMP_T::TIOCTL: DYNSRC Position      */
#define TAMP_TIOCTL_DYNSRC_Msk         (0x1ul << TAMP_TIOCTL_DYNSRC_Pos)                 /*!< TAMP_T::TIOCTL: DYNSRC Mask          */

#define TAMP_TIOCTL_SEEDRLD_Pos        (4)                                               /*!< TAMP_T::TIOCTL: SEEDRLD Position     */
#define TAMP_TIOCTL_SEEDRLD_Msk        (0x1ul << TAMP_TIOCTL_SEEDRLD_Pos)                /*!< TAMP_T::TIOCTL: SEEDRLD Mask         */

#define TAMP_TIOCTL_DYNRATE_Pos        (5)                                               /*!< TAMP_T::TIOCTL: DYNRATE Position     */
#define TAMP_TIOCTL_DYNRATE_Msk        (0x7ul << TAMP_TIOCTL_DYNRATE_Pos)                /*!< TAMP_T::TIOCTL: DYNRATE Mask         */

#define TAMP_TIOCTL_TAMP0EN_Pos        (8)                                               /*!< TAMP_T::TIOCTL: TAMP0EN Position     */
#define TAMP_TIOCTL_TAMP0EN_Msk        (0x1ul << TAMP_TIOCTL_TAMP0EN_Pos)                /*!< TAMP_T::TIOCTL: TAMP0EN Mask         */

#define TAMP_TIOCTL_TAMP0LV_Pos        (9)                                               /*!< TAMP_T::TIOCTL: TAMP0LV Position     */
#define TAMP_TIOCTL_TAMP0LV_Msk        (0x1ul << TAMP_TIOCTL_TAMP0LV_Pos)                /*!< TAMP_T::TIOCTL: TAMP0LV Mask         */

#define TAMP_TIOCTL_TAMP0DBEN_Pos      (10)                                              /*!< TAMP_T::TIOCTL: TAMP0DBEN Position   */
#define TAMP_TIOCTL_TAMP0DBEN_Msk      (0x1ul << TAMP_TIOCTL_TAMP0DBEN_Pos)              /*!< TAMP_T::TIOCTL: TAMP0DBEN Mask       */

#define TAMP_TIOCTL_TAMP1EN_Pos        (12)                                              /*!< TAMP_T::TIOCTL: TAMP1EN Position     */
#define TAMP_TIOCTL_TAMP1EN_Msk        (0x1ul << TAMP_TIOCTL_TAMP1EN_Pos)                /*!< TAMP_T::TIOCTL: TAMP1EN Mask         */

#define TAMP_TIOCTL_TAMP1LV_Pos        (13)                                              /*!< TAMP_T::TIOCTL: TAMP1LV Position     */
#define TAMP_TIOCTL_TAMP1LV_Msk        (0x1ul << TAMP_TIOCTL_TAMP1LV_Pos)                /*!< TAMP_T::TIOCTL: TAMP1LV Mask         */

#define TAMP_TIOCTL_TAMP1DBEN_Pos      (14)                                              /*!< TAMP_T::TIOCTL: TAMP1DBEN Position   */
#define TAMP_TIOCTL_TAMP1DBEN_Msk      (0x1ul << TAMP_TIOCTL_TAMP1DBEN_Pos)              /*!< TAMP_T::TIOCTL: TAMP1DBEN Mask       */

#define TAMP_TIOCTL_DYNPR0EN_Pos       (15)                                              /*!< TAMP_T::TIOCTL: DYNPR0EN Position    */
#define TAMP_TIOCTL_DYNPR0EN_Msk       (0x1ul << TAMP_TIOCTL_DYNPR0EN_Pos)               /*!< TAMP_T::TIOCTL: DYNPR0EN Mask        */

#define TAMP_TIOCTL_TAMP2EN_Pos        (16)                                              /*!< TAMP_T::TIOCTL: TAMP2EN Position     */
#define TAMP_TIOCTL_TAMP2EN_Msk        (0x1ul << TAMP_TIOCTL_TAMP2EN_Pos)                /*!< TAMP_T::TIOCTL: TAMP2EN Mask         */

#define TAMP_TIOCTL_TAMP2LV_Pos        (17)                                              /*!< TAMP_T::TIOCTL: TAMP2LV Position     */
#define TAMP_TIOCTL_TAMP2LV_Msk        (0x1ul << TAMP_TIOCTL_TAMP2LV_Pos)                /*!< TAMP_T::TIOCTL: TAMP2LV Mask         */

#define TAMP_TIOCTL_TAMP2DBEN_Pos      (18)                                              /*!< TAMP_T::TIOCTL: TAMP2DBEN Position   */
#define TAMP_TIOCTL_TAMP2DBEN_Msk      (0x1ul << TAMP_TIOCTL_TAMP2DBEN_Pos)              /*!< TAMP_T::TIOCTL: TAMP2DBEN Mask       */

#define TAMP_TIOCTL_TAMP3EN_Pos        (20)                                              /*!< TAMP_T::TIOCTL: TAMP3EN Position     */
#define TAMP_TIOCTL_TAMP3EN_Msk        (0x1ul << TAMP_TIOCTL_TAMP3EN_Pos)                /*!< TAMP_T::TIOCTL: TAMP3EN Mask         */

#define TAMP_TIOCTL_TAMP3LV_Pos        (21)                                              /*!< TAMP_T::TIOCTL: TAMP3LV Position     */
#define TAMP_TIOCTL_TAMP3LV_Msk        (0x1ul << TAMP_TIOCTL_TAMP3LV_Pos)                /*!< TAMP_T::TIOCTL: TAMP3LV Mask         */

#define TAMP_TIOCTL_TAMP3DBEN_Pos      (22)                                              /*!< TAMP_T::TIOCTL: TAMP3DBEN Position   */
#define TAMP_TIOCTL_TAMP3DBEN_Msk      (0x1ul << TAMP_TIOCTL_TAMP3DBEN_Pos)              /*!< TAMP_T::TIOCTL: TAMP3DBEN Mask       */

#define TAMP_TIOCTL_DYNPR1EN_Pos       (23)                                              /*!< TAMP_T::TIOCTL: DYNPR1EN Position    */
#define TAMP_TIOCTL_DYNPR1EN_Msk       (0x1ul << TAMP_TIOCTL_DYNPR1EN_Pos)               /*!< TAMP_T::TIOCTL: DYNPR1EN Mask        */

#define TAMP_TIOCTL_TAMP4EN_Pos        (24)                                              /*!< TAMP_T::TIOCTL: TAMP4EN Position     */
#define TAMP_TIOCTL_TAMP4EN_Msk        (0x1ul << TAMP_TIOCTL_TAMP4EN_Pos)                /*!< TAMP_T::TIOCTL: TAMP4EN Mask         */

#define TAMP_TIOCTL_TAMP4LV_Pos        (25)                                              /*!< TAMP_T::TIOCTL: TAMP4LV Position     */
#define TAMP_TIOCTL_TAMP4LV_Msk        (0x1ul << TAMP_TIOCTL_TAMP4LV_Pos)                /*!< TAMP_T::TIOCTL: TAMP4LV Mask         */

#define TAMP_TIOCTL_TAMP4DBEN_Pos      (26)                                              /*!< TAMP_T::TIOCTL: TAMP4DBEN Position   */
#define TAMP_TIOCTL_TAMP4DBEN_Msk      (0x1ul << TAMP_TIOCTL_TAMP4DBEN_Pos)              /*!< TAMP_T::TIOCTL: TAMP4DBEN Mask       */

#define TAMP_TIOCTL_TAMP5EN_Pos        (28)                                              /*!< TAMP_T::TIOCTL: TAMP5EN Position     */
#define TAMP_TIOCTL_TAMP5EN_Msk        (0x1ul << TAMP_TIOCTL_TAMP5EN_Pos)                /*!< TAMP_T::TIOCTL: TAMP5EN Mask         */

#define TAMP_TIOCTL_TAMP5LV_Pos        (29)                                              /*!< TAMP_T::TIOCTL: TAMP5LV Position     */
#define TAMP_TIOCTL_TAMP5LV_Msk        (0x1ul << TAMP_TIOCTL_TAMP5LV_Pos)                /*!< TAMP_T::TIOCTL: TAMP5LV Mask         */

#define TAMP_TIOCTL_TAMP5DBEN_Pos      (30)                                              /*!< TAMP_T::TIOCTL: TAMP5DBEN Position   */
#define TAMP_TIOCTL_TAMP5DBEN_Msk      (0x1ul << TAMP_TIOCTL_TAMP5DBEN_Pos)              /*!< TAMP_T::TIOCTL: TAMP5DBEN Mask       */

#define TAMP_TIOCTL_DYNPR2EN_Pos       (31)                                              /*!< TAMP_T::TIOCTL: DYNPR2EN Position    */
#define TAMP_TIOCTL_DYNPR2EN_Msk       (0x1ul << TAMP_TIOCTL_DYNPR2EN_Pos)               /*!< TAMP_T::TIOCTL: DYNPR2EN Mask        */

#define TAMP_SEED_SEED_Pos             (0)                                               /*!< TAMP_T::SEED: SEED Position          */
#define TAMP_SEED_SEED_Msk             (0xfffffffful << TAMP_SEED_SEED_Pos)              /*!< TAMP_T::SEED: SEED Mask              */

#define TAMP_SEED2_SEED2_Pos           (0)                                               /*!< TAMP_T::SEED2: SEED2 Position        */
#define TAMP_SEED2_SEED2_Msk           (0xfffffffful << TAMP_SEED2_SEED2_Pos)            /*!< TAMP_T::SEED2: SEED2 Mask            */

#define TAMP_ACTSTIOCTL1_ADYN1ISS_Pos  (0)                                               /*!< TAMP_T::ACTSTIOCTL1: ADYN1ISS Position*/
#define TAMP_ACTSTIOCTL1_ADYN1ISS_Msk  (0x1ul << TAMP_ACTSTIOCTL1_ADYN1ISS_Pos)          /*!< TAMP_T::ACTSTIOCTL1: ADYN1ISS Mask   */

#define TAMP_ACTSTIOCTL1_ADYNSRC_Pos   (3)                                               /*!< TAMP_T::ACTSTIOCTL1: ADYNSRC Position*/
#define TAMP_ACTSTIOCTL1_ADYNSRC_Msk   (0x1ul << TAMP_ACTSTIOCTL1_ADYNSRC_Pos)           /*!< TAMP_T::ACTSTIOCTL1: ADYNSRC Mask    */

#define TAMP_ACTSTIOCTL1_ADYNRATE_Pos  (5)                                               /*!< TAMP_T::ACTSTIOCTL1: ADYNRATE Position*/
#define TAMP_ACTSTIOCTL1_ADYNRATE_Msk  (0x7ul << TAMP_ACTSTIOCTL1_ADYNRATE_Pos)          /*!< TAMP_T::ACTSTIOCTL1: ADYNRATE Mask   */

#define TAMP_ACTSTIOCTL1_ATAMP0EN_Pos  (8)                                               /*!< TAMP_T::ACTSTIOCTL1: ATAMP0EN Position*/
#define TAMP_ACTSTIOCTL1_ATAMP0EN_Msk  (0x1ul << TAMP_ACTSTIOCTL1_ATAMP0EN_Pos)          /*!< TAMP_T::ACTSTIOCTL1: ATAMP0EN Mask   */

#define TAMP_ACTSTIOCTL1_ATAMP1EN_Pos  (12)                                              /*!< TAMP_T::ACTSTIOCTL1: ATAMP1EN Position*/
#define TAMP_ACTSTIOCTL1_ATAMP1EN_Msk  (0x1ul << TAMP_ACTSTIOCTL1_ATAMP1EN_Pos)          /*!< TAMP_T::ACTSTIOCTL1: ATAMP1EN Mask   */

#define TAMP_ACTSTIOCTL1_ADYNPR0EN_Pos (15)                                              /*!< TAMP_T::ACTSTIOCTL1: ADYNPR0EN Position*/
#define TAMP_ACTSTIOCTL1_ADYNPR0EN_Msk (0x1ul << TAMP_ACTSTIOCTL1_ADYNPR0EN_Pos)         /*!< TAMP_T::ACTSTIOCTL1: ADYNPR0EN Mask  */

#define TAMP_ACTSTIOCTL1_ATAMP2EN_Pos  (16)                                              /*!< TAMP_T::ACTSTIOCTL1: ATAMP2EN Position*/
#define TAMP_ACTSTIOCTL1_ATAMP2EN_Msk  (0x1ul << TAMP_ACTSTIOCTL1_ATAMP2EN_Pos)          /*!< TAMP_T::ACTSTIOCTL1: ATAMP2EN Mask   */

#define TAMP_ACTSTIOCTL1_ATAMP3EN_Pos  (20)                                              /*!< TAMP_T::ACTSTIOCTL1: ATAMP3EN Position*/
#define TAMP_ACTSTIOCTL1_ATAMP3EN_Msk  (0x1ul << TAMP_ACTSTIOCTL1_ATAMP3EN_Pos)          /*!< TAMP_T::ACTSTIOCTL1: ATAMP3EN Mask   */

#define TAMP_ACTSTIOCTL1_ADYNPR1EN_Pos (23)                                              /*!< TAMP_T::ACTSTIOCTL1: ADYNPR1EN Position*/
#define TAMP_ACTSTIOCTL1_ADYNPR1EN_Msk (0x1ul << TAMP_ACTSTIOCTL1_ADYNPR1EN_Pos)         /*!< TAMP_T::ACTSTIOCTL1: ADYNPR1EN Mask  */

#define TAMP_ACTSTIOCTL2_ADYN1ISS2_Pos (0)                                               /*!< TAMP_T::ACTSTIOCTL2: ADYN1ISS2 Position*/
#define TAMP_ACTSTIOCTL2_ADYN1ISS2_Msk (0x1ul << TAMP_ACTSTIOCTL2_ADYN1ISS2_Pos)         /*!< TAMP_T::ACTSTIOCTL2: ADYN1ISS2 Mask  */

#define TAMP_ACTSTIOCTL2_ADYNSRC2_Pos  (3)                                               /*!< TAMP_T::ACTSTIOCTL2: ADYNSRC2 Position*/
#define TAMP_ACTSTIOCTL2_ADYNSRC2_Msk  (0x1ul << TAMP_ACTSTIOCTL2_ADYNSRC2_Pos)          /*!< TAMP_T::ACTSTIOCTL2: ADYNSRC2 Mask   */

#define TAMP_ACTSTIOCTL2_SEEDRLD2_Pos  (4)                                               /*!< TAMP_T::ACTSTIOCTL2: SEEDRLD2 Position*/
#define TAMP_ACTSTIOCTL2_SEEDRLD2_Msk  (0x1ul << TAMP_ACTSTIOCTL2_SEEDRLD2_Pos)          /*!< TAMP_T::ACTSTIOCTL2: SEEDRLD2 Mask   */

#define TAMP_ACTSTIOCTL2_ADYNRATE2_Pos (5)                                               /*!< TAMP_T::ACTSTIOCTL2: ADYNRATE2 Position*/
#define TAMP_ACTSTIOCTL2_ADYNRATE2_Msk (0x7ul << TAMP_ACTSTIOCTL2_ADYNRATE2_Pos)         /*!< TAMP_T::ACTSTIOCTL2: ADYNRATE2 Mask  */

#define TAMP_ACTSTIOCTL2_ATAMP0EN2_Pos (8)                                               /*!< TAMP_T::ACTSTIOCTL2: ATAMP0EN2 Position*/
#define TAMP_ACTSTIOCTL2_ATAMP0EN2_Msk (0x1ul << TAMP_ACTSTIOCTL2_ATAMP0EN2_Pos)         /*!< TAMP_T::ACTSTIOCTL2: ATAMP0EN2 Mask  */

#define TAMP_ACTSTIOCTL2_ATAMP1EN2_Pos (12)                                              /*!< TAMP_T::ACTSTIOCTL2: ATAMP1EN2 Position*/
#define TAMP_ACTSTIOCTL2_ATAMP1EN2_Msk (0x1ul << TAMP_ACTSTIOCTL2_ATAMP1EN2_Pos)         /*!< TAMP_T::ACTSTIOCTL2: ATAMP1EN2 Mask  */

#define TAMP_ACTSTIOCTL2_ADYNPR0EN2_Pos (15)                                             /*!< TAMP_T::ACTSTIOCTL2: ADYNPR0EN2 Position*/
#define TAMP_ACTSTIOCTL2_ADYNPR0EN2_Msk (0x1ul << TAMP_ACTSTIOCTL2_ADYNPR0EN2_Pos)       /*!< TAMP_T::ACTSTIOCTL2: ADYNPR0EN2 Mask */

#define TAMP_ACTSTIOCTL2_ATAMP2EN2_Pos (16)                                              /*!< TAMP_T::ACTSTIOCTL2: ATAMP2EN2 Position*/
#define TAMP_ACTSTIOCTL2_ATAMP2EN2_Msk (0x1ul << TAMP_ACTSTIOCTL2_ATAMP2EN2_Pos)         /*!< TAMP_T::ACTSTIOCTL2: ATAMP2EN2 Mask  */

#define TAMP_ACTSTIOCTL2_ATAMP3EN2_Pos (20)                                              /*!< TAMP_T::ACTSTIOCTL2: ATAMP3EN2 Position*/
#define TAMP_ACTSTIOCTL2_ATAMP3EN2_Msk (0x1ul << TAMP_ACTSTIOCTL2_ATAMP3EN2_Pos)         /*!< TAMP_T::ACTSTIOCTL2: ATAMP3EN2 Mask  */

#define TAMP_ACTSTIOCTL2_ADYNPR1EN2_Pos (23)                                             /*!< TAMP_T::ACTSTIOCTL2: ADYNPR1EN2 Position*/
#define TAMP_ACTSTIOCTL2_ADYNPR1EN2_Msk (0x1ul << TAMP_ACTSTIOCTL2_ADYNPR1EN2_Pos)       /*!< TAMP_T::ACTSTIOCTL2: ADYNPR1EN2 Mask */

#define TAMP_CDBR_STOPBD_Pos           (0)                                               /*!< TAMP_T::CDBR: STOPBD Position        */
#define TAMP_CDBR_STOPBD_Msk           (0xfful << TAMP_CDBR_STOPBD_Pos)                  /*!< TAMP_T::CDBR: STOPBD Mask            */

#define TAMP_CDBR_FAILBD_Pos           (16)                                              /*!< TAMP_T::CDBR: FAILBD Position        */
#define TAMP_CDBR_FAILBD_Msk           (0xfful << TAMP_CDBR_FAILBD_Pos)                  /*!< TAMP_T::CDBR: FAILBD Mask            */

#define TAMP_PWRG_PCLKSEL_Pos          (0)                                               /*!< TAMP_T::PWRG: PCLKSEL Position       */
#define TAMP_PWRG_PCLKSEL_Msk          (0xful << TAMP_PWRG_PCLKSEL_Pos)                  /*!< TAMP_T::PWRG: PCLKSEL Mask           */

#define TAMP_PWRG_NCLKSEL_Pos          (4)                                               /*!< TAMP_T::PWRG: NCLKSEL Position       */
#define TAMP_PWRG_NCLKSEL_Msk          (0xful << TAMP_PWRG_NCLKSEL_Pos)                  /*!< TAMP_T::PWRG: NCLKSEL Mask           */

#define TAMP_PWRG_PDATSEL_Pos          (8)                                               /*!< TAMP_T::PWRG: PDATSEL Position       */
#define TAMP_PWRG_PDATSEL_Msk          (0xful << TAMP_PWRG_PDATSEL_Pos)                  /*!< TAMP_T::PWRG: PDATSEL Mask           */

#define TAMP_PWRG_NDATSEL_Pos          (12)                                              /*!< TAMP_T::PWRG: NDATSEL Position       */
#define TAMP_PWRG_NDATSEL_Msk          (0xful << TAMP_PWRG_NDATSEL_Pos)                  /*!< TAMP_T::PWRG: NDATSEL Mask           */

#define TAMP_PWRGEV_PWRECNTP_Pos       (0)                                               /*!< TAMP_T::PWRGEV: PWRECNTP Position    */
#define TAMP_PWRGEV_PWRECNTP_Msk       (0xfful << TAMP_PWRGEV_PWRECNTP_Pos)              /*!< TAMP_T::PWRGEV: PWRECNTP Mask        */

#define TAMP_PWRGEV_PWRECNTN_Pos       (8)                                               /*!< TAMP_T::PWRGEV: PWRECNTN Position    */
#define TAMP_PWRGEV_PWRECNTN_Msk       (0xfful << TAMP_PWRGEV_PWRECNTN_Pos)              /*!< TAMP_T::PWRGEV: PWRECNTN Mask        */

#define TAMP_LDOTRIM_TLDOTRIM_Pos      (0)                                               /*!< TAMP_T::LDOTRIM: TLDOTRIM Position   */
#define TAMP_LDOTRIM_TLDOTRIM_Msk      (0xful << TAMP_LDOTRIM_TLDOTRIM_Pos)              /*!< TAMP_T::LDOTRIM: TLDOTRIM Mask       */

#define TAMP_LDOTRIM_TLDOIQSEL_Pos     (8)                                               /*!< TAMP_T::LDOTRIM: TLDOIQSEL Position  */
#define TAMP_LDOTRIM_TLDOIQSEL_Msk     (0x3ul << TAMP_LDOTRIM_TLDOIQSEL_Pos)             /*!< TAMP_T::LDOTRIM: TLDOIQSEL Mask      */

#define TAMP_LBSTRIM_TLVDSEL_Pos       (0)                                               /*!< TAMP_T::LBSTRIM: TLVDSEL Position    */
#define TAMP_LBSTRIM_TLVDSEL_Msk       (0x7ul << TAMP_LBSTRIM_TLVDSEL_Pos)               /*!< TAMP_T::LBSTRIM: TLVDSEL Mask        */

#define TAMP_LBSTRIM_TOVDSEL_Pos       (4)                                               /*!< TAMP_T::LBSTRIM: TOVDSEL Position    */
#define TAMP_LBSTRIM_TOVDSEL_Msk       (0x1ul << TAMP_LBSTRIM_TOVDSEL_Pos)               /*!< TAMP_T::LBSTRIM: TOVDSEL Mask        */

#define TAMP_LBSTRIM_BSCMPLV_Pos       (8)                                               /*!< TAMP_T::LBSTRIM: BSCMPLV Position    */
#define TAMP_LBSTRIM_BSCMPLV_Msk       (0x3ul << TAMP_LBSTRIM_BSCMPLV_Pos)               /*!< TAMP_T::LBSTRIM: BSCMPLV Mask        */

#define TAMP_LBSTRIM_BSCMPOV_Pos       (10)                                              /*!< TAMP_T::LBSTRIM: BSCMPOV Position    */
#define TAMP_LBSTRIM_BSCMPOV_Msk       (0x3ul << TAMP_LBSTRIM_BSCMPOV_Pos)               /*!< TAMP_T::LBSTRIM: BSCMPOV Mask        */

#define TAMP_LBSTRIM_HYSCMPLV_Pos      (12)                                              /*!< TAMP_T::LBSTRIM: HYSCMPLV Position   */
#define TAMP_LBSTRIM_HYSCMPLV_Msk      (0x3ul << TAMP_LBSTRIM_HYSCMPLV_Pos)              /*!< TAMP_T::LBSTRIM: HYSCMPLV Mask       */

#define TAMP_LBSTRIM_HYSCMPOV_Pos      (14)                                              /*!< TAMP_T::LBSTRIM: HYSCMPOV Position   */
#define TAMP_LBSTRIM_HYSCMPOV_Msk      (0x3ul << TAMP_LBSTRIM_HYSCMPOV_Pos)              /*!< TAMP_T::LBSTRIM: HYSCMPOV Mask       */

/**@}*/ /* TAMPER_CONST */
/**@}*/ /* end of TAMPER register group */
/**@}*/ /* end of REGISTER group */

#endif /* __TAMPER_REG_H__ */
