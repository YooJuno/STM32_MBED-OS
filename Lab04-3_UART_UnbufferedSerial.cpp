// 4-3
#include "mbed.h" 

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE 80
char buf[MAXIMUM_BUFFER_SIZE];

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED1);

// Create a BufferedSerial object with a default baud rate.
static UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX);

void rx_handler(){
    char c;
    
    led = !led;
    if(pc.read(&c,1)){
        //Echo the input back to the terminal
        pc.write(&c, 1);
        if(c == '\r'){
            pc.write("\n",1);
        }
    }
}

int main(void){
    pc.baud(115200);
    pc.format(8, SerialBase::None, 1);
    
    pc.attach(rx_handler, SerialBase::RxIrq);

    
    sprintf(buf, "Hello New Serial function in mbed-os v.%.1f\r\n",6.0);
    pc.write(buf, strlen(buf));
    
    sprintf(buf, "Enter characters...\r\n");
    pc.write(buf, strlen(buf));
    
    while(1){
       
    }
    
}
