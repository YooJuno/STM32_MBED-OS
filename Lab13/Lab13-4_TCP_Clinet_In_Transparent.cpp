// 13-4

#include "mbed.h"
UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 230400); 
UnbufferedSerial wifi(ARDUINO_UNO_D8, ARDUINO_UNO_D2, 115200);

char buffer[80];
// main() runs in its own thread in the OS 
int main() {
    char ch;
    sprintf(buffer, "\r\n ***** key <---> WiFi *****\r\n"); 
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "\r\n TCP and UDP Operation Using AT Commands\r\n"); 
    pc.write(buffer, strlen(buffer));
    
    while (true) {
        if (pc.readable()) {
            pc.read(&ch, 1);
            if (ch == 0x03) { 
                //ctrl+c to exit from transparent mode ("+++")
                // that is, Back to a normal mode) 
                wifi.write("+++", 3);
            }   
            else { 
                wifi.write(&ch, 1);
            } 
        }
        if (wifi.readable()) { 
            wifi.read(&ch, 1); 
            pc.write(&ch, 1);
        } 
    }
}