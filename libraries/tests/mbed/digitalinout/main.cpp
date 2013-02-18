#include "test_env.h"

DigitalInOut d1(p5);
DigitalInOut d2(p25);

int main() {
    bool check = true;
    
    
    d1.output();
    d2.input();
    d1 = 1; wait(0.1); if (d2 != 1) check = false;
    d1 = 0; wait(0.1);  if (d2 != 0) check = false;
    
    d1.input();
    d2.output();
    d2 = 1; wait(0.1); if (d1 != 1) check = false;
    d2 = 0; wait(0.1); if (d1 != 0) check = false;
    
    notify_completion(check);
}
