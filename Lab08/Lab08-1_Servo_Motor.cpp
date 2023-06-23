// Lab8-1


// 8-1 : Servo Motor

#include "mbed.h" 
#include "math.h"
#define PWM_MIN 500 
#define PWM_MAX 2500

PwmOut servoMotor(D6); //PB_10, D6 
UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
char rx_buffer[10]; 
char tx_buffer[80]; 
int index = 0; 
volatile int flag ;
int angle = 550;
// int angle = 700;

void control_servo();

void rx_ISR(void) {
    char ch;
    pc.read(&ch, 1); 
    pc.write(&ch, 1); 
    rx_buffer[index++] = ch; 
    if (ch == '\r') { //CR
        pc.write("\n", 1); //LF 
        rx_buffer[--index] = '\0'; 
        // index = 0;
        flag = 1;
    } 
}

int main() {
    sprintf(tx_buffer, "PWM Test Program (Servo Motor)!\r\n"); 
    pc.write(tx_buffer, strlen(tx_buffer));
    pc.attach(rx_ISR);
    servoMotor.period_ms(20); // PWM period = 20ms 
    servoMotor.pulsewidth_us(PWM_MIN); //for 0 degree
    
    while(1) { 
        flag = 0;
        sprintf(tx_buffer, " Enter the rotation degree [0 - 180]: "); 
        pc.write(tx_buffer, strlen(tx_buffer));
        while(flag != 1);
        
        control_servo(); 
    }
}

void control_servo() {
    // make your code 
    int angle = 0;
    int ten = 1;
    for(int i=index-1 ; i>=0 ; i--){
       if(rx_buffer[i]<48 || rx_buffer[i]>57) continue;
       angle += (rx_buffer[i]-48)*ten;
       ten *= 10;
    }
    int time_ang = PWM_MIN + ((PWM_MAX-PWM_MIN)*angle)/180;
    sprintf(tx_buffer, "Pulse ON time = %d (%d)\r\n", time_ang ,angle); 
    pc.write(tx_buffer, strlen(tx_buffer));

    index = 0;
    servoMotor.pulsewidth_us(time_ang);

}