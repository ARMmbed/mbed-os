/**************************************************************************//**
 * @file     ecap.h
 * @version  V3.00
 * $Revision: 1 $
 * $Date: 16/08/12 9:31a $
 * @brief    EnHanced Input Capture Timer(ECAP) driver header file
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#ifndef __ECAP_H__
#define __ECAP_H__

#ifdef __cplusplus
extern "C"
{
#endif


/** @addtogroup Standard_Driver Standard Driver
  @{
*/

/** @addtogroup ECAP_Driver ECAP Driver
  @{
*/

/** @addtogroup ECAP_EXPORTED_CONSTANTS ECAP Exported Constants
  @{
*/

#define ECAP_IC0    (0UL)    /*!< ECAP IC0 Unit */
#define ECAP_IC1    (1UL)    /*!< ECAP IC1 Unit */
#define ECAP_IC2    (2UL)    /*!< ECAP IC2 Unit */

/*---------------------------------------------------------------------------------------------------------*/
/* ECAP CTL0 constant definitions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define ECAP_NOISE_FILTER_CLKDIV_1            (0UL)    /*!< Noise filter clock divide by 1  */
#define ECAP_NOISE_FILTER_CLKDIV_2            (1UL)    /*!< Noise filter clock divide by 2  */
#define ECAP_NOISE_FILTER_CLKDIV_4            (2UL)    /*!< Noise filter clock divide by 4  */
#define ECAP_NOISE_FILTER_CLKDIV_16           (3UL)    /*!< Noise filter clock divide by 16 */
#define ECAP_NOISE_FILTER_CLKDIV_32           (4UL)    /*!< Noise filter clock divide by 32  */
#define ECAP_NOISE_FILTER_CLKDIV_64           (5UL)    /*!< Noise filter clock divide by 64  */


#define ECAP_CAP_INPUT_SRC_FROM_IC            (0UL)    /*!< CAP input source from IC             */
#define ECAP_CAP_INPUT_SRC_FROM_CH            (2UL)    /*!< CAP input source from CH of QEI      */

#define ECAP_CNT_CLR_EVENT_DISABLE            (0UL)    /*!< Input channel counter cleared event disabled             */
#define ECAP_CNT_CLR_BY_CMP                   (1UL)    /*!< Input channel counter cleared by Compare-Match           */
#define ECAP_CNT_CLR_BY_CAPTURE               (2UL)    /*!< Input channel counter cleared by Capture                 */
#define ECAP_CNT_CLR_BY_CAMCMPF               (3UL)    /*!< Input channel counter cleared by Compare-Match & capture */

#define ECAP_DISABLE_COMPARE_RELOAD           0x00     /*!< Input capture compare and reload function disable*/
#define ECAP_RELOAD_FUNCTION                  0x01     /*!< Input capture reload function                    */
#define ECAP_COMPARE_FUNCTION                 0x02     /*!< Input capture compare function                   */
#define ECAP_RELOAD_COMPARE_FUNCTION          0x03     /*!< Input capture reload & compare function          */
/*---------------------------------------------------------------------------------------------------------*/
/* ECAP CTL1 constant definitions                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
#define ECAP_RISING_EDGE                       (0UL)    /*!< ECAP capture rising edge selection                      */
#define ECAP_FALLING_EDGE                      (1UL)    /*!< ECAP capture falling edge selection                     */
#define ECAP_RISING_FALLING_EDGE               (2UL)    /*!< ECAP capture either rising or falling edge selection    */

#define ECAP_RELOAD_TRIG_SRC_CAPF0             (0UL)    /*!< ECAP counter reload trigger source CAPF0 selection */
#define ECAP_RELOAD_TRIG_SRC_CAPF1             (1UL)    /*!< ECAP counter reload trigger source CAPF1 selection */
#define ECAP_RELOAD_TRIG_SRC_CAPF2             (2UL)    /*!< ECAP counter reload trigger source CAPF2 selection */
#define ECAP_RELOAD_TRIG_SRC_OVF               (4UL)    /*!< ECAP counter reload trigger source OVF selection   */

#define ECAP_CAPTURE_TIMER_CLKDIV_1            (0UL)    /*!< ECAP capture timer clock divide by 1   */
#define ECAP_CAPTURE_TIMER_CLKDIV_4            (1UL)    /*!< ECAP capture timer clock divide by 4   */
#define ECAP_CAPTURE_TIMER_CLKDIV_16           (2UL)    /*!< ECAP capture timer clock divide by 16  */
#define ECAP_CAPTURE_TIMER_CLKDIV_32           (3UL)    /*!< ECAP capture timer clock divide by 32  */
#define ECAP_CAPTURE_TIMER_CLKDIV_64           (4UL)    /*!< ECAP capture timer clock divide by 64  */
#define ECAP_CAPTURE_TIMER_CLKDIV_96           (5UL)    /*!< ECAP capture timer clock divide by 96  */
#define ECAP_CAPTURE_TIMER_CLKDIV_112          (6UL)    /*!< ECAP capture timer clock divide by 112 */
#define ECAP_CAPTURE_TIMER_CLKDIV_128          (7UL)    /*!< ECAP capture timer clock divide by 128 */

#define ECAP_CAPTURE_TIMER_CLK_SRC_CAP_CLK     (0UL)    /*!< ECAP capture timer/clock source from CAP_CLK */
#define ECAP_CAPTURE_TIMER_CLK_SRC_CAP0        (1UL)    /*!< ECAP capture timer/clock source from CAP0    */
#define ECAP_CAPTURE_TIMER_CLK_SRC_CAP1        (2UL)    /*!< ECAP capture timer/clock source from CAP1    */
#define ECAP_CAPTURE_TIMER_CLK_SRC_CAP2        (3UL)    /*!< ECAP capture timer/clock source from CAP2    */

/*@}*/ /* end of group ECAP_EXPORTED_CONSTANTS */

/** @addtogroup ECAP_EXPORTED_FUNCTIONS ECAP Exported Functions
  @{
*/

/**
  * @brief This macro is used to select noise filter clock pre-divide number
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32ClkSel The noise filter clock divide number
  *                  - \ref ECAP_NOISE_FILTER_CLKDIV_1
  *                  - \ref ECAP_NOISE_FILTER_CLKDIV_2
  *                  - \ref ECAP_NOISE_FILTER_CLKDIV_4
  *                  - \ref ECAP_NOISE_FILTER_CLKDIV_16
  * @return None
  * @details This macro will set the sampling frequency of the noise filter cock.
  */
#define ECAP_SET_NOISE_FILTER_CLKDIV(ecap, u32ClkSel) ((ecap)->CTL0 = ((ecap)->CTL0 & ~ECAP_CTL0_NFCLKSEL_Msk)|((u32ClkSel)<<ECAP_CTL0_NFCLKSEL_Pos))

/**
  * @brief This macro is used to disable noise filter
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will disable the noise filter of input capture.
  */
#define ECAP_NOISE_FILTER_DISABLE(ecap) ((ecap)->CTL0 |= ECAP_CTL0_NFDIS_Msk)

/**
  * @brief This macro is used to enable noise filter
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32ClkSel Select noise filter clock divide number
  *                  - \ref ECAP_NOISE_FILTER_CLKDIV_1
  *                  - \ref ECAP_NOISE_FILTER_CLKDIV_2
  *                  - \ref ECAP_NOISE_FILTER_CLKDIV_4
  *                  - \ref ECAP_NOISE_FILTER_CLKDIV_16
  * @return None
  * @details This macro will enable the noise filter of input capture and set noise filter clock divide.
  */
#define ECAP_NOISE_FILTER_ENABLE(ecap) ((ecap)->CTL0 = ((ecap)->CTL0 & ~ECAP_CTL0_NFDIS_Msk))

/**
  * @brief This macro is used to enable input channel unit
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Mask   The input channel mask
  *                  - \ref ECAP_IC0
  *                  - \ref ECAP_IC1
  *                  - \ref ECAP_IC2
  * @return None
  * @details This macro will enable the input channel_n to input capture.
  */
#define ECAP_ENABLE_INPUT_CHANNEL(ecap, u32Mask) ((ecap)->CTL0 |= 1<<(u32Mask+4))

/**
  * @brief This macro is used to disable input channel unit
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Mask  The input channel mask
  *                  - \ref ECAP_IC0
  *                  - \ref ECAP_IC1
  *                  - \ref ECAP_IC2
  * @return None
  * @details This macro will disable the input channel_n to input capture.
  */
#define ECAP_DISABLE_INPUT_CHANNEL(ecap, u32Mask) ((ecap)->CTL0 &= ~(1<<(u32Mask+4)))

/**
  * @brief This macro is used to select input channel source
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Index  The input channel number
  *                  - \ref ECAP_IC0
  *                  - \ref ECAP_IC1
  *                  - \ref ECAP_IC2
  * @param[in] u32Src    The input source
  *                  - \ref ECAP_CAP_INPUT_SRC_FROM_IC
  *                  - \ref ECAP_CAP_INPUT_SRC_FROM_CH
  * @return None
  * @details This macro will select the input soure from ICx, CPOx, CHx, ADCMPO or OPDO.
  */
#define ECAP_SEL_INPUT_SRC(ecap, u32Index, u32Src) ((ecap)->CTL0 = ((ecap)->CTL0 & ~(ECAP_CTL0_CAPSEL0_Msk<<((u32Index)<<1)))|((u32Src)<<(ECAP_CTL0_CAPSEL0_Pos)<<((u32Index)<<1)))

/**
  * @brief This macro is used to enable input channel interrupt
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Mask  The input channel mask
  *                  - \ref ECAP_CTL0_CAPIEN0_Msk
  *                  - \ref ECAP_CTL0_CAPIEN1_Msk
  *                  - \ref ECAP_CTL0_CAPIEN2_Msk
  * @return None
  * @details This macro will enable the input channel_n interrupt.
  */
#define ECAP_ENABLE_INT(ecap, u32Mask) ((ecap)->CTL0 |= (u32Mask))

/**
  * @brief This macro is used to disable input channel interrupt
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Mask   The input channel mask
  *                  - \ref ECAP_IC0
  *                  - \ref ECAP_IC1
  *                  - \ref ECAP_IC2
  * @return None
  * @details This macro will disable the input channel_n interrupt.
  */
#define ECAP_DISABLE_INT(ecap, u32Mask) ((ecap)->CTL0 &= ~(u32Mask))

/**
  * @brief This macro is used to enable input channel overflow interrupt
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will enable the input channel overflow interrupt.
  */
#define ECAP_ENABLE_OVF_INT(ecap) ((ecap)->CTL0 |= ECAP_CTL0_OVIEN_Msk)

/**
  * @brief This macro is used to disable input channel overflow interrupt
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will disable the input channel overflow interrupt.
  */
#define ECAP_DISABLE_OVF_INT(ecap) ((ecap)->CTL0 &= ~ECAP_CTL0_OVIEN_Msk)

/**
  * @brief This macro is used to enable input channel compare-match interrupt
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will enable the input channel compare-match interrupt.
  */
#define ECAP_ENABLE_CMP_MATCH_INT(ecap) ((ecap)->CTL0 |= ECAP_CTL0_CMPIEN_Msk)

/**
  * @brief This macro is used to disable input channel compare-match interrupt
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will disable the input channel compare-match interrupt.
  */
#define ECAP_DISABLE_CMP_MATCH_INT(ecap) ((ecap)->CTL0 &= ~ECAP_CTL0_CMPIEN_Msk)

/**
  * @brief This macro is used to start capture counter
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will start capture counter up-counting.
  */
#define ECAP_CNT_START(ecap) ((ecap)->CTL0 |= ECAP_CTL0_CNTEN_Msk)

/**
  * @brief This macro is used to stop capture counter
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will stop capture counter up-counting.
  */
#define ECAP_CNT_STOP(ecap) ((ecap)->CTL0 &= ~ECAP_CTL0_CNTEN_Msk)

/**
  * @brief This macro is used to set event to clear capture counter
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Event The input channel number
  *                  - \ref ECAP_CNT_CLR_EVENT_DISABLE
  *                  - \ref ECAP_CNT_CLR_BY_CMP
  *                  - \ref ECAP_CNT_CLR_BY_CAPTURE
  *                  - \ref ECAP_CNT_CLR_BY_CAMCMPF
  * @return None
  * @details This macro will enable and select compare or capture event that can clear capture counter.
  */
#define ECAP_SET_CNT_CLEAR_EVENT(ecap, u32Event) ((ecap)->CTL0 = ((ecap)->CTL0 & ~(ECAP_CTL0_CMPCLR_Msk | ECAP_CTL0_CPTCLR_Msk))|((u32Event) << ECAP_CTL0_CMPCLR_Pos))

/**
  * @brief This macro is used to enable reload function
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will enable the reload function.
  */
#define ECAP_ENABLE_RELOAD(ecap) ((ecap)->CTL0 |= ECAP_CTL0_RLDEN_Msk)

/**
  * @brief This macro is used to disable reload function
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will disable the reload function.
  */
#define ECAP_DISABLE_RELOAD(ecap) ((ecap)->CTL0 &= ~ECAP_CTL0_RLDEN_Msk)

/**
  * @brief This macro is used to enable compare function
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will enable the compare function.
  */
#define ECAP_ENABLE_CMP(ecap) ((ecap)->CTL0 |= ECAP_CTL0_CMPEN_Msk)

/**
  * @brief This macro is used to disable compare function
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will disable the compare function.
  */
#define ECAP_DISABLE_CMP(ecap) ((ecap)->CTL0 &= ~ECAP_CTL0_CMPEN_Msk)

/**
  * @brief This macro is used to enable input capture function.
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will enable input capture timer/counter.
  */
#define ECAP_ENABLE_CNT(ecap) ((ecap)->CTL0 |= ECAP_CTL0_CAPEN_Msk)

/**
  * @brief This macro is used to disable input capture function.
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will disable input capture timer/counter.
  */
#define ECAP_DISABLE_CNT(ecap) ((ecap)->CTL0 &= ~ECAP_CTL0_CAPEN_Msk)

/**
  * @brief This macro is used to select input channel edge detection
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Index  The input channel number
  *                  - \ref ECAP_IC0
  *                  - \ref ECAP_IC1
  *                  - \ref ECAP_IC2
  * @param[in] u32Edge   The input source
  *                  - \ref ECAP_RISING_EDGE
  *                  - \ref ECAP_FALLING_EDGE
  *                  - \ref ECAP_RISING_FALLING_EDGE
  * @return None
  * @details This macro will select input capture can detect falling edge, rising edge or either rising or falling edge change.
  */
#define ECAP_SEL_CAPTURE_EDGE(ecap, u32Index, u32Edge) ((ecap)->CTL1 = ((ecap)->CTL1 & ~(ECAP_CTL1_EDGESEL0_Msk<<((u32Index)<<1)))|((u32Edge)<<(ECAP_CTL1_EDGESEL0_Pos)<<((u32Index)<<1)))

/**
  * @brief This macro is used to select ECAP counter reload trigger source
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32TrigSrc The input source
  *                  - \ref ECAP_RELOAD_TRIG_SRC_CAPF0
  *                  - \ref ECAP_RELOAD_TRIG_SRC_CAPF1
  *                  - \ref ECAP_RELOAD_TRIG_SRC_CAPF2
  *                  - \ref ECAP_RELOAD_TRIG_SRC_OVF
  * @return None
  * @details This macro will select capture counter reload trigger source.
  */
#define ECAP_SEL_RELOAD_TRIG_SRC(ecap, u32TrigSrc) ((ecap)->CTL1 = ((ecap)->CTL1 & ~ECAP_CTL1_RLDSEL_Msk)|((u32TrigSrc)<<ECAP_CTL1_RLDSEL_Pos))

/**
  * @brief This macro is used to select capture timer clock divide.
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Clkdiv The input source
  *                  - \ref ECAP_CAPTURE_TIMER_CLKDIV_1
  *                  - \ref ECAP_CAPTURE_TIMER_CLKDIV_4
  *                  - \ref ECAP_CAPTURE_TIMER_CLKDIV_16
  *                  - \ref ECAP_CAPTURE_TIMER_CLKDIV_32
  *                  - \ref ECAP_CAPTURE_TIMER_CLKDIV_64
  *                  - \ref ECAP_CAPTURE_TIMER_CLKDIV_96
  *                  - \ref ECAP_CAPTURE_TIMER_CLKDIV_112
  *                  - \ref ECAP_CAPTURE_TIMER_CLKDIV_128
  * @return None
  * @details This macro will select capture timer clock has a pre-divider with eight divided option.
  */
#define ECAP_SEL_TIMER_CLK_DIV(ecap, u32Clkdiv) ((ecap)->CTL1 = ((ecap)->CTL1 & ~ECAP_CTL1_CLKSEL_Msk)|((u32Clkdiv)<<ECAP_CTL1_CLKSEL_Pos))

/**
  * @brief This macro is used to select capture timer/counter clock source
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32ClkSrc The input source
  *                  - \ref ECAP_CAPTURE_TIMER_CLK_SRC_CAP_CLK
  *                  - \ref ECAP_CAPTURE_TIMER_CLK_SRC_CAP0
  *                  - \ref ECAP_CAPTURE_TIMER_CLK_SRC_CAP1
  *                  - \ref ECAP_CAPTURE_TIMER_CLK_SRC_CAP2
  * @return None
  * @details This macro will select capture timer/clock clock source.
  */
#define ECAP_SEL_TIMER_CLK_SRC(ecap, u32ClkSrc) ((ecap)->CTL1 = ((ecap)->CTL1 & ~ECAP_CTL1_CNTSRC_Msk)|((u32ClkSrc)<<ECAP_CTL1_CNTSRC_Pos))

/**
  * @brief This macro is used to read input capture status
  * @param[in] ecap      Specify ECAP port
  * @return Input capture status flags
  * @details This macro will get the input capture interrupt status.
  */
#define ECAP_GET_INT_STATUS(ecap) ((ecap)->STATUS)

/**
  * @brief This macro is used to get input channel interrupt flag
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Mask  The input channel mask
  *                  - \ref ECAP_STATUS_CAPTF0_Msk
  *                  - \ref ECAP_STATUS_CAPTF1_Msk
  *                  - \ref ECAP_STATUS_CAPTF2_Msk
  *                  - \ref ECAP_STATUS_CAPOVF_Msk
  *                  - \ref ECAP_STATUS_CAPCMPF_Msk
  * @return None
  * @details This macro will write 1 to get the input channel_n interrupt flag.
  */
#define ECAP_GET_CAPTURE_FLAG(ecap, u32Mask) (((ecap)->STATUS & (u32Mask))?1:0)

/**
  * @brief This macro is used to clear input channel interrupt flag
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Mask  The input channel mask
  *                  - \ref ECAP_STATUS_CAPTF0_Msk
  *                  - \ref ECAP_STATUS_CAPTF1_Msk
  *                  - \ref ECAP_STATUS_CAPTF2_Msk
  *                  - \ref ECAP_STATUS_CAPOVF_Msk
  *                  - \ref ECAP_STATUS_CAPCMPF_Msk
  * @return None
  * @details This macro will write 1 to clear the input channel_n interrupt flag.
  */
#define ECAP_CLR_CAPTURE_FLAG(ecap, u32Mask) ((ecap)->STATUS = (u32Mask))
/**
  * @brief This macro is used to clear input capture-match interrupt flag
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will write 1 to clear the input capture-match interrupt flag.
  */
#define ECAP_CLR_CMP_MATCH_FLAG(ecap) ((ecap)->STATUS = ECAP_STATUS_CAPCMPF_Msk)

/**
  * @brief This macro is used to clear input overflow interrupt flag
  * @param[in] ecap      Specify ECAP port
  * @return None
  * @details This macro will write 1 to clear the input overflow interrupt flag.
  */
#define ECAP_CLR_OVF_FLAG(ecap) ((ecap)->STATUS = ECAP_STATUS_CAPOVF_Msk)

/**
  * @brief This macro is used to set input capture counter value
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Val    Counter value
  * @return None
  * @details This macro will set a counter value of input capture.
  */
#define ECAP_SET_CNT_VALUE(ecap, u32Val) ((ecap)->CNT = (u32Val))

/**
  * @brief This macro is used to get input capture counter value
  * @param[in] ecap      Specify ECAP port
  * @return Capture counter value
  * @details This macro will get a counter value of input capture.
  */
#define ECAP_GET_CNT_VALUE(ecap) ((ecap)->CNT)

/**
  * @brief This macro is used to get input capture counter hold value
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Index  The input channel number
  *                  - \ref ECAP_IC0
  *                  - \ref ECAP_IC1
  *                  - \ref ECAP_IC2
  * @return Capture counter hold value
  * @details This macro will get a hold value of input capture channel_n.
  */
#define ECAP_GET_CNT_HOLD_VALUE(ecap, u32Index) (*(__IO uint32_t *) (&((ecap)->HLD0) + (u32Index)))

/**
  * @brief This macro is used to set input capture counter compare value
  * @param[in] ecap      Specify ECAP port
  * @param[in] u32Val    Input capture compare value
  * @return None
  * @details This macro will set a compare value of input capture counter.
  */
#define ECAP_SET_CNT_CMP(ecap, u32Val) ((ecap)->CNTCMP = (u32Val))

void ECAP_Open(ECAP_T* ecap, uint32_t u32FuncMask);
void ECAP_Close(ECAP_T* ecap);
void ECAP_EnableINT(ECAP_T* ecap, uint32_t u32Mask);
void ECAP_DisableINT(ECAP_T* ecap, uint32_t u32Index);

/*@}*/ /* end of group ECAP_EXPORTED_FUNCTIONS */

/*@}*/ /* end of group ECAP_Driver */

/*@}*/ /* end of group Standard_Driver */

#ifdef __cplusplus
}
#endif

#endif //__ECAP_H__

/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/
