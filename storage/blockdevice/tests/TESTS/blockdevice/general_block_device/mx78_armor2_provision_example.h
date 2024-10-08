#ifndef MBED_MX78_ARMOR2_PROVISION_EXAMPLE_H
#define MBED_MX78_ARMOR2_PROVISION_EXAMPLE_H
#include <stdint.h>

#define AVAILABLE_PAIR_NUM 4
typedef struct {
    uint32_t app_id;
    uint32_t secure_zone_id;
} app_zone_pair_t;

const app_zone_pair_t app_zone_available_pair[AVAILABLE_PAIR_NUM] = {
    {0xFFFFFFFE, 0x00},
    {0xFFFFFFFD, 0x01},
    {0x00000BBA, 0x02},
    {0x00000BBE, 0x03},
};

#endif /* MBED_MX78_ARMOR2_PROVISION_EXAMPLE_H */