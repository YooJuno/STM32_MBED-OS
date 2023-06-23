// 4-4

#include "mbed.h" 
#include <cstdio>
#include <cstring>
#include <cstdlib>

// Maximum number of element the application buffer can contain
#define MAXIMUM_BUFFER_SIZE 80
char buf[MAXIMUM_BUFFER_SIZE];
char buf_temp[MAXIMUM_BUFFER_SIZE];
int i=0;
bool status_green = 0;
bool status_red = 0;

// Create a DigitalOutput object to toggle an LED whenever data is received.
static DigitalOut led(LED1);
static DigitalOut RedLED(PB_3);
static DigitalOut GreenLED(PB_5);


static UnbufferedSerial pc(D8, D2); // Bluetooth
static UnbufferedSerial pc1(CONSOLE_TX, CONSOLE_RX); // Serial 


void rx_handler(){
    char c;
    
    led = !led;
    if(pc.read(&c,1)){
        buf_temp[i++] = c;
        //Echo the input back to the terminal
        //pc.write(&c, 1);
        //pc1.write(&c, 1);
        if(c == '\r'){
            char buf_color[MAXIMUM_BUFFER_SIZE];
            char buf_on_off[MAXIMUM_BUFFER_SIZE];
            
            int idx_color=0;
            int j;
            for(j=0 ; buf_temp[j] != ' ' ; j++){
                if(buf_temp[j] == 10) continue;
                buf_color[idx_color++] = buf_temp[j];   
            }
            buf_color[idx_color] = NULL;
            
            int idx=0;
            for(int k=j ; buf_temp[k] != '\r' ; k++){
                if(buf_temp[k] == ' ') continue;
                buf_on_off[idx++] = buf_temp[k];
            }
            buf_on_off[idx] = NULL;
            
            sprintf(buf, "%s", buf_temp);
            pc1.write(buf, strlen(buf));
            if(strcmp("GreenLED", buf_color) == 0){
                if(strcmp("on", buf_on_off) == 0){
                    status_green = 1;
                    sprintf(buf, "GreenLED status: on\r");
                    pc1.write(buf, strlen(buf));
                    sprintf(buf, "GreenLED status: on\r");
                    pc.write(buf, strlen(buf));
                    GreenLED.write(1);
                }
                else if(strcmp("off", buf_on_off) == 0){
                    status_green = 0;
                    sprintf(buf, "GreenLED status: off\r");
                    pc1.write(buf, strlen(buf));
                    sprintf(buf, "GreenLED status: off\r");
                    pc.write(buf, strlen(buf));
                    GreenLED.write(0);
                }
                else if(strcmp("status", buf_on_off) == 0){
                    if(status_green == 1){
                        sprintf(buf, "GreenLED status: on\r");
                        pc1.write(buf, strlen(buf));
                        sprintf(buf, "GreenLED status: on\r");
                        pc.write(buf, strlen(buf));
                    }
                    else{
                        sprintf(buf, "GreenLED status: off\r");
                        pc1.write(buf, strlen(buf));
                        sprintf(buf, "GreenLED status: off\r");
                        pc.write(buf, strlen(buf));
                    }
                    
                
                }
                else{
                    sprintf(buf, "Undefined command\r");
                    pc1.write(buf, strlen(buf));
                    sprintf(buf, "Undefined command\r");
                    pc.write(buf, strlen(buf));
                }
            
            }
            
            else if(strcmp("RedLED", buf_color) == 0){
                if(strcmp("on", buf_on_off) == 0){
                    status_red = 1;
                    sprintf(buf, "RedLED status: on\r");
                    pc1.write(buf, strlen(buf));
                    sprintf(buf, "RedLED status: on\r");
                    pc.write(buf, strlen(buf));
                    RedLED.write(1);
                
                }
                else if(strcmp("off", buf_on_off) == 0){
                    status_red = 0;
                    sprintf(buf, "RedLED status: off\r");
                    pc1.write(buf, strlen(buf));
                    sprintf(buf, "RedLED status: off\r");
                    pc.write(buf, strlen(buf));
                    RedLED.write(0);
                
                }
                else if(strcmp("status", buf_on_off) == 0){
                    if(status_red == 1){
                        sprintf(buf, "RedLED status: on\r");
                        pc1.write(buf, strlen(buf));
                        sprintf(buf, "RedLED status: on\r");
                        pc.write(buf, strlen(buf));
                    }
                    else{
                        sprintf(buf, "RedLED status: off\r");
                        pc1.write(buf, strlen(buf));
                        sprintf(buf, "RedLED status: off\r");
                        pc.write(buf, strlen(buf));
                    }
                    
                
                }
                else{
                    sprintf(buf, "Undefined command\r");
                    pc1.write(buf, strlen(buf));
                    sprintf(buf, "Undefined command\r");
                    pc.write(buf, strlen(buf));
                }
            }
            
            else{
                sprintf(buf, "Undefined command\r");
                pc1.write(buf, strlen(buf));
                sprintf(buf, "Undefined command\r");
                pc.write(buf, strlen(buf));
            }
        
            
            for(int init=0 ; init<i ; init++) buf_temp[init] = '\0';
            i=0;
            
        }
    }
}

int main(void){
    pc.baud(9600);
    pc.format(8, SerialBase::None, 1);
    
    pc1.baud(115200);
    pc1.format(8, SerialBase::None, 1);
    
    pc.attach(rx_handler, SerialBase::RxIrq);

    sprintf(buf, "Hello New Serial function in mbed-os v.%.1f\r\n",6.0);
    pc1.write(buf, strlen(buf));
    sprintf(buf, "LED Control Example by a CellPhone\r\n");
    pc1.write(buf, strlen(buf));
    
    sprintf(buf, "Hello New Serial function in mbed-os v.%.1f\r\n",6.0);
    pc.write(buf, strlen(buf));
    sprintf(buf, "LED Control Example by a CellPhone\r\n");
    pc.write(buf, strlen(buf));
    
    
    while(1){
       
    }
    
}
