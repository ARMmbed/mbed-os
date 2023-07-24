# How to add new STM32 family
1. 	In path mbed-os\targets\TARGET_STM\ add new family folder, in our my case TARGET_STM32H5, and also do not forget add same name into CmakeLists.txt in same folder level.
2.	In path mbed-os\targets\TARGET_STM\TARGET_STM32H5\ prepare another folders. First is STM32Cube_FW and second CMSIS
3. 	Visit https://github.com/STMicroelectronics. Select package for your family and chose latest tag. Then download it as a zip.
	For example https://github.com/STMicroelectronics/STM32CubeH5/tree/v1.1.0
4. 	Exctract the STM32CubeH5-1.1.0.zip 
6.	From STM32CubeH5-1.1.0\STM32CubeH5-1.1.0\Drivers\CMSIS\Device\ST\STM32H5xx\Source\Templates copy system_stm32h5xx.c into mbed-os\targets\TARGET_STM\TARGET_STM32H5\STM32Cube_FW\
5.	From STM32CubeH5-1.1.0\Drivers\CMSIS\Device\ST\STM32H5xx\Include\ copy all .h files into  mbed-os\targets\TARGET_STM\TARGET_STM32H5\STM32Cube_FW\CMSIS\
6. 	From STM32CubeH5-1.1.0\Drivers copy dolder STM32H5xx_HAL_Driver folder into mbed-os\targets\TARGET_STM\TARGET_STM32H5\STM32Cube_FW\
8. 	In mbed-os\targets\TARGET_STM\TARGET_STM32H5\STM32Cube_FW\STM32H5xx_HAL_Driver could be deleted everyting exclude folders Include and Source. 
9. 	Whole content of folders Include and Source (from previous point) move to from their folder to one level up. Then delete both folders.
10.	From folder STM32Cube_FW\STM32H5xx_HAL_Driver move file stm32h5xx_hal_conf_template.h to one folder level up. 
	- Rest of templates could be deleted.
	- The moved file stm32h5xx_hal_conf_template.h should be renamed to stm32h5xx_hal_conf.h
	- inside of stm32h5xx_hal_conf_template.h file should be all macros #define  USE_HAL_XXXXXXXXX covered by macro #if !defined(USE_HAL_XXXXXXXXX) statement
	- inside of stm32h5xx_hal_conf_template.h file should be the true part of #ifdef  USE_FULL_ASSERT filled with #include "stm32_assert.h"
11.	In mbed-os\targets\TARGET_STM\TARGET_STM32H5\STM32Cube_FW\ do not forget create correct CmakeLists.txt
12.	In mbed-os\targets\TARGET_STM\TARGET_STM32H5\ do not forget create correct CmakeLists.txt
12. In mbed-os\targets\TARGET_STM\ do not forget update CmakeLists.txt with new family
13. Lets get add new targets for that family.