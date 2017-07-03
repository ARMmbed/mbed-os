/***************************************************************************//**
* \file cy_ble_clk.c
* \version 1.0
*
* \brief
*  Contains the source code for the BLE ECO clock.
*
********************************************************************************
* \copyright
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "cy_ble_clk.h"
#include "syslib/cy_syslib.h"
#include "sysclk/cy_sysclk.h"
#include "syspm/cy_syspm.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


static uint16_t Cy_BLE_HalRcbRegRead(uint16_t addr);
static void Cy_BLE_HalRcbRegWrite(uint16_t addr, uint16_t data);
static void Cy_BLE_HalEcoSetTrim(uint8_t trim);
static void Cy_BLE_HalMxdRadioEnableClocks(const cy_stc_ble_bless_eco_cfg_params_t *ecoConfig);


/*******************************************************************************
* Function Name: Cy_BLE_HAL_IsEcoCpuClockSrc
****************************************************************************//**
*
*  If ECO from BLESS in CLK_PATH_SELECT register is selected return 1.
*
*  \return
*  uint8_t: 0 - ECO from BLESS is not used in DWR
*           1 - ECO from BLESS is selected in CLK_PATH_SELECT register
*
*******************************************************************************/
uint8_t Cy_BLE_HAL_IsEcoCpuClockSrc(void)
{
    uint8_t returnVal = 0u;
    uint32_t i;

    for(i = 0u; i < SRSS_NUM_CLKPATH; i++)
    {
        if(Cy_SysClk_ClkPathGetSource(i) == CY_SYSCLK_CLKPATH_IN_ALTHF)
        {
            returnVal = 1u;
            break;
        }
    }

    return(returnVal);
}


/*******************************************************************************
* Function Name: Cy_BLE_HAL_IsWcoLfclkSrc
****************************************************************************//**
*
*  In CLK_SELECT register if WCO or PILO return 1
*
*  \return
*  uint8_t: 0 - WCO or PILO are not used in DWR
*           1 - WCO is selected in CLK_SELECT register
*
*******************************************************************************/
uint8_t Cy_BLE_HAL_IsWcoLfclkSrc(void)
{
    cy_en_lfclk_input_sources_t lfclkSrs;

    lfclkSrs = Cy_SysClk_LfClkGetSource();

    return(((lfclkSrs == CY_SYSCLK_LFCLK_IN_WCO) || (lfclkSrs == CY_SYSCLK_LFCLK_IN_PILO)) ? 1u : 0u);
}


/*******************************************************************************
* Function Name: Cy_BLE_HAL_Init
****************************************************************************//**
*
*  Initializes the BLESS port.
*
* \return
*  None
*
*******************************************************************************/
void Cy_BLE_HAL_Init(void)
{
    /* Configures pins for BLESS */
    const cy_stc_gpio_prt_config_t port2_cfg =
    {
        .cfg = CY_BLE_PORT2_CFG_VAL,
        .sel0Active = CY_BLE_PORT2_HSIOM_SEL0,
        .sel1Active = CY_BLE_PORT2_HSIOM_SEL1
    };
    const cy_stc_gpio_prt_config_t port3_cfg =
    {
        .cfg = CY_BLE_PORT3_CFG_VAL,
        .sel0Active = CY_BLE_PORT3_HSIOM_SEL0,
        .sel1Active = CY_BLE_PORT3_HSIOM_SEL1
    };
    const cy_stc_gpio_prt_config_t port4_cfg =
    {
        .cfg = CY_BLE_PORT4_CFG_VAL,
        .sel0Active = CY_BLE_PORT4_HSIOM_SEL0,
        .sel1Active = CY_BLE_PORT4_HSIOM_SEL1
    };

    Cy_GPIO_Port_Init(GPIO_PRT2, &port2_cfg);
    Cy_GPIO_Port_Init(GPIO_PRT3, &port3_cfg);
    Cy_GPIO_Port_Init(GPIO_PRT4, &port4_cfg);
}


/*******************************************************************************
* Function Name: Cy_BLE_HalRcbRegRead
****************************************************************************//**
*
*  Reads Radio register.
*
*  \param addr: Register address.
*
*  \return
*   Register data
*
*******************************************************************************/
static uint16_t Cy_BLE_HalRcbRegRead(uint16_t addr)
{
    uint32_t temp = (uint32_t)((0x8000UL | addr) << 16u);

    /* Write Address to the RCB */
    BLE->RCB.TX_FIFO_WR = temp;

    /* Wait for Data. */
    while((BLE->RCB.INTR & BLE_RCB_INTR_RCB_DONE_Msk) == 0u)
    {
    }

    BLE->RCB.INTR |= BLE_RCB_INTR_RCB_DONE_Msk;

    return((uint16_t)BLE->RCB.RX_FIFO_RD);
}


/*******************************************************************************
* Function Name: Cy_BLE_HalRcbRegWrite
****************************************************************************//**
*
*  Writes Radio register.
*
*  \param addr: Register address.
*  \param data: Register data.
*
*  \return
*  None
*
*******************************************************************************/
static void Cy_BLE_HalRcbRegWrite(uint16_t addr, uint16_t data)
{
    uint32_t temp = ((uint32_t)((0x7FFFUL & addr) << 16u)) | data;

    BLE->RCB.TX_FIFO_WR = temp;

    /* Wait for RCB done. */
    while((BLE->RCB.INTR & BLE_RCB_INTR_RCB_DONE_Msk) == 0u)
    {
    }

    BLE->RCB.INTR |= BLE_RCB_INTR_RCB_DONE_Msk;
}


/*******************************************************************************
* Function Name: Cy_BLE_HalEcoSetTrim
****************************************************************************//**
*
*  Sets ECO trim value.
*
*  \param trim: Trim value.
*
*  \return
*  None
*
*******************************************************************************/
static void Cy_BLE_HalEcoSetTrim(uint8_t trim)
{
    uint32_t reg;

    /* Read CY_BLE_RF_DCXO_CFG_REG */
    reg = Cy_BLE_HalRcbRegRead(CY_BLE_RF_DCXO_CFG_REG);
    /* Clear the CAP value */
    reg &= ~(CY_BLE_RF_DCXO_CFG_REG_DCXO_CAP_MASK << CY_BLE_RF_DCXO_CFG_REG_DCXO_CAP_SHIFT);
    /* Load the new CAP TRIM value */
    reg |= (uint32_t)trim << CY_BLE_RF_DCXO_CFG_REG_DCXO_CAP_SHIFT;
    /* Write the new value to the register */
    Cy_BLE_HalRcbRegWrite(CY_BLE_RF_DCXO_CFG_REG, CY_LO16(reg));
}


/*******************************************************************************
* Function Name: Cy_BLE_HalMxdRadioEnableClocks
****************************************************************************//**
*
*  Enables and configures radio clock.
*
*  \param ecoConfig: Clock configuration of type 'cy_stc_ble_bless_eco_cfg_params_t'.
*
*  \return
*  None
*
*******************************************************************************/
static void Cy_BLE_HalMxdRadioEnableClocks(const cy_stc_ble_bless_eco_cfg_params_t *ecoConfig)
{
    uint16_t temp;
    uint16_t blerdDivider;
    uint16_t ecoSysDivider;
    uint16_t blellDivider = 0u;

    /* Deassert active domain reset and enable clock buffer in MXD Radio */
    do
    {
        Cy_BLE_HalRcbRegWrite(CY_BLE_PMU_MODE_TRANSITION_REG, CY_BLE_MXD_RADIO_CLK_BUF_EN_VAL);
        temp = Cy_BLE_HalRcbRegRead(CY_BLE_PMU_MODE_TRANSITION_REG);
    } while(temp != CY_BLE_MXD_RADIO_CLK_BUF_EN_VAL);

    /* Check if ECO clock output is enabled
     * In MXD_VER2, ECO clock is stopped on overwriting this bit */
    temp = Cy_BLE_HalRcbRegRead(CY_BLE_PMU_PMU_CTRL_REG);
    if ((temp & CY_BLE_MXD_RADIO_DIG_CLK_OUT_EN_VAL) == 0u)
    {
        /* Enable digital ECO clock output from MXD Radio to BLESS */
        do
        {
            Cy_BLE_HalRcbRegWrite(CY_BLE_PMU_PMU_CTRL_REG, CY_BLE_MXD_RADIO_DIG_CLK_OUT_EN_VAL);
            temp = Cy_BLE_HalRcbRegRead(CY_BLE_PMU_PMU_CTRL_REG);
        } while(temp != CY_BLE_MXD_RADIO_DIG_CLK_OUT_EN_VAL);
    }

    /* Configure ECO clock frequency and clock divider in MXD Radio */

    /* Read the MXD Radio register */
    temp = Cy_BLE_HalRcbRegRead(CY_BLE_RF_DCXO_BUF_CFG_REG);
    /* Clear clock divider and select amp buffer output bits */
    temp &= CY_LO16(~((CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_MASK << CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_SHIFT) |
                      (CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_MASK << CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_SHIFT)));

    /* Total ECO divider consist of divider located on BLERD and BLESS divider
       Set BLERD divider to maximum value taking in to account that 8 Mhz is required for BLELL
       BLELL clock frequency is set to 8Mhz irrespective of the crystal value.
    */
    if(ecoConfig->ecoFreq == CY_BLE_BLESS_ECO_FREQ_32MHZ)
    {
        if(ecoConfig->ecoSysDiv >= (uint32_t)CY_BLE_BLESS_XTAL_CLK_DIV_4)
        {
            blerdDivider = (uint16_t)CY_BLE_MXD_RADIO_CLK_DIV_4;
        }
        else
        {
            blerdDivider = (uint16_t)ecoConfig->ecoSysDiv;
            blellDivider = (uint16_t)CY_BLE_MXD_RADIO_CLK_DIV_4 - blerdDivider;
        }
        temp |= CY_BLE_RF_DCXO_BUF_CFG_REG_XTAL_32M_SEL_BIT;
        temp |= (uint16_t)(CY_BLE_MXD_RADIO_CLK_BUF_AMP_32M_LARGE << CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_SHIFT);

        /* Update cy_BleEcoClockFreqHz for the proper Cy_SysLib_Delay functionality */
        cy_BleEcoClockFreqHz = CY_BLE_DEFAULT_ECO_CLK_FREQ_32MHZ / (1UL << (uint16_t)ecoConfig->ecoSysDiv);
    }
    else
    {
        if(ecoConfig->ecoSysDiv >= (uint32_t)CY_BLE_BLESS_XTAL_CLK_DIV_2)
        {
            blerdDivider = (uint16_t)CY_BLE_MXD_RADIO_CLK_DIV_2;
        }
        else
        {
            blerdDivider = (uint16_t)ecoConfig->ecoSysDiv;
            blellDivider = (uint16_t)CY_BLE_MXD_RADIO_CLK_DIV_2 - blerdDivider;
        }
        temp &= (uint16_t)~CY_BLE_RF_DCXO_BUF_CFG_REG_XTAL_32M_SEL_BIT;
        temp |= (uint16_t)(CY_BLE_MXD_RADIO_CLK_BUF_AMP_16M_LARGE << CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_SHIFT);

        /* Update cy_BleEcoClockFreqHz for the proper Cy_SysLib_Delay functionality */
        cy_BleEcoClockFreqHz = CY_BLE_DEFAULT_ECO_CLK_FREQ_16MHZ / (1UL << (uint16_t)ecoConfig->ecoSysDiv);
    }

    temp |= (uint16_t)(blerdDivider << CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_SHIFT);
    /* Write the MXD Radio register */
    Cy_BLE_HalRcbRegWrite(CY_BLE_RF_DCXO_BUF_CFG_REG, temp);

    /* Reduce BLESS divider by BLERD divider value */
    ecoSysDivider = (uint16_t)ecoConfig->ecoSysDiv - blerdDivider;
    temp = (uint16_t)(ecoSysDivider & BLE_BLESS_XTAL_CLK_DIV_CONFIG_SYSCLK_DIV_Msk);
    temp |= (uint16_t)(blellDivider << BLE_BLESS_XTAL_CLK_DIV_CONFIG_LLCLK_DIV_Pos);
    /* Set clock divider */
    BLE->BLESS.XTAL_CLK_DIV_CONFIG = temp;

}


/*******************************************************************************
* Function Name: Cy_BLE_EcoStop
****************************************************************************//**
*
*  Disables BLE ECO clock.
*
* \return
*  None
*
*******************************************************************************/
void Cy_BLE_EcoStop(void)
{
    /* Initiate Soft Reset */
    BLE->BLESS.LL_CLK_EN |= BLE_BLESS_LL_CLK_EN_BLESS_RESET_Msk;
}


/*******************************************************************************
* Function Name: Cy_BLE_EcoStart
****************************************************************************//**
*
*  Configures and starts BLE ECO clock.
*  By default, BLE stack starts BLE ECO clock with default parameters:
*   CY_BLE_DEFAULT_OSC_STARTUP_DELAY_LF
*   CY_BLE_DEFAULT_CAP_TRIM_VALUE
*   CY_BLE_DEFAULT_ECO_FREQ
*   CY_BLE_DEFAULT_ECO_DIV
*  In case BLE ECO is used with different parameters, enable it in DWR or call
*  this function with custom configuration.
*
*  This clock is stopped in the deep sleep and hibernate power modes.
*
*  \param ecoConfig: Clock configuration of type 'cy_stc_ble_bless_eco_cfg_params_t'.
*
*  \return
*   CY_BLE_ECO_SUCCESS - On successful operation.
*   CY_BLE_ECO_BAD_PARAM - Wrong input parameter.
*   CY_BLE_ECO_RCB_CONTROL_LL - If the RCB Control is with BLE Link Layer.
*   CY_BLE_ECO_ALREADY_STARTED - If BLE ECO clock is aready started
*
*******************************************************************************/
cy_en_ble_eco_status_t Cy_BLE_EcoStart(const cy_stc_ble_bless_eco_cfg_params_t *ecoConfig)
{
    cy_en_ble_eco_status_t status = CY_BLE_ECO_SUCCESS;
    uint32_t temp;

    if(ecoConfig == NULL)
    {
        status = CY_BLE_ECO_BAD_PARAM;
    }
    else
    {
        if((BLE->BLESS.MT_CFG & BLE_BLESS_MT_CFG_ENABLE_BLERD_Msk) != 0u)
        {
            status = CY_BLE_ECO_ALREADY_STARTED;
        }
        else
        {
            /* HAL Initialization */
            Cy_BLE_HAL_Init();

            /* BLESS MT Delays configuration */
            BLE->BLESS.MT_DELAY_CFG =
                (uint32_t)(CY_BLE_DEFAULT_HVLDO_STARTUP_DELAY << BLE_BLESS_MT_DELAY_CFG_HVLDO_STARTUP_DELAY_Pos) |
                (uint32_t)(CY_BLE_DEFAULT_ISOLATE_DEASSERT_DELAY << BLE_BLESS_MT_DELAY_CFG_ISOLATE_DEASSERT_DELAY_Pos) |
                (uint32_t)(CY_BLE_DEFAULT_ACT_TO_SWITCH_DELAY << BLE_BLESS_MT_DELAY_CFG_ACT_TO_SWITCH_DELAY_Pos) |
                (uint32_t)(CY_BLE_DEFAULT_HVLDO_DISABLE_DELAY << BLE_BLESS_MT_DELAY_CFG_HVLDO_DISABLE_DELAY_Pos);

            BLE->BLESS.MT_DELAY_CFG2 =
                (uint32_t)((uint32_t)ecoConfig->ecoXtalStartUpTime << BLE_BLESS_MT_DELAY_CFG2_OSC_STARTUP_DELAY_LF_Pos) |
                (uint32_t)(CY_BLE_DEFAULT_ACT_STARTUP_DELAY << BLE_BLESS_MT_DELAY_CFG2_ACT_STARTUP_DELAY_Pos) |
                (uint32_t)(CY_BLE_DEFAULT_DIG_LDO_STARTUP_DELAY << BLE_BLESS_MT_DELAY_CFG2_DIG_LDO_STARTUP_DELAY_Pos);

            BLE->BLESS.MT_DELAY_CFG3 =
                (uint32_t)(CY_BLE_DEFAULT_XTAL_DISABLE_DELAY << BLE_BLESS_MT_DELAY_CFG3_XTAL_DISABLE_DELAY_Pos) |
                (uint32_t)(CY_BLE_DEFAULT_DIG_LDO_DISABLE_DELAY << BLE_BLESS_MT_DELAY_CFG3_DIG_LDO_DISABLE_DELAY_Pos) |
                (uint32_t)(CY_BLE_DEFAULT_VDDR_STABLE_DELAY << BLE_BLESS_MT_DELAY_CFG3_VDDR_STABLE_DELAY_Pos);

            /* RCB Initialization */
            /* Check if the RCB Control is with LL. */
            if((BLE->RCB.RCBLL.CTRL & BLE_RCB_RCBLL_CTRL_RCBLL_CTRL_Msk) != 0u)
            {
                status = CY_BLE_ECO_RCB_CONTROL_LL;
            }
            else
            {
                uint32_t rcbDivider = 0u;

                /* If clock source for RCB is PeriClk */
                if((BLE_BLESS->LL_CLK_EN & BLE_BLESS_LL_CLK_EN_SEL_RCB_CLK_Msk) == 0u)
                {
                    if(cy_PeriClkFreqHz > CY_BLE_DEFAULT_RCB_CTRL_FREQ)
                    {
                        rcbDivider = (cy_PeriClkFreqHz / CY_BLE_DEFAULT_RCB_CTRL_FREQ) - 1u;
                    }
                }
                else
                {
                    rcbDivider = CY_BLE_DEFAULT_RCB_CTRL_DIV;
                }

                /* Configure default RCB Parameters. */
                BLE->RCB.CTRL |=
                    (BLE_RCB_CTRL_ENABLED_Msk |
                     BLE_RCB_CTRL_RX_CLK_EDGE_Msk |
                    (CY_BLE_DEFAULT_RCB_CTRL_LEAD << BLE_RCB_CTRL_LEAD_Pos) |
                    (CY_BLE_DEFAULT_RCB_CTRL_LAG << BLE_RCB_CTRL_LAG_Pos) |
                    ((rcbDivider > 0u) ?
                        ((rcbDivider << BLE_RCB_CTRL_DIV_Pos) |
                         BLE_RCB_CTRL_DIV_ENABLED_Msk)
                        : 0u)
                    );

                /* Configure LDO if Buck2 is not already enabled */
                if(!Cy_SysPm_SimoBuckOutputIsEnabled(CY_SYSPM_BUCK_VRF))
                {
                    BLE->BLESS.MISC_EN_CTRL |= (BLE_BLESS_MISC_EN_CTRL_ACT_REG_EN_CTRL_Msk |
                                                BLE_BLESS_MISC_EN_CTRL_BUCK_EN_CTRL_Msk);
                }

                /* Enable the VIO supply and LDO in standby mode for slow ramp */
                temp = BLE->BLESS.MT_CFG;
                temp |= BLE_BLESS_MT_CFG_HVLDO_BYPASS_Msk |
                        BLE_BLESS_MT_CFG_OVERRIDE_HVLDO_BYPASS_Msk |
                        BLE_BLESS_MT_CFG_HVLDO_EN_Msk |
                        BLE_BLESS_MT_CFG_OVERRIDE_HVLDO_EN_Msk;
                BLE->BLESS.MT_CFG = temp;
                Cy_SysLib_DelayUs(64u);

                if ((BLE->BLESS.MT_VIO_CTRL & BLE_BLESS_MT_VIO_CTRL_SRSS_SWITCH_EN_Msk) == 0u)
                {
                    /* Enable LDO */
                    BLE->BLESS.MT_VIO_CTRL = BLE_BLESS_MT_VIO_CTRL_SRSS_SWITCH_EN_Msk;
                    /* Wait for 64us after turning HVLDO ON */
                    Cy_SysLib_DelayUs(64u);
                    /* Enable LDO Delayed */
                    BLE->BLESS.MT_VIO_CTRL = BLE_BLESS_MT_VIO_CTRL_SRSS_SWITCH_EN_Msk |
                                             BLE_BLESS_MT_VIO_CTRL_SRSS_SWITCH_EN_DLY_Msk;
                    /* Wait for 64us */
                    Cy_SysLib_DelayUs(64u);
                }

                /* Disable override mode and let hardware take control of HVLDO */
                temp &= ~(BLE_BLESS_MT_CFG_OVERRIDE_HVLDO_BYPASS_Msk |
                          BLE_BLESS_MT_CFG_OVERRIDE_HVLDO_EN_Msk);
                BLE->BLESS.MT_CFG = temp;

                /* Wait for the VIO stable key write operation to complete */
                while ((GPIO->VDD_ACTIVE & 0x10u) == 0u)
                {
                }

                /* Clear the BLERD_ACTIVE_INTR */
                BLE->BLESS.INTR_STAT |= BLE_BLESS_INTR_STAT_BLERD_ACTIVE_INTR_Msk;

                if(!Cy_SysPm_SimoBuckOutputIsEnabled(CY_SYSPM_BUCK_VRF))
                {
                    temp |= BLE_BLESS_MT_CFG_ACT_LDO_NOT_BUCK_Msk;
                }
                /* Enable Radio */
                temp |= BLE_BLESS_MT_CFG_ENABLE_BLERD_Msk;
                /* Disable ECO in DeepSleep mode */
                temp &= ~BLE_BLESS_MT_CFG_DPSLP_ECO_ON_Msk;
                BLE->BLESS.MT_CFG = temp;

                while((BLE->BLESS.MT_STATUS & BLE_BLESS_MT_STATUS_BLESS_STATE_Msk) == 0u)
                {
                }

                /* Enable and configure radio clock */
                Cy_BLE_HalMxdRadioEnableClocks(ecoConfig);

                /* Set Load capacitance */
                Cy_BLE_HalEcoSetTrim(ecoConfig->loadCap);
            }
        }
    }
    return(status);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */


/* [] END OF FILE */
