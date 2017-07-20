#include "mbed.h"
#include "test_env.h"

#if !DEVICE_CAN
  #error [NOT_SUPPORTED] CAN not supported
#endif

#if defined(TARGET_LPC1549)
CAN can1(D9, D8);
#elif defined(TARGET_LPC1768) || defined(TARGET_LPC4088)
CAN can1(p9, p10);
#elif defined(TARGET_B96B_F446VE)
CAN can1(PD_0, PD_1);
#elif defined(TARGET_VK_RZ_A1H)
CAN can1(P5_9, P5_10);
#elif defined(TARGET_NUCLEO_F091RC) || defined(TARGET_NUCLEO_F072RB) || \
      defined(TARGET_NUCLEO_F042K6) || defined(TARGET_NUCLEO_F334R8) || \
      defined(TARGET_NUCLEO_F303RE) || defined(TARGET_NUCLEO_F303K8) || \
      defined(TARGET_NUCLEO_F302R8) || defined(TARGET_NUCLEO_F446RE) || \
      defined(TARGET_DISCO_F429ZI)  || \
      defined(TARGET_NUCLEO_F746ZG) || defined(TARGET_DISCO_L476VG)  || defined(TARGET_DISCO_L475VG_IOT01A) || \
      defined(TARGET_NUCLEO_F412ZG) || defined(TARGET_DISCO_F413ZH) || \
      defined(TARGET_NUCLEO_L476RG) || defined(TARGET_NUCLEO_L432KC)
CAN can1(PA_11, PA_12);
#elif defined(TARGET_DISCO_F469NI)  || defined(TARGET_DISCO_F746NG)  || \
      defined(TARGET_NUCLEO_F446ZE) || defined(TARGET_NUCLEO_F103RB) || \
      defined(TARGET_NUCLEO_F207ZG) || defined(TARGET_NUCLEO_F303ZE) || \
      defined(TARGET_DISCO_F769NI) || defined(TARGET_NUCLEO_F767ZI)  || \
      defined(TARGET_DISCO_F303VC)
CAN can1(PB_8, PB_9);
#endif

#define TEST_ITERATIONS     127

int main() {
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(dev_null);
    MBED_HOSTTEST_DESCRIPTION(CAN Loopback);
    MBED_HOSTTEST_START("MBED_A27");

#if !defined(TARGET_VK_RZ_A1H)
    can1.mode(CAN::Reset);
#endif

    if (!can1.mode(CAN::LocalTest)) {
        printf("Mode change failed\n");
    }

    char success_count = 0;
    for (char i=0; i < TEST_ITERATIONS; i++) {
        unsigned int id = 1337;
        CANMessage tx_msg(id, &i, sizeof(i));
        bool sent = false;
        if (can1.write(tx_msg)) {
            printf("Sent %u: %d\n", id, i);
            sent = true;
        }
        wait_ms(50);

        bool read = false;
        CANMessage rx_msg;
        if (can1.read(rx_msg)) {
            printf("Read %u: %d\n", rx_msg.id, rx_msg.data[0]);
            read = (rx_msg.id == id) && (rx_msg.data[0] == i);
        }

        bool success = sent && read;

        if (success) {
            success_count++;
        }
    }

    MBED_HOSTTEST_RESULT(success_count == TEST_ITERATIONS);
}
