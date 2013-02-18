#include "test_env.h"

AnalogIn in(p17);
AnalogOut out(p18);

#define ERROR_TOLLERANCE    0.05

int main() {
    bool check = true;
    
    for (float out_value=0.0; out_value<1.1; out_value+=0.1) {
        out.write(out_value);
        wait(0.1);
        
        float in_value = in.read();
        float diff = fabs(out_value - in_value);
        if (diff > ERROR_TOLLERANCE) {
            check = false;
            printf("ERROR (out:%.4f) - (in:%.4f) = (%.4f)"NL, out_value, in_value, diff);
        } else {
            printf("OK    (out:%.4f) - (in:%.4f) = (%.4f)"NL, out_value, in_value, diff);
        }
    }
    
    notify_completion(check);
}
