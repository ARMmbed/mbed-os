#include "mbed.h"		 #include "mbed.h"
+#include "spi_flash.h"
 		 
-DigitalOut myled(LED1);		+DigitalOut LED(LED1);
-		+ 
 int main() {		 int main() {
-    while(1) {		+
-        myled = 1;		+	spi_flash_init();
-        wait(0.2);		+    spi_flash_powerDown(); 
-        myled = 0;		+	
-        wait(0.2);		+    LED = 1;
-    }		+    wait(2);
-}