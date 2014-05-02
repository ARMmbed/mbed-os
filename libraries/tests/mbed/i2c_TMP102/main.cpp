#include "test_env.h"
#include "TMP102.h"

#if defined(TARGET_KL25Z)
TMP102 temperature(PTC9, PTC8, 0x90);

#elif defined(TARGET_LPC812)
TMP102 temperature(D10, D11, 0x90);

#elif defined(TARGET_LPC4088)
TMP102 temperature(p9, p10, 0x90);

#elif defined(TARGET_LPC2368)
TMP102 temperature(p28, p27, 0x90);

#else
TMP102 temperature(p28, p27, 0x90);
#endif

int main()
{
    float t = temperature.read();

    printf("TMP102: Temperature: %f\n\r", t);
    // In our test environment (ARM office) we should get a temperature within
    // the range ]15, 30[C
    bool result = (t > 15.0) && (t < 30.0);
    notify_completion(result);
}
