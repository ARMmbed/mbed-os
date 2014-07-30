#include "mbed.h"
#include "test_env.h"
#include <stdio.h>

#define ADDR1 (0xA0)
#define ADDR2 (0x78)
#define GENERAL_CALL_ADDR (0x00)
#define FREQ 100000

// ********************************************************
// This tests data transfer between two I2C interfaces on
// the same chip, one configured as master, the other as
// slave. Then it changes addresses of the slave and checks
// that it behaves correctly.
//
// Wiring:
// LPC1768:
//   p28 <-> p9
//   p27 <-> p10
//   pull-up resistors on both lines
// EFM32(W/G/L)G:
//   PC4 <-> PD6
//   PC5 <-> PD7
// ********************************************************

#if defined(TARGET_EFM32LG_STK3600) \
    ||defined(TARGET_EFM32GG_STK3700) \
    ||defined(TARGET_EFM32TG_STK3300) \
    ||defined(TARGET_EFM32_G8XX_STK) \
    ||defined(TARGET_EFM32WG_STK3800)
I2CSlave slave(PC4, PC5);
I2C master(PD6, PD7);
#elif defined(LPC1768)
I2CSlave slave(p9, p10);
I2C master(p28, p27);
#endif

void testWrongAddress(int addr){
    master.start();
    master.write(addr);
    if(slave.receive() != I2CSlave::NoData){
        printf("Response to wrong addr %d\n",addr);
        notify_completion(false);
    }
    master.stop();
}

void testRead(int addr){
    char send = 'n';
    char sendarr[1] = {'a'};
    char received = 0;
    master.start();
    master.write(addr);
    if(slave.receive() != I2CSlave::ReadAddressed){
        printf("Wrong addr response to %d\n",addr);
        notify_completion(false);
    }
    slave.write(send);
    received = master.read(true);
    if(send != received){
        printf("Sent %d to master from addr %d and got %d\n",send,addr,received);
        notify_completion(false);
    }
    slave.write(sendarr, 1);
    received = master.read(false);
    if(received != sendarr[0]){
        printf("Sent %d to master from addr %d and got %d\n",send,addr,received);
        notify_completion(false);
    }
    master.stop();
}

void testWrite(int addr, I2CSlave::RxStatus expectedRx, char send){
    char received = 0;
    master.start();
    master.write(addr);
    int response = slave.receive();
    if(response != expectedRx){
        printf("wrong addr response to %d. Got %d\n",addr, response);
        notify_completion(false);
    }
    master.write(send);
    slave.read(&received, 1);
    if(send != received){
        printf("Sent %d to addr %d and got %d\n",send,addr,received);
        notify_completion(false);
    }

    //Test the other slave read function
    received = 0;
    master.write(send);
    received = slave.read();
    master.stop();
    if(send != received){
        printf("Sent %d to addr %d and got %d\n",send,addr,received);
        notify_completion(false);
    }
}

void testAddress(int addr, int wrong_addr){
    char testChar = 'T';

    //Check that we do respond to a read from the correct address
    testRead(addr | 1);

    //Check that we do respond to a write to the correct address
    testWrite(addr, I2CSlave::WriteAddressed, testChar);

    //Check that we don't respond to a write to wrong address
    testWrongAddress(wrong_addr);

    //Check that we don't respond to a read to wrong address
    testWrongAddress(wrong_addr | 1);

    //Check that write to the general address works
    testWrite(GENERAL_CALL_ADDR, I2CSlave::WriteGeneral, testChar);
}

int main()
{
    wait_us(200);
    master.frequency(FREQ);
    slave.frequency(FREQ);

    slave.address(ADDR1);
    testAddress(ADDR1,ADDR2);

    printf("Change address\n");
    //Change address
    slave.address(ADDR2);
    testAddress(ADDR2,ADDR1);

    printf("Change back\n");
    //Change back
    slave.address(ADDR1);
    testAddress(ADDR1,ADDR2);

    //Done!
    notify_completion(true);
}

