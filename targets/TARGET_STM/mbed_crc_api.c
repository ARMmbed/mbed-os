#include "crc_api.h"
#include "device.h"

#include "platform/mbed_assert.h"

#ifdef DEVICE_CRC

static CRC_HandleTypeDef current_state;
static uint32_t final_xor;

bool hal_crc_is_supported(const crc_mbed_config_t *config)
{
    if (config == NULL) {
        return false;
    }

#if !defined(STM32L4)
    if (config->polynomial != POLY_32BIT_ANSI) {
        return false;
    }

    if (config->width != 32) {
        return false;
    }

    if ((config->final_xor != 0xFFFFFFFFU) && (config->final_xor != 0)) {
        return false;
    }
#endif
    return true;
}

void hal_crc_compute_partial_start(const crc_mbed_config_t *config)
{
    MBED_ASSERT(hal_crc_is_supported(config));

    __HAL_RCC_CRC_CLK_ENABLE();

    final_xor = config->final_xor;

    current_state.Instance                     = CRC;
    current_state.InputDataFormat              = CRC_INPUTDATA_FORMAT_BYTES;
    current_state.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_DISABLE;
    current_state.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_DISABLE;
    current_state.Init.InitValue               = config->initial_xor;
    current_state.Init.GeneratingPolynomial    = config->polynomial;

    switch (config->width) {
        case 32:
            current_state.Init.CRCLength = CRC_POLYLENGTH_32B;
            current_state.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
            break;
#if defined(STM32L4)
        case 16:
            current_state.Init.CRCLength = CRC_POLYLENGTH_16B;
            break;
        case 8:
            current_state.Init.CRCLength = CRC_POLYLENGTH_8B;
            break;
        case 7:
            current_state.Init.CRCLength = CRC_POLYLENGTH_7B;
            break;
#endif
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
    const uint32_t result = current_state.Instance->DR;

    return (final_xor == 0xFFFFFFFFU) ? ~result : result;
}

#endif // DEVICE_CRC
