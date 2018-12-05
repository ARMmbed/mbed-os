/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed_assert.h"
#include "i2c_api.h"

#if DEVICE_I2C

#include "cmsis.h"
#include "pinmap.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "peripheral_clock_defines.h"
#include "PeripheralPins.h"

/* Array of I2C peripheral base address. */
static I2C_Type *const i2c_addrs[] = I2C_BASE_PTRS;
/* Array of I2C bus clock frequencies */
static clock_name_t const i2c_clocks[] = I2C_CLOCK_FREQS;

void i2c_get_capabilities(i2c_capabilities_t *capabilities)
{
  if (capabilities == NULL) {
    return;
  }

  capabilities->minimum_frequency         = 1;
  capabilities->maximum_frequency         = 1000000;
  capabilities->supports_slave_mode       = true;
  capabilities->supports_10bit_addressing = true;
  capabilities->supports_multi_master     = true;
  capabilities->supports_clock_stretching = false;
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl, bool is_slave)
{
  uint32_t i2c_sda = pinmap_peripheral(sda, PinMap_I2C_SDA);
  uint32_t i2c_scl = pinmap_peripheral(scl, PinMap_I2C_SCL);

  PORT_Type *port_addrs[] = PORT_BASE_PTRS;
  PORT_Type *base = port_addrs[sda >> GPIO_PORT_SHIFT];

  obj->instance = pinmap_merge(i2c_sda, i2c_scl);
  obj->next_repeated_start = 0;
  obj->is_slave = is_slave;

  MBED_ASSERT((int)obj->instance != NC);

  const uint32_t clock_frequency = CLOCK_GetFreq(i2c_clocks[obj->instance]);

  if (is_slave) {
    i2c_slave_config_t slave_config;

    I2C_SlaveGetDefaultConfig(&slave_config);
    slave_config.slaveAddress = 0;
    slave_config.enableSlave  = true;

#if FSL_I2C_DRIVER_VERSION > MAKE_VERSION(2, 0, 1)
    I2C_SlaveInit(i2c_addrs[obj->instance], &slave_config, clock_frequency);
#else
    I2C_SlaveInit(i2c_addrs[obj->instance], &slave_config);
#endif
  } else {
    i2c_master_config_t master_config;

    I2C_MasterGetDefaultConfig(&master_config);
    master_config.enableMaster = true;

    I2C_MasterInit(i2c_addrs[obj->instance], &master_config, clock_frequency);
    I2C_EnableInterrupts(i2c_addrs[obj->instance], kI2C_GlobalInterruptEnable);
  }

  pinmap_pinout(sda, PinMap_I2C_SDA);
  pinmap_pinout(scl, PinMap_I2C_SCL);

#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
  base->PCR[sda & 0xFF] |= PORT_PCR_ODE_MASK;
  base->PCR[scl & 0xFF] |= PORT_PCR_ODE_MASK;
#else
  /* Enable internal pullup resistor */
  base->PCR[sda & 0xFF] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
  base->PCR[scl & 0xFF] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
#endif
}

void i2c_free(i2c_t *obj)
{
  I2C_Type *base = i2c_addrs[obj->instance];

  if (obj->is_slave) {
    I2C_SlaveDeinit(base);
  } else {
    I2C_MasterDeinit(base);
  }
}

bool i2c_start(i2c_t *obj)
{
  I2C_Type *base = i2c_addrs[obj->instance];

  const uint32_t statusFlags = I2C_MasterGetStatusFlags(base);

  /* Check if the bus is already in use. */
  if ((statusFlags & kI2C_BusBusyFlag) != 0) {
    /* Send a repeat START signal. */
    base->C1 |= I2C_C1_RSTA_MASK;
  } else {
    /* Send the START signal. */
    base->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK;
  }

#if defined(FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING) && FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING
  while (!(base->S2 & I2C_S2_EMPTY_MASK))
  {
  }
#endif /* FSL_FEATURE_I2C_HAS_DOUBLE_BUFFERING */

  return 0;
}

bool i2c_stop(i2c_t *obj)
{
  I2C_Type *base = i2c_addrs[obj->instance];

  return ((I2C_MasterStop(base)) == kStatus_Success);
}

uint32_t i2c_frequency(i2c_t *obj, uint32_t frequency)
{
  I2C_Type *base = i2c_addrs[obj->instance];

  const uint32_t busClock = CLOCK_GetFreq(i2c_clocks[obj->instance]);

  I2C_MasterSetBaudRate(base, frequency, busClock);

  return frequency;
}

void i2c_timeout(i2c_t *obj, uint32_t timeout)
{
  (void)obj;
  (void)timeout;
}

static int i2c_block_read(i2c_t *obj, uint16_t address, void *data, uint32_t length, bool last)
{
  I2C_Type *base = i2c_addrs[obj->instance];

  i2c_master_transfer_t transfer;
  memset(&transfer, 0, sizeof(transfer));
  transfer.slaveAddress = (address >> 1);
  transfer.direction    = kI2C_Read;
  transfer.data         = (uint8_t *)data;
  transfer.dataSize     = length;

  if (obj->next_repeated_start)
    transfer.flags |= kI2C_TransferRepeatedStartFlag;

  if (!last)
    transfer.flags |= kI2C_TransferNoStopFlag;

  obj->next_repeated_start = (transfer.flags & kI2C_TransferNoStopFlag) ? 1 : 0;

  // The below function will issue a STOP signal at the end of the transfer.
  // This is required by the hardware in order to receive the last byte
  if (I2C_MasterTransferBlocking(base, &transfer) != kStatus_Success)
    return I2C_ERROR_NO_SLAVE;

  return length;
}

#if DEVICE_I2CSLAVE

static int i2c_slave_read(i2c_t *obj, void *data, uint32_t length)
{
  I2C_Type *base = i2c_addrs[obj->instance];

  if (base->S & kI2C_AddressMatchFlag) {
    /* Slave receive, master writing to slave. */
    base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
    /* Read dummy to release the bus. */
    base->D;
  }

  I2C_SlaveReadBlocking(base, (uint8_t *)data, length);

  return length;
}

i2c_slave_status_t i2c_slave_status(i2c_t *obj)
{
  uint32_t status_flags = I2C_SlaveGetStatusFlags(i2c_addrs[obj->instance]);

  if ((status_flags & kI2C_AddressMatchFlag) == 0) {
    return NoData;
  }

  return ((status_flags & kI2C_TransferDirectionFlag) != 0) ? ReadAddressed
                                                            : WriteAddressed;
}

static int i2c_slave_write(i2c_t *obj, const void *data, uint32_t length)
{
  I2C_Type *base = i2c_addrs[obj->instance];

  I2C_SlaveWriteBlocking(base, (uint8_t *)data, length);

  /* Switch to receive mode. */
  base->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
  /* Read dummy to release bus. */
  base->D;

  return length;
}

void i2c_slave_address(i2c_t *obj, uint16_t address)
{
  i2c_addrs[obj->instance]->A1 = address & 0xfe;
}
#endif // DEVICE_I2CSLAVE

int32_t i2c_read(i2c_t *obj, uint16_t address, void *data, uint32_t length,
                 bool last)
{
  if ((length == 0) || (data == NULL)) {
    return 0;
  }

#if DEVICE_I2CSLAVE
  /* Slave block read */
  if (obj->is_slave) {
    return i2c_slave_read(obj, data, length);
  }
#endif // DEVICE_I2CSLAVE

  /* Master block read */
  return i2c_block_read(obj, address, data, length, last);
}

const PinMap *i2c_master_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

const PinMap *i2c_slave_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_slave_scl_pinmap()
{
    return PinMap_I2C_SCL;
}

int i2c_block_write(i2c_t *obj, uint16_t address, const void *data, uint32_t length, bool stop)
{
  I2C_Type *base = i2c_addrs[obj->instance];

  i2c_master_transfer_t master_xfer;

  if (length == 0) {
    if (I2C_MasterStart(base, address >> 1, kI2C_Write) != kStatus_Success) {
      return I2C_ERROR_NO_SLAVE;
    }

    while (!(base->S & kI2C_IntPendingFlag)) {
    }

    base->S = kI2C_IntPendingFlag;

    if (base->S & kI2C_ReceiveNakFlag) {
      i2c_stop(obj);
      return I2C_ERROR_NO_SLAVE;
    } else {
      i2c_stop(obj);
      return length;
    }
  }

  memset(&master_xfer, 0, sizeof(master_xfer));
  master_xfer.slaveAddress = address >> 1;
  master_xfer.direction = kI2C_Write;
  master_xfer.data = (uint8_t *)data;
  master_xfer.dataSize = length;

  if (obj->next_repeated_start) {
    master_xfer.flags |= kI2C_TransferRepeatedStartFlag;
  }

  if (!stop) {
    master_xfer.flags |= kI2C_TransferNoStopFlag;
  }

  obj->next_repeated_start =
      master_xfer.flags & kI2C_TransferNoStopFlag ? 1 : 0;

  if (I2C_MasterTransferBlocking(base, &master_xfer) != kStatus_Success) {
    return I2C_ERROR_NO_SLAVE;
  }

  return length;
}

int32_t i2c_write(i2c_t *obj, uint16_t address, const void *data,
                  uint32_t length, bool stop)
{
  if ((length == 0) || (data == NULL)) {
    return 0;
  }

#if DEVICE_I2CSLAVE
  /* Slave block write */
  if (obj->is_slave) {
    return i2c_slave_write(obj, data, length);
  }
#endif // DEVICE_I2CSLAVE

  /* Master block write */
  return i2c_block_write(obj, address, data, length, stop);
}

#endif // DEVICE_I2C
