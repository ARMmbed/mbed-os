

#include "i2c_api.h"
#include "BlueNRG1_sysCtrl.h"
#include "BlueNRG1_i2c.h"

void i2c_init(i2c_t *obj, PinName sda, PinName scl){

}

void i2c_frequency(i2c_t *obj, int hz){

}

int  i2c_start(i2c_t *obj){
}

int  i2c_stop(i2c_t *obj){

}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop){
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop){

}

void i2c_reset(i2c_t *obj){

}

/** Read one byte
 *
 *  @param obj The I2C object
 *  @param last Acknoledge
 *  @return The read byte
 */
int i2c_byte_read(i2c_t *obj, int last){

}

/** Write one byte
 *
 *  @param obj The I2C object
 *  @param data Byte to be written
 *  @return 0 if NAK was received, 1 if ACK was received, 2 for timeout.
 */
int i2c_byte_write(i2c_t *obj, int data){

}

#ifdef ANTONIO_armclang
const PinMap *i2c_master_sda_pinmap(void){}

const PinMap *i2c_master_scl_pinmap(void){}

const PinMap *i2c_slave_sda_pinmap(void){}

const PinMap *i2c_slave_scl_pinmap(void){}
#endif //ANTONIO_armclang


#if DEVICE_I2CSLAVE

/**
 * \defgroup SynchI2C Synchronous I2C Hardware Abstraction Layer for slave
 * @{
 */

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param enable_slave Enable i2c hardware so you can receive events with ::i2c_slave_receive
 *  @return non-zero if a value is available
 */
void i2c_slave_mode(i2c_t *obj, int enable_slave);

/** Check to see if the I2C slave has been addressed.
 *  @param obj The I2C object
 *  @return The status - 1 - read addresses, 2 - write to all slaves,
 *         3 write addressed, 0 - the slave has not been addressed
 */
int  i2c_slave_receive(i2c_t *obj);

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for receiving
 *  @param length  Number of bytes to read
 *  @return non-zero if a value is available
 */
int  i2c_slave_read(i2c_t *obj, char *data, int length);

/** Configure I2C as slave or master.
 *  @param obj The I2C object
 *  @param data    The buffer for sending
 *  @param length  Number of bytes to write
 *  @return non-zero if a value is available
 */
int  i2c_slave_write(i2c_t *obj, const char *data, int length);

/** Configure I2C address.
 *  @param obj     The I2C object
 *  @param idx     Currently not used
 *  @param address The address to be set
 *  @param mask    Currently not used
 */
void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask);

#endif
