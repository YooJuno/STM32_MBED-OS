// 1-2
#include "mbed.h"

DigitalOut redLED(D2);
DigitalOut greenLED(LED1);
InterruptIn button(BUTTON1);

volatile int interval;

void buttonPressed(){
    redLED = !redLED;    
}


// main() runs in its own thread in the OS
int main() {
    button.fall(&buttonPressed);
    button.mode(PullUp);
    
    while (true) {
        greenLED = !greenLED;
        ThisThread::sleep_for(500ms);
    }
}

