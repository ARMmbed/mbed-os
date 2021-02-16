/******************************************************************************
 * INCLUDE
 ******************************************************************************/

#include "mbed_assert.h"
#include "i2c_api.h"
#include "pinmap.h"
#include "PeripheralPins.h"

/******************************************************************************
 * CONST
 ******************************************************************************/

static unsigned int const DEFAULT_I2C_BAUDRATE = 100 * 1000; /* 100 kHz */

/******************************************************************************
 * FUNCTION DEFINITION
 ******************************************************************************/

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    /* Verify if both pins belong to the same I2C peripheral. */
    I2CName const i2c_sda = (I2CName)pinmap_peripheral(sda, PinMap_I2C_SDA);
    I2CName const i2c_scl = (I2CName)pinmap_peripheral(scl, PinMap_I2C_SCL);
    MBED_ASSERT(i2c_sda == i2c_scl);

    /* Obtain the pointer to the I2C hardware instance. */
    obj->dev = (i2c_inst_t *)pinmap_function(sda, PinMap_I2C_SDA);
    obj->baudrate = DEFAULT_I2C_BAUDRATE;

    /* Initialize the I2C module. */
    _i2c_init(obj->dev, obj->baudrate);

    /* Configure GPIO for I2C as alternate function. */
    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);

    /* Enable pull-ups for I2C pins. */
    gpio_pull_up(sda);
    gpio_pull_up(scl);
}

void i2c_frequency(i2c_t *obj, int hz)
{
    obj->baudrate = i2c_set_baudrate(obj->dev, hz);
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    int const bytes_read = i2c_read_blocking(obj->dev,
                                             (uint8_t)address,
                                             (uint8_t *)data,
                                             (size_t)length,
                                             /* nostop = */(stop == 0));
    if (bytes_read < 0)
        return I2C_ERROR_NO_SLAVE;
    else
        return bytes_read;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    int const bytes_written = i2c_write_blocking(obj->dev,
                                                 address,
                                                 (const uint8_t *)data,
                                                 (size_t)length,
                                                 /* nostop = */(stop == 0));
    if (bytes_written < 0)
        return I2C_ERROR_NO_SLAVE;
    else
        return bytes_written;
}

void i2c_reset(i2c_t *obj)
{
    i2c_deinit(obj->dev);
    _i2c_init(obj->dev, obj->baudrate);
}

const PinMap *i2c_master_sda_pinmap()
{
    return PinMap_I2C_SDA;
}

const PinMap *i2c_master_scl_pinmap()
{
    return PinMap_I2C_SCL;
}
