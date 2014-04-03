#include "test_env.h"

/******************************************************************************
This will test an I2C EEPROM connected to mbed by writing a predefined byte at
address 0 and then reading it back and comparing it with the known byte value a
number of times. This test was written specifically for reproducing the bug
reported here:

https://mbed.org/forum/bugs-suggestions/topic/4128/

Test configuration:

* set 'ntests' to the number of iterations
* set 'i2c_speed_hz' to the desired speed of the I2C interface
* set 'i2c_delay_us' to the delay that will be inserted between 'write' and
'read' I2C operations (https://mbed.org/users/mbed_official/code/mbed/issues/1
for more details). '0' disables the delay.
* define I2C_EEPROM_VERBOSE to get verbose output

The test ran with a 24LC256 external EEPROM memory, but any I2C EEPROM memory
that uses two byte addresses should work.
******************************************************************************/

// Test configuration block
static const int ntests = 10000;
static const int i2c_freq_hz = 400000;
static const int i2c_delay_us = 0;
#define I2C_EEPROM_VERBOSE
// End of test configuration block

#if defined(TARGET_KL25Z)
I2C i2c(PTE0, PTE1);
#elif defined(TARGET_NUCLEO_F103RB)
I2C i2c(I2C_SDA, I2C_SCL);
#elif defined(TARGET_K64F)
I2C i2c(PTE25, PTE24);
#else
I2C i2c(p28, p27);
#endif

#ifdef I2C_EEPROM_VERBOSE
#define dprintf printf
#else
int dprintf(const char* args, ...) {
  return 0;
}
#endif

int main() {
  const int addr = 0xA0;
  const char mark = 0x66;
  char data[3];
  int fw = 0, fr = 0, fc = 0;
  int i2c_stat;
  
  i2c.frequency(i2c_freq_hz);
  
  // Data write
  data[0] = data[1] = 0;
  data[2] = mark;
  if((i2c_stat = i2c.write(addr, data, 3)) != 0) {
    dprintf("Unable to write data to EEPROM (i2c_stat = 0x%02X), aborting\r\n", i2c_stat);
    notify_completion(false);
    return 1;
  }
  // ACK polling (assumes write will be successful eventually)
  while(i2c.write(addr, data, 0) != 0);

  // Data read (actual test)
  for(int i = 0; i < ntests; i ++)
  {
    data[0] = data[1] = 0;
    if((i2c_stat = i2c.write(addr, data, 2, true)) != 0)
    {
      dprintf("Test %d failed at write, i2c_stat is 0x%02X\r\n", i, i2c_stat);
      fw ++;
      continue;
    }
    if(i2c_delay_us != 0)
      wait_us(i2c_delay_us);
    if((i2c_stat = i2c.read(addr, data, 1)) != 0)
    {
      dprintf("Test %d failed at read, i2c_stat is 0x%02X\r\n", i, i2c_stat);
      fr ++;
      continue; 
    }
    if(data[0] != mark)
    {
      dprintf("Test %d failed at data match\r\n", i);
      fc ++;
    }
  }
  dprintf("Test finished.\r\n");
  if(fw + fr + fc == 0)
    dprintf("No failures in %d tests.\r\n", ntests);
  else
  {
    dprintf("Statistics:\r\n");
    dprintf("  Total tests:     %d\r\n", ntests);
    dprintf("  Failed at write: %d\r\n", fw);
    dprintf("  Failed at read:  %d\r\n", fr);
    dprintf("  Data mismatch:   %d\r\n", fc);
    dprintf("  Total failures:  %d\r\n", fw + fr + fc);
    notify_completion(false);
  }

  notify_completion(true);
}
