#include "mbed.h"
#include "test_env.h"
#include <stdio.h>

#if !DEVICE_I2C
  #error [NOT_SUPPORTED] I2C is not supported
#endif

#if !DEVICE_I2CSLAVE
  #error [NOT_SUPPORTED] I2C Slave is not supported
#endif

#if defined(TARGET_NUCLEO_F031K6) || \
    defined (TARGET_NUCLEO_L011K4) || \
    defined (TARGET_NUCLEO_L031K6) || \
    defined (TARGET_DISCO_F746NG) || \
    defined (TARGET_DISCO_L476VG) || \
    defined (TARGET_NUCLEO_F303K8) || \
    defined (TARGET_NUCLEO_F334R8) || \
    defined (TARGET_DISCO_F334C8) || \
    defined (TARGET_NUCLEO_F042K6)
  #error [NOT_SUPPORTED] Target has only one I2C instance
#endif

#define ADDR (0x80)
#define FREQ 100000

// ********************************************************
// This tests data transfer between two I2C interfaces on
// the same chip, one configured as master, the other as
// slave.
//
// Wiring: connect master SCL to slave SCL, and master SDA to slave SDA
// ********************************************************

#if defined (TARGET_DISCO_F429ZI) || \
    defined (TARGET_DISCO_L053C8)
I2C master(PB_9, PB_8);

#elif defined(TARGET_FF_ARDUINO)
I2C master(D14, D15); // I2C_SDA, I2C_SCL
#endif


#if defined (TARGET_NUCLEO_F429ZI) || \
    defined (TARGET_NUCLEO_F767ZI) || \
    defined (TARGET_NUCLEO_L053R8) || \
    defined (TARGET_NUCLEO_L073RZ) || \
    defined (TARGET_NUCLEO_L152RE) || \
    defined (TARGET_NUCLEO_L476RG) || \
    defined (TARGET_NUCLEO_F207ZG) || \
    defined (TARGET_NUCLEO_F103RB) || \
    defined (TARGET_NUCLEO_F091RC) || \
    defined (TARGET_DISCO_F429ZI) || \
    defined (TARGET_DISCO_F469NI) || \
    defined (TARGET_DISCO_L053C8) || \
    defined (TARGET_NUCLEO_F446ZE)
I2CSlave slave(PB_11, PB_10);

#elif defined (TARGET_NUCLEO_F303RE) || \
    defined (TARGET_NUCLEO_F302R8)
I2CSlave slave(D2, D8);

#elif defined (TARGET_NUCLEO_F303ZE)
I2CSlave slave(PC_9, PA_8);

#elif defined (TARGET_NUCLEO_F746ZG)
I2CSlave slave(D2, D4);

#elif defined (TARGET_NUCLEO_F030R8) || \
    defined (TARGET_NUCLEO_F070RB) || \
    defined (TARGET_NUCLEO_F072RB)
I2CSlave slave(PB_11, D6);

#elif defined (TARGET_DISCO_L475VG_IOT01A)
I2CSlave slave(A4, A5);

#else
I2CSlave slave(D3, D6);

#endif

int main()
{
    char sent = 'T', received;

    master.frequency(FREQ);
    slave.frequency(FREQ);
    slave.address(ADDR);

    // First transfer: master to slave
    master.start();
    master.write(ADDR);
    master.write(sent);
    while(slave.receive() != I2CSlave::WriteAddressed);

    slave.read(&received, 1);
    if(sent != received)
    {
        notify_completion(false);
        return 1;
    }
    master.stop();

    // Second transfer: slave to master
    master.start();
    master.write(ADDR | 1);
    while(slave.receive() != I2CSlave::ReadAddressed);

    slave.write(received);
    received = master.read(0);
    master.stop();
    notify_completion(received == sent);
}

