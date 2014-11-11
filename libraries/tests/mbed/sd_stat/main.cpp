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
}

int main()
{
    bool result = true;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("/sd/")) != NULL) {
        // print all the files and directories within directory

        printf("type\tmodified          \tsize\tname\r\n");
        while ((ent = readdir (dir)) != NULL) {
            if (ent->d_type == DT_UNKNOWN) {
                // If we still get unknown SD card file type...
                result = false;
            }
            struct stat statinfo;
            char name[256] = "/sd/";
            strcat(name, ent->d_name);
            int res = stat(name, &statinfo);

            int f_size = res ? 0 : statinfo.st_size;
            time_t f_mtime = res ? 0 : statinfo.st_mtime;

            char buffer[32] = {0};
            strftime(buffer, 32, "%Y-%m-%d %H:%M%p", localtime(&f_mtime));

            // Print file type and name
            bool is_dir = (ent->d_type == DT_DIR);
            const char *file_dir = is_dir ? "d" : "f";
            printf("%s\t%s\t%d\t%s\r\n", file_dir, buffer, f_size, ent->d_name);

        }
        closedir (dir);
    } else {
        // could not open directory
        printf("Could not open directory");
        result = false;
    }

    notify_completion(result);
}
