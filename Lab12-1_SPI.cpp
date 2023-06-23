
// Lab 12-1_SPI

#include "mbed.h"

SPI spi(ARDUINO_UNO_D11, ARDUINO_UNO_D12, ARDUINO_UNO_D13); // MOSI, MISO, SCK
DigitalOut cs(PA_4); // Chip Select
UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200); 
char buf[80];
AnalogIn ain0(ARDUINO_UNO_A0); // ADC...? 


char rx_buffer[10];
int index = 0;
volatile int flag = 0;

void set_MCP41010(void){
    int num;
    char tx_buff[2];
    
    num = atoi((const char *)rx_buffer);
    sprintf(buf,"Potentiometer = %d (%.0f [mV])\r\n", num, 3300.0*num/255.0);
    pc.write(buf, strlen(buf));
    
    tx_buff[0] = 0x11;
    tx_buff[1] = (char)num;
    cs = 0;
    spi.write(tx_buff, 2, 0, 0);
    cs = 1;
}

void rx_cb(void){
    char ch;
    if(pc.read(&ch,1)){
        if(ch != '\n')pc.write(&ch, 1);
        rx_buffer[index++] = ch;
        
        //if(ch == 0x0D) {
        if(ch == '\r'){
            pc.write("\n",1);
            rx_buffer[--index] = '\0';
            index = 0;
            flag = 1;
        }
    }
}

int main(){
    float meas;
    
    cs = 1;
    spi.format(8, 0);
    spi.frequency(1000000);
    
    sprintf(buf,"\r\n Test SPI Program \r\n");
    pc.write(buf, strlen(buf));
    pc.attach(callback(rx_cb));
    
    while(1){
        flag = 0;
        sprintf(buf,"\r\nEnter the value for the potentiometer [0, 255]: ");
        pc.write(buf, strlen(buf));
        
        while(flag != 1){}
        set_MCP41010();
        //wait(0.5);
        ThisThread::sleep_for(500ms);
        meas = ain0.read()*3300;
        sprintf(buf, "ADC value = %.0f  [mV]\r\n", meas);
        pc.write(buf, strlen(buf));
    }  
}