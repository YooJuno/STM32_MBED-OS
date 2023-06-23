// Lab09-2_Ultrasonic_Sensor_Dist_Library

#include "mbed.h"
#include "SRF05.h"


BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
SRF05 srf05(ARDUINO_UNO_D3, ARDUINO_UNO_D2);

char buffer[80];

int main(){
    
    sprintf(buffer, "Mbed OS version %d.%d.%d\r\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "\r\n Welcome to ultrasonic Sensor Lab! 9-2\r\n");
    pc.write(buffer, strlen(buffer));
    
    while(true){
        sprintf(buffer, "The distance is %.2f [cm] \n\r", srf05.read());
        pc.write(buffer, strlen(buffer));
        ThisThread::sleep_for(3000ms);   
    }
}

