/*
SPDX-License-Identifier: Beerware

"THE BEER-WARE LICENSE" (Revision 42):
<owen.lyke@sparkfun.com> added this file. As long as you retain this notice you 
can do whatever you want with this stuff. If we meet some day, and you think 
this stuff is worth it, you can buy me a beer in return

Owen Lyke
*/

//*****************************************************************************
//
//! @file HM01B0.c
//!
//
//*****************************************************************************

#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
#include "HM01B0.h"
#include "HM01B0_Walking1s_01.h"
#include "platform.h"

#define read_vsync()    (AM_REGVAL(AM_REGADDR(GPIO, RDA)) & (1 << HM01B0_PIN_VSYNC))
#define read_hsync()    (AM_REGVAL(AM_REGADDR(GPIO, RDA)) & (1 << HM01B0_PIN_HSYNC))
#define read_pclk()     (AM_REGVAL(AM_REGADDR(GPIO, RDA)) & (1 << HM01B0_PIN_PCLK))
#define read_byte()     (APBDMA->BBINPUT)

const am_hal_gpio_pincfg_t g_HM01B0_pin_int =
{
    .uFuncSel = 3,
    .eGPOutcfg      = AM_HAL_GPIO_PIN_OUTCFG_DISABLE,
    .eIntDir        = AM_HAL_GPIO_PIN_INTDIR_LO2HI,
    .eGPInput       = AM_HAL_GPIO_PIN_INPUT_ENABLE,
    .eGPRdZero      = AM_HAL_GPIO_PIN_RDZERO_READPIN
};

//*****************************************************************************
//
//! @brief Write HM01B0 registers
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//! @param ui16Reg              - Register address.
//! @param pui8Value            - Pointer to the data to be written.
//! @param ui32NumBytes         - Length of the data in bytes to be written.
//!
//! This function writes value to HM01B0 registers.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_write_reg(hm01b0_cfg_t *psCfg, \
                            uint16_t ui16Reg, uint8_t *pui8Value, uint32_t ui32NumBytes)
{
    am_hal_iom_transfer_t Transaction;

    //
    // Create the transaction.
    //
    Transaction.ui32InstrLen                = sizeof(uint16_t);
    Transaction.ui32Instr                   = (ui16Reg & 0x0000FFFF);
    Transaction.eDirection                  = AM_HAL_IOM_TX;
    Transaction.ui32NumBytes                = ui32NumBytes;
    Transaction.pui32TxBuffer               = (uint32_t *) pui8Value;
    Transaction.uPeerInfo.ui32I2CDevAddr    = (uint32_t) psCfg->ui16SlvAddr;
    Transaction.bContinue                   = false;
    Transaction.ui8RepeatCount              = 0;
    Transaction.ui32PauseCondition          = 0;
    Transaction.ui32StatusSetClr            = 0;

    //
    // Execute the transction over IOM.
    //
    if (am_hal_iom_blocking_transfer(psCfg->pIOMHandle, &Transaction))
    {
        return HM01B0_ERR_I2C;
    }

    return HM01B0_ERR_OK;

}

//*****************************************************************************
//
//! @brief Read HM01B0 registers
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//! @param ui16Reg              - Register address.
//! @param pui8Value            - Pointer to the buffer for read data to be put into.
//! @param ui32NumBytes         - Length of the data to be read.
//!
//! This function reads value from HM01B0 registers.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_read_reg(hm01b0_cfg_t *psCfg, \
                            uint16_t ui16Reg, uint8_t *pui8Value, uint32_t ui32NumBytes)
{
    am_hal_iom_transfer_t  Transaction;

    //
    // Create the transaction.
    //
    Transaction.ui32InstrLen                = sizeof(uint16_t);
    Transaction.ui32Instr                   = (ui16Reg & 0x0000FFFF);
    Transaction.eDirection                  = AM_HAL_IOM_RX;
    Transaction.ui32NumBytes                = ui32NumBytes;
    Transaction.pui32RxBuffer               = (uint32_t *) pui8Value;;
    Transaction.uPeerInfo.ui32I2CDevAddr    = (uint32_t) psCfg->ui16SlvAddr;
    Transaction.bContinue                   = false;
    Transaction.ui8RepeatCount              = 0;
    Transaction.ui32PauseCondition          = 0;
    Transaction.ui32StatusSetClr            = 0;

    //
    // Execute the transction over IOM.
    //
    if (am_hal_iom_blocking_transfer(psCfg->pIOMHandle, &Transaction))
    {
        return HM01B0_ERR_I2C;
    }

    return HM01B0_ERR_OK;
}

//*****************************************************************************
//
//! @brief Load HM01B0 a given script
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//! @param psScrip              - Pointer to the script to be loaded.
//! @param ui32ScriptCmdNum     - Number of entries in a given script.
//!
//! This function loads HM01B0 a given script.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_load_script(hm01b0_cfg_t *psCfg, hm_script_t *psScript, uint32_t ui32ScriptCmdNum)
{
    uint32_t ui32Err = HM01B0_ERR_OK;
    for (uint32_t idx = 0; idx < ui32ScriptCmdNum; idx++)
    {
        ui32Err = hm01b0_write_reg(psCfg, \
                                    (psScript + idx)->ui16Reg, \
                                    &((psScript + idx)->ui8Val), \
                                    sizeof(uint8_t));
        if (ui32Err != HM01B0_ERR_OK)
        {
            break;
        }
    }

    return ui32Err;   
}

//*****************************************************************************
//
//! @brief Power up HM01B0
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//!
//! This function powers up HM01B0.
//!
//! @return none.
//
//*****************************************************************************
void hm01b0_power_up(hm01b0_cfg_t *psCfg)
{
    // place holder
}

//*****************************************************************************
//
//! @brief Power down HM01B0
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//!
//! This function powers up HM01B0.
//!
//! @return none.
//
//*****************************************************************************
void hm01b0_power_down(hm01b0_cfg_t *psCfg)
{
    // place holder
}

//*****************************************************************************
//
//! @brief Enable MCLK
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//!
//! This function utilizes CTimer to generate MCLK for HM01B0.
//!
//! @return none.
//
//*****************************************************************************
void hm01b0_mclk_enable(hm01b0_cfg_t *psCfg)
{
#define MCLK_UI64PATTERN        0x55555555
#define MCLK_UI64PATTERNLEN     31

    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);

    //
    // Set up timer.
    //
    am_hal_ctimer_clear(psCfg->ui32CTimerModule, psCfg->ui32CTimerSegment);

    am_hal_ctimer_config_single(psCfg->ui32CTimerModule,
                                    psCfg->ui32CTimerSegment,
                                    (
                                        AM_HAL_CTIMER_FN_PTN_REPEAT |
                                        AM_HAL_CTIMER_HFRC_12MHZ
                                    )
                                );

    //
    // Set the pattern in the CMPR registers.
    //
    am_hal_ctimer_compare_set(psCfg->ui32CTimerModule, psCfg->ui32CTimerSegment, 0, 
                            (uint32_t)(MCLK_UI64PATTERN & 0xFFFF));
    am_hal_ctimer_compare_set(psCfg->ui32CTimerModule, psCfg->ui32CTimerSegment, 1, 
                            (uint32_t)((MCLK_UI64PATTERN >> 16) & 0xFFFF));

    //
    // Set the timer trigger and pattern length.
    //
    am_hal_ctimer_config_trigger(psCfg->ui32CTimerModule,
                                    psCfg->ui32CTimerSegment,
                                    (
                                        (MCLK_UI64PATTERNLEN << CTIMER_AUX0_TMRA0LMT_Pos) |
                                        (CTIMER_AUX0_TMRB0TRIG_DIS << CTIMER_AUX0_TMRA0TRIG_Pos)
                                    )
                                );

    //
    // Configure timer output pin.
    //
    am_hal_ctimer_output_config(psCfg->ui32CTimerModule,
                                psCfg->ui32CTimerSegment,
                                psCfg->ui32CTimerOutputPin, 
                                AM_HAL_CTIMER_OUTPUT_NORMAL, 
                                AM_HAL_GPIO_PIN_DRIVESTRENGTH_12MA);

    //
    // Start the timer.
    //
    am_hal_ctimer_start(psCfg->ui32CTimerModule, psCfg->ui32CTimerSegment);

}

//*****************************************************************************
//
//! @brief Disable MCLK
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//!
//! This function disable CTimer to stop MCLK for HM01B0.
//!
//! @return none.
//
//*****************************************************************************
void hm01b0_mclk_disable(hm01b0_cfg_t *psCfg)
{
    //
    // Stop the timer.
    //
    am_hal_ctimer_stop(psCfg->ui32CTimerModule, psCfg->ui32CTimerSegment);
    am_hal_gpio_pinconfig(psCfg->ui32CTimerOutputPin, g_AM_HAL_GPIO_DISABLE);

}

//*****************************************************************************
//
//! @brief Initialize interfaces
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//!
//! This function initializes interfaces.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_init_if(hm01b0_cfg_t *psCfg)
{
    void *pIOMHandle = NULL;

    if ( psCfg->ui32IOMModule > AM_REG_IOM_NUM_MODULES )
    {
        return HM01B0_ERR_I2C;
    }

    //
    // Enable fault detection.
    //
#if AM_APOLLO3_MCUCTRL
    am_hal_mcuctrl_control(AM_HAL_MCUCTRL_CONTROL_FAULT_CAPTURE_ENABLE, 0);
#else // AM_APOLLO3_MCUCTRL
    am_hal_mcuctrl_fault_capture_enable();
#endif // AM_APOLLO3_MCUCTRL

    //
    // Initialize the IOM instance.
    // Enable power to the IOM instance.
    // Configure the IOM for Serial operation during initialization.
    // Enable the IOM.
    //
    if (am_hal_iom_initialize(psCfg->ui32IOMModule, &pIOMHandle) ||
        am_hal_iom_power_ctrl(pIOMHandle, AM_HAL_SYSCTRL_WAKE, false) ||
        am_hal_iom_configure(pIOMHandle, &(psCfg->sIOMCfg)) ||
        am_hal_iom_enable(pIOMHandle))
    {
        return HM01B0_ERR_I2C;
    }
    else
    {
        //
        // Configure the IOM pins.
        //
        am_bsp_iom_pins_enable(psCfg->ui32IOMModule, psCfg->eIOMMode);

        psCfg->pIOMHandle = pIOMHandle;
    }

    // initialize pins for camera parallel interface.
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD0);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD1);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD2);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD3);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD4);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD5);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD6);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD7);

    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD0);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD1);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD2);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD3);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD4);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD5);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD6);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD7);

    am_hal_gpio_fast_pinconfig((uint64_t)0x1 << psCfg->ui8PinD0 |
                                 (uint64_t)0x1 << psCfg->ui8PinD1 |
                                 (uint64_t)0x1 << psCfg->ui8PinD2 |
                                 (uint64_t)0x1 << psCfg->ui8PinD3 |
                                 (uint64_t)0x1 << psCfg->ui8PinD4 |
                                 (uint64_t)0x1 << psCfg->ui8PinD5 |
                                 (uint64_t)0x1 << psCfg->ui8PinD6 |
                                 (uint64_t)0x1 << psCfg->ui8PinD7,
                                 g_AM_HAL_GPIO_INPUT, 0);

    am_hal_gpio_pinconfig(psCfg->ui8PinVSYNC,   g_AM_HAL_GPIO_INPUT);
    am_hal_gpio_pinconfig(psCfg->ui8PinHSYNC,   g_AM_HAL_GPIO_INPUT);
    am_hal_gpio_pinconfig(psCfg->ui8PinPCLK,    g_AM_HAL_GPIO_INPUT);

    am_hal_gpio_pinconfig(psCfg->ui8PinTrig,    g_AM_HAL_GPIO_OUTPUT);

    am_hal_gpio_pinconfig(psCfg->ui8PinInt,     g_AM_HAL_GPIO_DISABLE);
    // am_hal_gpio_pinconfig(psCfg->ui8PinInt,     g_HM01B0_pin_int);
    // am_hal_gpio_interrupt_clear(AM_HAL_GPIO_BIT(psCfg->ui8PinInt));
    // am_hal_gpio_interrupt_enable(AM_HAL_GPIO_BIT(psCfg->ui8PinInt));
    // NVIC_EnableIRQ(GPIO_IRQn);

    return HM01B0_ERR_OK;
}

//*****************************************************************************
//
//! @brief Deinitialize interfaces
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//!
//! This function deinitializes interfaces.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_deinit_if(hm01b0_cfg_t *psCfg)
{
    am_hal_iom_disable(psCfg->pIOMHandle);
    am_hal_iom_uninitialize(psCfg->pIOMHandle);

    am_hal_gpio_pinconfig(psCfg->ui8PinSCL,   g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(psCfg->ui8PinSDA,   g_AM_HAL_GPIO_DISABLE);

    // initialize pins for camera parallel interface.
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD0);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD1);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD2);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD3);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD4);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD5);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD6);
    am_hal_gpio_fastgpio_disable(psCfg->ui8PinD7);

    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD0);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD1);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD2);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD3);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD4);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD5);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD6);
    am_hal_gpio_fastgpio_clr(psCfg->ui8PinD7);

    am_hal_gpio_pinconfig(psCfg->ui8PinVSYNC,   g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(psCfg->ui8PinHSYNC,   g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(psCfg->ui8PinPCLK,    g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(psCfg->ui8PinTrig,    g_AM_HAL_GPIO_DISABLE);
    am_hal_gpio_pinconfig(psCfg->ui8PinInt,     g_AM_HAL_GPIO_DISABLE);


    return HM01B0_ERR_OK;
}

//*****************************************************************************
//
//! @brief Get HM01B0 Model ID
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//! @param pui16MID             - Pointer to buffer for the read back model ID.
//!
//! This function reads back HM01B0 model ID.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_get_modelid(hm01b0_cfg_t *psCfg, uint16_t *pui16MID)
{
    uint8_t ui8Data[1];
    uint32_t ui32Err;

    *pui16MID = 0x0000;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_MODEL_ID_H, ui8Data, sizeof(ui8Data));
    if (ui32Err == HM01B0_ERR_OK)
    {
        *pui16MID |= (ui8Data[0] << 8);
    }

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_MODEL_ID_L, ui8Data, sizeof(ui8Data));
    if (ui32Err == HM01B0_ERR_OK)
    {
        *pui16MID |= ui8Data[0];
    }

    return ui32Err;
}

//*****************************************************************************
//
//! @brief Initialize HM01B0
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//! @param psScript             - Pointer to HM01B0 initialization script.
//! @param ui32ScriptCmdNum     - No. of commands in HM01B0 initialization script.
//!
//! This function initilizes HM01B0 with a given script.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_init_system(hm01b0_cfg_t *psCfg, hm_script_t *psScript, uint32_t ui32ScriptCmdNum)
{
    return hm01b0_load_script(psCfg, psScript, ui32ScriptCmdNum);
}

//*****************************************************************************
//
//! @brief Set HM01B0 in the walking 1s test mode
//!
//! @param psCfg                - Pointer to HM01B0 configuration structure.
//!
//! This function sets HM01B0 in the walking 1s test mode.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_test_walking1s(hm01b0_cfg_t *psCfg)
{
    uint32_t ui32ScriptCmdNum = sizeof(sHM01b0TestModeScript_Walking1s) / sizeof(hm_script_t);
    hm_script_t *psScript = (hm_script_t *)sHM01b0TestModeScript_Walking1s;

    return hm01b0_load_script(psCfg, psScript, ui32ScriptCmdNum);
}

//*****************************************************************************
//
//! @brief Check the data read from HM01B0 in the walking 1s test mode
//!
//! @param pui8Buffer       - Pointer to data buffer.
//! @param ui32BufferLen    - Buffer length
//! @param ui32PrintCnt     - Number of mismatched data to be printed out
//!
//! This function sets HM01B0 in the walking 1s test mode.
//!
//! @return Error code.
//
//*****************************************************************************
void hm01b0_test_walking1s_check_data_sanity(uint8_t *pui8Buffer, uint32_t ui32BufferLen, uint32_t ui32PrintCnt)
{
    uint8_t ui8ByteData = *pui8Buffer;
    uint32_t ui32MismatchCnt = 0x00;

    for (uint32_t ui32Idx = 0; ui32Idx < ui32BufferLen; ui32Idx++)
    {
        if (*(pui8Buffer + ui32Idx) != ui8ByteData)
        {
            if (ui32PrintCnt)
            {
                am_util_stdio_printf("[0x%08X] actual 0x%02X expected 0x%02X\n", ui32Idx, *(pui8Buffer + ui32Idx), ui8ByteData);
                am_util_delay_ms(1);
                ui32PrintCnt--;
            }
            ui32MismatchCnt++;
        }

        if (ui8ByteData)
            ui8ByteData = ui8ByteData << 1;
        else
            ui8ByteData = 0x01;
    }

    am_util_stdio_printf("Mismatch Rate %d/%d\n", ui32MismatchCnt, ui32BufferLen);

}

//*****************************************************************************
//
//! @brief Software reset HM01B0
//!
//! @param psCfg        - Pointer to HM01B0 configuration structure.
//!
//! This function resets HM01B0 by issuing a reset command.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_reset_sw(hm01b0_cfg_t *psCfg)
{
    uint8_t ui8Data[1] = {0x00};
    return hm01b0_write_reg(psCfg, HM01B0_REG_SW_RESET, ui8Data, sizeof(ui8Data));
}

//*****************************************************************************
//
//! @brief Get current HM01B0 operation mode.
//!
//! @param psCfg        - Pointer to HM01B0 configuration structure.
//! @param pui8Mode     - Pointer to buffer
//!                     - for the read back operation mode to be put into
//!
//! This function get HM01B0 operation mode.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_get_mode(hm01b0_cfg_t *psCfg, uint8_t *pui8Mode)
{
    uint8_t ui8Data[1] = {0x01};
    uint32_t ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_MODE_SELECT, ui8Data, sizeof(ui8Data));

    *pui8Mode = ui8Data[0];

    return ui32Err;
}

//*****************************************************************************
//
//! @brief Set HM01B0 operation mode.
//!
//! @param psCfg        - Pointer to HM01B0 configuration structure.
//! @param ui8Mode      - Operation mode. One of:
//!     HM01B0_REG_MODE_SELECT_STANDBY
//!     HM01B0_REG_MODE_SELECT_STREAMING
//!     HM01B0_REG_MODE_SELECT_STREAMING_NFRAMES
//!     HM01B0_REG_MODE_SELECT_STREAMING_HW_TRIGGER
//! @param ui8FrameCnt  - Frame count for HM01B0_REG_MODE_SELECT_STREAMING_NFRAMES.
//!                     - Discarded if other modes.
//!
//! This function set HM01B0 operation mode.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_set_mode(hm01b0_cfg_t *psCfg, uint8_t ui8Mode, uint8_t ui8FrameCnt)
{
    uint32_t ui32Err = HM01B0_ERR_OK;

    if (ui8Mode == HM01B0_REG_MODE_SELECT_STREAMING_NFRAMES)
    {
        ui32Err = hm01b0_write_reg(psCfg, HM01B0_REG_PMU_PROGRAMMABLE_FRAMECNT, &ui8FrameCnt, sizeof(ui8FrameCnt));
    }

    if(ui32Err == HM01B0_ERR_OK)
    {
        ui32Err = hm01b0_write_reg(psCfg, HM01B0_REG_MODE_SELECT, &ui8Mode, sizeof(ui8Mode));
    }

    return ui32Err;
}

//*****************************************************************************
//
//! @brief Activate the updated settings to HM01B0.
//!
//! @param psCfg            - Pointer to HM01B0 configuration structure.
//!
//! Some settings updated to HM01B0 will only be affected after calling this function
//! 1. AE settings
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_cmd_update(hm01b0_cfg_t *psCfg)
{
    uint8_t     ui8Data = HM01B0_REG_GRP_PARAM_HOLD_HOLD;

    return hm01b0_write_reg(psCfg, HM01B0_REG_GRP_PARAM_HOLD, &ui8Data, sizeof(ui8Data));
}

//*****************************************************************************
//
//! @brief Get HM01B0 AE convergance
//!
//! @param psCfg            - Pointer to HM01B0 configuration structure.
//! @param psAECfg          - Pointer to the structure hm01b0_ae_cfg_t.
//!
//! This function checks if AE is converged or not and returns ui32Err accordingly.
//! If caller needs detailed AE settings, psAECfg has to be non NULL.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_get_ae(hm01b0_cfg_t *psCfg, hm01b0_ae_cfg_t *psAECfg)
{
    uint32_t    ui32Err = HM01B0_ERR_OK;
    uint8_t     ui8AETargetMean;
    uint8_t     ui8AEMinMean;
    uint8_t     ui8AEMean;
    uint8_t     ui8ConvergeInTh;
    uint8_t     ui8ConvergeOutTh;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_AE_TARGET_MEAN, &ui8AETargetMean, sizeof(ui8AETargetMean));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_AE_MIN_MEAN, &ui8AEMinMean, sizeof(ui8AEMinMean));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_CONVERGE_IN_TH, &ui8ConvergeInTh, sizeof(ui8ConvergeInTh));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_CONVERGE_OUT_TH, &ui8ConvergeOutTh, sizeof(ui8ConvergeOutTh));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, 0x2020, &ui8AEMean, sizeof(ui8AEMean));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    if ((ui8AEMean < (ui8AETargetMean - ui8ConvergeInTh)) || (ui8AEMean > (ui8AETargetMean + ui8ConvergeInTh)))
        ui32Err = HM01B0_ERR_AE_NOT_CONVERGED;

    if (psAECfg)
    {
        psAECfg->ui8AETargetMean    = ui8AETargetMean;
        psAECfg->ui8AEMinMean       = ui8AEMinMean;
        psAECfg->ui8ConvergeInTh    = ui8ConvergeInTh;
        psAECfg->ui8ConvergeOutTh   = ui8ConvergeOutTh;
        psAECfg->ui8AEMean          = ui8AEMean;
    }

    return ui32Err;
}

//*****************************************************************************
//
//! @brief AE calibration.
//!
//! @param psCfg            - Pointer to HM01B0 configuration structure.
//! @param ui8CalFrames     - Frame counts for calibratoin.
//! @param pui8Buffer       - Pointer to the frame buffer.
//! @param ui32BufferLen    - Framebuffer size.
//!
//! This function lets HM01B0 AE settled as much as possible within a given frame counts.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_cal_ae(hm01b0_cfg_t *psCfg, uint8_t ui8CalFrames, uint8_t *pui8Buffer, uint32_t ui32BufferLen)
{
    uint32_t        ui32Err     = HM01B0_ERR_OK;
    hm01b0_ae_cfg_t sAECfg;

    am_util_stdio_printf("[%s] +\n", __func__);

    hm01b0_set_mode(psCfg, HM01B0_REG_MODE_SELECT_STREAMING_NFRAMES, ui8CalFrames);

    for (uint8_t i = 0; i < ui8CalFrames; i++)
    {

        hm01b0_blocking_read_oneframe(psCfg, pui8Buffer, ui32BufferLen);

        ui32Err = hm01b0_get_ae(psCfg, &sAECfg);

        am_util_stdio_printf("AE Calibration(0x%02X) TargetMean 0x%02X, ConvergeInTh 0x%02X, AEMean 0x%02X\n", \
                                        ui32Err, sAECfg.ui8AETargetMean, sAECfg.ui8ConvergeInTh, sAECfg.ui8AEMean);

        // if AE calibration is done in ui8CalFrames, just exit to save some time.
        if (ui32Err == HM01B0_ERR_OK)
            break;
    }

    hm01b0_set_mode(psCfg, HM01B0_REG_MODE_SELECT_STANDBY, 0);

    am_util_stdio_printf("[%s] -\n", __func__);

    return ui32Err;
}


//*****************************************************************************
//
//! @brief Save HM01B0 exposure gain settings.
//!
//! @param psCfg            - Pointer to HM01B0 configuration structure.
//! @param psExpoGainCtrl   - Pointer to the structure hm01b0_snr_expo_gain_ctrl_t
//!
//! This function saves HM01B0 exposure gain settings.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_save_exposure_gains(hm01b0_cfg_t *psCfg, hm01b0_snr_expo_gain_ctrl_t *psExpoGainCtrl)
{
    uint32_t ui32Err = HM01B0_ERR_OK;
    uint8_t ui8IntegrationH;
    uint8_t ui8IntegrationL;
    uint8_t ui8AGain;
    uint8_t ui8DGain_H;
    uint8_t ui8DGain_L;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_INTEGRATION_H,     &ui8IntegrationH, sizeof(ui8IntegrationH));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_INTEGRATION_L,     &ui8IntegrationL, sizeof(ui8IntegrationL));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_ANALOG_GAIN,       &ui8AGain,        sizeof(ui8AGain));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_DIGITAL_GAIN_H,    &ui8DGain_H,      sizeof(ui8DGain_H));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_DIGITAL_GAIN_L,    &ui8DGain_L,      sizeof(ui8DGain_L));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    if (psExpoGainCtrl)
    {
        psExpoGainCtrl->ui8IntegrationH     = ui8IntegrationH;
        psExpoGainCtrl->ui8IntegrationL     = ui8IntegrationL;
        psExpoGainCtrl->ui8AGain            = ui8AGain;
        psExpoGainCtrl->ui8DGain_H          = ui8DGain_H;
        psExpoGainCtrl->ui8DGain_L          = ui8DGain_L;
    }

    return ui32Err;
}

//*****************************************************************************
//
//! @brief Restore HM01B0 exposure gain settings.
//!
//! @param psCfg            - Pointer to HM01B0 configuration structure.
//! @param psExpoGainCtrl   - Pointer to the structure hm01b0_snr_expo_gain_ctrl_t
//!
//! This function restores HM01B0 exposure gain settings. The call flow shall be
//! hm01b0_restore_exposure_gains() -> hm01b0_cmd_update() -> hm01b0_set_mode()
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_restore_exposure_gains(hm01b0_cfg_t *psCfg, hm01b0_snr_expo_gain_ctrl_t *psExpoGainCtrl)
{
    uint32_t    ui32Err = HM01B0_ERR_OK;
    uint8_t     ui8Tmp;

    ui32Err = hm01b0_write_reg(psCfg, HM01B0_REG_INTEGRATION_H,  &(psExpoGainCtrl->ui8IntegrationH), sizeof(psExpoGainCtrl->ui8IntegrationH));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_write_reg(psCfg, HM01B0_REG_INTEGRATION_L,  &(psExpoGainCtrl->ui8IntegrationL), sizeof(psExpoGainCtrl->ui8IntegrationL));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_ANALOG_GAIN,     &ui8Tmp,    sizeof(ui8Tmp));
    ui8Tmp = (ui8Tmp & ~(0x7 << 4)) | (psExpoGainCtrl->ui8AGain & (0x7 << 4));
    ui32Err = hm01b0_write_reg(psCfg, HM01B0_REG_ANALOG_GAIN,    &ui8Tmp,    sizeof(ui8Tmp));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_DIGITAL_GAIN_H,      &ui8Tmp,    sizeof(ui8Tmp));
    ui8Tmp = (ui8Tmp & ~(0x3 << 0)) | (psExpoGainCtrl->ui8DGain_H & (0x3 << 0));
    ui32Err = hm01b0_write_reg(psCfg, HM01B0_REG_DIGITAL_GAIN_H,     &ui8Tmp,    sizeof(ui8Tmp));
    if (ui32Err != HM01B0_ERR_OK) return ui32Err;

    ui32Err = hm01b0_read_reg(psCfg, HM01B0_REG_DIGITAL_GAIN_L,      &ui8Tmp,    sizeof(ui8Tmp));
    ui8Tmp = (ui8Tmp & ~(0x3F << 2)) | (psExpoGainCtrl->ui8DGain_L & (0x3F << 2));
    ui32Err = hm01b0_write_reg(psCfg, HM01B0_REG_DIGITAL_GAIN_L,     &ui8Tmp,    sizeof(ui8Tmp));

    return ui32Err;

}

//*****************************************************************************
//
//! @brief Hardware trigger HM01B0 to stream.
//!
//! @param psCfg        - Pointer to HM01B0 configuration structure.
//! @param bTrigger     - True to start streaming
//!                     - False to stop streaming
//!
//! This function triggers HM01B0 to stream by toggling the TRIG pin.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_hardware_trigger_streaming(hm01b0_cfg_t *psCfg, bool bTrigger)
{
    uint32_t ui32Err = HM01B0_ERR_OK;
    uint8_t ui8Mode;

    ui32Err = hm01b0_get_mode(psCfg, &ui8Mode);

    if (ui32Err != HM01B0_ERR_OK)
        goto end;

    if (ui8Mode != HM01B0_REG_MODE_SELECT_STREAMING_HW_TRIGGER)
    {
        ui32Err = HM01B0_ERR_MODE;
        goto end;
    }

    if (bTrigger)
    {
        am_hal_gpio_output_set(psCfg->ui8PinTrig);
    }
    else
    {
        am_hal_gpio_output_clear(psCfg->ui8PinTrig);
    }

end:
    return ui32Err;
}

//*****************************************************************************
//
//! @brief Set HM01B0 mirror mode.
//!
//! @param psCfg        - Pointer to HM01B0 configuration structure.
//! @param bHmirror     - Horizontal mirror
//! @param bVmirror     - Vertical mirror
//!
//! This function set HM01B0 mirror mode.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_set_mirror(hm01b0_cfg_t *psCfg, bool bHmirror, bool bVmirror)
{
    uint8_t ui8Data = 0x00;
    uint32_t ui32Err = HM01B0_ERR_OK;

    if (bHmirror)
    {
        ui8Data |= HM01B0_REG_IMAGE_ORIENTATION_HMIRROR;
    }

    if (bVmirror)
    {
        ui8Data |= HM01B0_REG_IMAGE_ORIENTATION_VMIRROR;
    }

    ui32Err = hm01b0_write_reg(psCfg, HM01B0_REG_IMAGE_ORIENTATION, &ui8Data, sizeof(ui8Data));

    if (ui32Err == HM01B0_ERR_OK)
    {
        ui8Data = HM01B0_REG_GRP_PARAM_HOLD_HOLD;
        ui32Err = hm01b0_write_reg(psCfg, HM01B0_REG_GRP_PARAM_HOLD, &ui8Data, sizeof(ui8Data));
    }

    return ui32Err;

}

//*****************************************************************************
//
//! @brief Read data of one frame from HM01B0.
//!
//! @param psCfg            - Pointer to HM01B0 configuration structure.
//! @param pui8Buffer       - Pointer to the frame buffer.
//! @param ui32BufferLen    - Framebuffer size.
//!
//! This function read data of one frame from HM01B0.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_blocking_read_oneframe(hm01b0_cfg_t *psCfg, uint8_t *pui8Buffer, uint32_t ui32BufferLen)
{
    uint32_t    ui32Err         = HM01B0_ERR_OK;
    uint32_t    ui32Idx         = 0x00;

    am_util_stdio_printf("[%s] +\n", __func__);

    uint32_t    ui32HsyncCnt    = 0x00;

    while((ui32HsyncCnt < HM01B0_PIXEL_Y_NUM))
    {
        while (0x00 == read_hsync());

        // read one row
        while(read_hsync())
        {
            while(0x00 == read_pclk());

            *(pui8Buffer + ui32Idx++) = read_byte();

            if (ui32Idx == ui32BufferLen) {
                goto end;
            }

            while(read_pclk());
        }

        ui32HsyncCnt++;
    }

end:
    am_util_stdio_printf("[%s] - Byte Counts %d\n", __func__, ui32Idx);

    return ui32Err;

}
