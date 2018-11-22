/*
** ###################################################################
**     Version:             rev. 1.2, 2017-06-08
**     Build:               b170609
**
**     Abstract:
**         Chip specific module features.
**
**     Copyright 2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     1. Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     2. Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     3. Neither the name of the copyright holder nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2016-08-12)
**         Initial version.
**     - rev. 1.1 (2016-11-25)
**         Update CANFD and Classic CAN register.
**         Add MAC TIMERSTAMP registers.
**     - rev. 1.2 (2017-06-08)
**         Remove RTC_CTRL_RTC_OSC_BYPASS.
**         SYSCON_ARMTRCLKDIV rename to SYSCON_ARMTRACECLKDIV.
**         Remove RESET and HALT from SYSCON_AHBCLKDIV.
**
** ###################################################################
*/

#ifndef _LPC54628_FEATURES_H_
#define _LPC54628_FEATURES_H_

/* SOC module features */

/* @brief ACMP availability on the SoC. */
#define FSL_FEATURE_SOC_ACMP_COUNT (0)
/* @brief ADC availability on the SoC. */
#define FSL_FEATURE_SOC_ADC_COUNT (1)
/* @brief ADC12 availability on the SoC. */
#define FSL_FEATURE_SOC_ADC12_COUNT (0)
/* @brief ADC16 availability on the SoC. */
#define FSL_FEATURE_SOC_ADC16_COUNT (0)
/* @brief ADC_5HC availability on the SoC. */
#define FSL_FEATURE_SOC_ADC_5HC_COUNT (0)
/* @brief AES availability on the SoC. */
#define FSL_FEATURE_SOC_AES_COUNT (0)
/* @brief AFE availability on the SoC. */
#define FSL_FEATURE_SOC_AFE_COUNT (0)
/* @brief AGC availability on the SoC. */
#define FSL_FEATURE_SOC_AGC_COUNT (0)
/* @brief AIPS availability on the SoC. */
#define FSL_FEATURE_SOC_AIPS_COUNT (0)
/* @brief AIPSTZ availability on the SoC. */
#define FSL_FEATURE_SOC_AIPSTZ_COUNT (0)
/* @brief ANATOP availability on the SoC. */
#define FSL_FEATURE_SOC_ANATOP_COUNT (0)
/* @brief AOI availability on the SoC. */
#define FSL_FEATURE_SOC_AOI_COUNT (0)
/* @brief APBH availability on the SoC. */
#define FSL_FEATURE_SOC_APBH_COUNT (0)
/* @brief ASMC availability on the SoC. */
#define FSL_FEATURE_SOC_ASMC_COUNT (0)
/* @brief ASRC availability on the SoC. */
#define FSL_FEATURE_SOC_ASRC_COUNT (0)
/* @brief ASYNC_SYSCON availability on the SoC. */
#define FSL_FEATURE_SOC_ASYNC_SYSCON_COUNT (1)
/* @brief ATX availability on the SoC. */
#define FSL_FEATURE_SOC_ATX_COUNT (0)
/* @brief AXBS availability on the SoC. */
#define FSL_FEATURE_SOC_AXBS_COUNT (0)
/* @brief BCH availability on the SoC. */
#define FSL_FEATURE_SOC_BCH_COUNT (0)
/* @brief BLEDP availability on the SoC. */
#define FSL_FEATURE_SOC_BLEDP_COUNT (0)
/* @brief BOD availability on the SoC. */
#define FSL_FEATURE_SOC_BOD_COUNT (0)
/* @brief CAAM availability on the SoC. */
#define FSL_FEATURE_SOC_CAAM_COUNT (0)
/* @brief CADC availability on the SoC. */
#define FSL_FEATURE_SOC_CADC_COUNT (0)
/* @brief CALIB availability on the SoC. */
#define FSL_FEATURE_SOC_CALIB_COUNT (0)
/* @brief CAN availability on the SoC. */
#define FSL_FEATURE_SOC_LPC_CAN_COUNT (2)
/* @brief CAU availability on the SoC. */
#define FSL_FEATURE_SOC_CAU_COUNT (0)
/* @brief CAU3 availability on the SoC. */
#define FSL_FEATURE_SOC_CAU3_COUNT (0)
/* @brief CCM availability on the SoC. */
#define FSL_FEATURE_SOC_CCM_COUNT (0)
/* @brief CCM_ANALOG availability on the SoC. */
#define FSL_FEATURE_SOC_CCM_ANALOG_COUNT (0)
/* @brief CHRG availability on the SoC. */
#define FSL_FEATURE_SOC_CHRG_COUNT (0)
/* @brief CMP availability on the SoC. */
#define FSL_FEATURE_SOC_CMP_COUNT (0)
/* @brief CMT availability on the SoC. */
#define FSL_FEATURE_SOC_CMT_COUNT (0)
/* @brief CNC availability on the SoC. */
#define FSL_FEATURE_SOC_CNC_COUNT (0)
/* @brief COP availability on the SoC. */
#define FSL_FEATURE_SOC_COP_COUNT (0)
/* @brief CRC availability on the SoC. */
#define FSL_FEATURE_SOC_CRC_COUNT (1)
/* @brief CS availability on the SoC. */
#define FSL_FEATURE_SOC_CS_COUNT (0)
/* @brief CSI availability on the SoC. */
#define FSL_FEATURE_SOC_CSI_COUNT (0)
/* @brief CT32B availability on the SoC. */
#define FSL_FEATURE_SOC_CT32B_COUNT (0)
/* @brief CTI availability on the SoC. */
#define FSL_FEATURE_SOC_CTI_COUNT (0)
/* @brief CTIMER availability on the SoC. */
#define FSL_FEATURE_SOC_CTIMER_COUNT (5)
/* @brief DAC availability on the SoC. */
#define FSL_FEATURE_SOC_DAC_COUNT (0)
/* @brief DAC32 availability on the SoC. */
#define FSL_FEATURE_SOC_DAC32_COUNT (0)
/* @brief DCDC availability on the SoC. */
#define FSL_FEATURE_SOC_DCDC_COUNT (0)
/* @brief DCP availability on the SoC. */
#define FSL_FEATURE_SOC_DCP_COUNT (0)
/* @brief DDR availability on the SoC. */
#define FSL_FEATURE_SOC_DDR_COUNT (0)
/* @brief DDRC availability on the SoC. */
#define FSL_FEATURE_SOC_DDRC_COUNT (0)
/* @brief DDRC_MP availability on the SoC. */
#define FSL_FEATURE_SOC_DDRC_MP_COUNT (0)
/* @brief DDR_PHY availability on the SoC. */
#define FSL_FEATURE_SOC_DDR_PHY_COUNT (0)
/* @brief DMA availability on the SoC. */
#define FSL_FEATURE_SOC_DMA_COUNT (1)
/* @brief DMAMUX availability on the SoC. */
#define FSL_FEATURE_SOC_DMAMUX_COUNT (0)
/* @brief DMIC availability on the SoC. */
#define FSL_FEATURE_SOC_DMIC_COUNT (1)
/* @brief DRY availability on the SoC. */
#define FSL_FEATURE_SOC_DRY_COUNT (0)
/* @brief DSPI availability on the SoC. */
#define FSL_FEATURE_SOC_DSPI_COUNT (0)
/* @brief ECSPI availability on the SoC. */
#define FSL_FEATURE_SOC_ECSPI_COUNT (0)
/* @brief EDMA availability on the SoC. */
#define FSL_FEATURE_SOC_EDMA_COUNT (0)
/* @brief EEPROM availability on the SoC. */
#define FSL_FEATURE_SOC_EEPROM_COUNT (1)
/* @brief EIM availability on the SoC. */
#define FSL_FEATURE_SOC_EIM_COUNT (0)
/* @brief EMC availability on the SoC. */
#define FSL_FEATURE_SOC_EMC_COUNT (1)
/* @brief EMVSIM availability on the SoC. */
#define FSL_FEATURE_SOC_EMVSIM_COUNT (0)
/* @brief ENC availability on the SoC. */
#define FSL_FEATURE_SOC_ENC_COUNT (0)
/* @brief ENET availability on the SoC. */
#define FSL_FEATURE_SOC_LPC_ENET_COUNT (1)
/* @brief EPDC availability on the SoC. */
#define FSL_FEATURE_SOC_EPDC_COUNT (0)
/* @brief EPIT availability on the SoC. */
#define FSL_FEATURE_SOC_EPIT_COUNT (0)
/* @brief ESAI availability on the SoC. */
#define FSL_FEATURE_SOC_ESAI_COUNT (0)
/* @brief EWM availability on the SoC. */
#define FSL_FEATURE_SOC_EWM_COUNT (0)
/* @brief FB availability on the SoC. */
#define FSL_FEATURE_SOC_FB_COUNT (0)
/* @brief FGPIO availability on the SoC. */
#define FSL_FEATURE_SOC_FGPIO_COUNT (0)
/* @brief FLASH availability on the SoC. */
#define FSL_FEATURE_SOC_FLASH_COUNT (0)
/* @brief FLEXCAN availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXCAN_COUNT (0)
/* @brief FLEXCOMM availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXCOMM_COUNT (10)
/* @brief FLEXIO availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXIO_COUNT (0)
/* @brief FLEXRAM availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXRAM_COUNT (0)
/* @brief FLEXSPI availability on the SoC. */
#define FSL_FEATURE_SOC_FLEXSPI_COUNT (0)
/* @brief FMC availability on the SoC. */
#define FSL_FEATURE_SOC_FMC_COUNT (1)
/* @brief FSKDT availability on the SoC. */
#define FSL_FEATURE_SOC_FSKDT_COUNT (0)
/* @brief FSP availability on the SoC. */
#define FSL_FEATURE_SOC_FSP_COUNT (0)
/* @brief FTFA availability on the SoC. */
#define FSL_FEATURE_SOC_FTFA_COUNT (0)
/* @brief FTFE availability on the SoC. */
#define FSL_FEATURE_SOC_FTFE_COUNT (0)
/* @brief FTFL availability on the SoC. */
#define FSL_FEATURE_SOC_FTFL_COUNT (0)
/* @brief FTM availability on the SoC. */
#define FSL_FEATURE_SOC_FTM_COUNT (0)
/* @brief FTMRA availability on the SoC. */
#define FSL_FEATURE_SOC_FTMRA_COUNT (0)
/* @brief FTMRE availability on the SoC. */
#define FSL_FEATURE_SOC_FTMRE_COUNT (0)
/* @brief FTMRH availability on the SoC. */
#define FSL_FEATURE_SOC_FTMRH_COUNT (0)
/* @brief GINT availability on the SoC. */
#define FSL_FEATURE_SOC_GINT_COUNT (2)
/* @brief GPC availability on the SoC. */
#define FSL_FEATURE_SOC_GPC_COUNT (0)
/* @brief GPC_PGC availability on the SoC. */
#define FSL_FEATURE_SOC_GPC_PGC_COUNT (0)
/* @brief GPIO availability on the SoC. */
#define FSL_FEATURE_SOC_GPIO_COUNT (1)
/* @brief GPMI availability on the SoC. */
#define FSL_FEATURE_SOC_GPMI_COUNT (0)
/* @brief GPT availability on the SoC. */
#define FSL_FEATURE_SOC_GPT_COUNT (0)
/* @brief HSADC availability on the SoC. */
#define FSL_FEATURE_SOC_HSADC_COUNT (0)
/* @brief I2C availability on the SoC. */
#define FSL_FEATURE_SOC_I2C_COUNT (10)
/* @brief I2S availability on the SoC. */
#define FSL_FEATURE_SOC_I2S_COUNT (2)
/* @brief ICS availability on the SoC. */
#define FSL_FEATURE_SOC_ICS_COUNT (0)
/* @brief IEE availability on the SoC. */
#define FSL_FEATURE_SOC_IEE_COUNT (0)
/* @brief IEER availability on the SoC. */
#define FSL_FEATURE_SOC_IEER_COUNT (0)
/* @brief IGPIO availability on the SoC. */
#define FSL_FEATURE_SOC_IGPIO_COUNT (0)
/* @brief II2C availability on the SoC. */
#define FSL_FEATURE_SOC_II2C_COUNT (0)
/* @brief INPUTMUX availability on the SoC. */
#define FSL_FEATURE_SOC_INPUTMUX_COUNT (1)
/* @brief INTMUX availability on the SoC. */
#define FSL_FEATURE_SOC_INTMUX_COUNT (0)
/* @brief IOCON availability on the SoC. */
#define FSL_FEATURE_SOC_IOCON_COUNT (1)
/* @brief IOMUXC availability on the SoC. */
#define FSL_FEATURE_SOC_IOMUXC_COUNT (0)
/* @brief IOMUXC_GPR availability on the SoC. */
#define FSL_FEATURE_SOC_IOMUXC_GPR_COUNT (0)
/* @brief IOMUXC_LPSR availability on the SoC. */
#define FSL_FEATURE_SOC_IOMUXC_LPSR_COUNT (0)
/* @brief IOMUXC_LPSR_GPR availability on the SoC. */
#define FSL_FEATURE_SOC_IOMUXC_LPSR_GPR_COUNT (0)
/* @brief IOMUXC_SNVS availability on the SoC. */
#define FSL_FEATURE_SOC_IOMUXC_SNVS_COUNT (0)
/* @brief IPWM availability on the SoC. */
#define FSL_FEATURE_SOC_IPWM_COUNT (0)
/* @brief IRQ availability on the SoC. */
#define FSL_FEATURE_SOC_IRQ_COUNT (0)
/* @brief IUART availability on the SoC. */
#define FSL_FEATURE_SOC_IUART_COUNT (0)
/* @brief KBI availability on the SoC. */
#define FSL_FEATURE_SOC_KBI_COUNT (0)
/* @brief KPP availability on the SoC. */
#define FSL_FEATURE_SOC_KPP_COUNT (0)
/* @brief L2CACHEC availability on the SoC. */
#define FSL_FEATURE_SOC_L2CACHEC_COUNT (0)
/* @brief LCD availability on the SoC. */
#define FSL_FEATURE_SOC_LCD_COUNT (1)
/* @brief LCDC availability on the SoC. */
#define FSL_FEATURE_SOC_LCDC_COUNT (0)
/* @brief LCDIF availability on the SoC. */
#define FSL_FEATURE_SOC_LCDIF_COUNT (0)
/* @brief LDO availability on the SoC. */
#define FSL_FEATURE_SOC_LDO_COUNT (0)
/* @brief LLWU availability on the SoC. */
#define FSL_FEATURE_SOC_LLWU_COUNT (0)
/* @brief LMEM availability on the SoC. */
#define FSL_FEATURE_SOC_LMEM_COUNT (0)
/* @brief LPADC availability on the SoC. */
#define FSL_FEATURE_SOC_LPADC_COUNT (0)
/* @brief LPCMP availability on the SoC. */
#define FSL_FEATURE_SOC_LPCMP_COUNT (0)
/* @brief LPDAC availability on the SoC. */
#define FSL_FEATURE_SOC_LPDAC_COUNT (0)
/* @brief LPI2C availability on the SoC. */
#define FSL_FEATURE_SOC_LPI2C_COUNT (0)
/* @brief LPIT availability on the SoC. */
#define FSL_FEATURE_SOC_LPIT_COUNT (0)
/* @brief LPSCI availability on the SoC. */
#define FSL_FEATURE_SOC_LPSCI_COUNT (0)
/* @brief LPSPI availability on the SoC. */
#define FSL_FEATURE_SOC_LPSPI_COUNT (0)
/* @brief LPTMR availability on the SoC. */
#define FSL_FEATURE_SOC_LPTMR_COUNT (0)
/* @brief LPTPM availability on the SoC. */
#define FSL_FEATURE_SOC_LPTPM_COUNT (0)
/* @brief LPUART availability on the SoC. */
#define FSL_FEATURE_SOC_LPUART_COUNT (0)
/* @brief LTC availability on the SoC. */
#define FSL_FEATURE_SOC_LTC_COUNT (0)
/* @brief MAILBOX availability on the SoC. */
#define FSL_FEATURE_SOC_MAILBOX_COUNT (0)
/* @brief MC availability on the SoC. */
#define FSL_FEATURE_SOC_MC_COUNT (0)
/* @brief MCG availability on the SoC. */
#define FSL_FEATURE_SOC_MCG_COUNT (0)
/* @brief MCGLITE availability on the SoC. */
#define FSL_FEATURE_SOC_MCGLITE_COUNT (0)
/* @brief MCM availability on the SoC. */
#define FSL_FEATURE_SOC_MCM_COUNT (0)
/* @brief MIPI_CSI2 availability on the SoC. */
#define FSL_FEATURE_SOC_MIPI_CSI2_COUNT (0)
/* @brief MIPI_DSI availability on the SoC. */
#define FSL_FEATURE_SOC_MIPI_DSI_COUNT (0)
/* @brief MIPI_DSI_HOST availability on the SoC. */
#define FSL_FEATURE_SOC_MIPI_DSI_HOST_COUNT (0)
/* @brief MMAU availability on the SoC. */
#define FSL_FEATURE_SOC_MMAU_COUNT (0)
/* @brief MMCAU availability on the SoC. */
#define FSL_FEATURE_SOC_MMCAU_COUNT (0)
/* @brief MMDC availability on the SoC. */
#define FSL_FEATURE_SOC_MMDC_COUNT (0)
/* @brief MMDVSQ availability on the SoC. */
#define FSL_FEATURE_SOC_MMDVSQ_COUNT (0)
/* @brief MPU availability on the SoC. */
#define FSL_FEATURE_SOC_MPU_COUNT (0)
/* @brief MRT availability on the SoC. */
#define FSL_FEATURE_SOC_MRT_COUNT (1)
/* @brief MSCAN availability on the SoC. */
#define FSL_FEATURE_SOC_MSCAN_COUNT (0)
/* @brief MSCM availability on the SoC. */
#define FSL_FEATURE_SOC_MSCM_COUNT (0)
/* @brief MTB availability on the SoC. */
#define FSL_FEATURE_SOC_MTB_COUNT (0)
/* @brief MTBDWT availability on the SoC. */
#define FSL_FEATURE_SOC_MTBDWT_COUNT (0)
/* @brief MU availability on the SoC. */
#define FSL_FEATURE_SOC_MU_COUNT (0)
/* @brief NFC availability on the SoC. */
#define FSL_FEATURE_SOC_NFC_COUNT (0)
/* @brief OCOTP availability on the SoC. */
#define FSL_FEATURE_SOC_OCOTP_COUNT (0)
/* @brief OPAMP availability on the SoC. */
#define FSL_FEATURE_SOC_OPAMP_COUNT (0)
/* @brief OSC availability on the SoC. */
#define FSL_FEATURE_SOC_OSC_COUNT (0)
/* @brief OSC32 availability on the SoC. */
#define FSL_FEATURE_SOC_OSC32_COUNT (0)
/* @brief OTFAD availability on the SoC. */
#define FSL_FEATURE_SOC_OTFAD_COUNT (0)
/* @brief PCC availability on the SoC. */
#define FSL_FEATURE_SOC_PCC_COUNT (0)
/* @brief PCIE_PHY_CMN availability on the SoC. */
#define FSL_FEATURE_SOC_PCIE_PHY_CMN_COUNT (0)
/* @brief PCIE_PHY_TRSV availability on the SoC. */
#define FSL_FEATURE_SOC_PCIE_PHY_TRSV_COUNT (0)
/* @brief PDB availability on the SoC. */
#define FSL_FEATURE_SOC_PDB_COUNT (0)
/* @brief PGA availability on the SoC. */
#define FSL_FEATURE_SOC_PGA_COUNT (0)
/* @brief PINT availability on the SoC. */
#define FSL_FEATURE_SOC_PINT_COUNT (1)
/* @brief PIT availability on the SoC. */
#define FSL_FEATURE_SOC_PIT_COUNT (0)
/* @brief PMC availability on the SoC. */
#define FSL_FEATURE_SOC_PMC_COUNT (0)
/* @brief PMU availability on the SoC. */
#define FSL_FEATURE_SOC_PMU_COUNT (0)
/* @brief PORT availability on the SoC. */
#define FSL_FEATURE_SOC_PORT_COUNT (0)
/* @brief PROP availability on the SoC. */
#define FSL_FEATURE_SOC_PROP_COUNT (0)
/* @brief PWM availability on the SoC. */
#define FSL_FEATURE_SOC_PWM_COUNT (0)
/* @brief PWT availability on the SoC. */
#define FSL_FEATURE_SOC_PWT_COUNT (0)
/* @brief PXP availability on the SoC. */
#define FSL_FEATURE_SOC_PXP_COUNT (0)
/* @brief QDEC availability on the SoC. */
#define FSL_FEATURE_SOC_QDEC_COUNT (0)
/* @brief QuadSPI availability on the SoC. */
#define FSL_FEATURE_SOC_QuadSPI_COUNT (0)
/* @brief RCM availability on the SoC. */
#define FSL_FEATURE_SOC_RCM_COUNT (0)
/* @brief RDC availability on the SoC. */
#define FSL_FEATURE_SOC_RDC_COUNT (0)
/* @brief RDC_SEMAPHORE availability on the SoC. */
#define FSL_FEATURE_SOC_RDC_SEMAPHORE_COUNT (0)
/* @brief RFSYS availability on the SoC. */
#define FSL_FEATURE_SOC_RFSYS_COUNT (0)
/* @brief RFVBAT availability on the SoC. */
#define FSL_FEATURE_SOC_RFVBAT_COUNT (0)
/* @brief RIT availability on the SoC. */
#define FSL_FEATURE_SOC_RIT_COUNT (1)
/* @brief RNG availability on the SoC. */
#define FSL_FEATURE_SOC_LPC_RNG_COUNT (1)
/* @brief RNGB availability on the SoC. */
#define FSL_FEATURE_SOC_RNGB_COUNT (0)
/* @brief ROM availability on the SoC. */
#define FSL_FEATURE_SOC_ROM_COUNT (0)
/* @brief ROMC availability on the SoC. */
#define FSL_FEATURE_SOC_ROMC_COUNT (0)
/* @brief RSIM availability on the SoC. */
#define FSL_FEATURE_SOC_RSIM_COUNT (0)
/* @brief RTC availability on the SoC. */
#define FSL_FEATURE_SOC_RTC_COUNT (1)
/* @brief SCG availability on the SoC. */
#define FSL_FEATURE_SOC_SCG_COUNT (0)
/* @brief SCI availability on the SoC. */
#define FSL_FEATURE_SOC_SCI_COUNT (0)
/* @brief SCT availability on the SoC. */
#define FSL_FEATURE_SOC_SCT_COUNT (1)
/* @brief SDHC availability on the SoC. */
#define FSL_FEATURE_SOC_SDHC_COUNT (0)
/* @brief SDIF availability on the SoC. */
#define FSL_FEATURE_SOC_SDIF_COUNT (1)
/* @brief SDIO availability on the SoC. */
#define FSL_FEATURE_SOC_SDIO_COUNT (0)
/* @brief SDMA availability on the SoC. */
#define FSL_FEATURE_SOC_SDMA_COUNT (0)
/* @brief SDMAARM availability on the SoC. */
#define FSL_FEATURE_SOC_SDMAARM_COUNT (0)
/* @brief SDMABP availability on the SoC. */
#define FSL_FEATURE_SOC_SDMABP_COUNT (0)
/* @brief SDMACORE availability on the SoC. */
#define FSL_FEATURE_SOC_SDMACORE_COUNT (0)
/* @brief SDMCORE availability on the SoC. */
#define FSL_FEATURE_SOC_SDMCORE_COUNT (0)
/* @brief SDRAM availability on the SoC. */
#define FSL_FEATURE_SOC_SDRAM_COUNT (0)
/* @brief SEMA4 availability on the SoC. */
#define FSL_FEATURE_SOC_SEMA4_COUNT (0)
/* @brief SEMA42 availability on the SoC. */
#define FSL_FEATURE_SOC_SEMA42_COUNT (0)
/* @brief SHA availability on the SoC. */
#define FSL_FEATURE_SOC_SHA_COUNT (1)
/* @brief SIM availability on the SoC. */
#define FSL_FEATURE_SOC_SIM_COUNT (0)
/* @brief SJC availability on the SoC. */
#define FSL_FEATURE_SOC_SJC_COUNT (0)
/* @brief SLCD availability on the SoC. */
#define FSL_FEATURE_SOC_SLCD_COUNT (0)
/* @brief SMARTCARD availability on the SoC. */
#define FSL_FEATURE_SOC_SMARTCARD_COUNT (2)
/* @brief SMC availability on the SoC. */
#define FSL_FEATURE_SOC_SMC_COUNT (0)
/* @brief SNVS availability on the SoC. */
#define FSL_FEATURE_SOC_SNVS_COUNT (0)
/* @brief SPBA availability on the SoC. */
#define FSL_FEATURE_SOC_SPBA_COUNT (0)
/* @brief SPDIF availability on the SoC. */
#define FSL_FEATURE_SOC_SPDIF_COUNT (0)
/* @brief SPI availability on the SoC. */
#define FSL_FEATURE_SOC_SPI_COUNT (10)
/* @brief SPIFI availability on the SoC. */
#define FSL_FEATURE_SOC_SPIFI_COUNT (1)
/* @brief SPM availability on the SoC. */
#define FSL_FEATURE_SOC_SPM_COUNT (0)
/* @brief SRC availability on the SoC. */
#define FSL_FEATURE_SOC_SRC_COUNT (0)
/* @brief SYSCON availability on the SoC. */
#define FSL_FEATURE_SOC_SYSCON_COUNT (1)
/* @brief TEMPMON availability on the SoC. */
#define FSL_FEATURE_SOC_TEMPMON_COUNT (0)
/* @brief TMR availability on the SoC. */
#define FSL_FEATURE_SOC_TMR_COUNT (0)
/* @brief TPM availability on the SoC. */
#define FSL_FEATURE_SOC_TPM_COUNT (0)
/* @brief TRGMUX availability on the SoC. */
#define FSL_FEATURE_SOC_TRGMUX_COUNT (0)
/* @brief TRIAMP availability on the SoC. */
#define FSL_FEATURE_SOC_TRIAMP_COUNT (0)
/* @brief TRNG availability on the SoC. */
#define FSL_FEATURE_SOC_TRNG_COUNT (0)
/* @brief TSC availability on the SoC. */
#define FSL_FEATURE_SOC_TSC_COUNT (0)
/* @brief TSI availability on the SoC. */
#define FSL_FEATURE_SOC_TSI_COUNT (0)
/* @brief TSTMR availability on the SoC. */
#define FSL_FEATURE_SOC_TSTMR_COUNT (0)
/* @brief UART availability on the SoC. */
#define FSL_FEATURE_SOC_UART_COUNT (0)
/* @brief USART availability on the SoC. */
#define FSL_FEATURE_SOC_USART_COUNT (10)
/* @brief USB availability on the SoC. */
#define FSL_FEATURE_SOC_USB_COUNT (1)
/* @brief USBHS availability on the SoC. */
#define FSL_FEATURE_SOC_USBHS_COUNT (0)
/* @brief USBDCD availability on the SoC. */
#define FSL_FEATURE_SOC_USBDCD_COUNT (0)
/* @brief USBFSH availability on the SoC. */
#define FSL_FEATURE_SOC_USBFSH_COUNT (1)
/* @brief USBHSD availability on the SoC. */
#define FSL_FEATURE_SOC_USBHSD_COUNT (1)
/* @brief USBHSDCD availability on the SoC. */
#define FSL_FEATURE_SOC_USBHSDCD_COUNT (0)
/* @brief USBHSH availability on the SoC. */
#define FSL_FEATURE_SOC_USBHSH_COUNT (1)
/* @brief USBNC availability on the SoC. */
#define FSL_FEATURE_SOC_USBNC_COUNT (0)
/* @brief USBPHY availability on the SoC. */
#define FSL_FEATURE_SOC_USBPHY_COUNT (0)
/* @brief USB_HSIC availability on the SoC. */
#define FSL_FEATURE_SOC_USB_HSIC_COUNT (0)
/* @brief USB_OTG availability on the SoC. */
#define FSL_FEATURE_SOC_USB_OTG_COUNT (0)
/* @brief USBVREG availability on the SoC. */
#define FSL_FEATURE_SOC_USBVREG_COUNT (0)
/* @brief USDHC availability on the SoC. */
#define FSL_FEATURE_SOC_USDHC_COUNT (0)
/* @brief UTICK availability on the SoC. */
#define FSL_FEATURE_SOC_UTICK_COUNT (1)
/* @brief VIU availability on the SoC. */
#define FSL_FEATURE_SOC_VIU_COUNT (0)
/* @brief VREF availability on the SoC. */
#define FSL_FEATURE_SOC_VREF_COUNT (0)
/* @brief VFIFO availability on the SoC. */
#define FSL_FEATURE_SOC_VFIFO_COUNT (0)
/* @brief WDOG availability on the SoC. */
#define FSL_FEATURE_SOC_WDOG_COUNT (0)
/* @brief WKPU availability on the SoC. */
#define FSL_FEATURE_SOC_WKPU_COUNT (0)
/* @brief WWDT availability on the SoC. */
#define FSL_FEATURE_SOC_WWDT_COUNT (1)
/* @brief XBAR availability on the SoC. */
#define FSL_FEATURE_SOC_XBAR_COUNT (0)
/* @brief XBARA availability on the SoC. */
#define FSL_FEATURE_SOC_XBARA_COUNT (0)
/* @brief XBARB availability on the SoC. */
#define FSL_FEATURE_SOC_XBARB_COUNT (0)
/* @brief XCVR availability on the SoC. */
#define FSL_FEATURE_SOC_XCVR_COUNT (0)
/* @brief XRDC availability on the SoC. */
#define FSL_FEATURE_SOC_XRDC_COUNT (0)
/* @brief XTALOSC availability on the SoC. */
#define FSL_FEATURE_SOC_XTALOSC_COUNT (0)
/* @brief XTALOSC24M availability on the SoC. */
#define FSL_FEATURE_SOC_XTALOSC24M_COUNT (0)
/* @brief ZLL availability on the SoC. */
#define FSL_FEATURE_SOC_ZLL_COUNT (0)

/* CAN module features */

/* @brief Support CANFD or not */
#define FSL_FEATURE_CAN_SUPPORT_CANFD (1)

/* DMA module features */

/* @brief Number of channels */
#define FSL_FEATURE_DMA_NUMBER_OF_CHANNELS (30)

/* EEPROM module features */

/* @brief Size of the EEPROM */
#define FSL_FEATURE_EEPROM_SIZE (0x00004000)
/* @brief Base address of the EEPROM */
#define FSL_FEATURE_EEPROM_BASE_ADDRESS (0x40108000)
/* @brief Page count of the EEPROM */
#define FSL_FEATURE_EEPROM_PAGE_COUNT (128)
/* @brief Command number for eeprom program */
#define FSL_FEATURE_EEPROM_PROGRAM_CMD (6)
/* @brief EEPROM internal clock freqency */
#define FSL_FEATURE_EEPROM_INTERNAL_FREQ (1500000)

/* IOCON module features */

/* @brief Func bit field width */
#define FSL_FEATURE_IOCON_FUNC_FIELD_WIDTH (4)

/* PINT module features */

/* @brief Number of connected outputs */
#define FSL_FEATURE_PINT_NUMBER_OF_CONNECTED_OUTPUTS (8)

/* SCT module features */

/* @brief Number of events */
#define FSL_FEATURE_SCT_NUMBER_OF_EVENTS (10)
/* @brief Number of states */
#define FSL_FEATURE_SCT_NUMBER_OF_STATES (10)
/* @brief Number of match capture */
#define FSL_FEATURE_SCT_NUMBER_OF_MATCH_CAPTURE (10)

/* SDIF module features */

/* @brief FIFO depth, every location is a WORD */
#define FSL_FEATURE_SDIF_FIFO_DEPTH_64_32BITS  (64)
/* @brief Max DMA buffer size */
#define FSL_FEATURE_SDIF_INTERNAL_DMA_MAX_BUFFER_SIZE  (4096)
/* @brief Max source clock in HZ */
#define FSL_FEATURE_SDIF_MAX_SOURCE_CLOCK  (52000000)

/* SPIFI module features */

/* @brief SPIFI start address */
#define FSL_FEATURE_SPIFI_START_ADDR (0x10000000)
/* @brief SPIFI end address */
#define FSL_FEATURE_SPIFI_END_ADDR (0x17FFFFFF)

/* SYSCON module features */

/* @brief Pointer to ROM IAP entry functions */
#define FSL_FEATURE_SYSCON_IAP_ENTRY_LOCATION (0x03000205)
/* @brief Flash page size in bytes */
#define FSL_FEATURE_SYSCON_FLASH_PAGE_SIZE_BYTES (256)
/* @brief Flash sector size in bytes */
#define FSL_FEATURE_SYSCON_FLASH_SECTOR_SIZE_BYTES (32768)
/* @brief Flash size in bytes */
#define FSL_FEATURE_SYSCON_FLASH_SIZE_BYTES (524288)

/* USB module features */

/* @brief Size of the USB dedicated RAM */
#define FSL_FEATURE_USB_USB_RAM (0x00002000)
/* @brief Base address of the USB dedicated RAM */
#define FSL_FEATURE_USB_USB_RAM_BASE_ADDRESS (0x40100000)

/* USBFSH module features */

/* @brief Size of the USB dedicated RAM */
#define FSL_FEATURE_USBFSH_USB_RAM (0x00002000)
/* @brief Base address of the USB dedicated RAM */
#define FSL_FEATURE_USBFSH_USB_RAM_BASE_ADDRESS (0x40100000)

/* USBHSD module features */

/* @brief Size of the USB dedicated RAM */
#define FSL_FEATURE_USBHSD_USB_RAM (0x00002000)
/* @brief Base address of the USB dedicated RAM */
#define FSL_FEATURE_USBHSD_USB_RAM_BASE_ADDRESS (0x40100000)

/* USBHSH module features */

/* @brief Size of the USB dedicated RAM */
#define FSL_FEATURE_USBHSH_USB_RAM (0x00002000)
/* @brief Base address of the USB dedicated RAM */
#define FSL_FEATURE_USBHSH_USB_RAM_BASE_ADDRESS (0x40100000)

#endif /* _LPC54628_FEATURES_H_ */

