#include "crc_api.h"

#include "drivers/fsl_crc.h"
#include "platform/mbed_assert.h"

#ifdef DEVICE_CRC

static crc_bits_t width;

bool hal_crc_is_supported(const uint32_t polynomial)
{
  switch (polynomial)
  {
    case POLY_16BIT_CCITT:
    case POLY_16BIT_IBM:
    case POLY_32BIT_ANSI:
      return true;
    case POLY_8BIT_CCITT:
    case POLY_7BIT_SD:
      return false;
    default:
      return false;
  }
}

void hal_crc_compute_partial_start(const uint32_t polynomial)
{
  crc_config_t config;

  switch (polynomial)
  {
    case POLY_32BIT_ANSI:
    {
      width = kCrcBits32;

      config.polynomial         = polynomial;
      config.seed               = 0xFFFFFFFFU;
      config.reflectIn          = true;
      config.reflectOut         = true;
      config.complementChecksum = true;
      config.crcBits            = width;
      config.crcResult          = kCrcFinalChecksum;

      break;
    }
    case POLY_16BIT_IBM:
    {
      width = kCrcBits16;

      config.polynomial         = polynomial;
      config.seed               = 0;
      config.reflectIn          = true;
      config.reflectOut         = true;
      config.complementChecksum = false;
      config.crcBits            = width;
      config.crcResult          = kCrcFinalChecksum;

      break;
    }
    case POLY_16BIT_CCITT:
    {
      width = kCrcBits16;

      config.polynomial         = polynomial;
      config.seed               = 0xFFFFFFFFU;
      config.reflectIn          = false;
      config.reflectOut         = false;
      config.complementChecksum = false;
      config.crcBits            = width;
      config.crcResult          = kCrcFinalChecksum;

      break;
    }
    default:
      MBED_ASSERT("Configuring Mbed CRC with unsupported polynomial");

      return;
  }

  CRC_Init(CRC0, &config);
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
