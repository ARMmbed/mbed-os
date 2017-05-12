#include <drivers/gpio/adi_gpio.h>

// ADI GPIO device driver state memory. Only one state memory is required globally.
uint8_t gpioMemory[ADI_GPIO_MEMORY_SIZE];

// Flag to indicate whether the GPIO driver has been initialized
uint8_t gpio_initialized = 0;
