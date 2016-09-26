#include "mbed.h"
#include "test_env.h"
#include <stdio.h>

#define ADDR (0x90)
#define FREQ 100000
#define SIZE 10

// ********************************************************
// This tests data transfer between two I2C interfaces on
// the same chip, one configured as master, the other as
// slave.
//
// Wiring: cf below
// ********************************************************

#if defined (TARGET_NUCLEO_F411RE) || defined (TARGET_NUCLEO_F446RE) || defined (TARGET_NUCLEO_F410RB)  || defined (TARGET_NUCLEO_F401RE)
I2C master(PB_9, PB_8); // I2C_1 (Arduino: D14/D15)
I2CSlave slave(PB_3, PB_10); // I2C_2 (Arduino: D3/D6)
#elif defined (TARGET_NUCLEO_F429ZI) || defined (TARGET_DISCO_F429ZI) || defined (TARGET_NUCLEO_F446ZE)
I2C master(PB_9, PB_8); // I2C_1 (Arduino: D14/D15)
I2CSlave slave(PB_11, PB_10); // I2C_2 
#endif

volatile int why;
volatile bool master_complete = false;
void cbmaster_done(int event) {
    printf("cbmaster_done\n");
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
    master.transfer(ADDR, buf_master, SIZE, 0, 0, callback, I2C_EVENT_ALL, true);

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
    master.transfer(ADDR, 0, 0, res_master, SIZE, callback, I2C_EVENT_ALL, true);

    while (!master_complete) {
        if(slave.receive() == I2CSlave::ReadAddressed) {
            slave.write(buf_slave, SIZE);
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
    master.transfer(ADDR, buf_master_tx, SIZE, buf_master_rx, SIZE, callback, I2C_EVENT_ALL, false);

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

