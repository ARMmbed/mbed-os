#ifndef MBED_OTA_API_H
#define MBED_OTA_API_H

#define FLASH_TOP           0x200000
#define FLASH_SECTOR_SIZE   0x1000
#define FLASH_SECTOR_MASK   ~(FLASH_SECTOR_SIZE - 1)
#define OTA_REGION1         0x0b000
#define OTA_REGION2         0xc0000
#define TAG_OFS             0xc
#define VER_OFS             0x10

#define TAG_DOWNLOAD        0x81950001
#define TAG_VERIFIED        0x81950003

typedef struct imginfo_s {
    uint32_t base;
    uint32_t tag;
    uint64_t ver;
} imginfo_t;

#ifdef __cplusplus
extern "C" {
#endif

extern void OTA_GetImageInfo(imginfo_t *info);
extern uint32_t OTA_GetBase(void);

extern uint32_t OTA_UpdateImage(uint32_t offset, uint32_t len, uint8_t *data);
extern uint32_t OTA_ReadImage(uint32_t offset, uint32_t len, uint8_t *data);
extern uint32_t OTA_MarkUpdateDone(void);
extern void OTA_ResetTarget(void);

#ifdef __cplusplus
}
#endif

#endif /* MBED_OTA_API_H */

