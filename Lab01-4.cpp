// 1-4
#include "mbed.h"

// D3 = PB_3, D5 = PB_4 , D4 = PB_5
// D3 : Red , D5 : Green , D4 : Yellow
PortOut RGYLeds(PortB, 0x0038);

// 1-4
// main() runs in its own thread in the OS
int main() {
    
    while (true) {
        
        for(int color=0 ; color<8 ; color++){
            RGYLeds = color << 3;
            ThisThread::sleep_for(500ms);   
        }
       
           
        
    }
}

