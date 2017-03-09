/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "test_env.h"
#include "mbed.h"
#include "C12832.h"

int main()
{
    MBED_HOSTTEST_TIMEOUT(15);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(SPI C12832 display);
    MBED_HOSTTEST_START("MBED_35");
    
    C12832 lcd(D11, D13, D12, D7, D10);

    // clear the screen
    lcd.cls();
    
    // print the first line and wait 3 sec
    lcd.locate(0,3);
    lcd.printf("mbed application board!");
    
    // print the counter prefix; the number will be printed in the while loop
    lcd.locate(0,15);
    lcd.printf("Counting:");

    int i=1, j=0;
    while(i++,i<=200) {
        lcd.locate(42,15);
        lcd.printf("%d", i);
        if (i % 50 == 0) {
            lcd.invert(j = j ? 0 : 1);
        };
        wait(0.001);
    }
    
    MBED_HOSTTEST_RESULT(1);
}