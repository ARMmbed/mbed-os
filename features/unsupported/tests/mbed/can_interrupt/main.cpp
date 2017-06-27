#include "mbed.h"

#if !DEVICE_CAN
  #error [NOT_SUPPORTED] CAN not supported
#endif

Ticker ticker;
DigitalOut led1(LED1);
DigitalOut led2(LED2);

#if defined(TARGET_LPC1549)
// LPC1549 support only single CAN channel
CAN can1(D2, D3);
#elif defined(TARGET_B96B_F446VE)
// B96B_F446VE support only single CAN channel
CAN can1(PD_0, PD_1);
#elif defined(TARGET_NUCLEO_F091RC) || defined(TARGET_NUCLEO_F072RB) || \
      defined(TARGET_NUCLEO_F042K6) || defined(TARGET_NUCLEO_F334R8) || \
      defined(TARGET_NUCLEO_F303RE) || defined(TARGET_NUCLEO_F303K8) || \
      defined(TARGET_NUCLEO_F302R8) || defined(TARGET_NUCLEO_F446RE) || \
      defined(TARGET_DISCO_F429ZI)  || defined(TARGET_NUCLEO_F103RB) || \
      defined(TARGET_NUCLEO_F746ZG) || defined(TARGET_NUCLEO_L476RG) || \
      defined(TARGET_NUCLEO_F412ZG) || defined(TARGET_DISCO_F413ZH) || \
      defined(TARGET_NUCLEO_L432KC) || defined(TARGET_DISCO_F303VC)
CAN can1(PA_11, PA_12);
#elif defined(TARGET_DISCO_F469NI) || defined(TARGET_DISCO_F746NG)
CAN can1(PB_8, PB_9);
#else
CAN can1(p9, p10);
#endif

#if defined(TARGET_LPC4088)
CAN can2(p34, p33);
#elif defined (TARGET_LPC1768)
CAN can2(p30, p29);
#elif defined(TARGET_NUCLEO_F446RE) || defined(TARGET_DISCO_F469NI) || \
      defined(TARGET_DISCO_F429ZI)  || defined(TARGET_NUCLEO_F746ZG) || \
      defined(TARGET_NUCLEO_F412ZG) || defined(TARGET_DISCO_F413ZH) || \
      defined(TARGET_DISCO_F746NG)
CAN can2(PB_5, PB_6);
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

#if (!defined (TARGET_LPC1549) && !defined(TARGET_B96B_F446VE) && \
     !defined(TARGET_NUCLEO_F091RC) && !defined(TARGET_NUCLEO_F072RB) && \
     !defined(TARGET_NUCLEO_F042K6) && !defined(TARGET_NUCLEO_F334R8) && \
     !defined(TARGET_NUCLEO_F303RE) && !defined(TARGET_NUCLEO_F303K8) && \
     !defined(TARGET_NUCLEO_F302R8) && !defined(TARGET_NUCLEO_F103RB) && \
     !defined(TARGET_DISCO_L476VG) && !defined(TARGET_DISCO_L475VG_IOT01A) && !defined(TARGET_NUCLEO_L476RG) && \
     !defined(TARGET_NUCLEO_L432KC) && !defined(TARGET_DISCO_F303VC))
void read() {
    CANMessage msg;
    printf("rx()\n");
    if(can2.read(msg)) {
        printmsg("Rx message:", &msg);
        led2 = !led2;
    }
}
#endif

int main() {
    printf("main()\n");
    ticker.attach(&send, 1);
#if (!defined (TARGET_LPC1549) && !defined(TARGET_B96B_F446VE) && \
     !defined(TARGET_NUCLEO_F091RC) && !defined(TARGET_NUCLEO_F072RB) && \
     !defined(TARGET_NUCLEO_F042K6) && !defined(TARGET_NUCLEO_F334R8) && \
     !defined(TARGET_NUCLEO_F303RE) && !defined(TARGET_NUCLEO_F303K8) && \
     !defined(TARGET_NUCLEO_F302R8) && !defined(TARGET_NUCLEO_F103RB) && \
     !defined(TARGET_DISCO_L476VG)  && !defined(TARGET_NUCLEO_L476RG) && \
     !defined(TARGET_NUCLEO_L432KC) && !defined(TARGET_DISCO_F303VC))
    can2.attach(&read);
#endif
    while(1) {
        printf("loop()\n");
        wait(1);
    }
}
