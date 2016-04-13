#include "mbed.h"
#include "test_env.h"

#if !DEVICE_I2CSLAVE
  #error [NOT_SUPPORTED] I2C Slave is not supported
#endif

#define SIZE (10)
#define ADDR (0x90)

#if defined(TARGET_KL25Z)
I2CSlave slave(PTE0, PTE1);
#elif defined(TARGET_LPC4088)
I2CSlave slave(p9, p10);
#elif defined(TARGET_SAMR21G18A)
I2CSlave slave(PA16, PA17);
#elif defined(TARGET_SAMD21J18A) || defined(TARGET_SAMD21G18A) || defined(TARGET_SAML21J18A)
I2CSlave slave(PA08, PA09);
#elif defined(TARGET_FF_ARDUINO)
I2CSlave slave(I2C_SDA, I2C_SCL);
#elif defined(TARGET_SAMG55J19)
I2CSlave slave(PA10, PA09);
#else
I2CSlave slave(p28, p27);
#endif

 int main() {
     char buf[SIZE];

     slave.address(ADDR);

     while (1) {
         int i = slave.receive();
         switch (i) {
             case I2CSlave::ReadAddressed:
                 slave.write(buf, SIZE);
				 for(int i = 0; i < SIZE; i++){
				}
                 break;
             case I2CSlave::WriteAddressed:
                 slave.read(buf, SIZE);
                 for(int i = 0; i < SIZE; i++){
					buf[i]++;
				}
                 break;
         }
     }
 }
