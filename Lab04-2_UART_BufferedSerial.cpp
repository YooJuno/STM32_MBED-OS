// 4-2
#include "mbed.h" 

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE 80
char buf[MAXIMUM_BUFFER_SIZE];

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED1);

// Create a BufferedSerial object with a default baud rate.
static BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
int flag = 0;
void rx_handler(){// 데이터가 도착했다는 것을 헨들러에서 캐치
    led = !led;
    //
    if(pc.readable()) flag = 1;
    //    
}

int main(void){
    pc.set_blocking(false); // non blocking
    pc.sigio(rx_handler);
    
    sprintf(buf, "Hello New Serial function in mbed-os v.%.1f\r\n",6.0);
    pc.write(buf, strlen(buf));
    sprintf(buf, "Enter characters...\r\n");
    pc.write(buf, strlen(buf));
    
    while(1){// 핸들러에서 도착했다는 신호를 통해서 읽고 쓰면 됨
        //
        
        if(flag == 1) {
            
            
            int num = pc.read(buf, sizeof(buf));// non blocking이라서...
            pc.write(buf, num);
            flag=0;
            if (buf[0] == '\r'){
                pc.write("\n", 1);
                
            }
            
        }
    }
    
}
