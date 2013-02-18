#include "test_env.h"

int main() {
    char uid[DEVICE_ID_LENGTH + 1];
    
    if (mbed_interface_uid(uid) == 0) {
        printf("UID: %s\n", uid);
    } else{
        printf("Error\n");
    }
    
    char mac[16];
    mbed_mac_address(mac);
    printf("mac address: %02x,%02x,%02x,%02x,%02x,%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    notify_completion(true);
}
