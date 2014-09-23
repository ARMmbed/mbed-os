#include "mbed.h"
#include "SDFileSystem.h"
#include "test_env.h"

#if defined(TARGET_KL25Z)
SDFileSystem sd(PTD2, PTD3, PTD1, PTD0, "sd");

#elif defined(TARGET_KL46Z)
SDFileSystem sd(PTD6, PTD7, PTD5, PTD4, "sd");

#elif defined(TARGET_K64F)
SDFileSystem sd(PTD2, PTD3, PTD1, PTD0, "sd");

#elif defined(TARGET_K20D50M)
SDFileSystem sd(PTD2, PTD3, PTD1, PTC2, "sd");

#elif defined(TARGET_nRF51822)
SDFileSystem sd(p12, p13, p15, p14, "sd");

#elif defined(TARGET_NUCLEO_F030R8) || \
      defined(TARGET_NUCLEO_F072RB) || \
      defined(TARGET_NUCLEO_F091RC) || \
      defined(TARGET_NUCLEO_F103RB) || \
      defined(TARGET_NUCLEO_F302R8) || \
      defined(TARGET_NUCLEO_F303RE) || \
      defined(TARGET_NUCLEO_F334R8) || \
      defined(TARGET_NUCLEO_F401RE) || \
      defined(TARGET_NUCLEO_F411RE) || \
      defined(TARGET_NUCLEO_L053R8) || \
      defined(TARGET_NUCLEO_L152RE)
SDFileSystem sd(D11, D12, D13, D10, "sd");

#elif defined(TARGET_DISCO_F051R8)
SDFileSystem sd(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS, "sd");

#elif defined(TARGET_LPC2368)
SDFileSystem sd(p11, p12, p13, p14, "sd");

#elif defined(TARGET_LPC11U68)
SDFileSystem sd(D11, D12, D13, D10, "sd");

#elif defined(TARGET_LPC1549)
SDFileSystem sd(D11, D12, D13, D10, "sd");

#else
SDFileSystem sd(p11, p12, p13, p14, "sd");
#endif

namespace {
const char *sd_file_path = "/sd/out.txt";
const int DATA_SIZE = 256;
}

int main()
{
    uint8_t data_written[DATA_SIZE] = { 0 };
    bool result = true;

    // Fill data_written buffer with random data
    // Write these data into the file
    {
        FILE *f = fopen(sd_file_path, "w");

        printf("SD: Writing ... ");
        for (int i = 0; i < DATA_SIZE; i++) {
            data_written[i] = rand() % 0XFF;
            fprintf(f, "%c", data_written[i]);
        }
        printf("[OK]\r\n");
        fclose(f);
    }

    // Read back the data from the file and store them in data_read
    {
        FILE *f = fopen(sd_file_path, "r");
        printf("SD: Reading data ... ");
        for (int i = 0; i < DATA_SIZE; i++) {
            uint8_t data = fgetc(f);
            if (data != data_written[i]) {
                result = false;
                break;
            }
        }
        printf("[%s]\r\n", result ? "OK" : "FAIL");
        fclose(f);
    }

    notify_completion(result);
}
