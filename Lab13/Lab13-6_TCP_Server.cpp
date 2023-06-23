// lab 13-6

#include "mbed.h"

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 230400); 
UnbufferedSerial wifi(ARDUINO_UNO_D8, ARDUINO_UNO_D2, 115200);

char buffer[80];
int flag=0;
// main() runs in its own thread in the OS 
int main() {
    SocketAddress sockAddr;

    char ch;
    sprintf(buffer, "\r\nWiFi TCP Server example\r\n"); 
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "\r\nSet SoftAP+Station mode\r\n"); 
    pc.write(buffer, strlen(buffer));
    
    sprintf(buffer, "AT+CWMODE=3\r\n"); // [mode]  1 = Station mode / 2 = softAP mode / 3 = softAP + Station mode
    wifi.write(buffer, strlen(buffer));
    pc.write(buffer, strlen(buffer));



    sprintf(buffer, "AT+CIPMUX=1\r\n"); // To use multi-connection
    wifi.write(buffer, strlen(buffer));
    pc.write(buffer, strlen(buffer));





    sprintf(buffer, "AT+CIPSERVER=1,50001\r\n"); // SERVER의 port ON 
    wifi.write(buffer, strlen(buffer));
    pc.write(buffer, strlen(buffer));

    sprintf(buffer, "AT+CWSAP=\"juno_ap\",\"19980220\",5,3\r\n"); // configuration of softAP
    wifi.write(buffer, strlen(buffer));
    pc.write(buffer, strlen(buffer));

    sprintf(buffer, "AT+CIPAP=\"192.168.2.20\"\r\n"); // Set IP address
    wifi.write(buffer, strlen(buffer));
    pc.write(buffer, strlen(buffer));
    

    char temp[80];
    int idx=0;
    while (true) {
        if (pc.readable()) {
            pc.read(&ch, 1);
            pc.write(&ch, 1);
            wifi.write(&ch, 1);
            
        }
        int clientID;
        char sendBuffer[100];
        int flag_OK;
        if (wifi.readable()) { 
            wifi.read(&ch, 1); 
            // pc.write(&ch, 1);


    
            // pc.write(&ch,1);
            if(flag==0 && ch=='I') flag = 1;
            else if(flag==1 && ch=='P') flag = 2;
            else if(flag==2 && ch=='D') flag = 3;
            else if(flag==3 && ch==',') flag = 4;
            else if(flag==4) {
                clientID = ch - '0'; // Convert ASCII to int
                flag = 5;
            }
            else if(flag==5 && ch==',') flag = 6;
            else if(flag>=6 && flag<8) flag++;
            else if(flag>=8){
                pc.write(&ch,1);
                temp[idx++] = ch;
                if( ch == '\r'){
                    temp[idx++] = '\n';
                    flag = 0;
                    temp[idx] = '\0'; // Make sure to null terminate your string
                    pc.write(temp,strlen(temp));
                    sprintf(sendBuffer, "AT+CIPSEND=%d,%d\r\n", clientID, strlen(temp)); // Prepare send command
                    wifi.write(sendBuffer, strlen(sendBuffer)); // Send send command
                    ThisThread::sleep_for(100ms); // Delay for ESP8266 to be ready to accept the data
                    wifi.write(temp, strlen(temp)); // Send the received data
                    idx = 0;
                }
            }
            
        }


    }
}