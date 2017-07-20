#include "mbed.h"
#include "test_env.h"
#include <stdio.h>

#if !DEVICE_I2C
  #error [NOT_SUPPORTED] I2C is not supported
#endif

#if !DEVICE_I2CSLAVE
  #error [NOT_SUPPORTED] I2C Slave is not supported
#endif

#if !DEVICE_I2C_ASYNCH
  #error [NOT_SUPPORTED] I2C Async is not supported
#endif

#define ADDR (0x80)
#define FREQ 100000
#define SIZE 10

// ********************************************************
// This tests data transfer between two I2C interfaces on
// the same chip, one configured as master, the other as
// slave.
//
// Wiring: connect master SCL to slave SCL, and master SDA to slave SDA
// ********************************************************

#if defined (TARGET_DISCO_F429ZI)
I2C master(PB_9, PB_8);
#elif defined(TARGET_FF_ARDUINO)
I2C master(D14, D15); // I2C_SDA, I2C_SCL
#endif

#if defined (TARGET_NUCLEO_F072RB) || \
    defined (TARGET_NUCLEO_F030R8) || \
    defined (TARGET_NUCLEO_F103RB) || \
    defined (TARGET_NUCLEO_F207ZG) || \
    defined (TARGET_NUCLEO_F446ZE) || \
    defined (TARGET_NUCLEO_F429ZI) || \
    defined (TARGET_DISCO_F429ZI) || \
    defined (TARGET_NUCLEO_F767ZI) || \
    defined (TARGET_NUCLEO_L053R8) || \
    defined (TARGET_NUCLEO_L073RZ) || \
    defined (TARGET_NUCLEO_L152RE) || \
    defined (TARGET_NUCLEO_L476RG)
I2CSlave slave(PB_11, PB_10);
#elif defined(TARGET_NUCLEO_F303RE)
I2CSlave slave(D2, D8);
#elif defined (TARGET_DISCO_L475VG_IOT01A)
I2CSlave slave(A4, A5);
#else
I2CSlave slave(D3, D6);
#endif

volatile int why;
volatile bool master_complete = false;
void cbmaster_done(int event) {
    master_complete = true;
    why = event;
}

int main()
{
    event_callback_t callback;
    bool success = true;
    char buf_master[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    char buf_slave[SIZE];
    char res_master[SIZE];
    char buf_master_tx[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    char buf_master_rx[SIZE];
    char buf_slave_txrx[SIZE];

    callback.attach(cbmaster_done);

    master.frequency(FREQ);
    slave.frequency(FREQ);
    slave.address(ADDR);


    // First transfer: master to slave
    printf("\nFirst transfer: Master Tx, Repeated Start\n");
    if(master.transfer(ADDR, buf_master, SIZE, 0, 0, callback, I2C_EVENT_ALL, true) != 0)
            notify_completion(false);

    while (!master_complete) {
        if(slave.receive() == I2CSlave::WriteAddressed) {
            slave.read(buf_slave, SIZE);
            for(int i = 0; i < SIZE; i++){
                buf_slave[i]++;
            }
        }
    }
    if (why != I2C_EVENT_TRANSFER_COMPLETE) {
        printf("Transfer result: 0x%x\n", why);
        notify_completion(false);
    }
    master_complete = false;
    why = 0;
    printf("Transfer result: OK\n");

    // Second transfer: slave to master
    printf("\nSecond transfer: Master Rx\n");
    if(master.transfer(ADDR, 0, 0, res_master, SIZE, callback, I2C_EVENT_ALL, true) != 0)
            notify_completion(false);

    while (!master_complete) {
        if(slave.receive() == I2CSlave::ReadAddressed) {
            if(slave.write(buf_slave, SIZE))
                notify_completion(false);
        }
    }
    if (why != I2C_EVENT_TRANSFER_COMPLETE) {
        printf("Transfer result: 0x%x\n", why);
        notify_completion(false);
    }
    master_complete = false;
    why = 0;
    printf("Transfer result: OK\n");

    // Check first exchange success
    for(int i = 0; i < SIZE; i++) {
        if (res_master[i] != (buf_master[i] + 1)) {
            printf("Buffer check KO\n");
            printf("res_master[%d]: %d, buf_master[%d]: %d\n",i,res_master[i],i,buf_master[i]);
            notify_completion(false);
            break;
        }
    }
    printf("Buffer check OK\n");

    // Third transfer: Tx/Rx
    printf("\nThird transfer: Master Tx/Rx\n");
    if(master.transfer(ADDR, buf_master_tx, SIZE, buf_master_rx, SIZE, callback, I2C_EVENT_ALL, false) != 0)
            notify_completion(false);

    while (!master_complete) {

        int i = slave.receive();

        if(i == I2CSlave::WriteAddressed) {
            slave.read(buf_slave_txrx, SIZE);
            for(int i = 0; i < SIZE; i++){
                buf_slave_txrx[i]++;
            }
        }

        if((i == I2CSlave::ReadAddressed) ) {
            slave.write(buf_slave_txrx, SIZE);
        }
    }
    if (why != I2C_EVENT_TRANSFER_COMPLETE) {
        printf("Transfer result: 0x%x\n", why);
        notify_completion(false);
    }
    master_complete = false;
    why = 0;
    printf("Transfer result: OK\n");

    for(int i = 0; i < SIZE; i++) {
        if (buf_master_rx[i] != (buf_master_tx[i] + 1)) {
            printf("Buffer check KO\n");
            printf("buf_master_rx[%d]: %d, buf_master_tx[%d]: %d\n",i,buf_master_rx[i],i,buf_master_tx[i]);
            notify_completion(false);
            break;
        }
    }
    printf("Buffer check OK\n");
    notify_completion(success);
}

