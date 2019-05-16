#include "crc_api.h"
#include "device.h"

#include "platform/mbed_assert.h"

#if DEVICE_CRC

static CRC_HandleTypeDef current_state;
static uint32_t final_xor;
static uint32_t crc_mask;

/*  STM32 CRC preipheral features
  +-------------------------+-----------------------+---------------+---------------+
  | Feature                 | F1/L1/F2/F4 series    | F0 series (#1)| L0/F3/L4/F7   |
  +-========================+=======================+===============+===============+
  | Reversibility option    |          NO           |              YES              |
  | on I/O data             |                       |                               |
  +-------------------------+-----------------------+---------------+---------------+
  | CRC initial Value       | Fixed to 0xFFFFFFFF   | Programmable  | Programmable  |
  |                         |                       | on 32 bits    | on 8,16,32    |
  +-------------------------+-----------------------+---------------+---------------+
  | Handled data size in bit|                  32                   | 8,16,32       |
  +-------------------------+---------------------------------------+---------------+
  | Polynomial size in bit  |                  32                   | 7,8,16,32     |
  +-------------------------+---------------------------------------+---------------+
  | Polynomial coefficients |         Fixed to 0x4C11DB7            | Programmable  |
  +-------------------------+---------------------------------------+---------------+

  #1 The STM32F0 series which supported polynomial in 7, 8, 16, 32 bits as below list:
     STM32F071xB
     STM32F072xB
     STM32F078xx
     STM32F091xC
     STM32F098xx
*/
bool hal_crc_is_supported(const crc_mbed_config_t *config)
{
    if (config == NULL) {
        return false;
    }

#if defined(TARGET_STM32F1) || defined(TARGET_STM32F2) || defined(TARGET_STM32F4) || defined(TARGET_STM32L1)
    /* Currently, mbed supported input data format fix on bytes,
       so those devices are not supported at default. */
    return false;
#elif !defined(CRC_POLYLENGTH_7B)
    /* Some targets are not support polynomial in 7, 8, 16 bits, ex. STM32F070RB,
       so those devices are not supported at default. */
    return false;
#else
    if (config->width != 32 && config->width != 16 && config->width != 8 && config->width != 7) {
        return false;
    }
    return true;
#endif
}

static uint32_t get_crc_mask(int width)
{
    return (width < 8 ? ((1u << 8) - 1) : (uint32_t)((uint64_t)(1ull << width) - 1));
}

void hal_crc_compute_partial_start(const crc_mbed_config_t *config)
{
    MBED_ASSERT(hal_crc_is_supported(config));

    __HAL_RCC_CRC_CLK_ENABLE();

    final_xor = config->final_xor;
    crc_mask = get_crc_mask(config->width);

    current_state.Instance                     = CRC;
#if !defined(TARGET_STM32F1) && !defined(TARGET_STM32F2) && !defined(TARGET_STM32F4) && !defined(TARGET_STM32L1) && defined(CRC_POLYLENGTH_7B)
    current_state.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;
    current_state.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE;
    current_state.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
    current_state.Init.InitValue               = config->initial_xor;
    current_state.Init.GeneratingPolynomial    = config->polynomial;

    switch (config->width) {
        case HAL_CRC_LENGTH_32B:
            current_state.Init.CRCLength = CRC_POLYLENGTH_32B;
            break;
        case HAL_CRC_LENGTH_16B:
            current_state.Init.CRCLength = CRC_POLYLENGTH_16B;
            break;
        case HAL_CRC_LENGTH_8B:
            current_state.Init.CRCLength = CRC_POLYLENGTH_8B;
            break;
        case HAL_CRC_LENGTH_7B:
            current_state.Init.CRCLength = CRC_POLYLENGTH_7B;
            break;
        default:
            MBED_ASSERT(false);
            break;
    }

    current_state.Init.InputDataInversionMode  =
        config->reflect_in ? CRC_INPUTDATA_INVERSION_BYTE
        : CRC_INPUTDATA_INVERSION_NONE;
    current_state.Init.OutputDataInversionMode =
        config->reflect_out ? CRC_OUTPUTDATA_INVERSION_ENABLE
        : CRC_OUTPUTDATA_INVERSION_DISABLE;
#endif

    if (HAL_CRC_Init(&current_state) != HAL_OK) {
        MBED_ASSERT(false);
    }
}

void hal_crc_compute_partial(const uint8_t *data, const size_t size)
{
    if (data && size) {
        HAL_CRC_Accumulate(&current_state, (uint32_t *)data, size);
    }
}

uint32_t hal_crc_get_result(void)
{
    uint32_t result = current_state.Instance->DR;

#if !defined(TARGET_STM32F1) && !defined(TARGET_STM32F2) && !defined(TARGET_STM32F4) && !defined(TARGET_STM32L1) && defined(CRC_POLYLENGTH_7B)
    /* The CRC-7 SD needs to shift left by 1 bit after obtaining the result, but the output
     * inversion of CRC peripheral will convert the result before shift left by 1 bit, so
     * the result seems to have shifted after the conversion.
     *
     * Example:
     *  [Gerenal setps]
     *  1. Before output inversion: 0x75 (0111 0101)
     *  2. Left shift by 1 bit:     0xEA (1110 1010)
     *  3. After output inversion:  0x57 (0101 0111)
     *
     *  [STM32 CRC peripheral steps]
     *  1. Before output inversion: 0x75 (0111 0101)
     *  2. After output inversion:  0x57 (0101 0111) <= no needs shift again
     */
    if (current_state.Init.CRCLength == CRC_POLYLENGTH_7B &&
            current_state.Init.GeneratingPolynomial == POLY_7BIT_SD &&
            current_state.Init.OutputDataInversionMode == CRC_OUTPUTDATA_INVERSION_DISABLE) {
        result = result << 1;
    }
#endif
    return (result ^ final_xor) & crc_mask;
}

#endif // DEVICE_CRC
