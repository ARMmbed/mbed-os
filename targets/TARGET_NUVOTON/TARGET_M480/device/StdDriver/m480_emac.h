/**************************************************************************//**
 * @file     emac.h
 * @version  V1.00
 * @brief    M480 EMAC driver header file
 *
 * @copyright (C) 2016 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef __EMAC_H__
#define __EMAC_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup M480_Device_Driver M480 Device Driver
  @{
*/

/** @addtogroup M480_EMAC_Driver EMAC Driver
  @{
*/

/** @addtogroup M480_EMAC_EXPORTED_CONSTANTS EMAC Exported Constants
  @{
*/

#define EMAC_PHY_ADDR     1UL    /*!<  PHY address, this address is board dependent \hideinitializer */
#define EMAC_RX_DESC_SIZE 4UL    /*!<  Number of Rx Descriptors, should be 2 at least \hideinitializer */
#define EMAC_TX_DESC_SIZE 4UL    /*!<  Number of Tx Descriptors, should be 2 at least \hideinitializer */


/*@}*/ /* end of group M480_EMAC_EXPORTED_CONSTANTS */


/** @addtogroup M480_EMAC_EXPORTED_FUNCTIONS EMAC Exported Functions
  @{
*/


/**
  * @brief  Enable EMAC Tx function
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_ENABLE_TX() (EMAC->CTL |= EMAC_CTL_TXON_Msk)


/**
  * @brief  Enable EMAC Rx function
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_ENABLE_RX() do{EMAC->CTL |= EMAC_CTL_RXON_Msk; EMAC->RXST = 0;}while(0)

/**
  * @brief  Disable EMAC Tx function
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_DISABLE_TX() (EMAC->CTL &= ~EMAC_CTL_TXON_Msk)


/**
  * @brief  Disable EMAC Rx function
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_DISABLE_RX() (EMAC->CTL &= ~EMAC_CTL_RXON_Msk)

/**
  * @brief  Enable EMAC Magic Packet Wakeup function
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_ENABLE_MAGIC_PKT_WAKEUP() (EMAC->CTL |= EMAC_CTL_WOLEN_Msk)


/**
  * @brief  Disable EMAC Magic Packet Wakeup function
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_DISABLE_MAGIC_PKT_WAKEUP() (EMAC->CTL &= ~EMAC_CTL_WOLEN_Msk)

/**
  * @brief  Enable EMAC MII interface
  * @param  None
  * @return None
  * @details After calling \ref EMAC_Open, EMAC use RMII interface by default, but can switch to
  *          MII interface by calling this macro
  * \hideinitializer
  */
#define EMAC_ENABLE_MII_INTF() (EMAC->CTL &= ~(EMAC_CTL_RMIIEN_Msk | EMAC_CTL_RMIIRXCTL_Msk))

/**
  * @brief  Enable EMAC to receive broadcast packets
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_ENABLE_RECV_BCASTPKT() (EMAC->CAMCTL |= EMAC_CAMCTL_ABP_Msk)

/**
  * @brief  Disable EMAC to receive broadcast packets
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_DISABLE_RECV_BCASTPKT() (EMAC->CAMCTL &= ~EMAC_CAMCTL_ABP_Msk)

/**
  * @brief  Enable EMAC to receive multicast packets
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_ENABLE_RECV_MCASTPKT() (EMAC->CAMCTL |= EMAC_CAMCTL_AMP_Msk)

/**
  * @brief  Disable EMAC Magic Packet Wakeup function
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_DISABLE_RECV_MCASTPKT() (EMAC->CAMCTL &= ~EMAC_CAMCTL_AMP_Msk)

/**
  * @brief  Check if EMAC time stamp alarm interrupt occurred or not
  * @param  None
  * @return If time stamp alarm interrupt occurred or not
  * @retval 0 Alarm interrupt does not occur
  * @retval 1 Alarm interrupt occurred
  * \hideinitializer
  */
#define EMAC_GET_ALARM_FLAG() (EMAC->INTSTS & EMAC_INTSTS_TSALMIF_Msk ? 1 : 0)

/**
  * @brief  Clear EMAC time stamp alarm interrupt flag
  * @param  None
  * @return None
  * \hideinitializer
  */
#define EMAC_CLR_ALARM_FLAG() (EMAC->INTSTS = EMAC_INTSTS_TSALMIF_Msk)


void EMAC_Open(uint8_t *pu8MacAddr);
void EMAC_Close(void);
void EMAC_SetMacAddr(uint8_t *pu8MacAddr);
void EMAC_EnableCamEntry(uint32_t u32Entry, uint8_t pu8MacAddr[]);
void EMAC_DisableCamEntry(uint32_t u32Entry);

uint32_t EMAC_RecvPkt(uint8_t *pu8Data, uint32_t *pu32Size);
uint32_t EMAC_RecvPktTS(uint8_t *pu8Data, uint32_t *pu32Size, uint32_t *pu32Sec, uint32_t *pu32Nsec);
void EMAC_RecvPktDone(void);

uint32_t EMAC_SendPkt(uint8_t *pu8Data, uint32_t u32Size);
uint32_t EMAC_SendPktDone(void);
uint32_t EMAC_SendPktDoneTS(uint32_t *pu32Sec, uint32_t *pu32Nsec);

void EMAC_EnableTS(uint32_t u32Sec, uint32_t u32Nsec);
void EMAC_DisableTS(void);
void EMAC_GetTime(uint32_t *pu32Sec, uint32_t *pu32Nsec);
void EMAC_SetTime(uint32_t u32Sec, uint32_t u32Nsec);
void EMAC_UpdateTime(uint32_t u32Neg, uint32_t u32Sec, uint32_t u32Nsec);
void EMAC_EnableAlarm(uint32_t u32Sec, uint32_t u32Nsec);
void EMAC_DisableAlarm(void);



/*@}*/ /* end of group M480_EMAC_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group M480_EMAC_Driver */

/*@}*/ /* end of group M480_Device_Driver */

#ifdef __cplusplus
}
#endif

#endif /* __EMAC_H__ */

/*** (C) COPYRIGHT 2016 Nuvoton Technology Corp. ***/
