#include <stdint.h>
#include <stdlib.h>
#include <string>

typedef enum {
    BASE64_SUCCESS = 0,
    BASE64_INVALID_PARAMETER = 1,
    BASE64_BUFFER_TOO_SMALL = 2,
    BASE64_ERROR = 3,
}base64_result_e;

base64_result_e esfs_EncodeBase64(const void* buffer, uint32_t bufferSize, char* string, uint32_t stringSize);
base64_result_e esfs_DecodeNBase64(const char* string, uint32_t stringMaxSize, void* buffer, uint32_t bufferSize, 
                                 uint32_t* lengthWritten, uint32_t* charsProcessed);


