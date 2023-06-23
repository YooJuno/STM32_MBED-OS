// 6-2
#include "mbed.h"

static DigitalOut RedLED(PB_3);
static DigitalOut GreenLED(PB_5);

// Used for blinking the RED
Ticker RED_ticker;

// Used for turning off the RED
Timeout GREEN_timeout;

// ISR for time out 
void blinking_RED(void) {
    RedLED = !RedLED; 
}

void turnoff_GREEN(void){
    GreenLED = 0;
}

int main() {
    RedLED = 1;
    GreenLED = 1;

    // timer ISR for blinking RED
    RED_ticker.attach(&blinking_RED, 500ms);

    // timeout ISR for turning off GREEN
    GREEN_timeout.attach(&turnoff_GREEN, 10s);

    while(1) {
    } 
}