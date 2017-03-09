#include "mbed.h"
#include "FATFileSystem.h"
#include "SDBlockDevice.h"
#include <stdio.h>
#include <errno.h>
/* mbed_retarget.h is included after errno.h so symbols are mapped to
 * consistent values for all toolchains */
#include "platform/mbed_retarget.h"


SDBlockDevice sd(MBED_CONF_APP_SPI_MOSI, MBED_CONF_APP_SPI_MISO, MBED_CONF_APP_SPI_CLK, MBED_CONF_APP_SPI_CS);
FATFileSystem fs("sd", &sd);

void return_error(int ret_val){
  if (ret_val)
    printf("Failure. %d\n", ret_val);
  else
    printf("done.\n");
}

void errno_error(void* ret_val){
  if (ret_val == NULL)
    printf(" Failure. %d \n", errno);
  else
    printf(" done.\n");
}

int main()
{
	int error = 0;
	printf("Welcome to the filesystem example.\n");

	printf("Opening a new file, numbers.txt.");
	FILE* fd = fopen("/sd/numbers.txt", "w+");
	errno_error(fd);

	for (int i = 0; i < 20; i++){
		printf("Writing decimal numbers to a file (%d/20)\r", i);
		fprintf(fd, "%d\n", i);
	}
	printf("Writing decimal numbers to a file (20/20) done.\n");

	printf("Closing file.");
	fclose(fd);
	printf(" done.\n");

	printf("Re-opening file read-only.");
	fd = fopen("/sd/numbers.txt", "r");
	errno_error(fd);

	printf("Dumping file to screen.\n");
	char buff[16] = {0};
	while (!feof(fd)){
		int size = fread(&buff[0], 1, 15, fd);
		fwrite(&buff[0], 1, size, stdout);
	}
	printf("EOF.\n");

	printf("Closing file.");
	fclose(fd);
	printf(" done.\n");

	printf("Opening root directory.");
	DIR* dir = opendir("/sd/");
	errno_error(fd);

	struct dirent* de;
	printf("Printing all filenames:\n");
	while((de = readdir(dir)) != NULL){
		printf("  %s\n", &(de->d_name)[0]);
	}

	printf("Closeing root directory. ");
	error = closedir(dir);
	return_error(error);
	printf("Filesystem Demo complete.\n");

	while (true) {}
}

