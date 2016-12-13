/**************************************************************************//**
 * @file     opa.h
 * @version  V3.00
 * @brief    M480 series OPA driver header file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
#ifndef __OPA_H__
#define __OPA_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup M480_Device_Driver M480 Device Driver
  @{
*/

/** @addtogroup OPA_Driver OPA Driver
  @{
*/

/** @addtogroup OPA_EXPORTED_CONSTANTS OPA Exported Constants
  @{
*/

#define OPA_CALIBRATION_CLOCK_1K                      (0UL)                     /*!< OPA calibration clock rate select 1KHz   */
#define OPA_CALIBRATION_CLOCK_5K                      (1UL)                     /*!< OPA calibration clock rate select 5KHz   */
#define OPA_CALIBRATION_CLOCK_10K                     (2UL)                     /*!< OPA calibration clock rate select 10KHz   */
#define OPA_CALIBRATION_CLOCK_100K                    (3UL)                     /*!< OPA calibration clock rate select 100KHz   */
#define OPA_CALIBRATION_LEVEL_1_4_AVDD                (0UL)                     /*!< OPA calibration level select 1/4 AVDD   */
#define OPA_CALIBRATION_LEVEL_2_4_AVDD                (1UL)                     /*!< OPA calibration level select 1/2 AVDD   */
#define OPA_CALIBRATION_LEVEL_3_4_AVDD                (2UL)                     /*!< OPA calibration level select 3/4 AVDD   */

/*@}*/ /* end of group OPA_EXPORTED_CONSTANTS */

/** @addtogroup OPA_EXPORTED_FUNCTIONS OPA Exported Functions
  @{
*/

/**
  * @brief This macro is used to power on the OPA circuit
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @return None
  * @details This macro will set OPx_EN (x=0, 1) bit of OPACR register to power on the OPA circuit.
  */
#define OPA_POWER_ON(opa, u32OpaNum) ((opa)->CTL |= (1<<(OPA_CTL_OPEN0_Pos+(u32OpaNum))))

/**
  * @brief This macro is used to power down the OPA circuit
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @return None
  * @details This macro will clear OPx_EN (x=0, 1) bit of OPACR register to power down the OPA circuit.
  */
#define OPA_POWER_DOWN(opa, u32OpaNum) ((opa)->CTL &= ~(1<<(OPA_CTL_OPEN0_Pos+(u32OpaNum))))

/**
  * @brief This macro is used to enable the OPA Schmitt trigger buffer
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @return None
  * @details This macro will set OPSCHx_EN (x=0, 1) bit of OPACR register to enable the OPA Schmitt trigger buffer.
  */
#define OPA_ENABLE_SCH_TRIGGER(opa, u32OpaNum) ((opa)->CTL |= (1<<(OPA_CTL_OPDOEN0_Pos+(u32OpaNum))))

/**
  * @brief This macro is used to disable the OPA Schmitt trigger buffer
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @return None
  * @details This macro will clear OPSCHx_EN (x=0, 1) bit of OPACR register to disable the OPA Schmitt trigger buffer.
  */
#define OPA_DISABLE_SCH_TRIGGER(opa, u32OpaNum) ((opa)->CTL &= ~(1<<(OPA_CTL_OPDOEN0_Pos+(u32OpaNum))))

/**
  * @brief This macro is used to enable OPA Schmitt trigger digital output interrupt
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @return None
  * @details This macro will set OPDIEx (x=0, 1) bit of OPACR register to enable the OPA Schmitt trigger digital output interrupt.
  */
#define OPA_ENABLE_INT(opa, u32OpaNum) ((opa)->CTL |= (1<<(OPA_CTL_OPDOIEN0_Pos+(u32OpaNum))))

/**
  * @brief This macro is used to disable OPA Schmitt trigger digital output interrupt
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @return None
  * @details This macro will clear OPDIEx (x=0, 1) bit of OPACR register to disable the OPA Schmitt trigger digital output interrupt.
  */
#define OPA_DISABLE_INT(opa, u32OpaNum) ((opa)->CTL &= ~(1<<(OPA_CTL_OPDOIEN0_Pos+(u32OpaNum))))

/**
  * @brief This macro is used to get OPA digital output state
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @return  OPA digital output state
  * @details This macro will return the OPA digital output value.
  */
#define OPA_GET_DIGITAL_OUTPUT(opa, u32OpaNum) (((opa)->STATUS & (OPA_STATUS_OPDO0_Msk<<(u32OpaNum)))?1:0)

/**
  * @brief This macro is used to get OPA interrupt flag
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @retval     0 OPA interrupt does not occur.
  * @retval     1 OPA interrupt occurs.
  * @details This macro will return the ACMP interrupt flag.
  */
#define OPA_GET_INT_FLAG(opa, u32OpaNum) (((opa)->STATUS & (OPA_STATUS_OPDO0_Msk<<(u32OpaNum)))?1:0)

/**
  * @brief This macro is used to clear OPA interrupt flag
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @return   None
  * @details This macro will write 1 to OPDFx (x=0,1) bit of OPASR register to clear interrupt flag.
  */
#define OPA_CLR_INT_FLAG(opa, u32OpaNum) ((opa)->STATUS = (OPA_STATUS_OPDO0_Msk<<(u32OpaNum)))


/**
  * @brief This function is used to configure and start OPA calibration
  * @param[in] opa The pointer of the specified OPA module
  * @param[in] u32OpaNum The OPA number. 0 for OPA0; 1 for OPA1; 2 for OPA2.
  * @param[in] u32ClockSel Select OPA calibration clock rate
  *                 - \ref OPA_CALIBRATION_CLOCK_1K
  *                 - \ref OPA_CALIBRATION_CLOCK_5K
  *                 - \ref OPA_CALIBRATION_CLOCK_10K
  *                 - \ref OPA_CALIBRATION_CLOCK_100K
  * @param[in] u32LevelSel Select OPA calibration level
  *                 - \ref OPA_CALIBRATION_LEVEL_1_4_AVDD
  *                 - \ref OPA_CALIBRATION_LEVEL_2_4_AVDD
  *                 - \ref OPA_CALIBRATION_LEVEL_3_4_AVDD
  * @retval      0 PMOS and NMOS calibration successfully.
  * @retval     -1 only PMOS calibration failed.
  * @retval     -2 only NMOS calibration failed.
  * @retval     -3 PMOS and NMOS calibration failed.
  */
__STATIC_INLINE uint32_t OPA_Calibration(OPA_T *opa,
        uint32_t u32OpaNum,
        uint32_t u32ClockSel,
        uint32_t u32LevelSel)
{
    uint32_t u32CALResult;
    int32_t i32Ret = 0;

    (opa)->CALCTL = (((opa)->CALCTL) & ~(OPA_CALCTL_CALCLK0_Msk << ((u32OpaNum) << 1))) | (((u32ClockSel) << OPA_CALCTL_CALCLK0_Pos) << ((u32OpaNum) << 1));
    (opa)->CALCTL = (((opa)->CALCTL) & ~(OPA_CALCTL_CALLV0_Msk << ((u32OpaNum) << 1))) | (((u32LevelSel) << OPA_CALCTL_CALLV0_Pos) << ((u32OpaNum) << 1));
    (opa)->CALCTL |= (OPA_CALCTL_CALTRG0_Msk << (u32OpaNum));
    while((opa)->CALCTL & (OPA_CALCTL_CALTRG0_Msk << (u32OpaNum)));

    u32CALResult = ((opa)->CALST >> ((u32OpaNum)*4)) & (OPA_CALST_CALNS0_Msk|OPA_CALST_CALPS0_Msk);
    if (u32CALResult == 0)
        i32Ret = 0;
    else if (u32CALResult == OPA_CALST_CALNS0_Msk)
        i32Ret = -2;
    else if (u32CALResult == OPA_CALST_CALPS0_Msk)
        i32Ret = -1;
    else if (u32CALResult == (OPA_CALST_CALNS0_Msk|OPA_CALST_CALPS0_Msk))
        i32Ret = -3;

    return i32Ret;
}

/*@}*/ /* end of group OPA_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group OPA_Driver */

/*@}*/ /* end of group M480_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif //__OPA_H__

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
