#include "mbed.h"
// 1-3
// D2 : Red, D3 : Green , D4 : Yellow
PinName pins[3] = {D2, D3, D4};

BusOut RGYLeds(pins);
// BusOut RGYLeds(D2, D3, D4);


// main() runs in its own thread in the OS
int main() {
    
    while (true) {
        for(int color=0 ; color<8 ; color++){
            RGYLeds = color;
            ThisThread::sleep_for(500ms);      
        }
    }
}

