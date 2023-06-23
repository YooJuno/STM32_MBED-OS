// 4-1
#include "mbed.h" 

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE 80
char buf[MAXIMUM_BUFFER_SIZE];

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED1);

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);

int main(void){
    printf("mbed-os version: %d. %d. %d.\r\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
    
    sprintf(buf, "Hello New Serial function in mbed-os v.%.1f\r\n", 6.0);
    pc.write(buf, strlen(buf));
    
    sprintf(buf, "Enter characters...\r\n");
    pc.write(buf, strlen(buf));
    
    while(1){
        
        led = !led;    
        
        // Echo the input back to the terminal.
        int num = pc.read(buf, sizeof(buf));
        pc.write(buf, num);
        if(buf[0] == '\r'){
            pc.write("\n",1);   
        }
    }
}
