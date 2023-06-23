// Lab8-3 : Piezo Buzzer Music

#include "mbed.h"
PwmOut buzzer(PB_4); //D5


float freq[]={  0,
    233.08, // Bb
    261.63, // C
    293.66, // D
    311.13, // Eb
    349.23, // F
    392.00, // G
    440.00, // A
    466.16
};
int sequence[] = {  
                    1,2  ,  3,5,5,3,2,5,5,2  ,  1,
                    1,2  ,  3,5,5,3,2,5,5,2  ,  1,
                    3,5  ,  6,8,8,6,5,3,2,1  ,  2,
                    1,2  ,  3,5,5,3,2,3,2,1  ,  1
                };

int beat[] = {
    1,1,  2,2,2,2,2,2,2,2,   14,1,1,  2,2,2,2,2,2,2,2,  14,
    1,1,  2,2,2,2,2,2,2,2,   14,1,1,  2,2,2,2,2,2,2,2,  14
};


int main() {
    int period_us; 
    int beat_ms;
    while (true) {
        for (int i=1; i <= 44 ; i++) {
            buzzer=1.0 - 0.01;
            // period_us = 1000000/freq[sequence[i]];
            period_us = 1000000/freq[sequence[i]];
            
            beat_ms = 200.5 * beat[i]; 

            buzzer.period_us(period_us);
            ThisThread::sleep_for(chrono::milliseconds(beat_ms));
        }
        for (int i=1; i <= 44 ; i++) {
            buzzer=1.0 - 0.01;
            // period_us = 1000000/freq[sequence[i]];
            period_us = 1000000/freq[sequence[i]];
            
            beat_ms = 200.5 * beat[i]; 

            buzzer.period_us(period_us);
            ThisThread::sleep_for(chrono::milliseconds(beat_ms));
        }
        
    } 
}
