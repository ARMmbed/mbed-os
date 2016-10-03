#include "mbed.h"
#include "test_env.h"
#include <stdio.h>

#define ADDR (0xA0)
#define FREQ 100000

// ********************************************************
// This tests data transfer between two I2C interfaces on
// the same chip, one configured as master, the other as
// slave. Works on:
//
// *LPC1768 mbed
//   p28 <-> p9
//   p27 <-> p10
//   pull-up resistors on both lines
// *STM32F4 boards
//   cf below for wiring
//
// ********************************************************

#if defined(TARGET_NUCLEO_F411RE) || defined (TARGET_NUCLEO_F446RE) || defined (TARGET_NUCLEO_F410RB) || defined (TARGET_NUCLEO_F401RE)
I2C master(PB_9, PB_8); // I2C_1 (Arduino: D14/D15)
I2CSlave slave(PB_3, PB_10); // I2C_2 (Arduino: D3/D6)
#elif defined (TARGET_NUCLEO_F429ZI) || defined (TARGET_DISCO_F429ZI) || defined (TARGET_NUCLEO_F446ZE)
I2C master(PB_9, PB_8); // I2C_1 (Arduino: D14/D15)
I2CSlave slave(PB_11, PB_10); // I2C_2
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
    if(slave.receive() != I2CSlave::WriteAddressed)
    {
        notify_completion(false);
        return 1;
    }
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
    if(slave.receive() != I2CSlave::ReadAddressed)
    {
        notify_completion(false);
        return 1;
    }
    slave.write(received);
    received = master.read(0);
    slave.stop();
    notify_completion(received == sent);
}

