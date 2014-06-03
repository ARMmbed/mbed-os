#include "mbed.h"

Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);
CAN can1(p9, p10);
#if defined(TARGET_LPC4088)
CAN can2(p34, p33);
#else
CAN can2(p30, p29);
#endif
char counter = 0;

void printmsg(char *title, CANMessage *msg) {
    printf("%s [%03X]", title, msg->id);
    for(char i = 0; i < msg->len; i++) {
        printf(" %02X", msg->data[i]);
    }
    printf("\n");
}

void send() {
    printf("send()\n");
    CANMessage msg = CANMessage(1337, &counter, 1);
    if(can1.write(msg)) {
        printmsg("Tx message:", &msg);
        counter++;
    }
    led1 = !led1;
}

void read() {
    CANMessage msg;
    printf("rx()\n");
    if(can2.read(msg)) {
        printmsg("Rx message:", &msg);
        led2 = !led2;
    }
}

int main() {
    printf("main()\n");
    ticker.attach(&send, 1);
    can2.attach(&read);
    while(1) {
        printf("loop()\n");
        wait(1);
    }
}
