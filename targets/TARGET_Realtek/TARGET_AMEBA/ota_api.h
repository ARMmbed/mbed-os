#ifndef MBED_OTA_API_H
#define MBED_OTA_API_H

#ifdef __cplusplus
 extern "C" {
#endif

extern uint32_t OTA_UpdateImage(uint32_t offset, uint32_t len, uint8_t *data);
extern uint32_t OTA_ReadImage(uint32_t offset, uint32_t len, uint8_t *data);
extern uint32_t OTA_MarkUpdateDone(void);
extern void OTA_ResetTarget(void);

#ifdef __cplusplus
}
#endif

#endif /* MBED_OTA_API_H */

