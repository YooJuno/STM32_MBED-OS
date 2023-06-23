// 9-4
#include "mbed.h"
#include "srf05.h"

srf05 ultrasonic(ARDUINO_UNO_D3, ARDUINO_UNO_D2);

UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);

char rx_buffer[10];
char tx_buffer[80];
int index = 0;
volatile int flag = 1;
volatile int rx_flag = 1;
int angle = 550;

void control_servo();

void rx_ISR(void) {
    char ch;
    pc.read(&ch, 1);
    pc.write(&ch, 1);
    rx_buffer[index++] = ch;
    if (ch == '\r') { //CR
        pc.write("\n", 1); //LF
        rx_buffer[--index] = '\0';
        index = 0;
        flag = 1;
    }
}

int main() {
    pc.attach(&rx_ISR);

    sprintf(tx_buffer, "Lab 9-4\r\n");
    pc.write(tx_buffer, strlen(tx_buffer));

    sprintf(tx_buffer, "Select the operation mode:\r\n[1] Single measurement\r\n[2] Repeated measurement\r\n[q] quit the program\r\n");
    pc.write(tx_buffer, strlen(tx_buffer));

    bool is_repeated_measurement = false;

    while (1) {
        if (flag) {
            char input = rx_buffer[0];
            flag = 0;

            if (input == 'q') {
                ultrasonic.measure_stop();
                sprintf(tx_buffer, "Program terminated.\r\n");
                pc.write(tx_buffer, strlen(tx_buffer));
                break;
            }
            else if (input == '1') {
                is_repeated_measurement = false;
                ultrasonic.measure_once();
                if (ultrasonic.is_available()) {
                    float distance = ultrasonic.getDistance();
                    sprintf(tx_buffer, "Distance: %.2f cm\r\n", distance);
                    pc.write(tx_buffer, strlen(tx_buffer));
                }
            }
            else if (input == '2' && !is_repeated_measurement) {
                sprintf(tx_buffer, "Enter the repetition interval in seconds: ");
                pc.write(tx_buffer, strlen(tx_buffer));
                while (!flag) {
                    ThisThread::sleep_for(chrono::milliseconds(10));
                }
                float period = atof(rx_buffer);
                ultrasonic.measure_repeat(period);
                is_repeated_measurement = true;
            }
        }

        if (is_repeated_measurement && ultrasonic.is_available()) {
            float distance = ultrasonic.getDistance();
            sprintf(tx_buffer, "Distance: %.2f cm\r\n", distance);
            pc.write(tx_buffer, strlen(tx_buffer));
            ThisThread::sleep_for(chrono::milliseconds(100));
        }
    }

    return 0;
}
