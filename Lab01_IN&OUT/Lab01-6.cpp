// 1-6

#include "mbed.h"


// D3 = PB_3, D5 = PB_4
// D3 : Red , D5 : Green
PortOut RGYLeds(PortB, 0x0038);
InterruptIn button(BUTTON1);

volatile int offset = 3; 
volatile int offset1 = 3; 
volatile int offset2 = 3; 
void buttonPressed(){
    offset += 1;

    if (offset == 6){
        offset = 3;
        offset1 = 3;
        offset2 = 3;
    }
    if (offset==4){
        offset1 = 5;
        offset2 = 2;
    }
    if (offset==5){
        offset1 = 4;
        offset2 = 4;
    }
}

int main() {

    button.fall(&buttonPressed);
    
    while (true) {
    
        for(int color=0 ; color<8 ; color++){
            int mask1 = (~(0b1<<1));
            int mask2 = ((~(0b1<<1)) << 1);
            int mask3 = ((~(0b1<<1)) << 2);

            ThisThread::sleep_for(500ms);   
            // RGYLeds = RGYLeds | (( color>>(6-offset) )<<3);
            RGYLeds = ((color & mask1)<<offset1) | ((color & mask2)<<offset2) | ((color & mask3) << (6-offset));
        }
        
    }
}

