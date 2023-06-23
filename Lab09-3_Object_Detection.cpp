// Lab09-3_Ultrasonic_Sensor_Dist

#include "mbed.h"

PwmOut buzzer(PB_4); //D5
float freq = 440;


DigitalOut trigger(ARDUINO_UNO_D3);
DigitalIn  echo(ARDUINO_UNO_D2);
Timer      timer1;

BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
char buffer[80];

int main(){
    int period_us; 
    int beat_ms;
    float distance;
    
    sprintf(buffer, "Mbed OS version %d.%d.%d\r\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "\r\n Welcome to ultrasonic Sensor Lab!\r\n");
    pc.write(buffer, strlen(buffer));
    
    // initialization
    trigger = 0;
    
    while(true){
        
        
        trigger = 1;
        wait_us(20);
        trigger = 0;
        
        timer1.reset();
        while(echo == 0){}    
        timer1.start();
        while(echo == 1){}    
        timer1.stop();
        
        distance = timer1.elapsed_time().count() * 0.017;
        sprintf(buffer, "The distance is %.2f [cm] \n\r", distance);
        pc.write(buffer, strlen(buffer));
        int alert = distance*4;
        if(distance <= 100){
            buzzer=1.0 - 0.03;
            period_us = 1000000/freq;
            beat_ms = 300;
            buzzer.period_us(period_us); 
            ThisThread::sleep_for(chrono::milliseconds(beat_ms));
            
            buzzer = 1; 
            ThisThread::sleep_for(chrono::milliseconds(alert));
        }
        
        else{
            ThisThread::sleep_for(chrono::milliseconds(300));
        }
        
    }
    
}