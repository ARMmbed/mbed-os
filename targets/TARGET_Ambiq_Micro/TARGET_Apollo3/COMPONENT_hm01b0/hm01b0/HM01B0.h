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
//! @file HM01B0.h
//
//*****************************************************************************
#ifndef HM01B0_H
#define HM01B0_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"

#define HM01B0_DRV_VERSION                              (0)
#define HM01B0_DRV_SUBVERSION                           (5)

#define HM01B0_DEFAULT_ADDRESS                          (0x24)

#define HM01B0_PIXEL_X_NUM                              (324)
#define HM01B0_PIXEL_Y_NUM                              (244)

#define HM01B0_REG_MODEL_ID_H                           (0x0000)
#define HM01B0_REG_MODEL_ID_L                           (0x0001)
#define HM01B0_REG_SILICON_REV                          (0x0002)
#define HM01B0_REG_FRAME_COUNT                          (0x0005)
#define HM01B0_REG_PIXEL_ORDER                          (0x0006)

#define HM01B0_REG_MODE_SELECT                          (0x0100)
#define HM01B0_REG_IMAGE_ORIENTATION                    (0x0101)
#define HM01B0_REG_SW_RESET                             (0x0103)
#define HM01B0_REG_GRP_PARAM_HOLD                       (0x0104)

#define HM01B0_REG_INTEGRATION_H                        (0x0202)
#define HM01B0_REG_INTEGRATION_L                        (0x0203)
#define HM01B0_REG_ANALOG_GAIN                          (0x0205)
#define HM01B0_REG_DIGITAL_GAIN_H                       (0x020E)
#define HM01B0_REG_DIGITAL_GAIN_L                       (0x020F)

#define HM01B0_REG_AE_TARGET_MEAN                       (0x2101)
#define HM01B0_REG_AE_MIN_MEAN                          (0x2102)
#define HM01B0_REG_CONVERGE_IN_TH                       (0x2103)
#define HM01B0_REG_CONVERGE_OUT_TH                      (0x2104)


#define HM01B0_REG_I2C_ID_SEL                           (0x3400)
#define HM01B0_REG_I2C_ID_REG                           (0x3401)

#define HM01B0_REG_PMU_PROGRAMMABLE_FRAMECNT            (0x3020)

// #define HM01B0_REG_MODE_SELECT (0x0100)
#define HM01B0_REG_MODE_SELECT_STANDBY                  (0x00)
#define HM01B0_REG_MODE_SELECT_STREAMING                (0x01)
#define HM01B0_REG_MODE_SELECT_STREAMING_NFRAMES        (0x03)
#define HM01B0_REG_MODE_SELECT_STREAMING_HW_TRIGGER     (0x05)

// #define HM01B0_REG_IMAGE_ORIENTATION                    (0x0101)
#define HM01B0_REG_IMAGE_ORIENTATION_DEFAULT            (0x00)
#define HM01B0_REG_IMAGE_ORIENTATION_HMIRROR            (0x01)
#define HM01B0_REG_IMAGE_ORIENTATION_VMIRROR            (0x02)
#define HM01B0_REG_IMAGE_ORIENTATION_HVMIRROR           (HM01B0_REG_IMAGE_ORIENTATION_HMIRROR | HM01B0_REG_IMAGE_ORIENTATION_HVMIRROR)

// #define HM01B0_REG_GRP_PARAM_HOLD                       (0x0104)
#define HM01B0_REG_GRP_PARAM_HOLD_CONSUME               (0x00)
#define HM01B0_REG_GRP_PARAM_HOLD_HOLD                  (0x01)

enum
{
    HM01B0_ERR_OK               = 0x00,
    HM01B0_ERR_I2C,
    HM01B0_ERR_MODE,
    HM01B0_ERR_AE_NOT_CONVERGED,
};

typedef struct
{
    uint16_t                ui16Reg;
    uint8_t                 ui8Val;
} hm_script_t;

typedef struct
{
    uint16_t                ui16SlvAddr;
    am_hal_iom_mode_e       eIOMMode;
    uint32_t                ui32IOMModule;
    am_hal_iom_config_t     sIOMCfg;
    void                    *pIOMHandle;

    uint32_t                ui32CTimerModule;
    uint32_t                ui32CTimerSegment;
    uint32_t                ui32CTimerOutputPin;

    uint8_t                 ui8PinSCL;
    uint8_t                 ui8PinSDA;
    uint8_t                 ui8PinD0;
    uint8_t                 ui8PinD1;
    uint8_t                 ui8PinD2;
    uint8_t                 ui8PinD3;
    uint8_t                 ui8PinD4;
    uint8_t                 ui8PinD5;
    uint8_t                 ui8PinD6;
    uint8_t                 ui8PinD7;
    uint8_t                 ui8PinVSYNC;
    uint8_t                 ui8PinHSYNC;
    uint8_t                 ui8PinPCLK;

    uint8_t                 ui8PinTrig;
    uint8_t                 ui8PinInt;
    void                    (*pfnGpioIsr)(void);
} hm01b0_cfg_t;

typedef struct
{
    uint8_t                 ui8AETargetMean;
    uint8_t                 ui8AEMinMean;
    uint8_t                 ui8ConvergeInTh;
    uint8_t                 ui8ConvergeOutTh;
    uint8_t                 ui8AEMean;
} hm01b0_ae_cfg_t;

typedef struct
{
    uint8_t                 ui8IntegrationH;
    uint8_t                 ui8IntegrationL;
    uint8_t                 ui8AGain;
    uint8_t                 ui8DGain_H;
    uint8_t                 ui8DGain_L;
} hm01b0_snr_expo_gain_ctrl_t;

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
                            uint16_t ui16Reg, uint8_t *pui8Value, uint32_t ui32NumBytes);

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
                            uint16_t ui16Reg, uint8_t *pui8Value, uint32_t ui32NumBytes);

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
uint32_t hm01b0_load_script(hm01b0_cfg_t *psCfg, hm_script_t *psScript, uint32_t ui32ScriptCmdNum);

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
void hm01b0_power_up(hm01b0_cfg_t *psCfg);

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
void hm01b0_power_down(hm01b0_cfg_t *psCfg);

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
void hm01b0_mclk_enable(hm01b0_cfg_t *psCfg);

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
void hm01b0_mclk_disable(hm01b0_cfg_t *psCfg);

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
uint32_t hm01b0_init_if(hm01b0_cfg_t *psCfg);

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
uint32_t hm01b0_deinit_if(hm01b0_cfg_t *psCfg);

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
uint32_t hm01b0_get_modelid(hm01b0_cfg_t *psCfg, uint16_t *pui16MID);

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
uint32_t hm01b0_init_system(hm01b0_cfg_t *psCfg, hm_script_t *psScript, uint32_t ui32ScriptCmdNum);

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
uint32_t hm01b0_test_walking1s(hm01b0_cfg_t *psCfg);

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
void hm01b0_test_walking1s_check_data_sanity(uint8_t *pui8Buffer, uint32_t ui32BufferLen, uint32_t ui32PrintCnt);

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
uint32_t hm01b0_reset_sw(hm01b0_cfg_t *psCfg);

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
uint32_t hm01b0_get_mode(hm01b0_cfg_t *psCfg, uint8_t *pui8Mode);

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
//! @param framecnt     - Frame count for HM01B0_REG_MODE_SELECT_STREAMING_NFRAMES.
//!                     - Discarded if other modes.
//!
//! This function set HM01B0 operation mode.
//!
//! @return Error code.
//
//*****************************************************************************
uint32_t hm01b0_set_mode(hm01b0_cfg_t *psCfg, uint8_t ui8Mode, uint8_t framecnt);


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
uint32_t hm01b0_cmd_update(hm01b0_cfg_t *psCfg);

//*****************************************************************************
//
//! @brief Get HM01B0 AE settings
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
uint32_t hm01b0_get_ae(hm01b0_cfg_t *psCfg, hm01b0_ae_cfg_t *psAECfg);

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
uint32_t hm01b0_cal_ae(hm01b0_cfg_t *psCfg, uint8_t ui8CalFrames, uint8_t *pui8Buffer, uint32_t ui32BufferLen);

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
uint32_t hm01b0_save_exposure_gains(hm01b0_cfg_t *psCfg, hm01b0_snr_expo_gain_ctrl_t *psExpoGainCtrl);

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
uint32_t hm01b0_restore_exposure_gains(hm01b0_cfg_t *psCfg, hm01b0_snr_expo_gain_ctrl_t *psExpoGainCtrl);

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
uint32_t hm01b0_hardware_trigger_streaming(hm01b0_cfg_t *psCfg, bool bTrigger);

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
uint32_t hm01b0_set_mirror(hm01b0_cfg_t *psCfg, bool bHmirror, bool bVmirror);


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
uint32_t hm01b0_blocking_read_oneframe(hm01b0_cfg_t *psCfg, \
                                    uint8_t *pui8Buffer, uint32_t ui32BufferLen);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_CTIMER_H
