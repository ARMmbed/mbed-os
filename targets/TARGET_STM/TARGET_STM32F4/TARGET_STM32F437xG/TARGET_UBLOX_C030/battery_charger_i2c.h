#ifndef BATTERY_CHARGER_I2C
#define BATTERY_CHARGER_I2C

#include "hal/i2c_api.h"

#ifdef __cplusplus
extern"C"{
#endif

class BatteryChargerI2c{

public:
    BatteryChargerI2c(PinName sda, PinName scl);
    bool read_from_i2c(int i2c_address, char *data_read, int length);
    bool write_to_i2c(int i2c_address, const char *data_write, int length);
    virtual ~BatteryChargerI2c() {}

private:
    i2c_t _i2c;
    int   _hz;
};

#ifdef __cplusplus
}
#endif

#endif //BATTERY_CHARGER_I2C
