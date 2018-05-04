#include "crc_api.h"

#include "drivers/fsl_crc.h"
#include "platform/mbed_assert.h"

#ifdef DEVICE_CRC

static crc_bits_t width;

bool hal_crc_is_supported(const crc_mbed_config_t* config)
{
  if (config == NULL)
    return false;

  if ((config->width != 32) || (config->width != 16))
    return false;

  return true;
}

void hal_crc_compute_partial_start(const crc_mbed_config_t* config)
{
  if (config == NULL)
    return;

  width = ((config->polynomial & 0xFFFF0000U) != 0) ? kCrcBits32 : kCrcBits16;

  crc_config_t platform_config;
  platform_config.polynomial         = config->polynomial;
  platform_config.seed               = config->initial_xor;
  platform_config.reflectIn          = config->reflect_in;
  platform_config.reflectOut         = config->reflect_out;
  platform_config.complementChecksum = true;
  platform_config.crcBits            = width;
  platform_config.crcResult          = kCrcFinalChecksum;

  CRC_Init(CRC0, &platform_config);
}

void hal_crc_compute_partial(const uint8_t *data, const size_t size)
{
  if (data == NULL)
    return;

  if (size == 0)
    return;

  CRC_WriteData(CRC0, data, size);
}

uint32_t hal_crc_get_result(void)
{
  switch (width)
  {
    case kCrcBits16:
      return CRC_Get16bitResult(CRC0);
    case kCrcBits32:
      return CRC_Get32bitResult(CRC0);
    default:
      MBED_ASSERT("Unhandled switch case");
      return 0;
  }
}

#endif // DEVICE_CRC
