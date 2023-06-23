 // 11-2
#include "mbed.h"
#define HTU21D_TEMP_READ (0xF3) // no hold command 
#define HTU21D_HUMI_READ (0xF5)
#define HTU21G_USER_WRITE (0xE6)
#define HTU21D_ADDR (0x40)

I2C i2c(PB_9, PB_8); // SDA , SCL

BufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 115200);
char buffer[80];


volatile bool read_HumTemp, crc_err1,crc_err2;
volatile float temp, hum;

#define CRC_POLYNOMIAL 0x131   //CRC polynomial for HTU21D

uint8_t crc8(const uint8_t *data, int len) {
    uint8_t crc = 0;   // Initial value
    for (int j = len; j; --j) {
        crc ^= (*data++);
        for (int i = 8; i; --i) {
            crc = (crc & 0x80) ? (crc << 1) ^ CRC_POLYNOMIAL : (crc << 1);
        }
    }
    return crc;
}

bool CRC_check(char *data, int len, char received_crc){
    char calculated_crc = crc8((uint8_t*)data, len);
    if (calculated_crc != received_crc) {
        return false;
    } 
    return true;
}



int main(){
    sprintf(buffer, "\r\nI2C Test Program for HTU21D Sensor\r\n");
    pc.write(buffer, strlen(buffer));
    i2c.frequency(100*1000);
    
    while(1){
        char tx_humid[1];
        char rx_humid[3];
        tx_humid[0] = HTU21D_HUMI_READ;
        i2c.write((HTU21D_ADDR << 1) , tx_humid, 1);
        ThisThread::sleep_for(16ms); // 12bits
        i2c.read((HTU21D_ADDR << 1), rx_humid, 3);
        ThisThread::sleep_for(1ms);
        unsigned int humidity = ((unsigned int) rx_humid[0] << 8) | (unsigned int) rx_humid[1];
        humidity &= 0xFFFE; // 1111 1111 1111 1110
        float RH = -6 + (125 * (humidity / 65536.0));
        
        char tx_temp[1];
        char rx_temp[3];
        tx_temp[0] = HTU21D_TEMP_READ; 
        i2c.write((HTU21D_ADDR << 1) , tx_temp, 1); // Soft Reset
        ThisThread::sleep_for(50ms); // 14bits
        i2c.read((HTU21D_ADDR << 1) , rx_temp, 3);
        
        ThisThread::sleep_for(1ms);
        unsigned int temperature = ((unsigned int) rx_temp[0] << 8) | (unsigned int) rx_temp[1];
        temperature &= 0xFFFC; // 1111 1111 1111 1100
        float Temp = -46.85 + (175.72 * (temperature / 65536.0));
        
        
        // rx_temp[0] ^= 0x01; 
        rx_humid[0] ^= 0x01; 

        
        bool temp_crc_check = CRC_check(rx_temp, 2, rx_temp[2]);
        bool hum_crc_check = CRC_check(rx_humid, 2, rx_humid[2]);


        if(!temp_crc_check){
            sprintf(buffer, "\r\nTemperature : %.2f [C]\r\n",Temp);
            pc.write(buffer, strlen(buffer));
        }
        else{
            sprintf(buffer, "\r\n CRC error \r\n");
            pc.write(buffer, strlen(buffer));
        }
            
        if(!hum_crc_check){
            sprintf(buffer, "Relative Humid : %.2f [%%]\r\n",RH);
            pc.write(buffer, strlen(buffer));
        }
        else{
            sprintf(buffer, "\r\n CRC error \r\n");
            pc.write(buffer, strlen(buffer));
        }
        ThisThread::sleep_for(3s);  
    }
}
