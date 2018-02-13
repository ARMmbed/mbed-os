
#ifndef __CMSIS_OS2_H__
#define __CMSIS_OS2_H__

//If conflicts, then remove these, copied from cmsis_os.h
typedef int32_t                  osStatus;

#define osOK 0



//These are from cmsis_os2.h
typedef void *osSemaphoreId_t;

typedef struct {
  const char                   *name;   ///< name of the semaphore
  uint32_t                 attr_bits;   ///< attribute bits
  void                      *cb_mem;    ///< memory for control block
  uint32_t                   cb_size;   ///< size of provided memory for control block
} osSemaphoreAttr_t;

#define osWaitForever         0xFFFFFFFFU


#endif
