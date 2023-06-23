// 7-2

#include "mbed.h"
BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
AnalogIn tempSensor(ADC_TEMP);
Ticker CDS_ticker;

char buf[80];
volatile bool tick=true;

void temp_tick(void){
    tick = true;
}

int main() { 
    sprintf(buf, ("\r\n *** ADC Example (Internal Temp Sensor) ***\r\n"));
    pc.write(buf, strlen(buf));
    
    CDS_ticker.attach(&temp_tick, 5s);
    
    // write your code

    while(1){
        if(tick){
            float meas = tempSensor.read();
            meas = ((meas*3.3 - 0.76)/0.0025)+25; 
            sprintf(buf, "Internal Temperature = %.5f [C]\r\n", meas);
            pc.write(buf, strlen(buf));
            tick = false;
        }
    }
    
}

// // 7-2

// #include "mbed.h"
// BufferedSerial pc(USBTX, USBRX, 115200);
// AnalogIn tempSensor(ADC_TEMP);

// char buf[80] ;

// int main() { 
//     sprintf(buf, ("\r\n *** ADC Example (Internal Temp Sensor) ***\r\n"));
//     pc.write(buf, strlen(buf));
    
//     while (true) {
//         double meas;
//         // write your code
        
//         meas = tempSensor.read();
//         meas = ((meas*3.3 - 0.76)/0.0025)+25; 
//         sprintf(buf, "Internal Temperature = %.5f [C]\r\n", meas);
//         pc.write(buf, strlen(buf));
        
//         ThisThread::sleep_for(chrono::seconds(5));
//     }
// }