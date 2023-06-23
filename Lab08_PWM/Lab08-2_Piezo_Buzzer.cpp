// Lab8-2 : Piezo Buzzer

#include "mbed.h"
PwmOut buzzer(PB_4); //D5

// C4, D4, E4, F4, G4, A4, B4, C5
float C_freq[]={261.626, 293.665, 329.628, 349.228, 391.995, 440.000, 493.883, 523.251}; // C scale

int beat[]={4, 4, 4, 4, 4, 4, 4, 4}; //beat array [ 1, ..., 16], 1 beat = 1/16 sec.

int main() {
    int period_us; 
    int beat_ms;
    while (true) {
        for (int i=0; i<8; i++) {
            buzzer=1.0 - 0.1;
            period_us = 1000000/C_freq[i];
            beat_ms = 62.5 * beat[i];
            buzzer.period_us(period_us); 
            ThisThread::sleep_for(chrono::milliseconds(beat_ms)); // hold for beat period 
            //buzzer = 1.0;
            //ThisThread::sleep_for(chrono::milliseconds(100));
        }
        
        for (int i=0; i < 8; i++) {
            buzzer=1.0 - 0.05; // set duty cycle 
            period_us = 1000000/C_freq[7 - i];
            beat_ms = 62.5 * beat[7 - i]; 
            buzzer.period_us(period_us); // set PWM period 
            ThisThread::sleep_for(chrono::milliseconds(beat_ms)); 
            //buzzer = 1.0; 
            //ThisThread::sleep_for(chrono::milliseconds(100));
        }
    } 
}