

#include "i2c_api.h"
#include "BlueNRG1_sysCtrl.h"
#include "BlueNRG1_i2c.h"

#if DEVICE_I2C

void i2c_init(i2c_t *obj, PinName sda, PinName scl){}

void i2c_frequency(i2c_t *obj, int hz){}

int  i2c_start(i2c_t *obj){}

int  i2c_stop(i2c_t *obj){}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop){}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop){}

void i2c_reset(i2c_t *obj){}

int i2c_byte_read(i2c_t *obj, int last){}

int i2c_byte_write(i2c_t *obj, int data){}

#ifdef ANTONIO_armclang
const PinMap *i2c_master_sda_pinmap(void){}

const PinMap *i2c_master_scl_pinmap(void){}

const PinMap *i2c_slave_sda_pinmap(void){}

const PinMap *i2c_slave_scl_pinmap(void){}
#endif //ANTONIO_armclang


#if DEVICE_I2CSLAVE

void i2c_slave_mode(i2c_t *obj, int enable_slave){}

int  i2c_slave_receive(i2c_t *obj){}

int  i2c_slave_read(i2c_t *obj, char *data, int length){}

int  i2c_slave_write(i2c_t *obj, const char *data, int length){}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask){}

#endif //DEVICE_I2CSLAVE
#endif //DEVICE_I2C
