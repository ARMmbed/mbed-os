/*
 * Copyright 2018-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fsl_nand_flash.h"
#include "fsl_semc_nand_flash.h"
#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
#include "fsl_cache.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NAND_READY_CHECK_INTERVAL_NORMAL           (0U)
#define NAND_MAX_FEATURE_ACCESS_TIME_tFEAT_US      (1000U)
#define NAND_MAX_READ_PARAMETER_PAGE_TIME_tR_US    (1000U)
#define NAND_FIRST_CMD_CHANGE_COLUMN_SETUP_TIME_NS (500U)
#define NAND_MAX_RST_TIME0_tRST_US                 (5U)
#define NAND_MAX_RST_TIME1_tRST_US                 (10U)
#define NAND_MAX_RST_TIME2_tRST_US                 (500U)
#define NAND_MAX_RST_TIME3_tRST_US                 (1000U)
#define NAND_ONFI_SIGNATURE_MIN_MATCHED_BYTES      (0x2U)
#define NAND_FLASH_SR_ONFI_PASSBITMASK             (0x01U)
#define NAND_FLASH_SR_ONFI_READYBITMASK            (0x40U)
#define NAND_FLASH_COLUMNBITSNUM_MAX               (16U)

/*! @brief Constructs the four character code for tag */
#if !defined(FOUR_CHAR_CODE)
#define FOUR_CHAR_CODE(a, b, c, d) (((d) << 24) | ((c) << 16) | ((b) << 8) | ((a)))
#endif

/*! @brief State information for the CRC16 algorithm. */
typedef struct Crc16Data
{
    uint16_t currentCrc; //!< Current CRC value.
} crc16_data_t;

/*! @brief Parallel NAND timing config */
typedef struct _nand_ac_timing_parameter
{
    uint8_t min_tCS_ns;
    uint8_t min_tCH_ns;
    uint8_t min_tCEITV_ns;
    uint8_t min_tWP_ns;
    uint8_t min_tWH_ns;
    uint8_t min_tRP_ns;
    uint8_t min_tREH_ns;
    uint8_t min_tTA_ns;
    uint8_t min_tWHR_ns;
    uint8_t min_tRHW_ns;
    uint8_t min_tADL_ns;
    uint8_t min_tRR_ns;
    uint8_t max_tWB_ns;
} nand_ac_timing_parameter_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void semc_nand_crc16_onfi_init(crc16_data_t *crc16Info);
static void semc_nand_crc16_onfi_update(crc16_data_t *crc16Info, const uint8_t *src, uint32_t lengthInBytes);
static void semc_nand_crc16_onfi_finalize(crc16_data_t *crc16Info, uint16_t *hash);
static status_t semc_nand_issue_reset(nand_handle_t *handle);
static status_t semc_nand_issue_read_mode(nand_handle_t *handle);
static status_t semc_nand_issue_access_feature(nand_handle_t *handle, nand_onfi_feature_config_t *featureConfig);
static status_t semc_nand_issue_read_parameter(nand_handle_t *handle, nand_onfi_parameter_config_t *parameterConfig);
static status_t semc_nand_issue_read_status(nand_handle_t *handle, uint8_t *stat);
static status_t semc_nand_issue_read_page(nand_handle_t *handle, uint32_t ipgCmdAddr);
/* Read onfi parameter data from device and make use of them*/
static status_t semc_nand_get_onfi_timing_configure(nand_handle_t *handle, nand_config_t *config);
/* Check the device ready status*/
static status_t semc_nand_wait_status_ready(nand_handle_t *handle, uint32_t readyCheckIntervalInUs, bool readOpen);
/* Enable the device ECC via feature setting*/
static status_t semc_nand_set_device_ecc(nand_handle_t *handle);
/* Validates data by using device ECC*/
static status_t semc_nand_is_device_ecc_check_passed(nand_handle_t *handle, bool *isEccPassed);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief Define the onfi timing mode. */
const nand_ac_timing_parameter_t s_nandAcTimingParameterTable[] = {
    /* ONFI 1.0, mode 0, 10MHz, 100ns */
    {.min_tCS_ns    = 70,
     .min_tCH_ns    = 20,
     .min_tCEITV_ns = 0,
     .min_tWP_ns    = 50,
     .min_tWH_ns    = 30,
     .min_tRP_ns    = 50,
     .min_tREH_ns   = 30,
     .min_tTA_ns    = 0,
     .min_tWHR_ns   = 120,
     .min_tRHW_ns   = 200,
     .min_tADL_ns   = 200,
     .min_tRR_ns    = 40,
     .max_tWB_ns    = 200},
    /* ONFI 1.0 mode 1, 20MHz, 50ns */
    {35, 10, 0, 25, 15, 25, 15, 0, 80, 100, 100, 20, 100},
    /* ONFI 1.0 mode 2, 28MHz, 35ns */
    {25, 10, 0, 17, 15, 17, 15, 0, 80, 100, 100, 20, 100},
    /* ONFI 1.0 mode 3, 33MHz, 30ns */
    {25, 5, 0, 15, 10, 15, 10, 0, 60, 100, 100, 20, 100},

    /* Note: From ONFI spec, The host shall use EDO data output cycle timings,
       when running with a tRC value less than 30 ns. (tRC = tRP + tREH) */
    /* ONFI 1.0 mode 4, 40MHz, 25ns */
    {20, 5, 0, 12, 10, 12, 10, 0, 60, 100, 70, 20, 100},
    /* ONFI 1.0 mode 5, 50MHz, 20ns */
    {15, 5, 0, 10, 7, 10, 7, 0, 60, 100, 70, 20, 100},
    /* Auto-Detection */
    {0}};

const char s_nandDeviceManufacturerList[][12] = {{'M', 'I', 'C', 'R', 'O', 'N', ' ', ' ', ' ', ' ', ' ', ' '},
                                                 {'S', 'P', 'A', 'N', 'S', 'I', 'O', 'N', ' ', ' ', ' ', ' '},
                                                 {'S', 'A', 'M', 'S', 'U', 'N', 'G', ' ', ' ', ' ', ' ', ' '},
                                                 {'W', 'I', 'N', 'B', 'O', 'N', 'D', ' ', ' ', ' ', ' ', ' '},
                                                 {'H', 'Y', 'N', 'I', 'X', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                                 {'T', 'O', 'S', 'H', 'I', 'B', 'A', ' ', ' ', ' ', ' ', ' '},
                                                 {'M', 'A', 'C', 'R', 'O', 'N', 'I', 'X', ' ', ' ', ' ', ' '},
                                                 {0}};

semc_mem_nand_handle_t semc_handle;

/*******************************************************************************
 * API
 ******************************************************************************/
/* ONFI parameters CRC related functions. */
static void semc_nand_crc16_onfi_init(crc16_data_t *crc16Info)
{
    assert(crc16Info);

    /* initialize running crc and byte count. */
    crc16Info->currentCrc = 0x4F4EU;
}

static void semc_nand_crc16_onfi_update(crc16_data_t *crc16Info, const uint8_t *src, uint32_t lengthInBytes)
{
    assert(crc16Info);
    assert(src);

    uint32_t crc = crc16Info->currentCrc;

    for (uint32_t i = 0; i < lengthInBytes; ++i)
    {
        uint32_t byte = src[i];
        crc ^= byte << 8;
        for (uint32_t j = 0; j < 8; ++j)
        {
            uint32_t temp = crc << 1;
            if (crc & 0x8000)
            {
                temp ^= 0x8005U;
            }
            crc = temp;
        }
    }

    crc16Info->currentCrc = crc;
}

static void semc_nand_crc16_onfi_finalize(crc16_data_t *crc16Info, uint16_t *hash)
{
    assert(crc16Info);
    assert(hash);

    *hash = crc16Info->currentCrc;
}

static void semc_get_default_timing_configure(semc_nand_timing_config_t *semcNandTimingConfig)
{
    assert(semcNandTimingConfig);

    /* Configure Timing mode 0 for timing parameter.*/
    const nand_ac_timing_parameter_t *nandTiming =
        &s_nandAcTimingParameterTable[kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz];
    semcNandTimingConfig->tCeSetup_Ns        = nandTiming->min_tCS_ns;
    semcNandTimingConfig->tCeHold_Ns         = nandTiming->min_tCH_ns;
    semcNandTimingConfig->tCeInterval_Ns     = nandTiming->min_tCEITV_ns;
    semcNandTimingConfig->tWeLow_Ns          = nandTiming->min_tWP_ns;
    semcNandTimingConfig->tWeHigh_Ns         = nandTiming->min_tWH_ns;
    semcNandTimingConfig->tReLow_Ns          = nandTiming->min_tRP_ns;
    semcNandTimingConfig->tReHigh_Ns         = nandTiming->min_tREH_ns;
    semcNandTimingConfig->tTurnAround_Ns     = nandTiming->min_tTA_ns;
    semcNandTimingConfig->tWehigh2Relow_Ns   = nandTiming->min_tWHR_ns;
    semcNandTimingConfig->tRehigh2Welow_Ns   = nandTiming->min_tRHW_ns;
    semcNandTimingConfig->tAle2WriteStart_Ns = nandTiming->min_tADL_ns;
    semcNandTimingConfig->tReady2Relow_Ns    = nandTiming->min_tRR_ns;
    semcNandTimingConfig->tWehigh2Busy_Ns    = nandTiming->max_tWB_ns;
}

static status_t semc_nand_get_onfi_timing_configure(nand_handle_t *handle, nand_config_t *config)
{
    assert(config->memControlConfig);
    assert(handle->deviceSpecific);

    status_t status = kStatus_Success;
    uint32_t ch;
    uint32_t nandOnfiTag                                 = FOUR_CHAR_CODE('O', 'N', 'F', 'I');
    nand_onfi_parameter_config_t nandOnfiParameterConfig = {0};
    uint8_t acTimingTableIndex                           = 0;
    semc_mem_nand_config_t *semcMemNandConfig            = (semc_mem_nand_config_t *)config->memControlConfig;
    semc_nand_config_t *semcConfig                       = semcMemNandConfig->semcNandConfig;
    semc_mem_nand_handle_t *semcHandle                   = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    /* Read First ONFI parameter data from device */
    status = semc_nand_issue_read_parameter(handle, &nandOnfiParameterConfig);
    if (status != kStatus_Success)
    {
        return status;
    }

    /*
       Validate ONFI parameter:
       From Device Spec, To insure data integrity, device contain more than one copies of the parameter page,
       The Integrity CRC (Cyclic Redundancy Check) field is used to verify that the contents of the parameters page
       were transferred correctly to the host.
    */
    if (nandOnfiParameterConfig.signature == nandOnfiTag)
    {
        /*
           Validate the interity CRC from ONFI spec:
           1. The CRC calculation covers all of data between byte 0 and byte 253 of the parameter page inclusive.
           2. The CRC shall be calculated on byte (8-bit) quantities starting with byte 0 in the parameter page.
              The bits in the 8-bit quantity are processed from the most significant bit (bit 7) to the least
           significant bit (bit 0).
           3. The CRC shall be calculated using the following 16-bit generator polynomial: G(X) = X16 + X15 + X2
           + 1. This polynomial in hex may be represented as 8005h.
           4. The CRC value shall be initialized with a value of 4F4Eh before the calculation begins.
           5. There is no XOR applied to the final CRC value after it is calculated.
           6. There is no reversal of the data bytes or the CRC calculated value.
        */
        uint16_t calculatedCrc   = 0;
        uint8_t *calculatedStart = (uint8_t *)&nandOnfiParameterConfig;
        uint32_t calculatedSize  = sizeof(nandOnfiParameterConfig) - 2U;

        crc16_data_t crc16Info;
        semc_nand_crc16_onfi_init(&crc16Info);
        semc_nand_crc16_onfi_update(&crc16Info, calculatedStart, calculatedSize);
        semc_nand_crc16_onfi_finalize(&crc16Info, &calculatedCrc);

        if (calculatedCrc != nandOnfiParameterConfig.integrityCRC)
        {
            return kStatus_Fail;
        }
    }
    else
    {
        return kStatus_Fail;
    }

    /* Get device vendor */
    handle->vendorType = kNandVendorType_Unknown;
    for (uint8_t vendorIndex = kNandVendorType_Micron; vendorIndex < (uint8_t)kNandVendorType_Unknown; vendorIndex++)
    {
        for (ch = 0; ch < sizeof(s_nandDeviceManufacturerList[vendorIndex]); ch++)
        {
            if (s_nandDeviceManufacturerList[vendorIndex][ch] != nandOnfiParameterConfig.deviceManufacturer[ch])
            {
                break;
            }
        }
        if (ch == sizeof(s_nandDeviceManufacturerList[vendorIndex]))
        {
            handle->vendorType = vendorIndex;
            break;
        }
    }

    /* Set NAND feature/command info in handler */
    semcHandle->isFeatureCommandSupport =
        nandOnfiParameterConfig.optionalCommands.setGetfeatures != 0x00U ? true : false;

    if (nandOnfiParameterConfig.optionalCommands.readStatusEnhanced != 0x00U)
    {
        semcHandle->statusCommandType = (uint8_t)kNandStatusCommandType_Enhanced;
    }
    else
    {
        semcHandle->statusCommandType = (uint8_t)kNandStatusCommandType_Common;
    }

    if (nandOnfiParameterConfig.optionalCommands.changeReadColumnEnhanced != 0x00U)
    {
        semcHandle->changeReadColumnType = (uint8_t)kNandChangeReadColumnCommandType_Enhanced;
    }
    else
    {
        semcHandle->changeReadColumnType = (uint8_t)kNandChangeReadColumnCommandType_Common;
    }

    handle->bytesInPageDataArea     = nandOnfiParameterConfig.dataBytesPerPage;
    handle->bytesInPageSpareArea    = nandOnfiParameterConfig.spareBytesPerPage;
    handle->pagesInBlock            = nandOnfiParameterConfig.pagesPerBlock;
    handle->blocksInPlane           = nandOnfiParameterConfig.blocksPerLUN;
    handle->planesInDevice          = nandOnfiParameterConfig.LUNsPerDevice;
    semcHandle->pageReadTimeInUs_tR = ((uint32_t)nandOnfiParameterConfig.maxPageReadTimeInUs[1] << 8) +
                                      nandOnfiParameterConfig.maxPageReadTimeInUs[0];
    semcHandle->blockEraseTimeInUs_tBERS = ((uint32_t)nandOnfiParameterConfig.maxBlockEraseTimeInUs[1] << 8) +
                                           nandOnfiParameterConfig.maxBlockEraseTimeInUs[0];
    semcHandle->PageProgramTimeInUs_tPROG = ((uint32_t)nandOnfiParameterConfig.maxPageProgramTimeInUs[1] << 8) +
                                            nandOnfiParameterConfig.maxPageProgramTimeInUs[0];
    semcHandle->pageReadTimeInUs_tR >>= 2;
    semcHandle->blockEraseTimeInUs_tBERS >>= 2;
    semcHandle->PageProgramTimeInUs_tPROG >>= 2;
    /* Set change cloumn setup time for AXI access */
    semcHandle->changeColumnSetupTimeInNs_tCCS =
        ((uint32_t)nandOnfiParameterConfig.minChangeColunmSetupTimeInNs[1] << 8) +
        nandOnfiParameterConfig.minChangeColunmSetupTimeInNs[0];

    switch (nandOnfiParameterConfig.addressCycles)
    {
        case 0x22u:
            semcConfig->arrayAddrOption = kSEMC_NandAddrOption_4byte_CA2RA2;
            break;
        case 0x21u:
            semcConfig->arrayAddrOption = kSEMC_NandAddrOption_3byte_CA2RA1;
            break;
        case 0x13u:
            semcConfig->arrayAddrOption = kSEMC_NandAddrOption_4byte_CA1RA3;
            break;
        case 0x12u:
            semcConfig->arrayAddrOption = kSEMC_NandAddrOption_3byte_CA1RA2;
            break;
        case 0x11u:
            semcConfig->arrayAddrOption = kSEMC_NandAddrOption_2byte_CA1RA1;
            break;
        case 0x23u:
        default:
            semcConfig->arrayAddrOption = kSEMC_NandAddrOption_5byte_CA2RA3;
            break;
    }

    /* Get onfi timing mode for timing mode setting only when appliation have no timing configuration. */
    if (semcConfig->timingConfig == NULL)
    {
        const nand_ac_timing_parameter_t *timingArray;

        if (nandOnfiParameterConfig.timingMode.mode5 != 0x00U)
        {
            acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode5_50MHz;
        }
        else if (nandOnfiParameterConfig.timingMode.mode4 != 0x00U)
        {
            acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode4_40MHz;
        }
        else if (nandOnfiParameterConfig.timingMode.mode3 != 0x00U)
        {
            acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode3_33MHz;
        }
        else if (nandOnfiParameterConfig.timingMode.mode2 != 0x00U)
        {
            acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode2_28MHz;
        }
        else if (nandOnfiParameterConfig.timingMode.mode1 != 0x00U)
        {
            acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode1_20MHz;
        }
        else
        {
            acTimingTableIndex = kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz;
        }

        /* Set the semc nand configuration again */
        timingArray                                  = &s_nandAcTimingParameterTable[acTimingTableIndex];
        semcConfig->timingConfig->tCeSetup_Ns        = timingArray->min_tCS_ns;
        semcConfig->timingConfig->tCeHold_Ns         = timingArray->min_tCH_ns;
        semcConfig->timingConfig->tCeInterval_Ns     = timingArray->min_tCEITV_ns;
        semcConfig->timingConfig->tWeLow_Ns          = timingArray->min_tWP_ns;
        semcConfig->timingConfig->tWeHigh_Ns         = timingArray->min_tWH_ns;
        semcConfig->timingConfig->tReLow_Ns          = timingArray->min_tRP_ns;
        semcConfig->timingConfig->tReHigh_Ns         = timingArray->min_tREH_ns;
        semcConfig->timingConfig->tTurnAround_Ns     = timingArray->min_tTA_ns;
        semcConfig->timingConfig->tWehigh2Relow_Ns   = timingArray->min_tWHR_ns;
        semcConfig->timingConfig->tRehigh2Welow_Ns   = timingArray->min_tRHW_ns;
        semcConfig->timingConfig->tAle2WriteStart_Ns = timingArray->min_tADL_ns;
        semcConfig->timingConfig->tReady2Relow_Ns    = timingArray->min_tRR_ns;
        semcConfig->timingConfig->tWehigh2Busy_Ns    = timingArray->max_tWB_ns;

        /* Change the timing mode: on onfi spec, enable EDO mode when using timing mode 4 and 5. */
        if ((acTimingTableIndex == (uint8_t)kNandAcTimingTableIndex_ONFI_1p0_Mode4_40MHz) ||
            (acTimingTableIndex == (uint8_t)kNandAcTimingTableIndex_ONFI_1p0_Mode5_50MHz))
        {
            semcConfig->edoModeEnabled = true;
        }

        if ((acTimingTableIndex > (uint8_t)kNandAcTimingTableIndex_ONFI_1p0_Mode0_10MHz) &&
            (nandOnfiParameterConfig.optionalCommands.setGetfeatures != 0x00U))
        {
            /* Switch to specific timing mode */
            nand_onfi_feature_config_t featureConfig = {
                .command   = kNandDeviceCmd_ONFI_SetFeatures,
                .address   = 0x01, /* Feature address for timing mode. */
                .parameter = {acTimingTableIndex, 0, 0, 0},
            };
            status = semc_nand_issue_access_feature(handle, &featureConfig);
            if (status != kStatus_Success)
            {
                return status;
            }

            /* Get current timing mode to double check */
            featureConfig.command      = kNandDeviceCmd_ONFI_GetFeatures;
            featureConfig.parameter[0] = 0;
            status                     = semc_nand_issue_access_feature(handle, &featureConfig);
            if (status != kStatus_Success)
            {
                return status;
            }

            if (featureConfig.parameter[0] != acTimingTableIndex)
            {
                return kStatus_Fail;
            }
        }
    }

    return status;
}

static status_t semc_nand_wait_status_ready(nand_handle_t *handle, uint32_t readyCheckIntervalInUs, bool readOpen)
{
    status_t status                    = kStatus_Success;
    bool isDeviceReady                 = false;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    if (semcHandle->delayUS == NULL)
    {
        return kStatus_Fail;
    }

    while (!isDeviceReady)
    {
        if (semcHandle->readyCheckOption == (uint8_t)kNandReadyCheckOption_SR)
        {
            /* Get SR value from Device by issuing READ STATUS commmand */
            uint8_t stat = 0;

            semcHandle->delayUS(readyCheckIntervalInUs);
            status = semc_nand_issue_read_status(handle, &stat);
            if (status != kStatus_Success)
            {
                return status;
            }
            /* stat[RDY] = 0, Busy, stat[RDY] = 1, Ready */
            isDeviceReady = (stat & NAND_FLASH_SR_ONFI_READYBITMASK) == 0x00U ? false : true;
        }
        else if (semcHandle->readyCheckOption == (uint8_t)kNandReadyCheckOption_RB)
        {
            /* Monitors the target's R/B# signal to determine the progress. */
            isDeviceReady = SEMC_IsNandReady((SEMC_Type *)handle->driverBaseAddr);
            if (!isDeviceReady)
            {
                semcHandle->delayUS(readyCheckIntervalInUs);
                isDeviceReady = SEMC_IsNandReady((SEMC_Type *)handle->driverBaseAddr);
            }
        }
        else
        {
            ; /* Intentional empty for MISRA c-2012 rule 15.7 */
        }
    }

    /* Note: If the ReadStatus command is used to monitor for command completion, the
    ReadMode command must be used to re-enable data output mode.
    */
    if ((readOpen) && (semcHandle->readyCheckOption == (uint8_t)kNandReadyCheckOption_SR))
    {
        status = semc_nand_issue_read_mode(handle);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
    return kStatus_Success;
}

static status_t semc_nand_issue_reset(nand_handle_t *handle)
{
    status_t status;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    /* The RESET command may be executed with the target in any state */
    uint16_t commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_Reset,
                                                   kSEMC_NANDAM_ColumnRow, // Don't care
                                                   kSEMC_NANDCM_Command);  // Command Only
    status = SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, semcHandle->ctlAccessMemAddr1,
                                commandCode, 0, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* For ONFI 1.0 Timing mode 0, the max tRST = 1000us
       For ONFI 1.O other timing modes, the max tRST = 5/10/500us
      The target is allowed a longer maximum reset time when a program or erase operation is in progress. The maximums
      correspond to:
        1. The target is not performing an erase or program operation.
        2. The target is performing a program operation.
        3. The target is performing an erase operation.
    */
    status = semc_nand_wait_status_ready(handle, NAND_MAX_RST_TIME3_tRST_US, false);

    return status;
}

static status_t semc_nand_set_device_ecc(nand_handle_t *handle)
{
    status_t status;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    if (semcHandle->eccCheckType == kNandEccCheckType_DeviceECC)
    {
        if (handle->vendorType == (uint8_t)kNandVendorType_Micron)
        {
            if (semcHandle->isFeatureCommandSupport)
            {
                nand_onfi_feature_config_t featureConfig = {
                    .command   = kNandDeviceCmd_ONFI_SetFeatures,
                    .address   = kNandDeviceFeature_ArrayOperationMode_Address,
                    .parameter = {kNandDeviceFeature_ArrayOperationMode_EnableECC, 0, 0, 0},
                };
                status = semc_nand_issue_access_feature(handle, &featureConfig);
                if (status != kStatus_Success)
                {
                    return status;
                }
            }
        }
        else if (handle->vendorType == (uint8_t)kNandVendorType_Macronix)
        {
            if (semcHandle->isFeatureCommandSupport)
            {
                /* The internal ECC is always enabled for MX30LF serials. */
            }
        }
        else
        {
            ; /* Intentional empty */
        }
    }
    else
    {
        if (handle->vendorType == (uint8_t)kNandVendorType_Micron)
        {
            if (semcHandle->isFeatureCommandSupport)
            {
                nand_onfi_feature_config_t featureConfig = {
                    .command   = kNandDeviceCmd_ONFI_SetFeatures,
                    .address   = kNandDeviceFeature_ArrayOperationMode_Address,
                    .parameter = {kNandDeviceFeature_ArrayOperationMode_DisableECC, 0, 0, 0},
                };
                status = semc_nand_issue_access_feature(handle, &featureConfig);
                if (status != kStatus_Success)
                {
                    return status;
                }
            }
        }
    }

    return kStatus_Success;
}

static status_t semc_nand_issue_read_mode(nand_handle_t *handle)
{
    status_t status                    = kStatus_Success;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    /* READ MODE command is accepted by device when it is ready (RDY = 1, ARDY = 1). */
    status = semc_nand_wait_status_ready(handle, NAND_READY_CHECK_INTERVAL_NORMAL, false);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* The READ MODE command disables status output and enables data output */
    uint16_t commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_ReadMode,
                                                   kSEMC_NANDAM_ColumnRow, // Don't care
                                                   kSEMC_NANDCM_Command);  // Command Only
    status = SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, semcHandle->ctlAccessMemAddr1,
                                commandCode, 0, NULL);

    return status;
}

static status_t semc_nand_issue_access_feature(nand_handle_t *handle, nand_onfi_feature_config_t *featureConfig)
{
    status_t status                    = kStatus_Success;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    /* SET/GET FEATURES command is accepted by the target only when all die (LUNs) on the target are idle. */
    uint16_t commandCode = SEMC_BuildNandIPCommand(featureConfig->command,
                                                   kSEMC_NANDAM_ColumnCA0,       // CA1
                                                   kSEMC_NANDCM_CommandAddress); // Commmand Address
    status = SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, featureConfig->address,
                                commandCode, 0, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (featureConfig->command == (uint8_t)kNandDeviceCmd_ONFI_SetFeatures)
    {
        status = SEMC_IPCommandNandWrite((SEMC_Type *)handle->driverBaseAddr, 0, featureConfig->parameter,
                                         sizeof(featureConfig->parameter));
        if (status != kStatus_Success)
        {
            return status;
        }
        /* Note: From Spec, both R/B and SR can be used to determine the progress,
            but actually only when we choose R/B, then it works well on the EVB and FPGA. */
        if (semcHandle->readyCheckOption == (uint8_t)kNandReadyCheckOption_RB)
        {
            status = semc_nand_wait_status_ready(handle, NAND_READY_CHECK_INTERVAL_NORMAL, false);
        }
        else
        {
            /* Just delay some time to workaround issue */
            semcHandle->delayUS(NAND_MAX_FEATURE_ACCESS_TIME_tFEAT_US);
        }
    }
    else if (featureConfig->command == (uint8_t)kNandDeviceCmd_ONFI_GetFeatures)
    {
        /* Note: From Spec, both R/B and SR can be used to determine the progress,
            but actually only when we choose R/B, then it works well on the EVB and FPGA. */
        if (semcHandle->readyCheckOption == (uint8_t)kNandReadyCheckOption_RB)
        {
            status = semc_nand_wait_status_ready(handle, NAND_READY_CHECK_INTERVAL_NORMAL, true);
            if (status != kStatus_Success)
            {
                return status;
            }
        }
        else
        {
            /* Just delay some time to workaround issue */
            semcHandle->delayUS(NAND_MAX_FEATURE_ACCESS_TIME_tFEAT_US);
        }

        status = SEMC_IPCommandNandRead((SEMC_Type *)handle->driverBaseAddr, 0, featureConfig->parameter,
                                        sizeof(featureConfig->parameter));
    }
    else
    {
        ; /* Intentional empty */
    }

    return status;
}

static status_t semc_nand_issue_read_parameter(nand_handle_t *handle, nand_onfi_parameter_config_t *parameterConfig)
{
    status_t status = kStatus_Success;
    uint32_t readyCheckIntervalInUs;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    uint16_t commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_ReadParameterPage,
                                                   kSEMC_NANDAM_ColumnCA0,       // 1 byte address
                                                   kSEMC_NANDCM_CommandAddress); // Command Address
    status = SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, 0, commandCode, 0, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    // Note2: ReadStatus may be used to check the status of Read Parameter Page during execution.
    // Note3: Use of the ReadStatusEnhanced command is prohibited during the power-on
    //  Reset command and when OTP mode is enabled. It is also prohibited following
    //  some of the other reset, identification, and configuration operations.
    readyCheckIntervalInUs = (NAND_MAX_READ_PARAMETER_PAGE_TIME_tR_US > semcHandle->pageReadTimeInUs_tR) ?
                                 NAND_MAX_READ_PARAMETER_PAGE_TIME_tR_US :
                                 semcHandle->pageReadTimeInUs_tR;
    if (semcHandle->readyCheckOption == (uint8_t)kNandReadyCheckOption_RB)
    {
        status = semc_nand_wait_status_ready(handle, readyCheckIntervalInUs, true);
        if (status != kStatus_Success)
        {
            return status;
        }
    }
    else
    {
        semcHandle->delayUS(readyCheckIntervalInUs);
    }

    // Only ipg command is supported here
    status = SEMC_IPCommandNandRead((SEMC_Type *)handle->driverBaseAddr, 0, (uint8_t *)parameterConfig,
                                    sizeof(nand_onfi_parameter_config_t));

    return status;
}

status_t semc_nand_issue_read_status(nand_handle_t *handle, uint8_t *stat)
{
    status_t status      = kStatus_Success;
    uint32_t readoutData = 0x00U;
    uint16_t commandCode;
    uint32_t slaveAddress;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    /* Note: If there is only one plane per target, the READ STATUS (70h) command can be used
       to return status following any NAND command.
       Note: In devices that have more than one plane per target, during and following interleaved
       die (multi-plane) operations, the READ STATUS ENHANCED command must be used to select the
       die (LUN) that should report status.
    */
    if (semcHandle->statusCommandType == (uint8_t)kNandStatusCommandType_Enhanced)
    {
        /* READ STATUS ENHANCED command is accepted by all planes in device even when they are busy (RDY = 0).*/
        commandCode  = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_ReadStatusEnhanced,
                                              kSEMC_NANDAM_RawRA0RA1RA2,        // 3 Byte-RA0/RA1/RA2
                                              kSEMC_NANDCM_CommandAddressRead); // Commmand Address Read
        slaveAddress = semcHandle->rowAddressToGetSR;
    }
    else
    {
        /* READ STATUS command is accepted by device even when it is busy (RDY = 0). */
        commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_ReadStatus,
                                              kSEMC_NANDAM_ColumnRow,    // Don't care
                                              kSEMC_NANDCM_CommandRead); // Commmand Read

        /* Note: For those command without address, the address should be valid as well,
          it shouldn't be out of IPG memory space, or SEMC IP will ignore this command.
        */
        slaveAddress = semcHandle->ctlAccessMemAddr1;
    }
    status = SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, slaveAddress, commandCode, 0,
                                &readoutData);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Set SR value according to readout data from Device */
    *stat = (uint8_t)readoutData;

    return kStatus_Success;
}

static status_t semc_nand_issue_read_page(nand_handle_t *handle, uint32_t ipgCmdAddr)
{
    status_t status                    = kStatus_Success;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    /* READ PAGE command is accepted by the device when it is ready (RDY = 1, ARDY = 1). */
    status = semc_nand_wait_status_ready(handle, NAND_READY_CHECK_INTERVAL_NORMAL, false);
    if (status != kStatus_Success)
    {
        return status;
    }

    uint16_t commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_ReadPageSetup,
                                                   kSEMC_NANDAM_ColumnRow,       // Address value
                                                   kSEMC_NANDCM_CommandAddress); // Command Address
    status =
        SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, ipgCmdAddr, commandCode, 0, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_ReadPageConfirm,
                                          kSEMC_NANDAM_ColumnRow,    // Don't care
                                          kSEMC_NANDCM_CommandHold); // Commmand Hold
    status =
        SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, ipgCmdAddr, commandCode, 0, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Monitors the target's R/B# signal or Reads the status register
      to determine the progress of the page data transfer. */
    status = semc_nand_wait_status_ready(handle, semcHandle->pageReadTimeInUs_tR, true);
    if (status != kStatus_Success)
    {
        return status;
    }

    return status;
}

static status_t semc_nand_is_device_ecc_check_passed(nand_handle_t *handle, bool *isEccPassed)
{
    status_t status = kStatus_Success;
    uint8_t SR      = 0;

    /* During READ operations the device executes the internal ECC engine (n-bit detection
        and (n-1)-bit error correction). When the READ operaton is complete, read status bit 0 must
        be checked to determine whether errors larger than n bits have occurred. */

    /* Note1: For MT29 series device: We just need to check SR[PASS] to see the ECC result for
        all types of operation(PROGRAM/ERASE/READ)
       Note2: For S34 series device: Error Detection Code check is a feature that can be used during the
      copy back program operation. For common program/erase, the Status Bit SR[PASS] may be checked. The
      internal write/erase verify detects only errors for 1’s/0's that are not successfully programmed to 0’s/1's.
      */
    status = semc_nand_issue_read_status(handle, &SR);
    if (status == kStatus_Success)
    {
        /* SR[PASS] = 0, Successful PROGRAM/ERASE/READ; SR[PASS] = 1, Error in PROGRAM/ERASE/READ */
        *isEccPassed = (SR & NAND_FLASH_SR_ONFI_PASSBITMASK) == 0x00U ? true : false;
    }
    else
    {
        *isEccPassed = false;
    }

    /* READ MODE command should be issued in case read cycle is folowing. */
    status = semc_nand_issue_read_mode(handle);
    if (status != kStatus_Success)
    {
        return status;
    }

    return kStatus_Success;
}

status_t Nand_Flash_Read_Page_Partial(
    nand_handle_t *handle, uint32_t pageIndex, uint32_t offset_bytes, uint8_t *buffer, uint32_t length)
{
    status_t status     = kStatus_Success;
    uint32_t pageSize   = handle->bytesInPageDataArea + handle->bytesInPageSpareArea;
    uint32_t ipgCmdAddr = pageIndex * (1UL << ((semc_mem_nand_handle_t *)handle->deviceSpecific)->columnWidth);
    uint32_t pageNum    = handle->pagesInBlock * handle->blocksInPlane * handle->planesInDevice;
    bool eccCheckPassed;

    /* Validate given length */
    if (((offset_bytes + length) > pageSize) || (pageIndex >= pageNum))
    {
        return kStatus_Fail;
    }

    /* Issues the page read command to device */
    status = semc_nand_issue_read_page(handle, ipgCmdAddr);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (((semc_mem_nand_handle_t *)handle->deviceSpecific)->eccCheckType == kNandEccCheckType_DeviceECC)
    {
        /* Check NAND Device ECC status if applicable */
        status = semc_nand_is_device_ecc_check_passed(handle, &eccCheckPassed);
        if ((status != kStatus_Success) || (!eccCheckPassed))
        {
            return kStatus_Fail;
        }
    }

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
#if __CORTEX_M == 7
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        DCACHE_InvalidateByRange(((semc_mem_nand_handle_t *)handle->deviceSpecific)->ctlAccessMemAddr2, pageSize);
    }
#elif __CORTEX_M == 4
    if (LMEM_PCCCR_ENCACHE_MASK == (LMEM_PCCCR_ENCACHE_MASK & LMEM->PCCCR))
    {
        DCACHE_InvalidateByRange(((semc_mem_nand_handle_t *)handle->deviceSpecific)->ctlAccessMemAddr2, pageSize);
    }
#endif
#endif

    /* Read data from device through AXI */
    (void)memcpy(buffer,
                 (uint8_t *)(((semc_mem_nand_handle_t *)handle->deviceSpecific)->ctlAccessMemAddr2 + offset_bytes),
                 length);
    while (!SEMC_IsInIdle((SEMC_Type *)handle->driverBaseAddr))
    {
    }

    return status;
}

/* Initialize Parallel NAND Flash device */
status_t Nand_Flash_Init(nand_config_t *config, nand_handle_t *handle)
{
    assert(config);
    assert(handle);

    semc_nand_timing_config_t semcNandTimingConfig;
    semc_mem_nand_config_t *semcConfig = (semc_mem_nand_config_t *)config->memControlConfig;
    semc_mem_nand_handle_t *semcHandle;
    bool setFlag = false;
    status_t status;

    (void)memset(handle, 0, sizeof(nand_handle_t));

    /* Store all needs for NAND operations. */
    handle->deviceSpecific        = (void *)&semc_handle;
    handle->driverBaseAddr        = config->driverBaseAddr;
    semcHandle                    = (semc_mem_nand_handle_t *)handle->deviceSpecific;
    semcHandle->delayUS           = semcConfig->delayUS;
    semcHandle->eccCheckType      = semcConfig->eccCheckType;
    semcHandle->readyCheckOption  = semcConfig->readyCheckOption;
    semcHandle->ctlAccessMemAddr1 = semcConfig->semcNandConfig->ipgAddress;
    semcHandle->ctlAccessMemAddr2 = semcConfig->semcNandConfig->axiAddress;
    semcHandle->columnWidth = (NAND_FLASH_COLUMNBITSNUM_MAX - (uint8_t)semcConfig->semcNandConfig->columnAddrBitNum);
    /* Currently we only support ONFI device. */
    if (semcConfig->onfiVersion == kNandOnfiVersion_None)
    {
        return kStatus_Fail;
    }

    if (semcConfig->semcNandConfig->timingConfig == NULL)
    {
        /* Prepare the NAND configuration part one: get timing parameter in SEMC NAND configure structure. */
        semc_get_default_timing_configure(&semcNandTimingConfig);
        semcConfig->semcNandConfig->timingConfig = &semcNandTimingConfig;
        setFlag                                  = true;
    }

    /* Configure NAND flash. */
    status = SEMC_ConfigureNAND((SEMC_Type *)config->driverBaseAddr, semcConfig->semcNandConfig, semcConfig->clkSrc_Hz);
    if (status != kStatus_Success)
    {
        if (setFlag)
        {
            /* Clear the given timing configure variable. */
            semcConfig->semcNandConfig->timingConfig = NULL;
        }
        return status;
    }

    /* Issues the RESET command to device, make sure that we have clean NAND device status */
    status = semc_nand_issue_reset(handle);
    if (status != kStatus_Success)
    {
        if (setFlag)
        {
            /* Clear the given timing configure variable. */
            semcConfig->semcNandConfig->timingConfig = NULL;
        }
        return status;
    }

    /* Try to read ONFI parameter and reset the configure parameters. */
    status = semc_nand_get_onfi_timing_configure(handle, config);
    if (status != kStatus_Success)
    {
        if (setFlag)
        {
            /* Clear the given timing configure variable. */
            semcConfig->semcNandConfig->timingConfig = NULL;
        }
        return status;
    }

    /* Re-Init SEMC NAND module using new parameter */
    status = SEMC_ConfigureNAND((SEMC_Type *)config->driverBaseAddr, semcConfig->semcNandConfig, semcConfig->clkSrc_Hz);
    if (status != kStatus_Success)
    {
        if (setFlag)
        {
            /* Clear the given timing configure variable. */
            semcConfig->semcNandConfig->timingConfig = NULL;
        }
        return status;
    }

    /* Enable/Disable device ECC if necessary */
    status = semc_nand_set_device_ecc(handle);
    if (status != kStatus_Success)
    {
        if (setFlag)
        {
            /* Clear the given timing configure variable. */
            semcConfig->semcNandConfig->timingConfig = NULL;
        }
        return status;
    }
    /* Clear the given timing configure variable. */
    if (setFlag)
    {
        semcConfig->semcNandConfig->timingConfig = NULL;
    }
    return status;
}

status_t Nand_Flash_Read_Page(nand_handle_t *handle, uint32_t pageIndex, uint8_t *buffer, uint32_t length)
{
    status_t status = kStatus_Success;
    uint32_t ipgCmdAddr;
    bool eccCheckPassed                = false;
    uint32_t pageSize                  = handle->bytesInPageDataArea + handle->bytesInPageSpareArea;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;
    uint32_t pageNum                   = handle->pagesInBlock * handle->blocksInPlane * handle->planesInDevice;

    ipgCmdAddr = pageIndex * (1UL << semcHandle->columnWidth);
    /* Validate given length */
    if ((length > pageSize) || (pageIndex >= pageNum))
    {
        return kStatus_Fail;
    }
    semcHandle->rowAddressToGetSR = ipgCmdAddr;

    /* Issues the page read command to device */
    status = semc_nand_issue_read_page(handle, ipgCmdAddr);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (semcHandle->eccCheckType == kNandEccCheckType_DeviceECC)
    {
        /* Check NAND Device ECC status if applicable */
        status = semc_nand_is_device_ecc_check_passed(handle, &eccCheckPassed);
        if ((status != kStatus_Success) || (!eccCheckPassed))
        {
            return kStatus_Fail;
        }
    }

#if defined(CACHE_MAINTAIN) && CACHE_MAINTAIN
#if __CORTEX_M == 7
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR))
    {
        DCACHE_InvalidateByRange(((semc_mem_nand_handle_t *)handle->deviceSpecific)->ctlAccessMemAddr2, pageSize);
    }
#elif __CORTEX_M == 4
    if (LMEM_PCCCR_ENCACHE_MASK == (LMEM_PCCCR_ENCACHE_MASK & LMEM->PCCCR))
    {
        DCACHE_InvalidateByRange(((semc_mem_nand_handle_t *)handle->deviceSpecific)->ctlAccessMemAddr2, pageSize);
    }
#endif
#endif

    /* Read Page data from device through AXI */
    (void)memcpy(buffer, (uint8_t *)semcHandle->ctlAccessMemAddr2, length);
    while (!SEMC_IsInIdle((SEMC_Type *)handle->driverBaseAddr))
    {
    }

    return status;
}

status_t Nand_Flash_Page_Program(nand_handle_t *handle, uint32_t pageIndex, const uint8_t *src, uint32_t length)
{
    status_t status  = kStatus_Success;
    uint32_t pageNum = handle->pagesInBlock * handle->blocksInPlane * handle->planesInDevice;
    uint32_t ipgCmdAddr;
    bool eccCheckPassed = false;
    uint16_t commandCode;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;

    if ((length > (handle->bytesInPageDataArea + handle->bytesInPageSpareArea)) || (pageIndex >= pageNum))
    {
        return kStatus_Fail;
    }

    ipgCmdAddr                    = pageIndex * (1UL << semcHandle->columnWidth);
    semcHandle->rowAddressToGetSR = ipgCmdAddr;

    /* PROGRAM PAGE command is excuteable when semc in idle. */
    while (!SEMC_IsInIdle((SEMC_Type *)handle->driverBaseAddr))
    {
    }

    /* PROGRAM PAGE command is accepted by the device when it is ready (RDY = 1, ARDY = 1). */
    status = semc_nand_wait_status_ready(handle, NAND_READY_CHECK_INTERVAL_NORMAL, false);
    if (status != kStatus_Success)
    {
        return status;
    }
    commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_ProgramPageSetup,
                                          kSEMC_NANDAM_ColumnRow,       // Address value
                                          kSEMC_NANDCM_CommandAddress); // Command Address
    status =
        SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, ipgCmdAddr, commandCode, 0, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    /* Write Page data to device
       Note: the memory address(row, column address) is always assigned by ipg commmand,
       the given address for axi command is just byte offset of available page array. */
    (void)memcpy((uint8_t *)semcHandle->ctlAccessMemAddr2, (uint8_t *)src, length);
    while (!SEMC_IsInIdle((SEMC_Type *)handle->driverBaseAddr))
    {
    }

    /* Issues the page program command to device */
    commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_ProgramPageConfirm,
                                          kSEMC_NANDAM_ColumnRow, // Don't care
                                          kSEMC_NANDCM_Command);  // Commmand Only
    status =
        SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, ipgCmdAddr, commandCode, 0, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* Monitors the target's R/B# signal or Reads the status register
      to determine the progress of the page data transfer. */
    status = semc_nand_wait_status_ready(handle, semcHandle->PageProgramTimeInUs_tPROG, true);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (semcHandle->eccCheckType == kNandEccCheckType_DeviceECC)
    {
        /* Check NAND Device ECC status if applicable */
        status = semc_nand_is_device_ecc_check_passed(handle, &eccCheckPassed);
        if ((status != kStatus_Success) || (!eccCheckPassed))
        {
            return kStatus_Fail;
        }
    }

    return status;
}

status_t Nand_Flash_Erase_Block(nand_handle_t *handle, uint32_t blockIndex)
{
    status_t status = kStatus_Success;
    uint32_t ipgCmdAddr;
    bool eccCheckPassed                = false;
    semc_mem_nand_handle_t *semcHandle = (semc_mem_nand_handle_t *)handle->deviceSpecific;
    ipgCmdAddr                         = blockIndex * handle->pagesInBlock * (1UL << semcHandle->columnWidth);

    if (blockIndex >= handle->blocksInPlane * handle->planesInDevice)
    {
        return kStatus_Fail;
    }

    semcHandle->rowAddressToGetSR = ipgCmdAddr;

    /* Issues the block erase command to device
      ERASE BLOCK command is accepted by the device when it is ready (RDY = 1, ARDY = 1).*/
    status = semc_nand_wait_status_ready(handle, NAND_READY_CHECK_INTERVAL_NORMAL, false);
    if (status != kStatus_Success)
    {
        return status;
    }
    /* SA = blockIndex * pagesInBlock * pageDataSize */
    uint16_t commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_EraseBlockSetup,
                                                   kSEMC_NANDAM_RawRA0RA1RA2,    // Address value
                                                   kSEMC_NANDCM_CommandAddress); // Command Address
    status =
        SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, ipgCmdAddr, commandCode, 0, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    commandCode = SEMC_BuildNandIPCommand(kNandDeviceCmd_ONFI_EraseBlockConfirm,
                                          kSEMC_NANDAM_RawRA0RA1RA2, // Don't care
                                          kSEMC_NANDCM_CommandHold); // Commmand Hold
    status =
        SEMC_SendIPCommand((SEMC_Type *)handle->driverBaseAddr, kSEMC_MemType_NAND, ipgCmdAddr, commandCode, 0, NULL);
    if (status != kStatus_Success)
    {
        return status;
    }

    status = semc_nand_wait_status_ready(handle, semcHandle->blockEraseTimeInUs_tBERS, false);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (semcHandle->eccCheckType == kNandEccCheckType_DeviceECC)
    {
        /* Check NAND Device ECC status if applicable */
        status = semc_nand_is_device_ecc_check_passed(handle, &eccCheckPassed);
        if ((status != kStatus_Success) || (!eccCheckPassed))
        {
            return kStatus_Fail;
        }
    }

    return kStatus_Success;
}
