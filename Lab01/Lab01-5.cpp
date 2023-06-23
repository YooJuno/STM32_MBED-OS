// 1-5

#include "mbed.h"

// D3 = PB_3, D5 = PB_4
// D3 : Red , D5 : Green
PortOut RGYLeds(PortB, 0x0038);

int main() {
    
    while (true) {
        for(int color=0 ; color<8 ; color++){
            ThisThread::sleep_for(500ms);   
            RGYLeds = ((color & (~(0xff<<1)))<<3) | ((color & ((~(0xff<<1)) << 1))<<4) | ((color & ((~(0xff<<1)) << 2)) << 2);
        }
        
    }
}

